#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <chrono>

//#define DEFAULT_MAX_OPS 16

// Структура для хранения информации об асинхронной операции
struct aio_op {
    struct aiocb cb;
    char *buffer;
    int is_write; // Флаг: 0 – операция чтения, 1 – операция записи
    off_t offset; // Смещение, с которого выполняется операция
    size_t data_size; // Фактически обработанное число байт
};

int fd_src, fd_dst; // Дескрипторы исходного и целевого файлов
off_t total_file_size = 0; // Размер исходного файла
off_t current_offset = 0; // Сколько байт уже запрошено к чтению
volatile int ops_in_progress = 0; // Число активных асинхронных операций
volatile int read_finished = 0; // Флаг: все операции чтения запущены
int max_concurrent_ops; // Максимальное число одновременных операций
std::chrono::_V2::system_clock::time_point start;
std::chrono::_V2::system_clock::time_point end;

// Прототип функции планирования операции чтения
void schedule_read(off_t offset, size_t block_size);

// Обработчик завершения асинхронной операции
void aio_handler(sigval_t sigval) {
    struct aio_op *op = (struct aio_op *) sigval.sival_ptr;
    // Проверяем, нет ли ошибки
    int err = aio_error(&op->cb);
    ssize_t bytes = aio_return(&op->cb);
    
    if (err != 0) {
        fprintf(stderr, "AIO error at offset %ld: %s\n", op->offset, strerror(err));
    } else {
        if (!op->is_write) {
            // Завершена операция чтения
            if (bytes > 0) {
                printf("Read %zd bytes with offset = %ld\n", bytes, op->offset);
                // Создаём новую структуру для операции записи
                struct aio_op *wop = (struct aio_op*)malloc(sizeof(struct aio_op));
                if (!wop) {
                    perror("read malloc error");
                    free(op->buffer);
                    free(op);
                    ops_in_progress--;
                    return;
                }
                wop->buffer = op->buffer; // Передаём буфер, считанный из файла
                wop->is_write = 1; // Помечаем, что это операция записи
                wop->offset = op->offset;
                wop->data_size = bytes; // Размер записи равен числу реально прочитанных байт

                // Инициализируем блок управления для записи
                memset(&wop->cb, 0, sizeof(struct aiocb));
                wop->cb.aio_fildes = fd_dst;
                wop->cb.aio_buf = wop->buffer;
                wop->cb.aio_nbytes = bytes;
                wop->cb.aio_offset = wop->offset;
                wop->cb.aio_sigevent.sigev_notify = SIGEV_THREAD;
                wop->cb.aio_sigevent.sigev_notify_function = aio_handler;
                wop->cb.aio_sigevent.sigev_notify_attributes = NULL;
                wop->cb.aio_sigevent.sigev_value.sival_ptr = wop;

                if (aio_write(&wop->cb) < 0) {
                    perror("aio_write error");
                    free(wop->buffer);
                    free(wop);
                } else {
                    ops_in_progress++; // Учёт новой операции записи
                }
            } else {
                // Если bytes == 0, значит достигнут конец файла
                printf("Endfile with offset = %ld\n", op->offset);
            }
        } else {
            // Завершена операция записи
            printf("Written %zd bytes with offset = %ld\n", bytes, op->offset);
            free(op->buffer);
        }
    }
    
    free(op);
    ops_in_progress--;

    // Если все операции чтения запущены и активных операций нет – завершаем копирование
    if (read_finished && ops_in_progress <= 0) {
        end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        printf("File copied successfully. Time: %ld ms\n", elapsed.count());

        close(fd_src);
        close(fd_dst);
        exit(0);
    }
}

// Функция для планирования асинхронного чтения
void schedule_read(off_t offset, size_t block_size) {
    struct aio_op *rop = (struct aio_op*)malloc(sizeof(struct aio_op));
    if (!rop) {
        perror("malloc error");
        return;
    }
    rop->buffer = (char*)malloc(block_size);
    if (!rop->buffer) {
        perror("buffer error");
        free(rop);
        return;
    }
    rop->is_write = 0;
    rop->offset = offset;
    rop->data_size = block_size; // ожидаем прочитать block_size байт

    // Заполняем блок управления для асинхронного чтения
    memset(&rop->cb, 0, sizeof(struct aiocb));
    rop->cb.aio_fildes = fd_src;
    rop->cb.aio_buf = rop->buffer;
    rop->cb.aio_nbytes = block_size;
    rop->cb.aio_offset = offset;
    rop->cb.aio_sigevent.sigev_notify = SIGEV_THREAD;
    rop->cb.aio_sigevent.sigev_notify_function = aio_handler;
    rop->cb.aio_sigevent.sigev_notify_attributes = NULL;
    rop->cb.aio_sigevent.sigev_value.sival_ptr = rop;

    if (aio_read(&rop->cb) < 0) {
        perror("aio_read failed");
        free(rop->buffer);
        free(rop);
        return;
    }
    ops_in_progress++;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Using: %s <source_file> <destination_file> <block_size> [max_operations]\n", argv[0]);
        return 1;
    }
    
    const char *src_filename = argv[1];
    const char *dst_filename = argv[2];
    size_t block_size = (size_t)atoi(argv[3]);
    max_concurrent_ops = atoi(argv[4]);
    
    // Открываем исходный файл
    fd_src = open(src_filename, O_RDONLY | O_NONBLOCK);
    if (fd_src < 0) {
        perror("Failed opemimg source file");
        return 1;
    }
    
    // Открываем или создаём файл для записи
    fd_dst = open(dst_filename, O_CREAT | O_WRONLY | O_TRUNC | O_NONBLOCK, 0666);
    if (fd_dst < 0) {
        perror("Failed opening destination file");
        close(fd_src);
        return 1;
    }
    
    // Получаем размер исходного файла
    struct stat st;
    if (fstat(fd_src, &st) < 0) {
        perror("fstat failed");
        close(fd_src);
        close(fd_dst);
        return 1;
    }
    total_file_size = st.st_size;
    printf("File size: %ld bytes\n", total_file_size);
    
    start = std::chrono::system_clock::now();
    
    // Планируем операции чтения до конца файла, контролируя число одновременных операций
    while (current_offset < total_file_size) {
        if (ops_in_progress < max_concurrent_ops) {
            // Если оставшихся байт меньше размера блока, читаем только необходимое количество
            size_t bytes_to_read = block_size;
            if (current_offset + bytes_to_read > total_file_size) {
                bytes_to_read = total_file_size - current_offset;
            }
            schedule_read(current_offset, bytes_to_read);
            current_offset += bytes_to_read;
        } else {
            usleep(1000);
        }
    }
    // Отметка, что все операции чтения запущены
    read_finished = 1;
    
    // Ожидаем завершения всех активных операций
    while (ops_in_progress > 0) {
        usleep(1000);
    }
    
    close(fd_src);
    close(fd_dst);
    return 0;
}
