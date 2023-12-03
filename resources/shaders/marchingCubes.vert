#version 430 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
    mat4 model;
    mat4 view;
    mat4 projection;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{   

    vs_out.model = model;
    vs_out.view = view;
    vs_out.projection = projection;

    gl_Position = vec4(aPos,1.0);
  
 
}