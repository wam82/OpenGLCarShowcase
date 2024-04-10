//
// Created by William-Alexandre Messier on 2024-04-06.
//

#ifndef CAROPENGL_CAMERA_H
#define CAROPENGL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

class Camera{
public:
    glm::vec3 position, front, direction, right, up;

    float fov, nearPlane, farPlane, pitch, yaw;

    glm::mat4 viewMatrix;

    Camera(glm::vec3 position, glm::vec3 front){
        this->position = position;
        this->front = front;
        this->direction = glm::normalize(this->position - this->front);
        this->right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), this->direction));
        this->up = glm::normalize((glm::cross(this->direction, this->right)));


        glm::vec3 tempDirection = glm::normalize(this->front - this->position);
        float tempPitch = glm::degrees(asin(tempDirection.y));
        if(tempPitch > 90.0f){
            tempPitch = 90.0f;
        }
        if(tempPitch < -90.0f){
            tempPitch = -90.0f;
        }
        float tempYaw = glm::degrees(acos(direction.x / cos(tempPitch)));
        this->pitch = tempPitch;
        this->yaw = tempYaw;
        updateView();
    }

    void updateView(){
        this->viewMatrix = glm::lookAt(this->position, this->position - this->direction, this->up);
    }

    void goForward(float speed){
        this->position -= speed * this->direction;
        updateView();
    }

    void goBackward(float speed){
        this->position += speed * this->direction;
        updateView();
    }

    void goUp(float speed){
        this->position += speed * this->up;
        updateView();
    }

    void goDown(float speed){
        this->position -= speed * this->up;
        updateView();
    }

    void strafeRight(float speed){
        this->position +=  speed * this->right;
        updateView();
    }

    void strafeLeft(float speed){
        this->position -= speed * this->right;
        updateView();
    }

    void panUp(){
        this->pitch -= 1.f;

        if(this->pitch > 89.0f){
            this->pitch = 90.0f;
        }
        if(this->pitch < -89.0f){
            this->pitch = -90.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->direction = glm::normalize(front);
        this->right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), this->direction));
        this->up = glm::normalize((glm::cross(this->direction, this->right)));
        updateView();
    }

    void panDown(){
        this->pitch += 1.f;

        if(this->pitch > 89.0f){
            this->pitch = 89.0f;
        }
        if(this->pitch < -89.0f){
            this->pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->direction = glm::normalize(front);
        this->right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), this->direction));
        this->up = glm::normalize((glm::cross(this->direction, this->right)));
        updateView();
    }

    void panRight(){
        this->yaw += 1.f;

        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->direction = glm::normalize(front);
        this->right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), this->direction));
        this->up = glm::normalize((glm::cross(this->direction, this->right)));
        updateView();
    }

    void panLeft(){
        this->yaw -= 1.f;

        glm::vec3 front;
        front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        front.y = sin(glm::radians(this->pitch));
        front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
        this->direction = glm::normalize(front);
        this->right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), this->direction));
        this->up = glm::normalize((glm::cross(this->direction, this->right)));
        updateView();
    }
};

#endif //CAROPENGL_CAMERA_H
