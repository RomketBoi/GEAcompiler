/**
 * File: main.cpp
 * Project: GEA_Compiler
 * File Created: 10/28/2023
 * Author: Luke Houston
 * -----
 * Last Modified: 10/28/2023
 * Modified By: Luke Houston
 * -----
 * License: MIT License
 * -----
 * Description: Main file for the GEA_Compiler
 */

#include <gea_compiler/scan_dir.h>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <directory>" << std::endl;
        return 1;
    }

    scan_dir(argv[1]);

    return 0;
}