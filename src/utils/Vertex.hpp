#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

struct Vertex
{
  glm::vec3 m_Position;
  glm::vec3 m_Normal;
  glm::vec2 m_TexCoords;

  static unsigned long sizeOfVertex()
  {
    return sizeof(position) + sizeof(normal) + sizeof(texCoords);
  }
};