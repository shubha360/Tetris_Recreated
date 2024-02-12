#include "ImageLoader.h"

ImageLoader::ImageLoader() {
    //stbi_set_flip_vertically_on_load(1);
}

void ImageLoader::loadTextureFromImage32(const std::string& imagePath, TextureData& texture) {

    auto it = m_textureCache.find(imagePath);

    if (it != m_textureCache.end()) { // Found in the cache
        texture = it->second;
    }
    else { // Not found in the cache

        freeTexture(texture);
        deleteTexture(texture);

        texture.path = imagePath;
        texture.bitsPerPixel = 4;

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
        m_textureCache.insert(make_pair(imagePath, texture));
    }    
}

void ImageLoader::bufferTextureData(TextureData& texture) {

    if (texture.id != 0) {
        printf("Error at bufferTextureData() in ImageLoader.cpp\n"
            "Texture data is already buffered!\n");
    }

    if (texture.data != nullptr) {
        glGenTextures(1, &texture.id);

        glBindTexture(GL_TEXTURE_2D, texture.id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height,
            0, GL_RGBA, GL_UNSIGNED_BYTE, &texture.data[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    else {
        printf("Error at bufferTextureData() in ImageLoader.cpp\n"
            "Texture has no pixel data!\n");
    }
}

void ImageLoader::freeTexture(TextureData& texture) {
    if (texture.data != nullptr) {
        stbi_image_free(texture.data);
        texture.data = nullptr;
    }
}

void ImageLoader::deleteTexture(TextureData& texture) {
    if (texture.id != 0) {
        glDeleteTextures(1, &texture.id);
        texture.id = 0;
    }
}
