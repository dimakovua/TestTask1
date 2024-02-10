# TestTask1

## Description

TestTask1 is a sample project demonstrating a C++ application for downloading files from the internet and optionally executing downloaded files. It utilizes libcurl for handling HTTP requests and demonstrates modern C++ practices, including resource management and error handling.

## Features

- Download files from specified URLs.
- (Optional) Execute downloaded files, supporting various platforms.
- Utilize modern C++17/20 features for efficient and safe code.

## Prerequisites

Before you begin, ensure you have met the following requirements:
- A modern C++ compiler supporting C++17 or C++20 (e.g., GCC, Clang, MSVC).
- CMake (version 3.15 or higher) for building the project.
- libcurl installed on your system for handling HTTP requests.

## Setting Up TestTask1

To set up TestTask1, follow these steps:

```cmd
git clone https://github.com/dimakovua/TestTask1.git
cd TestTask1
mkdir build && cd build
cmake .. -G "Visual Studio 16 2019" # Adjust for your version of Visual Studio
cmake --build . --config Release
```

## Using TestTask1

To use TestTask1, follow these steps:

```bash
./TestTask1 <URL> <download_path> [executable_parameters]
```
- `<URL>`: URL of the file to download.
- `<download_path>`: Path where the downloaded file will be saved.
- `[executable_parameters]`: (Optional) Parameters to pass to the executable if it is run after download.

Example:
```bash
./TestTask1 https://example.com/file.zip /path/to/save/file.zip
```

