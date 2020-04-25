#include "export.h"
#include "lodepng.h"
#include "filters.h"
#include <iostream>
#include <Windows.h>

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

int ScaleSingleImage(const char* inputPath, const char* outputPath, const char* algorithm)
{
	int scaleType, scaleFactor;

	if (strcmp(algorithm, "xbr2x") == 0)
	{
		scaleType = 1;
		scaleFactor = 2;
	}
	else if (strcmp(algorithm, "xbr3x") == 0)
	{
		scaleType = 1;
		scaleFactor = 3;
	}
	else if (strcmp(algorithm, "xbr4x") == 0)
	{
		scaleType = 1;
		scaleFactor = 4;
	}
	else if (strcmp(algorithm, "hq2x") == 0)
	{
		scaleType = 2;
		scaleFactor = 2;
	}
	else if (strcmp(algorithm, "hq3x") == 0)
	{
		scaleType = 2;
		scaleFactor = 3;
	}
	else if (strcmp(algorithm, "hq4x") == 0)
	{
		scaleType = 2;
		scaleFactor = 4;
	}
	else
	{
		printf("Unrecognized scale mode\n");
		return EXIT_FAILURE;
	}

	std::vector<uint8_t> imageData;
	unsigned width;
	unsigned height;

	unsigned error = lodepng::decode(imageData, width, height, inputPath);

	if (error != 0)
	{
		std::cout << "Error " << error << " loading " << inputPath << ": " << lodepng_error_text(error) << std::endl;
		return EXIT_FAILURE;
	}

	unsigned inSize = width * height * 4;
	unsigned outSize = inSize * scaleFactor * scaleFactor;

	uint8_t* outBuffer = new uint8_t[outSize];
	xbr_data* xbrData = new xbr_data();
	xbr_params xbrParams;

	uint8_t * inBuffer = &imageData[0];

	xbr_init_data(xbrData);

	xbrParams.data = xbrData;
	xbrParams.input = inBuffer;
	xbrParams.output = outBuffer;
	xbrParams.inWidth = width;
	xbrParams.inHeight = height;
	xbrParams.inPitch = width * 4;
	xbrParams.outPitch = width * scaleFactor * 4;

	if (scaleType == 1)
	{
		switch (scaleFactor)
		{
		case 2: xbr_filter_xbr2x(&xbrParams);
			break;
		case 3: xbr_filter_xbr3x(&xbrParams);
			break;
		case 4: xbr_filter_xbr4x(&xbrParams);
			break;
		}
	}
	else if (scaleType == 2)
	{
		switch (scaleFactor)
		{
		case 2: xbr_filter_hq2x(&xbrParams);
			break;
		case 3: xbr_filter_hq3x(&xbrParams);
			break;
		case 4: xbr_filter_hq4x(&xbrParams);
			break;
		}
	}

	std::vector<uint8_t> encoded;

	error = lodepng::encode(encoded, outBuffer, width * scaleFactor, height * scaleFactor);

	if (error != 0)
	{
		std::cout << "Error" << error << " encoding " << inputPath << ": " << lodepng_error_text(error) << std::endl;
		return EXIT_FAILURE;
	}

	error = lodepng::save_file(encoded, outputPath);

	if (error != 0)
	{
		std::cout << "Error" << error << " saving file " << inputPath << ": " << lodepng_error_text(error) << std::endl;
		return EXIT_FAILURE;
	}

	delete[] outBuffer;
	delete xbrData;

	return 0;
}

// Converts all given .png files in the given inputFolder (no recursion) and outputs them at outputFolder with the same name
int ConvertAllInFolder(const char* inputPath, const char* outputPath, const char* algo)
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