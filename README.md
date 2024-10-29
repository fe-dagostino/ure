[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language](https://img.shields.io/badge/language-c++-red.svg)](https://en.cppreference.com/)
[![gcc & clang](https://github.com/fe-dagostino/ure/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=master)](https://github.com/fe-dagostino/ure/actions/workflows/cmake-multi-platform.yml)
[![emscripten](https://github.com/fe-dagostino/ure/actions/workflows/cmake-emsdk.yml/badge.svg?branch=master)](https://github.com/fe-dagostino/ure/actions/workflows/cmake-emsdk.yml)


# ure
Underlying Rendering Engine

## Build on Linux

In order to build under linux there are some system dependency that should be installed.

As first, please make sure that you have at least support for C++20 either for GCC or CLang.

Following installation and configuration for gcc and g++ on debian based system.
```
sudo apt-get update
sudo apt install -y gcc-12 g++-12 
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-12 100 --slave /usr/bin/g++ g++ /usr/bin/g++-12 --slave /usr/bin/gcov gcov /usr/bin/gcov-12
sudo update-alternatives --set gcc /usr/bin/gcc-12
```

You also need `cmake` and optionally `ninja` 
```
sudo apt install -y cmake ninja-build 
```

Then some libraries used by ure, all other libraries are fetched directly by cmake.

```
sudo apt install -y libfreetype-dev libcurlpp-dev
sudo apt install -y libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libxxf86vm-dev libxkbcommon-dev
sudo apt install -y libwayland-dev libwayland-bin
```

So once you system is updated, we can clone and build:

```
git clone https://github.com/fe-dagostino/ure.git
cd ure
mkdir build
cd build
cmake ..
make -j16
```

By default both static and dynamic library are produced.
