#pragma once
#include "GL_Debugger.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
public:
	unsigned int ID;

	Shader(const std::string& vertexPath,const std::string& fragmentPath);
	Shader(const std::string& vertexPath,const std::string& fragmentPath, const std::string& geometryPath);
	~Shader() {
		glDeleteProgram(ID);
	}
	void Activate() const;
	void Deactivate() const;
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetMatrix4(const std::string& name, glm::mat4& m4) const;
	void SetVec3(const std::string& name, glm::vec3& v3) const;
	void SetIntArray(const std::string& name, const int* array, int arraysize);
	inline void isItCompipledAndLinked()const {
		checkCompileErrors(ID, "PROGRAM");
		

	}
private:
	void checkCompileErrors(unsigned int shader, std::string type) const;
	const std::string getShaderCode(const std::string& path) const;
};

