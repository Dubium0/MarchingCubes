#version 330 core

out vec4 FragColor;

in vec3 Normal;
uniform vec3 lightDir;
void main()
{
   
    vec3 ambient = vec3(0.2) * vec3(0.2);
    
    
    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDirection = normalize(-lightDir);
    float diff = max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diff*vec3(1.0)* vec3(0.4);
    
   
    vec3 result =  ambient  + diffuse;
    FragColor = vec4(result,1.0);


}