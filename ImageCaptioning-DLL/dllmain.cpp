// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "internal.h"
#include "ImageCaptioningLib.h"
#include<string>
#include<iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

std::string result;

int doDetection(wchar_t* encoder_path, wchar_t* decoder_path, char* vocab_file_path, char* image_path) {
    result = _getCaption(encoder_path, decoder_path, vocab_file_path, image_path);
    return result.length();
}

int getCaption(char* caption, size_t size) {
    strcpy_s(caption, size, result.c_str());
    return strlen(caption);
}
