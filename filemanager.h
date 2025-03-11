#pragma once
#include <windows.h>
#include <iostream>
#include <iostream>
#include <filesystem>
#include <ctime>


namespace fManager
{
	class FileManager
	{
	public:
		static void PrintDrives();
		static void PrintFlags(DWORD flags);
		static void PrintDriveInfo(const std::string& driveName);
		static int CreateDir(std::filesystem::path path);
		static int RemoveDir(std::filesystem::path path);
		static int CrFile(std::filesystem::path path);
		static int CpyFile(std::filesystem::path efName, std::filesystem::path nfName);
		static int MvFile(std::filesystem::path src, std::filesystem::path dst);
		static void PrintFileAtts(std::filesystem::path path);
		static int SetFileAtts(std::filesystem::path path, DWORD atts);
		static void PrintFileInfo(std::filesystem::path path);
		static void PrintFileTime(std::filesystem::path path);
		static int SetFileCreationTime(std::filesystem::path path, time_t time);
		static int SetFileLastAccessTime(std::filesystem::path path, time_t time);
		static int SetFileLastWriteTime(std::filesystem::path path, time_t time);
	};
}