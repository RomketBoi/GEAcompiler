#include <gea_compiler/scan_dir.h>

#include <gea_compiler/convert.h>
#include <iostream>
#include <filesystem>

void scan_dir(const char* dir)
{
    std::filesystem::path path(dir);

    std::cout << "Scanning assets directory: " << path << std::endl;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.is_directory()) continue;

        std::cout << "Found file: " << entry.path() << std::endl;

        if (entry.path().extension() == ".gea")
        {
            std::cout << "File already has .gea extension, nothing to do" << std::endl;
        }
        else if (entry.path().extension() == ".obj")
        {
            std::cout << "File is a model" << std::endl;

            auto new_path = entry.path();
            new_path.replace_extension(".gea");
            convert_mesh(entry.path(), new_path);
        }
        else if (entry.path().extension() == ".png")
        {
            std::cout << "File is a texture" << std::endl;

            auto new_path = entry.path();
            new_path.replace_extension(".gea");
            convert_texture(entry.path(), new_path);
        }
        else
        {
            std::cout << "File is an invalid type" << std::endl;
        }
    }
}