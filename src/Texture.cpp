#include "Texture.h"


Texture::Texture(const std::string& _path, GLint _wrapParam, GLint _filterMode) : path(_path),id(0), WIDTH(0),HEIGHT(0),NR_CHANNELS(0),data(nullptr){
    

    glGenTextures(1, &id);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, id);
    glCheckError();
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrapParam);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrapParam);
    glCheckError();
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filterMode);
    glCheckError();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filterMode);
    glCheckError();
    // load image, create texture and generate mipmaps
   
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    data = stbi_load(path.c_str(), &WIDTH, &HEIGHT, &NR_CHANNELS, 4);
    if (data)
    {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glCheckError();
        glGenerateMipmap(GL_TEXTURE_2D);
        glCheckError();

    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();
    stbi_image_free(data);
    
	
}
Texture::~Texture() {
    glDeleteTextures(1, &id);
    glCheckError();
}

void Texture::Bind(unsigned int channel) const {
    glActiveTexture(GL_TEXTURE0 + channel);
    glCheckError();
    glBindTexture(GL_TEXTURE_2D, id);
    glCheckError();

}

void Texture::UnBind() const {

    glBindTexture(GL_TEXTURE_2D, 0);
    glCheckError();

}

	