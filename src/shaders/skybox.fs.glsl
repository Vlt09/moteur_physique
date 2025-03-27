#version 330 core

in vec3 vTexCoords;

uniform samplerCube skybox;

out vec4 FragColor;

void main()
{    
    vec4 tex = texture(skybox, vTexCoords);
    FragColor = tex;
    // FragColor = vec4(tex.x, 1., 1., 1.);
}