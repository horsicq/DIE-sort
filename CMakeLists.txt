cmake_minimum_required(VERSION 3.18)
project(DIESort)

option(X_ENABLE_ASAN "Build with AddressSanitizer" OFF)

add_subdirectory(src)
