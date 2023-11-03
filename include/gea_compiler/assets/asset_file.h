#pragma once

#include <string>
#include <vector>
#include <fstream>

struct AssetFile
{
    char type[4];           // e.g. "MESH"
    uint32_t version[3];    // [major, minor, patch]
    std::string json;       // JSON data
    std::vector<char> data; // Binary data
};

enum class CompressionMode : uint32_t
{
    None = 0,
    LZ4,
    Count
};

bool save_assetfile(std::string path, const AssetFile& assetFile)
{
    std::ofstream file(path, std::ios::binary | std::ios::out);
    if (!file.is_open()) return false;

    // 4 characters to represent the type of file (e.g. "MESH")
    file.write(assetFile.type, sizeof(assetFile.type));

    // 3 `uint32_t`s to represent the version of the file (e.g. 1.0.0)
    file.write(reinterpret_cast<const char*>(&assetFile.version), sizeof(assetFile.version));

    // size of the JSON data
    uint32_t jsonSize = static_cast<uint32_t>(assetFile.json.size());
    file.write(reinterpret_cast<const char*>(&jsonSize), sizeof(jsonSize));

    // JSON data
    file.write(assetFile.json.c_str(), jsonSize);

    // size of the binary data
    uint32_t dataSize = static_cast<uint32_t>(assetFile.data.size());
    file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));

    // binary data
    file.write(assetFile.data.data(), dataSize);

    return 0;
}