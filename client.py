from ctypes import *

encoder_path = 'C:/Users/dell/source/repos/ImageCaptioning-DLL/data/encoder.onnx'
decoder_path = 'C:/Users/dell/source/repos/ImageCaptioning-DLL/data/decoder.onnx'
vocab_file_path = 'C:/Users/dell/source/repos/ImageCaptioning-DLL/data/vocab.txt'
image_path = 'C:/Users/dell/source/repos/ImageCaptioning-DLL/test.jpg'

# load dependant DLLs
_ = CDLL("C:/Users/dell/source/repos/ImageCaptioning-DLL/Release/opencv_core430.dll")
_ = CDLL("C:/Users/dell/source/repos/ImageCaptioning-DLL/Release/opencv_imgproc430.dll")
_ = CDLL("C:/Users/dell/source/repos/ImageCaptioning-DLL/Release/opencv_imgcodecs430.dll")
_ = CDLL("C:/Users/dell/source/repos/ImageCaptioning-DLL/Release/onnxruntime.dll")

# load required DLL
lib = CDLL("C:/Users/dell/source/repos/ImageCaptioning-DLL/Release/ImageCaptioning-DLL.dll")

# define return type and arguments of 'doDetection' function
lib.doDetection.restype = c_int
lib.doDetection.argtypes = [c_wchar_p, c_wchar_p, c_char_p, c_char_p]

# call 'doDetection' function and get length of result
res = lib.doDetection(c_wchar_p(encoder_path), c_wchar_p(decoder_path), c_char_p(vocab_file_path.encode('utf-8')),
					c_char_p(image_path.encode('utf-8')))

# continue if there is a result
if res != 0:
	# define return type and arguments of 'getCaption' function
	lib.getCaption.restype = c_int
	lib.getCaption.argtypes = [c_char_p, c_size_t]

	# define string buffer to store caption and call 'getCaption' function
	caption = create_string_buffer(res)
	size = lib.getCaption(caption, res)

	# get only relevant portion of byte string and decode it
	string_caption = (caption.raw[:size - 1]).decode('utf-8')
	print(string_caption)
