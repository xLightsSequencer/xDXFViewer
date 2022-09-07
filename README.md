### xDXFViewer

This tool to view DXF files from xLights.

### Building
Uses C++20, QT 5.15, dxflib, spdlog, and cMake.

```git clone https://github.com/xLightsSequencer/xDXFViewer.git```

To build on Windows, run bat, open sln in cmake_vs folder with Visual Studio 2022

```VS2022.bat```

If you get a qt cmake error, update the QT location in batch file.

To build on Linux with g++(tested on Mint Linux 21).

```
mkdir build
cd build
cmake ..
cmake --build ..
./xDXFViewer
```
