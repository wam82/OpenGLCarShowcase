//
// Created by William-Alexandre Messier on 2024-03-24.
//

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL


#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Camera.h"
#include "OBJ_Loader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "stb_image.cpp"
#include <unordered_map>

using namespace std;

//Vertices for a pyramid
//Vertex vertices[] = {
//        //Position                                              //Color                                           //Texture                                  //Normals
//        //Triangle front
//        glm::vec3 (0.f, 0.5f, 0.f),         glm::vec3 (1.f, 0.f, 0.f),      glm::vec2 (0.f, 1.f),       glm::vec3(0.f, 0.f, 1.f),
//        glm::vec3 (-0.5f, -0.5f, 0.5f),     glm::vec3 (0.f, 1.f, 0.f),       glm::vec2 (0.f, 0.f),       glm::vec3(0.f, 0.f, 1.f),
//        glm::vec3 (0.5f, -0.5f, 0.5f),      glm::vec3 (0.f, 0.f, 1.f),      glm::vec2 (1.f, 0.f),       glm::vec3(0.f, 0.f, 1.f),
//
//        //Triangle left
//        glm::vec3 (0.f, 0.5f, 0.f),         glm::vec3 (1.f, 1.f, 0.f),      glm::vec2 (0.f, 1.f),       glm::vec3(-1.f, 0.f, 0.f),
//        glm::vec3 (-0.5f, -0.5f, -0.5f),    glm::vec3 (0.f, 0.f, 1.f),      glm::vec2 (0.f, 0.f),       glm::vec3(-1.f, 0.f, 0.f),
//        glm::vec3 (-0.5f, -0.5f, 0.5f),     glm::vec3 (0.f, 0.f, 1.f),      glm::vec2 (1.f, 0.f),       glm::vec3(-1.f, 0.f, 0.f),
//
//        //Triangle back
//        glm::vec3 (0.f, 0.5f, 0.f),         glm::vec3 (1.f, 1.f, 0.f),      glm::vec2 (0.f, 1.f),       glm::vec3(0.f, 0.f, -1.f),
//        glm::vec3 (0.5f, -0.5f, -0.5f),     glm::vec3 (0.f, 0.f, 1.f),       glm::vec2 (0.f, 0.f),       glm::vec3(0.f, 0.f, -1.f),
//        glm::vec3 (-0.5f, -0.5f, -0.5f),     glm::vec3 (0.f, 0.f, 1.f),      glm::vec2 (1.f, 0.f),       glm::vec3(0.f, 0.f, -1.f),
//
//        //Triangle right
//        glm::vec3 (0.f, 0.5f, 0.f),         glm::vec3 (1.f, 1.f, 0.f),      glm::vec2 (0.f, 1.f),       glm::vec3(1.f, 0.f, 0.f),
//        glm::vec3 (0.5f, -0.5f, 0.5f),     glm::vec3 (0.f, 0.f, 1.f),       glm::vec2 (0.f, 0.f),       glm::vec3(1.f, 0.f, 0.f),
//        glm::vec3 (0.5f, -0.5f, -0.5f),      glm::vec3 (0.f, 0.f, 1.f),      glm::vec2 (1.f, 0.f),       glm::vec3(1.f, 0.f, 0.f)
//};
//unsigned nbOfVertices = sizeof(vertices) / sizeof(Vertex);
//GLuint indices[] = {
//        0, 1, 2,
//        0,2,3
//};
//unsigned nbOfIndices = sizeof(indices) / sizeof (GLuint);

unsigned nbOfIndices = 0;
int windowWidth = 1000;
int windowHeight = 600;
int frameBufferWidth = 0;
int frameBufferHeight = 0;
bool isOpened = false;

GLuint loadTexture(const char* path) {
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

void controlCamera(GLFWwindow* window, float &delta, Camera &cam){
    const float cameraSpeed = 1.f * delta;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        cam.goForward(cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        cam.goBackward(cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        cam.strafeLeft(cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        cam.strafeRight(cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
        cam.goUp(cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){
        cam.goDown(cameraSpeed);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        cam.panUp();
    }
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        cam.panDown();
    }
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
        cam.panRight();
    }
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
        cam.panLeft();
    }
}

//void initImgui(GLFWwindow* window) {
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
//    ImGui::StyleColorsDark();
//
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 150");
//}
//
//void renderImgui() {
//    constexpr ImGuiWindowFlags imgui_default_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
//
//    const ImVec2 entity_box_size = ImVec2(0.2 * windowWidth, 0.4 * windowHeight); //Box size to modify
//    const ImVec2 entity_box_pos = ImVec2(10, 10); //Box position to modify
//
//    // Start ImGui frame
//    ImGui_ImplOpenGL3_NewFrame();
//    ImGui_ImplGlfw_NewFrame();
//    ImGui::NewFrame();
//
//    const char* title = "R32";
//    // ImGui content goes here
//    ImGui::Begin(title, nullptr, imgui_default_flags);
//    ImGui::SetWindowPos(entity_box_pos);
//    ImGui::SetWindowSize(entity_box_size);
//
//    ImGui::TextWrapped("This is some useful text.");
//
//    ImGui::End();
//
//    // Rendering
//    ImGui::Render();
//    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//}

void updateInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void frameBufferResize(GLFWwindow* window, int width, int height){
    frameBufferWidth = width;
    frameBufferHeight = height;
    glViewport(0, 0, frameBufferWidth, frameBufferHeight);
}

void changeColor(GLFWwindow* window, vector<carPart>& R32, glm::vec3& newColor){
    if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        newColor = glm::vec3(1.0f, 0.5f, 0.31f);
        for(carPart& part : R32) {
            if (part.name.find("Tire")) {
                part.material.color = newColor;
            }
        }
    }
}

void openHood(carPart& part){
    Vertex anchor = part.vertices[0];
    Vertex anchor2;

    for(Vertex position : part.vertices){
        if(position.position.y > anchor.position.y){
            if(position.position.z < anchor.position.z){
                anchor = position;
            }
        }
    }

    anchor2 = anchor;
    anchor2.position.x = -anchor2.position.x;

    glm::vec3 symmetry;
    symmetry.x = (anchor.position.x + anchor2.position.x)/2;
    symmetry.y = anchor.position.y;
    symmetry.z = anchor.position.z;

    part.modelMatrix = glm::translate(part.modelMatrix, symmetry);
    part.modelMatrix = glm::rotate(part.modelMatrix, glm::radians(-45.f), glm::vec3(1.f, 0.f, 0.f));
    part.modelMatrix = glm::translate(part.modelMatrix, -symmetry);
}

void closeHood(carPart& part){
    Vertex anchor = part.vertices[0];
    Vertex anchor2;

    for(Vertex position : part.vertices){
        if(position.position.y > anchor.position.y){
            if(position.position.z < anchor.position.z){
                anchor = position;
            }
        }
    }

    anchor2 = anchor;
    anchor2.position.x = -anchor2.position.x;

    glm::vec3 symmetry;
    symmetry.x = (anchor.position.x + anchor2.position.x)/2;
    symmetry.y = anchor.position.y;
    symmetry.z = anchor.position.z;

    part.modelMatrix = glm::translate(part.modelMatrix, symmetry);
    part.modelMatrix = glm::rotate(part.modelMatrix, glm::radians(45.f), glm::vec3(1.f, 0.f, 0.f));
    part.modelMatrix = glm::translate(part.modelMatrix, -symmetry);
}

void openLeftDoor(carPart& part, carPart& part2, carPart& part3, carPart& part4, carPart& part5, carPart& part6){
    cout<<endl;
    Vertex anchor = part.vertices[0];
    Vertex anchor2 = part.vertices[0];

    for(Vertex position : part.vertices){
        if(position.position.y < anchor.position.y){
            if(position.position.z > anchor.position.z){
                anchor = position;
            }
        }
    }

    for(Vertex position : part.vertices){
        if(position.position.y > anchor2.position.y){
            anchor2 = position;
        }
    }

    glm::vec3 symmetry;
    symmetry.x = anchor.position.x;
    symmetry.y = (anchor2.position.y + anchor.position.y)/2;
    symmetry.z = anchor.position.z;

    part.modelMatrix = glm::translate(part.modelMatrix, symmetry);
    part.modelMatrix = glm::rotate(part.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part.modelMatrix = glm::translate(part.modelMatrix, -symmetry);

    part2.modelMatrix = glm::translate(part2.modelMatrix, symmetry);
    part2.modelMatrix = glm::rotate(part2.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part2.modelMatrix = glm::translate(part2.modelMatrix, -symmetry);

    part3.modelMatrix = glm::translate(part3.modelMatrix, symmetry);
    part3.modelMatrix = glm::rotate(part3.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part3.modelMatrix = glm::translate(part3.modelMatrix, -symmetry);

    part4.modelMatrix = glm::translate(part4.modelMatrix, symmetry);
    part4.modelMatrix = glm::rotate(part4.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part4.modelMatrix = glm::translate(part4.modelMatrix, -symmetry);

    part5.modelMatrix = glm::translate(part5.modelMatrix, symmetry);
    part5.modelMatrix = glm::rotate(part5.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part5.modelMatrix = glm::translate(part5.modelMatrix, -symmetry);

    part6.modelMatrix = glm::translate(part6.modelMatrix, symmetry);
    part6.modelMatrix = glm::rotate(part6.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part6.modelMatrix = glm::translate(part6.modelMatrix, -symmetry);
}

void closeLeftDoor(carPart& part, carPart& part2, carPart& part3, carPart& part4, carPart& part5, carPart& part6){
    Vertex anchor = part.vertices[0];
    Vertex anchor2 = part.vertices[0];

    for(Vertex position : part.vertices){
        if(position.position.y < anchor.position.y){
            if(position.position.z > anchor.position.z){
                anchor = position;
            }
        }
    }

    for(Vertex position : part.vertices){
        if(position.position.y > anchor2.position.y){
            anchor2 = position;
        }
    }

    glm::vec3 symmetry;
    symmetry.x = anchor.position.x;
    symmetry.y = (anchor2.position.y + anchor.position.y)/2;
    symmetry.z = anchor.position.z;

    part.modelMatrix = glm::translate(part.modelMatrix, symmetry);
    part.modelMatrix = glm::rotate(part.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part.modelMatrix = glm::translate(part.modelMatrix, -symmetry);

    part2.modelMatrix = glm::translate(part2.modelMatrix, symmetry);
    part2.modelMatrix = glm::rotate(part2.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part2.modelMatrix = glm::translate(part2.modelMatrix, -symmetry);

    part3.modelMatrix = glm::translate(part3.modelMatrix, symmetry);
    part3.modelMatrix = glm::rotate(part3.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part3.modelMatrix = glm::translate(part3.modelMatrix, -symmetry);

    part4.modelMatrix = glm::translate(part4.modelMatrix, symmetry);
    part4.modelMatrix = glm::rotate(part4.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part4.modelMatrix = glm::translate(part4.modelMatrix, -symmetry);

    part5.modelMatrix = glm::translate(part5.modelMatrix, symmetry);
    part5.modelMatrix = glm::rotate(part5.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part5.modelMatrix = glm::translate(part5.modelMatrix, -symmetry);

    part6.modelMatrix = glm::translate(part6.modelMatrix, symmetry);
    part6.modelMatrix = glm::rotate(part6.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part6.modelMatrix = glm::translate(part6.modelMatrix, -symmetry);
}

void openRightDoor(carPart& part, carPart& part2, carPart& part3, carPart& part4, carPart& part5, carPart& part6){
    cout<<endl;
    Vertex anchor = part.vertices[0];
    Vertex anchor2 = part.vertices[0];

    for(Vertex position : part.vertices){
        if(position.position.y < anchor.position.y){
            if(position.position.z > anchor.position.z){
                anchor = position;
            }
        }
    }

    for(Vertex position : part.vertices){
        if(position.position.y > anchor2.position.y){
            anchor2 = position;
        }
    }

    glm::vec3 symmetry;
    symmetry.x = anchor.position.x;
    symmetry.y = (anchor2.position.y + anchor.position.y)/2;
    symmetry.z = anchor.position.z;

    part.modelMatrix = glm::translate(part.modelMatrix, symmetry);
    part.modelMatrix = glm::rotate(part.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part.modelMatrix = glm::translate(part.modelMatrix, -symmetry);

    part2.modelMatrix = glm::translate(part2.modelMatrix, symmetry);
    part2.modelMatrix = glm::rotate(part2.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part2.modelMatrix = glm::translate(part2.modelMatrix, -symmetry);

    part3.modelMatrix = glm::translate(part3.modelMatrix, symmetry);
    part3.modelMatrix = glm::rotate(part3.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part3.modelMatrix = glm::translate(part3.modelMatrix, -symmetry);

    part4.modelMatrix = glm::translate(part4.modelMatrix, symmetry);
    part4.modelMatrix = glm::rotate(part4.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part4.modelMatrix = glm::translate(part4.modelMatrix, -symmetry);

    part5.modelMatrix = glm::translate(part5.modelMatrix, symmetry);
    part5.modelMatrix = glm::rotate(part5.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part5.modelMatrix = glm::translate(part5.modelMatrix, -symmetry);

    part6.modelMatrix = glm::translate(part6.modelMatrix, symmetry);
    part6.modelMatrix = glm::rotate(part6.modelMatrix, glm::radians(45.f), glm::vec3(0.f, 1.f, 0.f));
    part6.modelMatrix = glm::translate(part6.modelMatrix, -symmetry);
}

void closeRightDoor(carPart& part, carPart& part2, carPart& part3, carPart& part4, carPart& part5, carPart& part6){
    Vertex anchor = part.vertices[0];
    Vertex anchor2 = part.vertices[0];

    for(Vertex position : part.vertices){
        if(position.position.y < anchor.position.y){
            if(position.position.z > anchor.position.z){
                anchor = position;
            }
        }
    }

    for(Vertex position : part.vertices){
        if(position.position.y > anchor2.position.y){
            anchor2 = position;
        }
    }

    glm::vec3 symmetry;
    symmetry.x = anchor.position.x;
    symmetry.y = (anchor2.position.y + anchor.position.y)/2;
    symmetry.z = anchor.position.z;

    part.modelMatrix = glm::translate(part.modelMatrix, symmetry);
    part.modelMatrix = glm::rotate(part.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part.modelMatrix = glm::translate(part.modelMatrix, -symmetry);

    part2.modelMatrix = glm::translate(part2.modelMatrix, symmetry);
    part2.modelMatrix = glm::rotate(part2.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part2.modelMatrix = glm::translate(part2.modelMatrix, -symmetry);

    part3.modelMatrix = glm::translate(part3.modelMatrix, symmetry);
    part3.modelMatrix = glm::rotate(part3.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part3.modelMatrix = glm::translate(part3.modelMatrix, -symmetry);

    part4.modelMatrix = glm::translate(part4.modelMatrix, symmetry);
    part4.modelMatrix = glm::rotate(part4.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part4.modelMatrix = glm::translate(part4.modelMatrix, -symmetry);

    part5.modelMatrix = glm::translate(part5.modelMatrix, symmetry);
    part5.modelMatrix = glm::rotate(part5.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part5.modelMatrix = glm::translate(part5.modelMatrix, -symmetry);

    part6.modelMatrix = glm::translate(part6.modelMatrix, symmetry);
    part6.modelMatrix = glm::rotate(part6.modelMatrix, glm::radians(-45.f), glm::vec3(0.f, 1.f, 0.f));
    part6.modelMatrix = glm::translate(part6.modelMatrix, -symmetry);
}

void openTrunk(carPart &part1, carPart &part2, carPart &part3, carPart &part4, carPart &part5, carPart &part6, carPart &part7) {
    Vertex anchor = part1.vertices[0];
    Vertex anchor2 = part1.vertices[0];

    for(Vertex position : part1.vertices){
        if(position.position.y > anchor.position.y){
            anchor = position;
        }
    }

    anchor2 = anchor;
    anchor2.position.x = -anchor2.position.x;

    glm::vec3 symmetry;
    symmetry.x = (anchor.position.x + anchor2.position.x)/2;
    symmetry.y = anchor.position.y;
    symmetry.z = anchor.position.z;

    part1.modelMatrix = glm::translate(part1.modelMatrix, symmetry);
    part1.modelMatrix = glm::rotate(part1.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part1.modelMatrix = glm::translate(part1.modelMatrix, -symmetry);

    part2.modelMatrix = glm::translate(part2.modelMatrix, symmetry);
    part2.modelMatrix = glm::rotate(part2.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part2.modelMatrix = glm::translate(part2.modelMatrix, -symmetry);

    part3.modelMatrix = glm::translate(part3.modelMatrix, symmetry);
    part3.modelMatrix = glm::rotate(part3.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part3.modelMatrix = glm::translate(part3.modelMatrix, -symmetry);

    part4.modelMatrix = glm::translate(part4.modelMatrix, symmetry);
    part4.modelMatrix = glm::rotate(part4.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part4.modelMatrix = glm::translate(part4.modelMatrix, -symmetry);

    part5.modelMatrix = glm::translate(part5.modelMatrix, symmetry);
    part5.modelMatrix = glm::rotate(part5.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part5.modelMatrix = glm::translate(part5.modelMatrix, -symmetry);

    part6.modelMatrix = glm::translate(part6.modelMatrix, symmetry);
    part6.modelMatrix = glm::rotate(part6.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part6.modelMatrix = glm::translate(part6.modelMatrix, -symmetry);

    part7.modelMatrix = glm::translate(part7.modelMatrix, symmetry);
    part7.modelMatrix = glm::rotate(part7.modelMatrix, glm::radians(80.f), glm::vec3(1.f, 0.f, 0.f));
    part7.modelMatrix = glm::translate(part7.modelMatrix, -symmetry);
}

void closeTrunk(carPart &part1, carPart &part2, carPart &part3, carPart &part4, carPart &part5, carPart &part6, carPart &part7) {
    Vertex anchor = part1.vertices[0];
    Vertex anchor2 = part1.vertices[0];

    for(Vertex position : part1.vertices){
        if(position.position.y > anchor.position.y){
            anchor = position;
        }
    }

    anchor2 = anchor;
    anchor2.position.x = -anchor2.position.x;

    glm::vec3 symmetry;
    symmetry.x = (anchor.position.x + anchor2.position.x)/2;
    symmetry.y = anchor.position.y;
    symmetry.z = anchor.position.z;

    part1.modelMatrix = glm::translate(part1.modelMatrix, symmetry);
    part1.modelMatrix = glm::rotate(part1.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part1.modelMatrix = glm::translate(part1.modelMatrix, -symmetry);

    part2.modelMatrix = glm::translate(part2.modelMatrix, symmetry);
    part2.modelMatrix = glm::rotate(part2.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part2.modelMatrix = glm::translate(part2.modelMatrix, -symmetry);

    part3.modelMatrix = glm::translate(part3.modelMatrix, symmetry);
    part3.modelMatrix = glm::rotate(part3.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part3.modelMatrix = glm::translate(part3.modelMatrix, -symmetry);

    part4.modelMatrix = glm::translate(part4.modelMatrix, symmetry);
    part4.modelMatrix = glm::rotate(part4.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part4.modelMatrix = glm::translate(part4.modelMatrix, -symmetry);

    part5.modelMatrix = glm::translate(part5.modelMatrix, symmetry);
    part5.modelMatrix = glm::rotate(part5.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part5.modelMatrix = glm::translate(part5.modelMatrix, -symmetry);

    part6.modelMatrix = glm::translate(part6.modelMatrix, symmetry);
    part6.modelMatrix = glm::rotate(part6.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part6.modelMatrix = glm::translate(part6.modelMatrix, -symmetry);

    part7.modelMatrix = glm::translate(part7.modelMatrix, symmetry);
    part7.modelMatrix = glm::rotate(part7.modelMatrix, glm::radians(-80.f), glm::vec3(1.f, 0.f, 0.f));
    part7.modelMatrix = glm::translate(part7.modelMatrix, -symmetry);
}

void explodeVehicle(GLFWwindow* window, vector<carPart>& car){
    carPart* trunk = nullptr;
    carPart* trunkBars = nullptr;
    carPart* trunkLight = nullptr;
    carPart* trunkGlassLeft = nullptr;
    carPart* trunkGlassRight = nullptr;
    carPart* trunkLogo = nullptr;
    carPart* spoiler = nullptr;
    carPart* leftDoorExterior = nullptr;
    carPart* leftDoorInterior = nullptr;
    carPart* leftDoorHandle = nullptr;
    carPart* leftDoorWindow = nullptr;
    carPart* leftMirrorFrame = nullptr;
    carPart* leftMirrorGlass = nullptr;
    carPart* rightDoorExterior = nullptr;
    carPart* rightDoorInterior = nullptr;
    carPart* rightDoorHandle = nullptr;
    carPart* rightDoorWindow = nullptr;
    carPart* rightMirrorFrame = nullptr;
    carPart* rightMirrorGlass = nullptr;
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !isOpened){ //Open
        for(carPart& part : car){
            if(part.name=="Hood"){
                openHood(part);
            }
            else if(part.name == "Trunk"){
                trunk = &part;
            }
            else if(part.name == "Trunk_Bars"){
                trunkBars = &part;
            }
            else if(part.name == "Trunklight_Left_Glass"){
                trunkGlassLeft = &part;
            }
            else if(part.name == "Trunklight_Light"){
                trunkLight = &part;
            }
            else if(part.name == "Trunklight_Right_Glass"){
                trunkGlassRight = &part;
            }
            else if(part.name == "Logo_Back"){
                trunkLogo = &part;
            }
            else if(part.name == "Spoiler_Carbon"){
                spoiler = &part;
            }
            else if(part.name == "Door_Left"){
                leftDoorExterior = &part;
            }
            else if(part.name == "Door_Left_Interior"){
                leftDoorInterior = &part;
            }
            else if(part.name == "Door_Left_Handle_Carbon"){
                leftDoorHandle = &part;
            }
            else if(part.name == "Window_Door_Left"){
                leftDoorWindow = &part;
            }
            else if(part.name == "Mirror_Left_Glass"){
                leftMirrorGlass = &part;
            }
            else if(part.name == "Mirror_Left_Carbon"){
                leftMirrorFrame = &part;
            }
            else if(part.name == "Door_Right"){
                rightDoorExterior = &part;
            }
            else if(part.name == "Door_Right_Interior"){
                rightDoorInterior = &part;
            }
            else if(part.name == "Door_Right_Handle_Carbon"){
                rightDoorHandle = &part;
            }
            else if(part.name == "Window_Door_Right"){
                rightDoorWindow = &part;
            }
            else if(part.name == "Mirror_Right_Glass"){
                rightMirrorGlass = &part;
            }
            else if(part.name == "Mirror_Right_Carbon"){
                rightMirrorFrame = &part;
            }
        }
        if(leftDoorExterior != nullptr &&
            leftDoorInterior != nullptr &&
            leftDoorHandle != nullptr &&
            leftDoorWindow != nullptr &&
            leftMirrorGlass != nullptr &&
            leftMirrorFrame != nullptr){

            openLeftDoor(*leftDoorExterior, *leftDoorInterior, *leftDoorHandle, *leftDoorWindow, *leftMirrorFrame, *leftMirrorGlass);
        }

        if(rightDoorExterior != nullptr &&
            rightDoorInterior != nullptr &&
            rightDoorHandle != nullptr &&
            rightDoorWindow != nullptr &&
            rightMirrorGlass != nullptr &&
            rightMirrorFrame != nullptr){

            openRightDoor(*rightDoorExterior, *rightDoorInterior, *rightDoorHandle, *rightDoorWindow, *rightMirrorFrame, *rightMirrorGlass);
        }

        if(trunkBars != nullptr &&
            trunk != nullptr &&
            trunkLight != nullptr &&
            trunkGlassLeft != nullptr &&
            trunkGlassRight != nullptr &&
            trunkLogo != nullptr &&
            spoiler != nullptr){

            openTrunk(*trunkBars, *trunk, *trunkLight, *trunkGlassLeft, *trunkGlassRight, *trunkLogo, *spoiler);
        }
        isOpened = true;
    }
    else if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && isOpened){ //Close
        for(carPart& part : car){
            if(part.name == "Hood"){
                closeHood(part);
            }
            else if(part.name == "Trunk"){
                trunk = &part;
            }
            else if(part.name == "Trunk_Bars"){
                trunkBars = &part;
            }
            else if(part.name == "Trunklight_Left_Glass"){
                trunkGlassLeft = &part;
            }
            else if(part.name == "Trunklight_Light"){
                trunkLight = &part;
            }
            else if(part.name == "Trunklight_Right_Glass"){
                trunkGlassRight = &part;
            }
            else if(part.name == "Logo_Back"){
                trunkLogo = &part;
            }
            else if(part.name == "Spoiler_Carbon"){
                spoiler = &part;
            }
            else if(part.name == "Door_Left"){
                leftDoorExterior = &part;
            }
            else if(part.name == "Door_Left_Interior"){
                leftDoorInterior = &part;
            }
            else if(part.name == "Door_Left_Handle_Carbon"){
                leftDoorHandle = &part;
            }
            else if(part.name == "Window_Door_Left"){
                leftDoorWindow = &part;
            }
            else if(part.name == "Mirror_Left_Glass"){
                leftMirrorGlass = &part;
            }
            else if(part.name == "Mirror_Left_Carbon"){
                leftMirrorFrame = &part;
            }
            else if(part.name == "Door_Right"){
                rightDoorExterior = &part;
            }
            else if(part.name == "Door_Right_Interior"){
                rightDoorInterior = &part;
            }
            else if(part.name == "Door_Right_Handle_Carbon"){
                rightDoorHandle = &part;
            }
            else if(part.name == "Window_Door_Right"){
                rightDoorWindow = &part;
            }
            else if(part.name == "Mirror_Right_Glass"){
                rightMirrorGlass = &part;
            }
            else if(part.name == "Mirror_Right_Carbon"){
                rightMirrorFrame = &part;
            }
        }
        if(leftDoorExterior != nullptr &&
           leftDoorInterior != nullptr &&
           leftDoorHandle != nullptr &&
           leftDoorWindow != nullptr &&
           leftMirrorGlass != nullptr &&
           leftMirrorFrame != nullptr){

            closeLeftDoor(*leftDoorExterior, *leftDoorInterior, *leftDoorHandle, *leftDoorWindow, *leftMirrorFrame, *leftMirrorGlass);
        }

        if(rightDoorExterior != nullptr &&
           rightDoorInterior != nullptr &&
           rightDoorHandle != nullptr &&
           rightDoorWindow != nullptr &&
           rightMirrorGlass != nullptr &&
           rightMirrorFrame != nullptr){

            closeRightDoor(*rightDoorExterior, *rightDoorInterior, *rightDoorHandle, *rightDoorWindow, *rightMirrorFrame, *rightMirrorGlass);
        }

        if(trunkBars != nullptr &&
           trunk != nullptr &&
           trunkLight != nullptr &&
           trunkGlassLeft != nullptr &&
           trunkGlassRight != nullptr &&
           trunkLogo != nullptr &&
           spoiler != nullptr){

            closeTrunk(*trunkBars, *trunk, *trunkLight, *trunkGlassLeft, *trunkGlassRight, *trunkLogo, *spoiler);
        }

        isOpened = false;
    }
}

//void controlTransformation(GLFWwindow* window, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale){
//    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
//        position.z += 0.01f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
//        position.z -= 0.01f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
//        position.x -= 0.01f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
//        position.x += 0.01f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
//        rotation.y -= 1.f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
//        rotation.y += 1.f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
//        rotation.x -= 1.f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
//        rotation.x += 1.f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
//        position.y -= 0.01f;
//    }
//    if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
//        position.y += 0.01f;
//    }
//}

bool compareOpacity(const carPart& part1, const carPart& part2){
    return part1.material.opacity > part2.material.opacity;
}


int main(){
    //Init GLFW
    glfwInit();

    vector<carPart> M4 = loadModel("../assets/M4_Chopped_Trunk.obj");

    //Using OpenGL version 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "Car Showcase", NULL, NULL);

    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    glfwSetFramebufferSizeCallback(window, frameBufferResize);

    glfwMakeContextCurrent(window);

    //Init GLEW (Requires window & OpenGL context)
    glewExperimental = true;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        exit(1);
    }

    //OpenGL options
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CCW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //Shader initialisation
    Shader core_program("../shaders/vertex_core.glsl", "../shaders/fragment_core.glsl");

    glm::vec3 newColor(1.f);

    //Model
    //VAO, VBO, EBO
//    unordered_map<string, carPart> carParts;
    for(carPart& part : M4){
        glGenVertexArrays(1, &part.VAO);
        glBindVertexArray(part.VAO);

        glGenBuffers(1, &part.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, part.VBO);
        glBufferData(GL_ARRAY_BUFFER, part.vertices.size() * sizeof(Vertex), &part.vertices[0], GL_STATIC_DRAW);

        //Set VertexAttributePointers and enable
        //Position
        GLuint positionLocation = core_program.setAttributeLocation("vertex_position");
        glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *) offsetof(Vertex, position));
        glEnableVertexAttribArray(positionLocation);

        //Texture
        GLuint textureLocation = core_program.setAttributeLocation("vertex_texture_coord");
        glVertexAttribPointer(textureLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *) offsetof(Vertex, texture));
        glEnableVertexAttribArray(textureLocation);

        //Normal
        GLuint normalLocation = core_program.setAttributeLocation("vertex_normal");
        glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (GLvoid *) offsetof(Vertex, normal));
        glEnableVertexAttribArray(normalLocation);

        //Bind VAO 0
        glBindVertexArray(0);

        glm::vec3 carPartColor = glm::vec3 (1.f);
        float opacity = 1.f;

        if(part.name == "Antenna" || part.name == "Underbody" || part.name == "Suspension_Front_Tierod"){ //Black
            carPartColor = glm::vec3(0.1f);
        }
        else if(part.name == "Tires" || part.name == "Mudflaps"){
            carPartColor = glm::vec3(0.1f);
            //Set material properties
        }
        else if(part.name == "Body" || part.name == "Bumper_Front" || part.name == "Bumper_Rear" || part.name == "Door_Left" ||
        part.name == "Door_Right" || part.name == "Fender_Left" || part.name == "Fender_Right" || part.name == "Hood" ||
        part.name == "Sideskirt" || part.name == "Trunk" || part.name == "Trunk_Bars"){ //Blue
            carPartColor = glm::vec3 ((float) 39/255, (float) 182/255, (float) 225/255);
        }
        else if(part.name == "Brake_Bolts" || part.name == "Brake_Disc" || part.name == "Brake_Holders"){
            carPartColor = glm::vec3 ((float) 51/255, (float) 50/255, (float) 57/255);
        }
        else if(part.name == "Brake_Calippers" || part.name == "Needles"){
            carPartColor = glm::vec3 ((float) 240/255, (float) 0/255, (float) 10/255);
        }
        else if(part.name == "Bumper_Front_Grille_Bottom"){
            carPartColor = glm::vec3 ((float) 0/255, (float) 0/255, (float) 15/255);
        }
        else if(part.name == "Bumper_Front_Grille_Top"){
            carPartColor = glm::vec3 ((float) 50/255, (float) 50/255, (float) 50/255);
        }
        else if(part.name == "Exhaust_Output"){
            carPartColor = glm::vec3 ((float) 150/255, (float) 150/255, (float) 150/255);
            //Texture here
        }
        else if(part.name == "Exhaust_Pipe" || part.name == "Pedal_Brake" || part.name == "Pedal_Gas"){
            carPartColor = glm::vec3 ((float) 150/255, (float) 150/255, (float) 150/255);
        }
        else if(part.name == "Suspension_Back" || part.name == "Suspension_Front" || part.name == "Suspension_Lower_Arm_Back"){ //Engine Color
            carPartColor = glm::vec3 ((float) 150/255, (float) 150/255, (float) 150/255);
        }
        else if(part.name == "M4_Badge_Dark_Blue"){
            carPartColor = glm::vec3 ((float) 14/255, (float) 57/255, (float) 118/255);
        }
        else if(part.name == "M4_Badge_Light_Blue"){
            carPartColor = glm::vec3 ((float) 20/255, (float) 95/255, (float) 175/255);
        }
        else if(part.name == "M4_Badge_Red"){
            carPartColor = glm::vec3 ((float) 223/255, (float) 29/255, (float) 30/255);
        }
        else if(part.name == "M4_Badge_Frame" || part.name == "M4_Badge_M4"){
            carPartColor = glm::vec3 ((float) 41/255, (float) 41/255, (float) 49/255);
        }
        else if(part.name == "Window_Back" || part.name == "Window_Rear_Left" || part.name == "Window_Rear_Right"){
            carPartColor = glm::vec3 ((float) 35/255, (float) 42/255, (float) 42/255);
            opacity = 0.2f;
        }
        else if(part.name == "Window_Front" || part.name == "Window_Door_Left" || part.name == "Window_Door_Right"){
            carPartColor = glm::vec3 ((float) 218/255, (float) 236/255, (float) 254/255);
            opacity = 0.15f;
        }
        else if(part.name == "Mirror_Left_Glass" || part.name == "Mirror_Right_Glass"){
            carPartColor = glm::vec3 ((float) 245/255, (float) 245/255, (float) 245/255);
            opacity = 0.4f;
        }
        else if(part.name == "Headlight_Left_Glass" || part.name == "Headlight_Right_Glass"){
            carPartColor = glm::vec3 ((float) 218/255, (float) 236/255, (float) 254/255);
            opacity = 0.1f;
        }
        else if(part.name == "Headlight_Left_Light" || part.name == "Headlight_Right_Light"){
            carPartColor = glm::vec3 ((float) 74/255, (float) 74/255, (float) 82/255);
        }
        else if(part.name == "Taillight_Left_Glass" || part.name == "Taillight_Right_Glass" || part.name == "Trunklight_Left_Glass" || part.name == "Trunklight_Right_Glass"){
            carPartColor = glm::vec3 ((float) 117/255, (float) 7/255, (float) 19/255);
            opacity = 0.25f;
        }
        else if(part.name.find("Interior") == 0 || part.name.find("Seat") == 0 || part.name == "Door_Left_Interior" ||
        part.name == "Door_Right_Interior" || part.name == "Flasher"){
            carPartColor = glm::vec3 (0.2f);
        }
        else if(part.name == "Spoiler_Carbon" || part.name == "Bumper_Front_Accent_Carbon" || part.name == "Bumper_Front_Carbon" ||
        part.name == "Bumper_Rear_Accent_Carbon" || part.name == "Door_Left_Handle_Carbon" || part.name == "Door_Right_Handle_Carbon" ||
        part.name == "Diffuser_Rear_Carbon" || part.name == "Mag_Front_Left" || part.name == "Mag_Front_Right" || part.name == "Mag_Rear_Left" ||
        part.name == "Mag_Rear_Right" || part.name == "Mirror_Left_Carbon" || part.name == "Mirror_Right_Carbon" || part.name == "Roof" ||
        part.name == "Splitter_Carbon" || part.name == "Steering_Wheel_Carbon"){
            part.texture = loadTexture("../assets/Textures/common_carbon.jpg");
            carPartColor = glm::vec3(1.f);
            part.hasTexture = true;
        }
        else if(part.name == "Logo_Front" || part.name == "Logo_Back"){ //Bugged
            part.texture = loadTexture("../assets/Textures/BMW_logo.jpg");
            carPartColor = glm::vec3(1.f);
            part.texture = true;
        }
        part.material.color = carPartColor;
        part.material.opacity = opacity;
    }

    std::sort(M4.begin(), M4.end(), compareOpacity);
//    cout<<"Update: "<<endl;
    for(carPart part : M4){
        cout<<part.name<<endl;
    }

    //Basic camera initialisation
    Camera camera(glm::vec3(0.f, 0.f, 3.f), glm::vec3 (0.f, 0.f, 0.f));
    camera.fov = 45.f;

    glm::mat4 ViewMatrix(1.f);
    glm::mat4 ProjectionMatrix(1.f);

    ViewMatrix = camera.viewMatrix;

    float fov = camera.fov;
    float nearPlane = 0.1f;
    float farPlane = 1000.f;

    ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float> (windowWidth) / windowHeight, nearPlane, farPlane);

    //Lights
    glm::vec3 lightPosition0 = camera.position;
    glm::vec3 lightColorInterior((float) 240/255, (float) 255/255, (float) 255/255);


    core_program.use();

    core_program.setMat4fv(ViewMatrix, "ViewMatrix");
    core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

    core_program.setVector3f(lightPosition0, "lightPosition0");
    core_program.setVector3f(camera.position, "cameraPosition");

    core_program.setVector3f(lightColorInterior, "lightColor");

    core_program.unuse();

//    initImgui(window);
    float lastFrame = 0;
    //Main Loop
    while(!glfwWindowShouldClose(window)){
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Update input --
        glfwPollEvents();
        controlCamera(window, deltaTime, camera);
        ViewMatrix = camera.viewMatrix;
        changeColor(window, M4, newColor);

        //Update params --
        updateInput(window);

        //Draw --
        //Clear
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        core_program.use();

        //Translate, Rotate, Scale
        explodeVehicle(window, M4);

        lightPosition0 = camera.position;
        core_program.setVector3f(lightPosition0, "lightPosition0");

        core_program.setMat4fv(ViewMatrix, "ViewMatrix");

        glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
        ProjectionMatrix = glm::perspective(glm::radians(fov), static_cast<float> (frameBufferWidth) / frameBufferHeight, nearPlane, farPlane);
        core_program.setMat4fv(ProjectionMatrix, "ProjectionMatrix");

        //Bind vertex array object
        for(carPart& part : M4){
            core_program.setBool(part.hasTexture, "hasTexture");
            core_program.setMat4fv(part.modelMatrix, "ModelMatrix");
            int nbOfVertices = (int) part.vertices.size();
            core_program.setVector3f(part.material.color, "modifiedColor");
            core_program.set1f(part.material.opacity, "opacity");
            if(part.hasTexture){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, part.texture);
            }
            glBindVertexArray(part.VAO);
            //Draw
//            if(nbOfIndices == 0){
            glDrawArrays(GL_TRIANGLES, 0, nbOfVertices);
//            }
//            else {
            glDrawElements(GL_TRIANGLES, nbOfIndices, GL_UNSIGNED_INT, 0);
//            }
        }

        //End draw
//        renderImgui();
        glfwSwapBuffers(window);
        glFlush();
    }

    //End of Program
    glfwDestroyWindow(window);
    glfwTerminate();
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();

    return 0;
}