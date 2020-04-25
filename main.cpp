#include <string>
#include <filesystem>
#include "export.h"

#include <iostream>

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