#pragma once

#include "asset_file.h"
#include "../lib/rapidjson/include/rapidjson/document.h"    // TODO: Fix this include path
#include "../lib/rapidjson/include/rapidjson/rapidjson.h"   // TODO: Fix this include path
#include <lz4.h>

enum class TextureFormat : uint32_t
{
    Unknown = 0,
    RGBA8,
    RGBA16,
    RGBA32,
    RGB8,
    RGB16,
    RGB32,
    RG8,
    RG16,
    RG32,
    R8,
    R16,
    R32,
    Depth16,
    Depth24,
    Depth32,
    Depth32F,
    Depth24Stencil8,
    Depth32FStencil8,
    Stencil8,
    BC1,
    BC2,
    BC3,
    BC4,
    BC5,
    BC6H,
    BC7,
    ETC1,
    ETC2,
    ETC2A,
    ETC2A1,
    PVRTC1_RGB2,
    PVRTC1_RGBA2,
    PVRTC1_RGB4,
    PVRTC1_RGBA4,
    PVRTC2_RGBA2,
    PVRTC2_RGBA4,
    ASTC_4x4,
    ASTC_5x4,
    ASTC_5x5,
    ASTC_6x5,
    ASTC_6x6,
    ASTC_8x5,
    ASTC_8x6,
    ASTC_8x8,
    ASTC_10x5,
    ASTC_10x6,
    ASTC_10x8,
    ASTC_10x10,
    ASTC_12x10,
    ASTC_12x12,
    Count
};

struct TextureInfo
{
    uint64_t textureSize;
    TextureFormat format;
    CompressionMode compressionMode;
    uint32_t pixelSize[3];
    std::string originalFile;
};

AssetFile pack_texture(TextureInfo& textureInfo, void* pixelData)
{
    AssetFile assetFile;

    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    document.AddMember("size", textureInfo.textureSize, allocator);
    document.AddMember("format", static_cast<uint32_t>(textureInfo.format), allocator);
    document.AddMember("compressionMode", static_cast<uint32_t>(textureInfo.compressionMode), allocator);

    rapidjson::Value pixelSize(rapidjson::kArrayType);
    pixelSize.PushBack(textureInfo.pixelSize[0], allocator);
    pixelSize.PushBack(textureInfo.pixelSize[1], allocator);
    pixelSize.PushBack(textureInfo.pixelSize[2], allocator);
    document.AddMember("pixelSize", pixelSize, allocator);

    rapidjson::Value originalFile(textureInfo.originalFile.c_str(), allocator);
    document.AddMember("originalFile", originalFile, allocator);

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    assetFile.type[0] = 'T';
    assetFile.type[1] = 'E';
    assetFile.type[2] = 'X';
    assetFile.type[3] = 'T';

    assetFile.version[0] = GEA_COMPILER_VERSION_MAJOR;
    assetFile.version[1] = GEA_COMPILER_VERSION_MINOR;
    assetFile.version[2] = GEA_COMPILER_VERSION_PATCH;

    assetFile.json = buffer.GetString();

    if (textureInfo.compressionMode == CompressionMode::LZ4)
    {
        int maxCompressedSize = LZ4_compressBound(textureInfo.textureSize);
        assetFile.data.resize(maxCompressedSize);

        int compressedSize = LZ4_compress_default(static_cast<const char*>(pixelData), assetFile.data.data(), textureInfo.textureSize, assetFile.data.size());
        assetFile.data.resize(compressedSize);
    }
    else
    {
        assetFile.data.resize(textureInfo.textureSize);
        memcpy(assetFile.data.data(), pixelData, textureInfo.textureSize);
    }

    return assetFile;
}