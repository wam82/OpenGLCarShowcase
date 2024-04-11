//
// Created by William-Alexandre Messier on 2024-04-07.
//

#ifndef CAROPENGL_MATERIAL_H
#define CAROPENGL_MATERIAL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "stb_image.cpp"

static GLuint loadTexture(const char* path) {
    GLuint texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

class Material{
public:
    glm::vec3 color;
    float ka, kd, ks, shininess, opacity;
    GLuint texture;
    bool hasTexture = false;
    Material() {

    }
};

static vector<GLuint> createTextureVector(){
    GLuint t1 = loadTexture("../assets/Textures/iridescent.jpg");
    vector<GLuint> textures;
    textures.push_back(t1);
    return textures;
}

#endif //CAROPENGL_MATERIAL_H
