cmake_minimum_required(VERSION 3.31.2)
project(CPPLox)

include(CTest)
enable_testing()

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src sourceFiles)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/include includeFiles)
add_executable(CPPLox main.cpp ${includeFiles} ${sourceFiles})

set_property(TARGET CPPLox PROPERTY CXX_STANDARD 23)

