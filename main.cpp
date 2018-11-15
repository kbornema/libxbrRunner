#include <string>
#include <filesystem>
#include <Windows.h>

#include "export.h"

// Function found on https://stackoverflow.com/a/20847429
std::vector<std::string> get_all_files_names_within_folder(std::string folder)
{
	std::vector<std::string> names;
	std::string search_path = folder + "/*.png";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) 
	{
		do 
		{
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
				names.push_back(fd.cFileName);
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

// Converts all given .png files in the given inputFolder (no recursion) and outputs them at outputFolder with the same name
int ConvertAllInFolder(char* inputPath, char* outputPath, char* algo)
{
	auto files = get_all_files_names_within_folder(inputPath);

	for (auto & curFile : files)
	{
		std::string inFilePath = inputPath + curFile;
		std::string outFilePath = outputPath + curFile;
		ScaleSingleImage(inputPath, outputPath, algo);
	}

	return 0;
}

int main(int argc, char** argv)
{	
	const int INPUT_PATH_INDEX = 1;
	const int OUTPUT_PATH_INDEX = 2;
	const int ALGORITHM_INDEX = 3;
	return ScaleSingleImage(argv[INPUT_PATH_INDEX], argv[OUTPUT_PATH_INDEX], argv[ALGORITHM_INDEX]);
}