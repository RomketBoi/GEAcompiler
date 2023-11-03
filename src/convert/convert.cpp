#include <gea_compiler/convert.h>

#include <gea_compiler/assets/meshes.h>
#include <gea_compiler/assets/textures.h>
#include <gea_compiler/types.h>
#include <iostream>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb/stb_image.h"   // TODO: Fix this include path

#define TINYOBJLOADER_IMPLEMENTATION
#include "../lib/tinyobjloader/tiny_obj_loader.h"   // TODO: Fix this include path
#include <vector>

void convert_mesh(const std::filesystem::path& input, const std::filesystem::path& output)
{
    std::cout << "Converting mesh: " << input << " -> " << output << std::endl;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials; // TODO: support materials

    std::string warn;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, input.string().c_str()))
    {
        throw std::runtime_error(warn + err);
    }

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex;

            // Position
            vertex.position.x = attrib.vertices[3 * index.vertex_index + 0];
            vertex.position.y = attrib.vertices[3 * index.vertex_index + 1];
            vertex.position.z = attrib.vertices[3 * index.vertex_index + 2];

            // Color
            if (!attrib.colors.empty())
            {
                vertex.color.r = attrib.colors[3 * index.vertex_index + 0];
                vertex.color.g = attrib.colors[3 * index.vertex_index + 1];
                vertex.color.b = attrib.colors[3 * index.vertex_index + 2];
            }
            else
            {
                vertex.color.r = 1.0f;
                vertex.color.g = 1.0f;
                vertex.color.b = 1.0f;
            }

            // Normal
            if (!attrib.normals.empty())
            {
                vertex.normal.x = attrib.normals[3 * index.normal_index + 0];
                vertex.normal.y = attrib.normals[3 * index.normal_index + 1];
                vertex.normal.z = attrib.normals[3 * index.normal_index + 2];
            }
            else
            {
                vertex.normal.x = 0.0f;
                vertex.normal.y = 0.0f;
                vertex.normal.z = 0.0f;
            }

            // UV
            if (!attrib.texcoords.empty())
            {
                vertex.uv.x = attrib.texcoords[2 * index.texcoord_index + 0];
                vertex.uv.y = attrib.texcoords[2 * index.texcoord_index + 1];
            }
            else
            {
                vertex.uv.x = 0.0f;
                vertex.uv.y = 0.0f;
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    MeshInfo mesh_info;
    mesh_info.vertexBufferSize = vertices.size() * sizeof(Vertex);
    mesh_info.indexBufferSize = indices.size() * sizeof(uint32_t);
    mesh_info.vertexCount = vertices.size();
    mesh_info.indexCount = indices.size();
    mesh_info.vertexFormat = VertexFormat::PositionColorNormalUV;
    mesh_info.indexFormat = IndexFormat::UInt32;
    mesh_info.compressionMode = CompressionMode::LZ4;
    mesh_info.originalFile = input.filename().string();

    AssetFile asset_file = pack_mesh(mesh_info, reinterpret_cast<char*>(vertices.data()), reinterpret_cast<char*>(indices.data()));

    save_assetfile(output.string(), asset_file);
}

void convert_texture(const std::filesystem::path& input, const std::filesystem::path& output)
{
    std::cout << "Converting texture: " << input << " -> " << output << std::endl;

    int width, height, channels;
    stbi_uc* pixels = stbi_load(input.string().c_str(), &width, &height, &channels, STBI_rgb_alpha);

    if (!pixels)
    {
        throw std::runtime_error("Failed to load texture");
    }

    TextureInfo texture_info;
    texture_info.textureSize = width * height * 4;
    texture_info.format = TextureFormat::RGBA8;
    texture_info.compressionMode = CompressionMode::LZ4;
    texture_info.pixelSize[0] = width;
    texture_info.pixelSize[1] = height;
    texture_info.pixelSize[2] = 4;
    texture_info.originalFile = input.filename().string();

    AssetFile asset_file = pack_texture(texture_info, pixels);

    save_assetfile(output.string(), asset_file);

    stbi_image_free(pixels);
}