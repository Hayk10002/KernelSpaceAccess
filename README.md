# KernelSpaceAccess

## Table of Contents
- [Introduction](#introduction)
- [Build and Run](#build-and-run)
- [How does this work](#how-does-this-work)

## Introduction
This program estimates the memory regions allocated for local, global initialized, and global uninitialized variables. 

## Build and Run
To clone and run this project, you'll need [Git](https://git-scm.com) and [CMake](https://cmake.org/) installed on your computer. From your command line:

```bash
# Clone this repository
$ git clone https://github.com/Hayk10002/KernelSpaceAccess

# Go into the repository
$ cd KernelSpaceAccess

# Generate the build files
# Use debug to not let compiler optimize the allocations
$ cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build

# Build the project
$ cmake --build build --config Debug

# Run the executable with the path to the target file as an argument.
$ your/path/to/exe/main.exe
# example - .../main.exe
```

## How does this work

This program tries to estimate the memory regions by starting from somewhere in the middle of the regions, and then tries accessing the memory going to the start and end of the regions. When it fails, that means that the start of the end of the region is reached.