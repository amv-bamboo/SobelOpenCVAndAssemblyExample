# SobelOpenCVAndAssemblyExample

Edge detection using Sobel operator written in OpenCV and assembly (clang-GCC). macOS only.

## Setup

To launch the demo, you first need to setup the build environment.

```bash
xcode-select --install # if you don't have Xcode installed already
brew install opencv sse2neon cmake ninja
mkdir build
cd build
```

To build the app:

```bash
cmake ../
cmake --build .
```

To launch the app:

```bash
./build/main
```
