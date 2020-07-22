#include <iostream>
#include <ImageCaptioning-DLL/ImageCaptioningLib.h>

int main()
{
    wchar_t encoder_path[] = L"C:/Users/dell/source/repos/ImageCaptioning-DLL/data/encoder.onnx";
    wchar_t decoder_path[] = L"C:/Users/dell/source/repos/ImageCaptioning-DLL/data/decoder.onnx";
    char vocab_file_path[] = "C:/Users/dell/source/repos/ImageCaptioning-DLL/data/vocab.txt";
    char image_path[] = "C:/Users/dell/source/repos/ImageCaptioning-DLL/4.jpg";

    int result = doDetection(encoder_path, decoder_path, vocab_file_path, image_path);
    char* caption = (char*) malloc(sizeof(char) * result);
    int size = getCaption(caption, result);
    std::cout << caption << std::endl;
    free(caption);
}