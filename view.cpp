#include "view.h"
#include <locale.h>

void View::Run()
{
    setlocale(LC_ALL, "russian");
    int menu = -1;

    while (menu != 0)
    {
        std::cout << "����" << std::endl;
        std::cout << "1. �������� ������ ������" << std::endl;
        std::cout << "2. �������� ���������� � �����" << std::endl;
        std::cout << "3. ������� ����������" << std::endl;
        std::cout << "4. ������� ����������" << std::endl;
        std::cout << "5. ������� ����" << std::endl;
        std::cout << "6. ����������� ����" << std::endl;
        std::cout << "7. ���������� ����" << std::endl;
        std::cout << "8. ������� ���������� � �����" << std::endl;
        std::cout << "9. �������� �������� �����" << std::endl;
        std::cout << "10. ������� ����� ��������, ���������� ������� � ��������� ������ � ����" << std::endl;
        std::cout << "11. �������� ����� �������� / ���������� ������� / ��������� ������ � ����" << std::endl;
        std::cout << "0. �����" << std::endl;

        std::cout << "������� ����� ����:" << std::endl;
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
            std::cout << "1. ����� �������� �����" << std::endl;
            std::cout << "2. ����� ���������� ������� � ����" << std::endl;
            std::cout << "3. ����� ��������� ������ � ����" << std::endl;
            std::cout << "����� ����� �� ������ ��������? ";
            int option;
            std::cin >> option;
            setFileTime(option);
            break;
        case 0:
            return;
            break;
        default:
            std::cout << "�������� �����" << std::endl;
            break;
        }
        system("pause");
        system("cls");
    }
}

void View::printDriveInfo()
{
    std::string name{};
    std::cout << "������� ��� ����� (������: C:\): ";
    std::cin >> name;
    FileManager::PrintDriveInfo(name);
}

void View::createDir()
{
    std::filesystem::path path{};
    std::cout << "������� ���� ���������� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    if (FileManager::CreateDir(path) == 0)
        std::cout << "���������� �������" << std::endl;
    else
        std::cout << "������ �������� ����������" << std::endl;
}

void View::removeDir()
{
    std::filesystem::path path{};
    std::cout << "������� ���� ���������� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    if (FileManager::RemoveDir(path) == 0)
        std::cout << "���������� �������" << std::endl;
    else
        std::cout << "������ �������� ����������" << std::endl;
}

void View::createFile()
{
    std::filesystem::path path{};
    std::cout << "������� ���� � ����� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    if (FileManager::CrFile(path) != 0)
        std::cout << "���� ������" << std::endl;
    else
        std::cout << "������ �������� �����" << std::endl;
}

void View::copyFile()
{
    std::filesystem::path src{};
    std::filesystem::path dst{};

    std::cout << "������� ���� �����, ������� ���������� ����������� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> src;

    std::cout << "������� ���� �����, ���� ���� ����� ���������� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> dst;

    if (FileManager::CpyFile(src, dst) == 0)
        std::cout << "���� ����������" << std::endl;
    else
        std::cout << "������ ����������� �����" << std::endl;
}

void View::moveFile()
{
    std::filesystem::path src{};
    std::filesystem::path dst{};

    std::cout << "������� ���� �����, ������� ���������� ����������� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> src;

    std::cout << "������� ����� ���� ����� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> dst;

    if (FileManager::MvFile(src, dst) == 0)
        std::cout << "���� ���������" << std::endl;
    else
        std::cout << "������ ����������� �����" << std::endl;
}

void View::setFileAtts()
{
    std::filesystem::path path{};
    std::cout << "������� ���� � ����� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    std::cout << "�������� �����: " << std::endl;
    FileManager::PrintFileAtts(path);

    std::string strAtts{};
    DWORD dwAtts = FILE_ATTRIBUTE_NORMAL;
    std::cout << "1. READ-ONLY" << std::endl;
    std::cout << "2. HIDDEN" << std::endl;
    std::cout << "3. DIRECTORY" << std::endl;
    std::cout << "4. ARCHIVE" << std::endl;
    std::cout << "5. NORMAL" << std::endl;
    std::cout << "�������� �������� �� ������ � �������� �� ������ ��� ��������-������������ (������: 123): ";
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
            std::cout << "�������� �����" << std::endl;
            return;
            break;
        }
    }

    if (FileManager::SetFileAtts(path, dwAtts) == 0)
        std::cout << "�������� ���� ��������" << std::endl;
    else
        std::cout << "������ ��������� ��������� �����" << std::endl;
}

void View::printFileInfo()
{
    std::filesystem::path path{};
    std::cout << "������� ���� � ����� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    FileManager::PrintFileInfo(path);
    FileManager::PrintFileAtts(path);
}

void View::printFileTime()
{
    std::filesystem::path path{};
    std::cout << "������� ���� � ����� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    FileManager::PrintFileTime(path);
}

void View::setFileTime(int option)
{
    std::filesystem::path path{};

    std::cout << "������� ���� � ����� (/ ��� \\\\ � �������� �����������): ";
    std::cin >> path;

    int year, month, day, hour, min, sec;
    std::cout << "������� ����, �����, ���, ���, ������ � �������: ";
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
                std::cout << "����� �������� ����� ��������" << std::endl;
            else
                std::cout << "������ ��� ��������� �����" << std::endl;
            break;
        case 2:
            if (FileManager::SetFileLastAccessTime(path, timet) == 0)
                std::cout << "����� �������� ����� ��������" << std::endl;
            else
                std::cout << "������ ��� ��������� �����" << std::endl;
            break;
        case 3:
            if (FileManager::SetFileLastWriteTime(path, timet) == 0)
                std::cout << "����� �������� ����� ��������" << std::endl;
            else
                std::cout << "������ ��� ��������� �����" << std::endl;
            break;
        default:
            std::cout << "������������ �����" << std::endl;
            break;
    }
}
