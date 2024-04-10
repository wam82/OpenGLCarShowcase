//
// Created by William-Alexandre Messier on 2024-03-31.
//

#ifndef CAROPENGL_SHADER_H
#define CAROPENGL_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Shader{
private:
    GLuint id;

    string loadShaderSource(const char* fileName){
        string currentLine = "";
        string src = "";

        ifstream in_file;

        //Vertex shader
        in_file.open(fileName);

        if(in_file.is_open()){
            while(getline(in_file, currentLine)){
                src += currentLine + "\n";
            }
        }
        else{
            cout<<"An error occurred when trying to open the file: "<<fileName<<endl;
        }
        in_file.close();

        return src;
    }

    GLuint loadShader(GLenum type, const char* fileName){
        char errorLog[512];
        GLint success;

        GLuint shader = glCreateShader(type);
        string shaderSource = this->loadShaderSource(fileName);
        const GLchar* source = shaderSource.c_str();
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(shader, 512, NULL, errorLog);
            cout<<"An error occurred when trying to compile the shader: "<< fileName <<endl;
            cout<<errorLog<<endl;
        }
        return shader;
    }

    void linkProgram(GLuint vertexShader, GLuint fragmentShader){
        char errorLog[512];
        GLint success;

        this->id = glCreateProgram();

        glAttachShader(this->id, vertexShader);

        glAttachShader(this->id, fragmentShader);

        glLinkProgram(this->id);

        glGetProgramiv(this->id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(this->id, 512, NULL, errorLog);
            cout<<"An error occurred when trying to link the program"<<endl;
            cout<<errorLog<<endl;
        }
        glUseProgram(0);
    }
public:
    Shader(const char* vertexFile, const char* fragmentFile){
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;

        vertexShader = loadShader(GL_VERTEX_SHADER, vertexFile);
        fragmentShader = loadShader(GL_FRAGMENT_SHADER, fragmentFile);


        this->linkProgram(vertexShader, fragmentShader);

        //End
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    ~Shader(){
        glDeleteProgram(this -> id);
    }

    void use(){
        glUseProgram(this->id);
    }

    void unuse(){
        glUseProgram(0);
    }

    void setBool(GLboolean value, const GLchar* name){
        glUniform1i(glGetUniformLocation(this->id, name), value);
    }

    void set1f(GLfloat value, const GLchar* name){
//        this->use();

        glUniform1f(glGetUniformLocation(this->id, name), value);

//        this->unuse();
    }

    void setVector2f(glm::fvec2 value, const GLchar* name){
//        this->use();

        glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

//        this->unuse();
    }

    void setVector3f(glm::fvec3 value, const GLchar* name){
//        this->use();

        glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

//        this->unuse();
    }

    void setVector4f(glm::fvec4 value, const GLchar* name){
//        this->use();

        glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));

//        this->unuse();
    }

    void setMat3fv(glm::mat3 value, const GLchar* name, GLboolean transpose = GL_FALSE){
//        this->use();

        glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose,glm::value_ptr(value));

//        this->unuse();
    }

    void setMat4fv(glm::mat4 value, const GLchar* name, GLboolean transpose = GL_FALSE){
//        this->use();

        glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose,glm::value_ptr(value));

//        this->unuse();
    }

    GLuint setAttributeLocation(const GLchar* name){
        return glGetAttribLocation(this->id, name);
    }
};

#endif //CAROPENGL_SHADER_H
