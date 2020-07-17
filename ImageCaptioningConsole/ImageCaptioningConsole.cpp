#include <iostream>
#include <ImageCaptioning-DLL/ImageCaptioningLib.h>

int main()
{
    wchar_t encoder_path[] = L"C:/Users/dell/source/repos/ImageCaptioning/data/encoder.onnx";
    wchar_t decoder_path[] = L"C:/Users/dell/source/repos/ImageCaptioning/data/decoder.onnx";
    char vocab_file_path[] = "C:/Users/dell/source/repos/ImageCaptioning/data/vocab.txt";
    char image_path[] = "C:/Users/dell/source/repos/ImageCaptioning/test.jpg";

    const char* caption = new char[20];
    getCaption(encoder_path, decoder_path, vocab_file_path, image_path, caption);
    std::cout << caption;
}