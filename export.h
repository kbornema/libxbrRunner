#pragma once

#define DLL_EXPORT_API __declspec(dllexport)

#include <string>

extern "C"
{
	DLL_EXPORT_API int ScaleSingleImage(char* inputPath, char* outputPath, char* algorithm);
}