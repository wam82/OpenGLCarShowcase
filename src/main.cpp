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

void initImgui(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void renderImgui() {
    constexpr ImGuiWindowFlags imgui_default_flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    const ImVec2 entity_box_size = ImVec2(0.2 * windowWidth, 0.4 * windowHeight); //Box size to modify
    const ImVec2 entity_box_pos = ImVec2(10, 10); //Box position to modify

    // Start ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const char* title = "R32";
    // ImGui content goes here
    ImGui::Begin(title, nullptr, imgui_default_flags);
    ImGui::SetWindowPos(entity_box_pos);
    ImGui::SetWindowSize(entity_box_size);

    ImGui::TextWrapped("This is some useful text.");

    ImGui::End();

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

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
    cout<<"close"<<endl;
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

void explodeVehicle(GLFWwindow* window, vector<carPart>& car){
    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !isOpened){ //Open
        for(carPart& part : car){
            if(part.name=="Hood"){
                openHood(part);
            }
        }
        isOpened = true;
    }
    else if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS && isOpened){ //Close
        for(carPart& part : car){
            if(part.name=="Hood"){
                closeHood(part);
            }
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

    vector<carPart> M4 = loadModel("../assets/M4_Chopped_Simplified.obj");

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

        if(part.name == "Antenna" || part.name == "Tires"){ //Black
            carPartColor = glm::vec3(0.1f);
        }
        else if(part.name == "Body"){ //Purple
            carPartColor = glm::vec3 ((float) 99/255, (float) 30/255, (float) 96/255);
        }
        else if(part.name.find("Window") == 0 || part.name == "Taillight_Left_Glass" || part.name == "Taillight_Right_Glass"
                    || part.name == "Headlight_Left_Glass" || part.name == "Headlight_Right_Glass"){
            carPartColor = glm::vec3 ((float) 218/255, (float) 236/255, (float) 254/255);
            opacity = 0.1f;
        }
        else if(part.name.find("Interior") == 0 || part.name.find("Seat") == 0){
            carPartColor = glm::vec3 (0.1f);
        }
        part.material.color = carPartColor;
        part.material.opacity = opacity;
    }

    std::sort(M4.begin(), M4.end(), compareOpacity);
//    cout<<"Update: "<<endl;
//    for(carPart part : R32){
//        cout<<part.name<<endl;
//    }

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

    initImgui(window);
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
            core_program.setMat4fv(part.modelMatrix, "ModelMatrix");
            int nbOfVertices = (int) part.vertices.size();
            core_program.setVector3f(part.material.color, "modifiedColor");
            core_program.set1f(part.material.opacity, "opacity");
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
        renderImgui();
        glfwSwapBuffers(window);
        glFlush();
    }

    //End of Program
    glfwDestroyWindow(window);
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}