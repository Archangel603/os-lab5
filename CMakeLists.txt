cmake_minimum_required(VERSION 3.20)
project(lab5)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab5 main.cpp VFS.cpp VFS.h DirectAccessor.cpp DirectAccessor.h ExternalAccessor.cpp ExternalAccessor.h File.h Mount.h FileAccessor.h utils.h utils.cpp)
