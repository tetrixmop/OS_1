#include "filemanager.h"

namespace fManager
{
	void FileManager::PrintDrives()
	{
		DWORD dwBufferLength = MAX_PATH;
		wchar_t szBuffer[MAX_PATH] = { 0 };
		DWORD dwResList = GetLogicalDriveStrings(dwBufferLength, szBuffer);
		if (dwResList > 0 && dwResList <= MAX_PATH)
		{
			wchar_t* szDrive = szBuffer;
			while (*szDrive)
			{
				wprintf(L"Drive: %s\n", szDrive);
				szDrive += wcslen(szDrive) + 1;
			}
		}
	}

	void FileManager::PrintDriveInfo(const std::string& driveName)
	{
		UINT driveType = GetDriveTypeA(driveName.c_str());

		switch (driveType)
		{
			case 0:
				std::cout << "The drive type cannot be determined.\n";
				return;
				break;
			case 1:
				std::cout << "The root path is invalid.\n";
				return;
				break;
			case 2:
				std::cout << "The drive has removable media.\n";
				break;
			case 3:
				std::cout<< "The drive has fixed media.\n";
				break;
			case 4:
				std::cout << "The drive is a remote (network) drive. \n";
				break;
			case 5:
				std::cout << "The drive is a CD-ROM drive.\n";
				break;
			case 6:
				std::cout << "The drive is a RAM disk.\n";
				break;
		}

		char nameBuffer[MAX_PATH];
		DWORD dwMaximumComponentLength;
		DWORD dwFileSystemFlags;
		DWORD dwSerialNumber;
		char fileSystemName[MAX_PATH];

		GetVolumeInformationA(driveName.c_str(), nameBuffer, MAX_PATH, &dwSerialNumber, &dwMaximumComponentLength, &dwFileSystemFlags, fileSystemName, MAX_PATH);
		printf("Volume Name: %s\n", nameBuffer);
		printf("Volume Serial Number: %d\n", dwSerialNumber);
		printf("Maximun Component Length: %d\n", dwMaximumComponentLength);
		printf("File System Flags: %d\n", dwFileSystemFlags);
		printf("File System Name: %s\n", fileSystemName);

		ULARGE_INTEGER FreeBytesAvailableToCaller;
		ULARGE_INTEGER TotalNumberOfFreeBytes;

		GetDiskFreeSpaceExA(driveName.c_str(), &FreeBytesAvailableToCaller, NULL, &TotalNumberOfFreeBytes);
		printf("Free Bytes: %llu / %llu.\n", TotalNumberOfFreeBytes, FreeBytesAvailableToCaller);
	}

	int FileManager::CreateDir(std::filesystem::path path)
	{
		if (CreateDirectoryA(path.string().c_str(), NULL))
			return 0;
		else
			return 1;
	}

	int FileManager::RemoveDir(std::filesystem::path path)
	{
		if (RemoveDirectoryA(path.string().c_str()))
			return 0;
		else
			return 1;
	}

	int FileManager::CrFile(std::filesystem::path path)
	{
		HANDLE hFile = CreateFileA(path.string().c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile = INVALID_HANDLE_VALUE)
			return 1;
		else
			return 0;
	}

	int FileManager::CpyFile(std::filesystem::path efName, std::filesystem::path nfName)
	{
		if (CopyFileA(efName.string().c_str(), nfName.string().c_str(), TRUE))
			return 0;
		else
			return 1;
	}

	int FileManager::MvFile(std::filesystem::path src, std::filesystem::path dst)
	{
		if (MoveFileA(src.string().c_str(), dst.string().c_str()))
			return 0;
		else
			return 1;
	}

	void FileManager::PrintFileAtts(std::filesystem::path path)
	{
		DWORD dwAtts = GetFileAttributesA(path.string().c_str());
		switch (dwAtts)
		{
		case FILE_ATTRIBUTE_READONLY:
			std::cout << "The file is read - only.\n";
			break;

		case FILE_ATTRIBUTE_HIDDEN:
			std::cout << "The file or directory is hidden.\n";
			break;

		case FILE_ATTRIBUTE_DIRECTORY:
			std::cout << "The file is a directory.\n";
			break;

		case FILE_ATTRIBUTE_ARCHIVE:
			std::cout << "The file or directory is an archive file or directory.\n";
			break;

		case FILE_ATTRIBUTE_NORMAL:
			std::cout << "The file does not have attributes set.\n";
			break;
		}
	}

	int FileManager::SetFileAtts(std::filesystem::path path, DWORD atts)
	{
		if (SetFileAttributesA(path.string().c_str(), atts))
			return 0;
		else
			return 1;
	}


	void FileManager::PrintFileInfo(std::filesystem::path path)
	{
		BY_HANDLE_FILE_INFORMATION fileInfo;
		HANDLE hFile = CreateFileA(path.string().c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			if (GetFileInformationByHandle(hFile, &fileInfo))
			{
				SYSTEMTIME stCreationTime;
				FileTimeToSystemTime(&fileInfo.ftCreationTime, &stCreationTime);

				SYSTEMTIME stLastAccessTime;
				FileTimeToSystemTime(&fileInfo.ftLastAccessTime, &stLastAccessTime);

				SYSTEMTIME stLastWriteTime;
				FileTimeToSystemTime(&fileInfo.ftLastWriteTime, &stLastWriteTime);

				std::cout << "Creation time: " << stCreationTime.wDay << "." << stCreationTime.wMonth << "." << stCreationTime.wYear << " " << stCreationTime.wHour << ":" << stCreationTime.wMinute << std::endl;
				std::cout << "Last Access time: " << stLastAccessTime.wDay << "." << stLastAccessTime.wMonth << "." << stLastAccessTime.wYear << " " << stLastAccessTime.wHour << ":" << stLastAccessTime.wMinute << std::endl;
				std::cout << "Last Write time: " << stLastWriteTime.wDay << "." << stLastWriteTime.wMonth << "." << stLastWriteTime.wYear << " " << stLastWriteTime.wHour << ":" << stLastWriteTime.wMinute << std::endl;
				std::cout << "Volume Serial Number: " << fileInfo.dwVolumeSerialNumber << std::endl;
				std::cout << "File Size High: " << fileInfo.nFileSizeHigh << std::endl;
				std::cout << "File Size Low: " << fileInfo.nFileSizeLow << std::endl;
				std::cout << "Number Of Links: " << fileInfo.nNumberOfLinks << std::endl;
				std::cout << "File Index High: " << fileInfo.nFileIndexHigh << std::endl;
				std::cout << "File Index Low: " << fileInfo.nFileIndexLow << std::endl;
			}
		}
		CloseHandle(hFile);
	}

	void FileManager::PrintFileTime(std::filesystem::path path)
	{
		HANDLE hFile = CreateFileA(path.string().c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			FILETIME ftCreationTime;
			FILETIME ftLastAccessTime;
			FILETIME ftLastWriteTime;

			GetFileTime(hFile, &ftCreationTime, &ftLastAccessTime, &ftLastWriteTime);

			SYSTEMTIME stCreationTime;
			SYSTEMTIME stLastAccessTime;
			SYSTEMTIME stLastWriteTime;

			FileTimeToSystemTime(&ftCreationTime, &stCreationTime);
			FileTimeToSystemTime(&ftLastAccessTime, &stLastAccessTime);
			FileTimeToSystemTime(&ftLastWriteTime, &stLastWriteTime);

			std::cout << "Creation time: " << stCreationTime.wDay << "." << stCreationTime.wMonth << "." << stCreationTime.wYear << " " << stCreationTime.wHour << ":" << stCreationTime.wMinute << std::endl;
			std::cout << "Last Access time: " << stLastAccessTime.wDay << "." << stLastAccessTime.wMonth << "." << stLastAccessTime.wYear << " " << stLastAccessTime.wHour << ":" << stLastAccessTime.wMinute << std::endl;
			std::cout << "Last Write time: " << stLastWriteTime.wDay << "." << stLastWriteTime.wMonth << "." << stLastWriteTime.wYear << " " << stLastWriteTime.wHour << ":" << stLastWriteTime.wMinute << std::endl;
		}
		CloseHandle(hFile);
	}
	
	void TimetToFileTime(time_t t, LPFILETIME pft)
	{
		ULARGE_INTEGER time_value;
		time_value.QuadPart = (t * 10000000LL) + 116444736000000000LL;
		pft->dwLowDateTime = time_value.LowPart;
		pft->dwHighDateTime = time_value.HighPart;
	}

	int FileManager::SetFileCreationTime(std::filesystem::path path, time_t time)
	{
		if (time != -1)
		{
			FILETIME fTime;
			HANDLE hFile = CreateFileA(path.string().c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				TimetToFileTime(time, &fTime);

				if (SetFileTime(hFile, &fTime, NULL, NULL))
				{
					CloseHandle(hFile);
					return 0;
				}
				else
				{
					std::cout << GetLastError();
					CloseHandle(hFile);
					return 1;
				}
			}
			else
				return 1;
		}
		else
			return 1;
	}

	int FileManager::SetFileLastAccessTime(std::filesystem::path path, time_t time) 
	{
		if (time != -1)
		{
			FILETIME fTime;
			HANDLE hFile = CreateFileA(path.string().c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				TimetToFileTime(time, &fTime);

				if (SetFileTime(hFile, NULL, &fTime, NULL))
				{
					CloseHandle(hFile);
					return 0;
				}
				else
				{
					std::cout << GetLastError();
					CloseHandle(hFile);
					return 1;
				}
			}
			else
				return 1;
		}
		else
			return 1;
	}

	int FileManager::SetFileLastWriteTime(std::filesystem::path path, time_t time) 
	{
		if (time != -1)
		{
			FILETIME fTime;
			HANDLE hFile = CreateFileA(path.string().c_str(), 0x00, 0x00, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile != INVALID_HANDLE_VALUE)
			{
				TimetToFileTime(time, &fTime);

				if (SetFileTime(hFile, NULL, NULL, &fTime))
				{
					CloseHandle(hFile);
					return 0;
				}
				else
				{
					std::cout << GetLastError();
					CloseHandle(hFile);
					return 1;
				}
			}
			else
				return 1;
		}
		else
			return 1;
	}
}