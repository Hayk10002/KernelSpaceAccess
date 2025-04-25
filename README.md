# KernelSpaceAccess

## Table of Contents
- [Introduction](#introduction)
- [Build and Run](#build-and-run)

## Introduction
This program scans the memory segment starting from the start of the stack, ending at the end of the heap, trying to access the memory locations with a step, then it handles the segfaults or access violations to show what regions are accessible. 

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
