#pragma once

#define DLL_EXPORT_API __declspec(dllexport)

#include <string>

extern "C"
{
	DLL_EXPORT_API int ScaleSingleImage(const char* inputPath, const char* outputPath, const char* algorithm);

	DLL_EXPORT_API int ConvertAllInFolder(const char* inputPath, const char* outputPath, const char* algo);
}