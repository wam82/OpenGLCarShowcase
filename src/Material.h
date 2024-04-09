//
// Created by William-Alexandre Messier on 2024-04-07.
//

#ifndef CAROPENGL_MATERIAL_H
#define CAROPENGL_MATERIAL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Material{
public:
    bool isIridescent;
    bool isCarbon;
    glm::vec3 color;
    float ka, kd, ks, shininess, opacity;

    Material(){

    }
};

#endif //CAROPENGL_MATERIAL_H
