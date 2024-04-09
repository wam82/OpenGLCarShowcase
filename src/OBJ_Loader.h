//
// Created by William-Alexandre Messier on 2024-04-04.
//

#ifndef CAROPENGL_OBJ_LOADER_H
#define CAROPENGL_OBJ_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "Material.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

using namespace std;

struct Vertex{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
};

struct carPart {
    vector<Vertex> vertices;
    string name;
    Material material;

    glm::mat4 modelMatrix = glm::mat4(1.f);
    GLuint VAO;
    GLuint VBO;
};


static vector<carPart> loadModel(const string path)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        std::vector<carPart> carParts;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str())) {
            throw std::runtime_error(warn + err);
        }

        for(tinyobj::shape_t shape: shapes){
            cout<<shape.name<<endl;
            carPart currentPart;
            currentPart.name = shape.name;
            for(tinyobj::index_t index : shape.mesh.indices){
                Vertex currentVertex;
                currentVertex.position = {
                        attrib.vertices[3 * index.vertex_index + 0],
                        attrib.vertices[3 * index.vertex_index + 1],
                        attrib.vertices[3 * index.vertex_index + 2]
                };
                currentVertex.normal = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                };
                currentPart.vertices.push_back(currentVertex);
            }
            carParts.push_back(currentPart);
        }
        return carParts;
};



#endif //CAROPENGL_OBJ_LOADER_H
