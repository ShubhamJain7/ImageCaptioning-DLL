# ImageCaptioning-DLL

This repository is a part of my [GSoC 2020 project](https://summerofcode.withgoogle.com/projects/#6039693356957696) and the DLL generated is used by the [Image Captioning NVDA Add-on](https://github.com/ShubhamJain7/imageCaptioning-NVDA-Addon/).
Image captioning is performed using a variation of the Say-Look-Tell model found [here](https://github.com/yunjey/pytorch-tutorial/tree/master/tutorials/03-advanced/image_captioning). This model was converted to ONNX (explained below) and run using [ONNX Runtime](https://github.com/microsoft/onnxruntime). 

This repo consists of a Visual Studio Solution that contains 2 projects:
1. __ImageCaptioning-DLL__: A Microsoft Windows DLL that runs inference on the ONNX model and returns the caption generated for the input image.
2. __ImageCaptioningConsole__: A Simple console application that interfaces with the _ImageCaptioning-DLL_ to perform image captioning on a sample input image.

The `Converter.ipynb` file contains the code to convert the PyTorch models to ONNX and also the vocab file to the format used by this DLL project.

The repository also includes a python client, `client.py` that uses Cython to run a sample image captioning process on a test image, much like the _ImageCaptioningConsole_ project.

## How to Build and Run
----
1. Clone this repository
2. Create the folders `x86/vc16/Debug` and `x86/vc16/Release` in the `opencv` directory.
3. Download [OpenCV](https://opencv.org/releases/) (version 4.3 was used for this project) and build it by following the steps in this [tutorial](https://hackeradam.com/installing-opencv-4-on-windows-10/). Make sure to select the option to build a 32-bit library in Step 4 of the tutorial.
4. Create `lib` directories in the `Debug` & `Relese` folders and copy the library files for the following modules in the corresponding directories:
    - opencv_core
    - opencv_imgcodecs
    - opencv_imgproc
(debug versions end with a _d_)
5. ONNX Runtimex86-1.3.0 is already included in this repository but you can also download it from [here](https://github.com/microsoft/onnxruntime/releases/download/v1.3.0/onnxruntime-win-x86-1.3.0.zip) and extract it to the root of this repository.
6. Open the __ImageCaptioning-DLL.sln__ file in Visual Studio.
7. Open the properties menu of the __ImageCaptioning-DLL__ project and select _C/C++ -> General_. Add `$(SolutionDir)opencv\include` and `$(SolutionDir)onnxruntime-win-x86-1.3.0\include` to the _Additional include directories_.
8. In the properties menu, select _Linker -> General_ and add `$(SolutionDir)\x86\vc16\Release\lib` or `$(SolutionDir)\x86\vc16\Debug\lib` (depending on your selected configuration) and `$(SolutionDir)onnxruntime-win-x86-1.3.0\lib` to _Additional Library Directories_.
9. Again, in the properties menu select _Linker -> Input_ and add the names of the library files to _Additional Dependencies_. (Names of the library files in the directory mentioned in the previous step. Mind the selected configuration here too, Debug libraries end with a _d_. You don't need to worry about the configuration for the ONNX library).
10. Use the `Converter.ipynb` file to convert the model and vocab files and copy the output files (`encoder.onnx`, `decoder.onnx`, `vocab.txt`) to the `data` directory.
11. You can now build the __ImageCaptioning-DLL__ project by selecting the _Build Solution_ option in the build menu of Visual Studio or just press `ctrl+shift+b`. Depending on the selected build configuration, a Debug or Release directory with the  built DLL will be created in the root directory of the repository.
12. Copy the DLLs for the OpenCV modules mentioned in step 4 along with `onnxruntime-win-x86-1.3.0\lib\onnxruntime.dll` to the Release or Debug directory created in the last step.
13. Build and run the __ImageCaptioningConsole__ project to test the model by pressing `ctrl+f5`. Alternatively, you can run `python client.py` (only works with Release builds). Your output should (ignoring all the warnings) look like:
```
<start> a red double decker bus driving down a street . <end>
```

If you've done everything right, the file structure of this repo should look something like:
```
C:.
│   .gitignore
│   client.py
│   Converter.ipynb
│   ImageCaptioning-DLL.sln
│   LICENSE
│   readme.md
│   test.jpg
│
├───data
│       decoder.onnx
│       encoder.onnx
│       vocab.txt
│
├───Debug
│       ImageCaptioning-DLL.dll
│       ImageCaptioning-DLL.exp
│       ImageCaptioning-DLL.ilk
│       ImageCaptioning-DLL.lib
│       ImageCaptioning-DLL.pdb
│       ImageCaptioningConsole.exe
│       ImageCaptioningConsole.ilk
│       ImageCaptioningConsole.pdb
│       onnxruntime.dll
│       opencv_core430d.dll
│       opencv_imgcodecs430d.dll
│       opencv_imgproc430d.dll
│
├───ImageCaptioning-DLL
│   │   dllmain.cpp
│   │   framework.h
│   │   ImageCaptioning-DLL.vcxproj
│   │   ImageCaptioning-DLL.vcxproj.filters
│   │   ImageCaptioning-DLL.vcxproj.user
│   │   ImageCaptioningLib.h
│   │   internal.cpp
│   │   internal.h
│   │   pch.cpp
│   │   pch.h
│   │
│   ├───Debug
│   │   │   dllmain.obj
│   │   │   ImageCaptioning-DLL.log
│   │   │   ImageCaptioning-DLL.pch
│   │   │   internal.obj
│   │   │   pch.obj
│   │   │   vc142.idb
│   │   │   vc142.pdb
│   │   │
│   │   └───ImageCap.304E229E.tlog
│   │           CL.command.1.tlog
│   │           CL.read.1.tlog
│   │           CL.write.1.tlog
│   │           ImageCaptioning-DLL.lastbuildstate
│   │           ImageCaptioning-DLL.write.1u.tlog
│   │           link.11144.delete.1.tlog
│   │           link.5680.delete.1.tlog
│   │           link.command.1.tlog
│   │           link.read.1.tlog
│   │           link.write.1.tlog
│   │
│   └───Release
│       │   dllmain.obj
│       │   ImageCaptioning-DLL.log
│       │   ImageCaptioning-DLL.pch
│       │   internal.obj
│       │   pch.obj
│       │   vc142.pdb
│       │
│       └───ImageCap.304E229E.tlog
│               CL.command.1.tlog
│               CL.read.1.tlog
│               CL.write.1.tlog
│               ImageCaptioning-DLL.lastbuildstate
│               ImageCaptioning-DLL.write.1u.tlog
│               link.command.1.tlog
│               link.read.1.tlog
│               link.write.1.tlog
│
├───ImageCaptioningConsole
│   │   ImageCaptioningConsole.cpp
│   │   ImageCaptioningConsole.vcxproj
│   │   ImageCaptioningConsole.vcxproj.filters
│   │   ImageCaptioningConsole.vcxproj.user
│   │
│   ├───Debug
│   │   │   ImageCaptioningConsole.log
│   │   │   ImageCaptioningConsole.obj
│   │   │   vc142.idb
│   │   │   vc142.pdb
│   │   │
│   │   └───ImageCap.F422011C.tlog
│   │           CL.command.1.tlog
│   │           CL.read.1.tlog
│   │           CL.write.1.tlog
│   │           ImageCaptioningConsole.lastbuildstate
│   │           link.command.1.tlog
│   │           link.read.1.tlog
│   │           link.write.1.tlog
│   │
│   └───Release
│       │   ImageCaptioningConsole.log
│       │   ImageCaptioningConsole.obj
│       │   vc142.pdb
│       │
│       └───ImageCap.F422011C.tlog
│               CL.command.1.tlog
│               CL.read.1.tlog
│               CL.write.1.tlog
│               ImageCaptioningConsole.lastbuildstate
│               ImageCaptioningConsole.write.1u.tlog
│               link.command.1.tlog
│               link.read.1.tlog
│               link.write.1.tlog
│
├───onnxruntime-win-x86-1.3.0
│   │   C_API.md
│   │   GIT_COMMIT_ID
│   │   LICENSE
│   │   Privacy.md
│   │   README.md
│   │   ThirdPartyNotices.txt
│   │   VERSION_NUMBER
│   │
│   ├───include
│   │       cpu_provider_factory.h
│   │       cuda_provider_factory.h
│   │       onnxruntime_cxx_api.h
│   │       onnxruntime_cxx_inline.h
│   │       onnxruntime_c_api.h
│   │
│   └───lib
│           onnxruntime.dll
│           onnxruntime.lib
│
├───opencv
│   │   LICENSE
│   │   OpenCVConfig-version.cmake
│   │   OpenCVConfig.cmake
│   │   setup_vars_opencv4.cmd
│   │
│   └───x86
│       └───vc16
│           ├───Debug
│           │   └───lib
│           │           opencv_core430d.lib
│           │           opencv_imgcodecs430d.lib
│           │           opencv_imgproc430d.lib
│           │
│           └───Release
│               └───lib
│                       opencv_core430.lib
│                       opencv_imgcodecs430.lib
│                       opencv_imgproc430.lib
│
└───Release
        ImageCaptioning-DLL.dll
        ImageCaptioning-DLL.exp
        ImageCaptioning-DLL.iobj
        ImageCaptioning-DLL.ipdb
        ImageCaptioning-DLL.lib
        ImageCaptioning-DLL.pdb
        ImageCaptioningConsole.exe
        ImageCaptioningConsole.iobj
        ImageCaptioningConsole.ipdb
        ImageCaptioningConsole.pdb
        onnxruntime.dll
        opencv_core430.dll
        opencv_imgcodecs430.dll
        opencv_imgproc430.dll
```
(`opencv\include` and `opencv\etc` not included)