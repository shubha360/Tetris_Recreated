#include "ImageLoader.h"

std::map<std::string, TextureData> ImageLoader::m_textureCache;

void ImageLoader::LoadTextureFromImage(const std::string& imagePath, TextureData& texture, 
    const unsigned int colorChannels) {

    if (colorChannels != 1 && colorChannels != 4) {
        printf("Error at loadTextureFromImage32() in ImageLoader.cpp\n"
            "Invalid color channel %d\n", colorChannels);

        return;
    }

    auto it = m_textureCache.find(imagePath);

    if (it != m_textureCache.end()) { // Found in the cache
        texture = it->second;
        printf("Loaded from cache.\n\n");
    }
    else { // Not found in the cache

        FreeTexture(texture);
        DeleteTexture(texture);

        texture.path = imagePath;
        texture.bitsPerPixel = colorChannels;

        texture.data =
            stbi_load(
                imagePath.c_str(),
                &texture.width,
                &texture.height,
                &texture.bitsPerPixelInFile,
                texture.bitsPerPixel
            );

        if (texture.data == nullptr) {
            printf("Error at loadTextureFromImage32() in ImageLoader.cpp\n"
                "Failed to load image at %s\n", imagePath.c_str());
            texture.path = "";
            texture.bitsPerPixel = 0;
        }
        else {
            m_textureCache.insert(make_pair(imagePath, texture));
            printf("Loaded from file.\n\n");
        }
    }    
}

void ImageLoader::BufferTextureData(TextureData& texture) {

    if (texture.id != 0) {
        printf("Error at bufferTextureData() in ImageLoader.cpp\n"
            "Texture data is already buffered!\n");
        return;
    }

    if (texture.data == nullptr) {
        printf("Error at bufferTextureData() in ImageLoader.cpp\n"
            "Texture has no pixel data!\n");
        return;
    }

    glGenTextures(1, &texture.id);

    glBindTexture(GL_TEXTURE_2D, texture.id);

    GLint internalFormat = 0;
    GLenum pixelFormat = 0;

    switch (texture.bitsPerPixel) {
    case 1:
        internalFormat = GL_RED;
        pixelFormat = GL_RED;
        break;

    case 4:
        internalFormat = GL_RGBA8;
        pixelFormat = GL_RGBA;
        break;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, texture.width, texture.height,
        0, pixelFormat, GL_UNSIGNED_BYTE, &texture.data[0]);


    if (internalFormat == GL_RED && pixelFormat == GL_RED) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void ImageLoader::FreeTexture(TextureData& texture) {
    if (texture.data != nullptr) {
        stbi_image_free(texture.data);
        texture.data = nullptr;
    }
}

void ImageLoader::DeleteTexture(TextureData& texture) {
    if (texture.id != 0) {
        glDeleteTextures(1, &texture.id);
        texture.id = 0;
    }
}
