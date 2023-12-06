#pragma once
#include "GL_Debugger.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>


// it is not possible to create a single program with shaders and compute shaders so I will seperate this
// most of the functionality is same with "Shader" class so I copied and pasted most of the things


class ComputeShader {
public:
    unsigned int ID;
    ComputeShader(const std::string& path) {
        std::string code = getShaderCode(path);

        const char* cShaderCode = code.c_str();

        // 2. compile shaders
        unsigned int computeShader;
        // COMPUTE shader
        computeShader = glCreateShader(GL_COMPUTE_SHADER);
        glCheckError();
        glShaderSource(computeShader, 1, &cShaderCode, NULL);
        glCheckError();
        glCompileShader(computeShader);
        glCheckError();
        checkCompileErrors(computeShader, "COMPUTE");
        // shader Program
        ID = glCreateProgram();
        glCheckError();
        glAttachShader(ID, computeShader);
        glCheckError();
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        glCheckError();
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(computeShader);
        glCheckError();





    }
    void Activate() const {
        glUseProgram(ID);
        glCheckError();
    }
    void Deactivate() const {
        glUseProgram(0);
        glCheckError();
    }

    const std::string getShaderCode(const std::string& path) const {
        std::string code;
        std::ifstream file;

        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try
        {
            // open files
            file.open(path);
            std::stringstream shaderStream;
            // read file's buffer contents into streams
            shaderStream << file.rdbuf();
            // close file handlers
            file.close();
            // convert stream into string
            code = shaderStream.str();

        }
        catch (std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;

        }
        return code;

    }

    void SetBool(const std::string& name, bool value) const {
        Activate();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
        glCheckError();
        Deactivate();
    }
    void SetInt(const std::string& name, int value) const {
        Activate();
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
        glCheckError();
        Deactivate();
    }
    void SetFloat(const std::string& name, float value) const {
        Activate();
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
        glCheckError();
        Deactivate();
    }
    void SetMatrix4(const std::string& name, glm::mat4& m4) const {
        Activate();
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m4));
        glCheckError();
        Deactivate();
    }
    void SetVec3(const std::string& name, glm::vec3& v3) const {
        Activate();
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v3));
        glCheckError();
        Deactivate();
    }
    void SetIntArray(const std::string& name, const int* array, int arraysize) {
        Activate();
        glUniform1iv(glGetUniformLocation(ID, name.c_str()), arraysize, array);

    }


private:
    void checkCompileErrors(unsigned int shader, std::string type)const
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            glCheckError();
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                glCheckError();
                std::cout << "ERROR::COMPUTE_SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else {
                std::cout << "SUCCESS::COMPUTE_SHADER_COMPILATION" << std::endl;
            }

        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            glCheckError();
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                glCheckError();
                std::cout << "ERROR::COMPUTE_SHADER_PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else {
                std::cout << "SUCCESS::COMPUTE_SHADER_LINKING" << std::endl;
            }
        }
    }


};
