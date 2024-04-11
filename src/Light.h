//
// Created by William-Alexandre Messier on 2024-04-10.
//

#ifndef CAROPENGL_LIGHT_H
#define CAROPENGL_LIGHT_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Light{
public:
    bool lightsOn;
    glm::vec3 position;
    glm::vec3 color;
    Light(){

    }
};

#endif //CAROPENGL_LIGHT_H
