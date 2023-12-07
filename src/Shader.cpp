#include "Shader.h"


Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCheckError();
    glCompileShader(vertex);
    glCheckError();
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCheckError();
    glCompileShader(fragment);
    glCheckError();
    checkCompileErrors(fragment, "FRAGMENT");
    // shader Program
    ID = glCreateProgram();
    glCheckError();
    glAttachShader(ID, vertex);
    glCheckError();
    glAttachShader(ID, fragment);
    glCheckError();
    glLinkProgram(ID);
    glCheckError();
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glCheckError();
    glDeleteShader(fragment);
    glCheckError();
}
    const std::string Shader::getShaderCode(const std::string& path) const {
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


Shader::Shader(const std::string& vertexPath,const std::string& fragmentPath, const std::string& geometryPath)
{

    std::string vShader = getShaderCode(vertexPath);
    std::string fShader = getShaderCode(fragmentPath);
    std::string gShader = getShaderCode(geometryPath);

   
    const char* vShaderCode = vShader.c_str();
    const char* fShaderCode = fShader.c_str();
    const char* gShaderCode = gShader.c_str();
   // std::cout << "Vertex Shader" << std::endl << vShaderCode << std::endl;
    //std::cout << "Fragment Shader" << std::endl << fShaderCode << std::endl;
    //std::cout << "Geometry Shader" << std::endl << gShaderCode << std::endl;
   
    // 2. compile shaders
    unsigned int vertex, fragment, geometry;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glCheckError();
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCheckError();
    glCompileShader(vertex);
    glCheckError();
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glCheckError();
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCheckError();
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    glCheckError();
    // geometry Shader
    geometry = glCreateShader(GL_GEOMETRY_SHADER);
    glCheckError();
    glShaderSource(geometry, 1, &gShaderCode, NULL);
    glCheckError();
    glCompileShader(geometry);
    checkCompileErrors(geometry, "GEOMETRY");
    glCheckError();
    // shader Program
    ID = glCreateProgram();
    glCheckError();
    glAttachShader(ID, vertex);
    glCheckError();
    glAttachShader(ID, fragment);
    glCheckError();
    glAttachShader(ID, geometry);
    glCheckError();
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glCheckError();
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glCheckError();
    glDeleteShader(fragment);
    glCheckError();
    glDeleteShader(geometry);
    glCheckError();
}


void Shader::Activate() const {
	glUseProgram(ID);
    glCheckError();

}
void Shader::Deactivate() const{
    glUseProgram(0);
    glCheckError();

}

void Shader::SetBool(const std::string& name, bool value) const {
    Activate();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    glCheckError();
    
}
void Shader::SetInt(const std::string& name, int value) const {
    Activate();
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    glCheckError();
    
}
void Shader::SetFloat(const std::string& name, float value) const {
    Activate();
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    glCheckError();
   
}
void Shader::SetMatrix4(const std::string& name, glm::mat4& m4) const {
    Activate();
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(m4));
    glCheckError();
    
}
void Shader::SetVec3(const std::string& name,glm::vec3& v3) const {
    Activate();
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(v3));
    glCheckError();
    
}
void Shader::SetIntArray(const std::string& name, const int* array, int arraysize) {
    Activate();
    glUniform1iv(glGetUniformLocation(ID, name.c_str()), arraysize, array);

}
void Shader::checkCompileErrors(unsigned int shader, std::string type)const
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
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
        else {
            std::cout << "SUCCESS::SHADER_COMPILATION" << std::endl;
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
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
        else {
            std::cout << "SUCCESS::SHADER_LINKING" << std::endl;
        }
    }
}