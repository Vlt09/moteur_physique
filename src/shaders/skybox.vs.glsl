#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 vTexCoords;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

void main()
{
    vTexCoords = aPos;
    gl_Position =  uModelViewProjMatrix * vec4(aPos, 1);
}