#include <string>
#include <filesystem>
#include <Windows.h>
#include "export.h"

#include <iostream>

// Function found on https://stackoverflow.com/a/20847429
std::vector<std::string> get_all_files_names_within_folder(const std::string& folder)
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
int ConvertAllInFolder(const char* inputPath, const char* outputPath, char* algo)
{
	auto files = get_all_files_names_within_folder(inputPath);

	for (auto & curFile : files)
	{
		std::string inFilePath = inputPath + curFile;
		std::string outFilePath = outputPath + curFile;

		auto inFilePath_cStr = inFilePath.c_str();
		auto outFilePath_cStr = outFilePath.c_str();

		ScaleSingleImage(inFilePath_cStr, outFilePath_cStr, algo);
	}

	return 0;
}

int main(int argc, char** argv)
{
	const int WORK_MODE_INDEX = 1;
	const int INPUT_PATH_INDEX = 2;
	const int OUTPUT_PATH_INDEX = 3;
	const int ALGORITHM_INDEX = 4;

	const int NUM_INPUT_ARGS = 5;

	if (argc != NUM_INPUT_ARGS)
	{
		printf("Usage: %s [file/folder] [inputFile/inputFolder/] [outputFile/outputFolder/] [xbr2x|xbr3x|xbr4x|hq2x|hq3x|hq4x]\n", argv[0]);
		return EXIT_FAILURE;
	}
	
	int result = -1; 

	if (strcmp(argv[WORK_MODE_INDEX], "folder") == 0)
	{
		result = ConvertAllInFolder(argv[INPUT_PATH_INDEX], argv[OUTPUT_PATH_INDEX], argv[ALGORITHM_INDEX]);
	}

	else if (strcmp(argv[WORK_MODE_INDEX], "file") == 0)
	{
		result = ScaleSingleImage(argv[INPUT_PATH_INDEX], argv[OUTPUT_PATH_INDEX], argv[ALGORITHM_INDEX]);
	}

	return result;
}