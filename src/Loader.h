//
// Created by William-Alexandre Messier on 2024-04-01.
//

#ifndef CAROPENGL_LOADER_H
#define CAROPENGL_LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

using namespace std;

struct Vertex{
    glm::vec3 position;
    glm::vec2 texture;
    glm::vec3 normal;
};

struct carPart {
    vector<Vertex> vertices;
    string name;
    glm::vec3 color;

    float ka, kd, ks, shininess, opacity;

    GLuint VAO;
    GLuint VBO;
};

static vector<carPart> loadOBJ(const char* fileName){
    vector<carPart> parts;

    vector<glm::fvec3> vertexPosition;
    vector<glm::fvec3> vertexNormal;
    vector<glm::fvec2> vertexTexture;

    vector<GLint> vertexPositionIndices;
    vector<GLint> vertexNormalIndices;
    vector<GLint> vertexTextureIndices;

    vector<Vertex> vertices;

    ifstream in_file(fileName);
    istringstream currentLine;
    string line = "";
    string token = "";
    glm::vec3 v3;
    glm::vec2 v2;
    GLint glint;
    string currentName;
    carPart currentPart;

    if(!in_file.is_open()){
        throw "Could not open obj file";
    }

    while(getline(in_file, line)){
        currentLine.clear();
        currentLine.str(line);
        currentLine >> token;

        if(token == "#"){

        }
        else if(token == "o"){
            if(!currentPart.name.empty()){
                parts.push_back(currentPart);
                cout<<currentPart.name<<" constructed."<<endl;
            }
            currentLine>>currentName;
            currentPart.name = currentName;
            cout<<"Constructing "<<currentPart.name<<"..."<<endl;
        }
        else if(token == "v"){ //Vertex position
            currentLine >> v3.x >> v3.y >> v3.z;
            vertexPosition.push_back(v3);
        }
        else if(token == "vt"){
            currentLine >> v2.x >> v2.y;
            vertexTexture.push_back(v2);
        }
        else if(token == "vn"){
            currentLine >> v3.x >> v3.y >> v3.z;
            vertexNormal.push_back(v3);
        }
        else if(token == "f"){
            int counter = 0;
            while(currentLine >> glint){
                if(counter == 0){
                    vertexPositionIndices.push_back(glint);
                }
                else if(counter == 1){
                    vertexTextureIndices.push_back(glint);
                }
                else if(counter == 2){
                    vertexNormalIndices.push_back(glint);
                }

                if(currentLine.peek() == '/'){
                    counter++;
                    currentLine.ignore(1, '/');
                }
                else if(currentLine.peek() == ' '){
                    counter++;
                    currentLine.ignore(1, ' ');
                }

                if(counter > 2){
                    counter = 0;
                }
            }
        }
        else{

        }
        vertices.resize(vertexPositionIndices.size(), Vertex());
        for (int i = 0; i < vertices.size(); ++i) {
            vertices[i].position = vertexPosition[vertexPositionIndices[i] - 1];
            vertices[i].texture = vertexTexture[vertexTextureIndices[i] - 1];
            vertices[i].normal = vertexNormal[vertexNormalIndices[i] - 1];
        }
        currentPart.vertices = vertices;
//        cout<<"Part processed: "<<currentPart.name<<endl;
    }
//    if(!currentPart.name.empty()){
//        parts.push_back(currentPart);
//        cout<<currentPart.name<<" constructed."<<endl;
//    }

    cout<<"OBJ parsed. Here are your parts: "<<endl;
    for(int i = 0; i < parts.size(); i++){
        cout<<"- "<<parts[i].name<<endl;
    }
    cout<<"Ready to continue"<<endl;
    return parts;
}

#endif //CAROPENGL_LOADER_H
