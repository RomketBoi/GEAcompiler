# File: tests/CMakeLists.txt
# Project: GEA_Compiler
# Created Date: 10/28/2023
# Author: Luke Houston
# -----
# Last Modified: 10/28/2023
# Modified By: Luke Houston
# -----
# License: MIT License
# -----
# Description: CMake file for the tests directory

# Set test source files
set(TEST_SOURCES
    test1.cpp
)

# Create the test executable
add_executable(gea_test ${TEST_SOURCES})

# Link the test executable to the GEA library
target_link_libraries(gea_test PRIVATE gea_compiler_lib)

# Add the test executable to the test suite
add_test(NAME gea_test COMMAND gea_test)
