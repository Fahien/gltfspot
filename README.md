# GltfSpoT

GltfSpoT is a [glTF](https://github.com/KhronosGroup/glTF) loading library, developed as a module for [SunSpoT](https://github.com/Fahien/sunspot).

## Prerequisites

In order to build GltfSpoT, you will need the following dependencies:
- [CMake 3.9.+](https://cmake.org), Cross-Platform Make;
- [Python 3.6.+](https://www.python.org/downloads), header files and a static library.

## Build

Generate a project for your platform with cmake.
```bash
cmake -H. -Bbuild
```

Compile it.
```bash
cmake --build build --config Release
```

If you want, you can run the tests.
```bash
cd build
ctest -C Release -T test
```

## License

Apache License © 2017 [Antonio Caggiano](http://www.fahien.me)
