#pragma once
#include "framework.h"
#include "pch.h"
#include<string>

#ifdef IMAGECAPTIONINGDLL_EXPORTS
#define IMAGECAPTIONING_API __declspec(dllexport)
#else
#define IMAGECAPTIONING_API __declspec(dllimport)
#endif

extern "C" IMAGECAPTIONING_API const char* getCaption(IN wchar_t* encoder_path, IN wchar_t* decoder_path, IN char* vocab_file_path, IN char* image_path);