#pragma once

#include "asset_file.h"
#include "../lib/rapidjson/include/rapidjson/document.h"        // TODO: Fix this include path
#include "../lib/rapidjson/include/rapidjson/rapidjson.h"       // TODO: Fix this include path
#include "../lib/rapidjson/include/rapidjson/stringbuffer.h"    // TODO: Fix this include path
#include "../lib/rapidjson/include/rapidjson/writer.h"          // TODO: Fix this include path
#include <lz4.h>

enum class VertexFormat : uint32_t
{
    Unknown = 0,
    PositionColorNormalUV,
    PositionColorUV,
    PositionNormalUV,
    PositionUV,
    PositionColor,
    Position,
    Count
};

enum class IndexFormat : uint32_t
{
    Unknown = 0,
    UInt16,
    UInt32,
    Count
};

struct MeshInfo
{
    uint64_t vertexBufferSize;
    uint64_t indexBufferSize;
    uint32_t vertexCount;
    uint32_t indexCount;
    VertexFormat vertexFormat;
    IndexFormat indexFormat;
    CompressionMode compressionMode;
    std::string originalFile;
};

AssetFile pack_mesh(MeshInfo& meshInfo, char* vertexData, char* indexData)
{
    AssetFile assetFile;

    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("vertexBufferSize", meshInfo.vertexBufferSize, allocator);
    document.AddMember("indexBufferSize", meshInfo.indexBufferSize, allocator);
    document.AddMember("vertexCount", meshInfo.vertexCount, allocator);
    document.AddMember("indexCount", meshInfo.indexCount, allocator);
    document.AddMember("vertexFormat", static_cast<uint32_t>(meshInfo.vertexFormat), allocator);
    document.AddMember("indexFormat", static_cast<uint32_t>(meshInfo.indexFormat), allocator);
    document.AddMember("compressionMode", static_cast<uint32_t>(meshInfo.compressionMode), allocator);

    rapidjson::Value originalFile(meshInfo.originalFile.c_str(), allocator);
    document.AddMember("originalFile", originalFile, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    assetFile.type[0] = 'M';
    assetFile.type[1] = 'E';
    assetFile.type[2] = 'S';
    assetFile.type[3] = 'H';

    assetFile.version[0] = GEA_COMPILER_VERSION_MAJOR;
    assetFile.version[1] = GEA_COMPILER_VERSION_MINOR;
    assetFile.version[2] = GEA_COMPILER_VERSION_PATCH;

    assetFile.json = buffer.GetString();

    if (meshInfo.compressionMode == CompressionMode::LZ4)
    {
        int maxCompressedSize = LZ4_compressBound(meshInfo.vertexBufferSize + meshInfo.indexBufferSize);
        assetFile.data.resize(maxCompressedSize);

        int compressedSize = LZ4_compress_default(static_cast<const char*>(vertexData), assetFile.data.data(),                  meshInfo.vertexBufferSize, assetFile.data.size());
        compressedSize    += LZ4_compress_default(static_cast<const char*>(indexData),  assetFile.data.data() + compressedSize, meshInfo.indexBufferSize,  assetFile.data.size() - compressedSize);
        assetFile.data.resize(compressedSize);
    }
    else
    {
        assetFile.data.resize(meshInfo.vertexBufferSize + meshInfo.indexBufferSize);
        memcpy(assetFile.data.data(), vertexData, meshInfo.vertexBufferSize);
        memcpy(assetFile.data.data() + meshInfo.vertexBufferSize, indexData, meshInfo.indexBufferSize);
    }

    return assetFile;
}