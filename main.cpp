#include <iostream>
#include "lodepng.h"
#include "filters.h"
#include <string>
#include <iostream>
#include <filesystem>

#include <Windows.h>

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

int main(int argc, char** argv)
{	
	if (argc != 4) 
	{
		printf("Usage: %s [inputFolder/] [outputFolder/] [xbr2x|xbr3x|xbr4x|hq2x|hq3x|hq4x]\n", argv[0]);
		return EXIT_FAILURE;
	}

	int scaleType, scaleFactor;

	if (strcmp(argv[3], "xbr2x") == 0) 
	{
		scaleType = 1; 
		scaleFactor = 2;
	}
	else if (strcmp(argv[3], "xbr3x") == 0) 
	{
		scaleType = 1; 
		scaleFactor = 3;
	}
	else if (strcmp(argv[3], "xbr4x") == 0) 
	{
		scaleType = 1; 
		scaleFactor = 4;
	}
	else if (strcmp(argv[3], "hq2x") == 0) 
	{
		scaleType = 2; 
		scaleFactor = 2;
	}
	else if (strcmp(argv[3], "hq3x") == 0) 
	{
		scaleType = 2; 
		scaleFactor = 3;
	}
	else if (strcmp(argv[3], "hq4x") == 0) 
	{
		scaleType = 2; 
		scaleFactor = 4;
	}
	else 
	{
		printf("Unrecognized scale mode\n");
		return EXIT_FAILURE;
	}

	auto files = get_all_files_names_within_folder(argv[1]);

	for (auto & curFile : files)
	{
		std::vector<uint8_t> imageData;
		unsigned width;
		unsigned height;

		std::string inFilePath = argv[1] + curFile;
		std::string outFilePath = argv[2] + curFile;

		unsigned error = lodepng::decode(imageData, width, height, inFilePath.c_str());

		if (error != 0)
		{
			std::cout << "Error " << error << " loading "  << inFilePath << ": " << lodepng_error_text(error) << std::endl;
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
			std::cout << "Error" << error << " encoding " << inFilePath <<  ": " << lodepng_error_text(error) << std::endl;
			return EXIT_FAILURE;
		}
		
		error = lodepng::save_file(encoded, outFilePath.c_str());
		
		if (error != 0)
		{
			std::cout << "Error" << error << " saving file " << inFilePath <<  ": " << lodepng_error_text(error) << std::endl;
			return EXIT_FAILURE;
		}
		
		delete[] outBuffer;
		delete xbrData;
	}

	return 0;
}