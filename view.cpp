#include "view.h"
#include <locale.h>

void View::Run()
{
    setlocale(LC_ALL, "russian");
    int menu = -1;

    while (menu != 0)
    {
        std::cout << "Меню" << std::endl;
        std::cout << "1. Получить список дисков" << std::endl;
        std::cout << "2. Получить информацию о диске" << std::endl;
        std::cout << "3. Создать директорию" << std::endl;
        std::cout << "4. Удалить директорию" << std::endl;
        std::cout << "5. Создать файл" << std::endl;
        std::cout << "6. Переместить файл" << std::endl;
        std::cout << "7. Копировать файл" << std::endl;
        std::cout << "8. Вывести инфорамцию о файле" << std::endl;
        std::cout << "9. Изменить атрибуты файла" << std::endl;
        std::cout << "10. Вывести время создания, последнего доступа и последней записи в файл" << std::endl;
        std::cout << "11. Изменить время создания / последнего доступа / последней записи в файл" << std::endl;
        std::cout << "0. Выход" << std::endl;

        std::cout << "Введите пункт меню:" << std::endl;
        std::cin >> menu;

        switch (menu)
        {
        case 1:
            FileManager::PrintDrives();
            break;
        case 2:
            printDriveInfo();
            break;
        case 3:
            createDir();
            break;
        case 4:
            removeDir();
            break;
        case 5:
            createFile();
            break;
        case 6:
            moveFile();
            break;
        case 7:
            copyFile();
            break;
        case 8:
            printFileInfo();
            break;
        case 9:
            setFileAtts();
            break;
        case 10:
            printFileTime();
            break;
        case 11:
            std::cout << "1. Время создания файла" << std::endl;
            std::cout << "2. Время последнего доступа в файл" << std::endl;
            std::cout << "3. Время последней записи в файл" << std::endl;
            std::cout << "Какое время вы хотите изменить? ";
            int option;
            std::cin >> option;
            setFileTime(option);
            break;
        case 0:
            return;
            break;
        default:
            std::cout << "Неверный номер" << std::endl;
            break;
        }
        system("pause");
        system("cls");
    }
}

void View::printDriveInfo()
{
    std::string name{};
    std::cout << "Введите имя диска (пример: C:\): ";
    std::cin >> name;
    FileManager::PrintDriveInfo(name);
}

void View::createDir()
{
    std::filesystem::path path{};
    std::cout << "Введите путь директории (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    if (FileManager::CreateDir(path) == 0)
        std::cout << "Директория создана" << std::endl;
    else
        std::cout << "Ошибка создания директории" << std::endl;
}

void View::removeDir()
{
    std::filesystem::path path{};
    std::cout << "Введите путь директории (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    if (FileManager::RemoveDir(path) == 0)
        std::cout << "Директория удалена" << std::endl;
    else
        std::cout << "Ошибка удаления директории" << std::endl;
}

void View::createFile()
{
    std::filesystem::path path{};
    std::cout << "Введите путь к файлу (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    if (FileManager::CrFile(path) != 0)
        std::cout << "Файл создан" << std::endl;
    else
        std::cout << "Ошибка создания файла" << std::endl;
}

void View::copyFile()
{
    std::filesystem::path src{};
    std::filesystem::path dst{};

    std::cout << "Введите путь файла, который необходимо скопировать (/ или \\\\ в качестве разделителя): ";
    std::cin >> src;

    std::cout << "Введите путь файла, куда файл будет скопирован (/ или \\\\ в качестве разделителя): ";
    std::cin >> dst;

    if (FileManager::CpyFile(src, dst) == 0)
        std::cout << "Файл скопирован" << std::endl;
    else
        std::cout << "Ошибка копирования файла" << std::endl;
}

void View::moveFile()
{
    std::filesystem::path src{};
    std::filesystem::path dst{};

    std::cout << "Введите путь файла, который необходимо переместить (/ или \\\\ в качестве разделителя): ";
    std::cin >> src;

    std::cout << "Введите новый путь файла (/ или \\\\ в качестве разделителя): ";
    std::cin >> dst;

    if (FileManager::MvFile(src, dst) == 0)
        std::cout << "Файл перемещен" << std::endl;
    else
        std::cout << "Ошибка перемещения файла" << std::endl;
}

void View::setFileAtts()
{
    std::filesystem::path path{};
    std::cout << "Введите путь к файлу (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    std::cout << "Атрибуты файла: " << std::endl;
    FileManager::PrintFileAtts(path);

    std::string strAtts{};
    DWORD dwAtts = FILE_ATTRIBUTE_NORMAL;
    std::cout << "1. READ-ONLY" << std::endl;
    std::cout << "2. HIDDEN" << std::endl;
    std::cout << "3. DIRECTORY" << std::endl;
    std::cout << "4. ARCHIVE" << std::endl;
    std::cout << "5. NORMAL" << std::endl;
    std::cout << "Выберите атрибуты из списка и напишите их номера без символов-разделителей (пример: 123): ";
    std::cin >> strAtts;

    for (char c : strAtts)
    {
        switch (c)
        {
        case '1':
            dwAtts |= FILE_ATTRIBUTE_READONLY;
            break;
        case '2':
            dwAtts |= FILE_ATTRIBUTE_HIDDEN;
            break;
        case '3':
            dwAtts |= FILE_ATTRIBUTE_DIRECTORY;
            break;
        case '4':
            dwAtts |= FILE_ATTRIBUTE_ARCHIVE;
            break;
        case '5':
            break;
        default:
            std::cout << "Неверная опция" << std::endl;
            return;
            break;
        }
    }

    if (FileManager::SetFileAtts(path, dwAtts) == 0)
        std::cout << "Атрибуты фала изменены" << std::endl;
    else
        std::cout << "Ошибка изменения атрибутов файла" << std::endl;
}

void View::printFileInfo()
{
    std::filesystem::path path{};
    std::cout << "Введите путь к файлу (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    FileManager::PrintFileInfo(path);
    FileManager::PrintFileAtts(path);
}

void View::printFileTime()
{
    std::filesystem::path path{};
    std::cout << "Введите путь к файлу (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    FileManager::PrintFileTime(path);
}

void View::setFileTime(int option)
{
    std::filesystem::path path{};

    std::cout << "Введите путь к файлу (/ или \\\\ в качестве разделителя): ";
    std::cin >> path;

    int year, month, day, hour, min, sec;
    std::cout << "Введите день, месяц, год, час, минуту и секунду: ";
    std::cin >> day >> month >> year >> hour >> min >> sec;

    struct tm datetime;
    datetime.tm_year = year - 1900;
    datetime.tm_mon = month - 1;
    datetime.tm_mday = day;
    datetime.tm_hour = hour - 1;
    datetime.tm_min = min;
    datetime.tm_sec = sec;
    datetime.tm_isdst = 0;

    time_t timet;
    timet = mktime(&datetime);

    switch(option)
    {
        case 1:
            if (FileManager::SetFileCreationTime(path, timet) == 0)
                std::cout << "Время создания файла изменено" << std::endl;
            else
                std::cout << "Ошибка при изменении файла" << std::endl;
            break;
        case 2:
            if (FileManager::SetFileLastAccessTime(path, timet) == 0)
                std::cout << "Время создания файла изменено" << std::endl;
            else
                std::cout << "Ошибка при изменении файла" << std::endl;
            break;
        case 3:
            if (FileManager::SetFileLastWriteTime(path, timet) == 0)
                std::cout << "Время создания файла изменено" << std::endl;
            else
                std::cout << "Ошибка при изменении файла" << std::endl;
            break;
        default:
            std::cout << "Неправильная опция" << std::endl;
            break;
    }
}
