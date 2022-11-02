# c-httpie
Express-inspired http server written in C

# Features
* Routing system
* Supports query parameters parsing
* Supports heap-allocated and static response data
* Single executable, cross-platform, zero dependencies, less than 512kb of idle RAM usage
* Use as static library or copy-paste files in your project

# Usage
* Run at default address 127.0.0.1:8080
```console
c-httpie
```
* Run at user-defined address
```console
c-httpie -a 0.0.0.0 -p 12345
```
* More information
```console
c-httpie --help
```

# Build the single perceptron shape guesser demo

* Windows + MSVC:
```console
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --target demo --config Release
```

* Linux + GCC:
```console
mkdir build
cd build
cmake .. -G "Unix Makefiles"
cmake --build . --target demo --config Release
```

* Guesser demo running in browser
<p align="center">
  <img height="auto" align="center" src="perceptron-demo.png">
</p>

# TODO
1. Multithread pool support
2. Configuration via environment variables
