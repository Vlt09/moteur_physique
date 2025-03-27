#pragma once

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

// Represents a single vertex of a quad
struct QuadVertex
{

  QuadVertex(
      const glm::vec3 &pos, const glm::vec3 &norm, const glm::vec2 &tex) :
      position{pos}, normal{norm}, texCoords{tex}
  {
  }

  static unsigned long sizeOfVertex()
  {
    return sizeof(position) + sizeof(normal) + sizeof(texCoords);
  }

  glm::vec3 position;  // Vertex position
  glm::vec3 normal;    // Normal vector
  glm::vec2 texCoords; // Texture coordinates
};

// Represents a discretized quad (rectangle) in local space.
// Its default plane lies on the XZ plane, centered at (0, 0, 0) in local
// coordinates. The quad has texture coordinates and normal vectors.
class QuadCustom
{
public:
  // Constructor: Allocates data and builds the vertex attributes.
  QuadCustom(GLfloat width, GLfloat height) : m_nVertexCount(0)
  {
    build(width, height); // Build method (implementation in the .cpp)
  }

  // Returns a pointer to the data
  const QuadVertex *getDataPointer() const { return &m_Vertices[0]; }

  // Returns the number of vertices
  GLsizei getVertexCount() const { return m_nVertexCount; }

  const unsigned long getVertexSize() const
  {
    return QuadVertex::sizeOfVertex();
  }

  void initObj(GLuint vPos, GLuint vNorm, GLuint vTex)
  {
    initVboPointer();
    initVaoPointer(vPos, vNorm, vTex);
  }

  void draw(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix,
      const glm::mat4 &projMatrix, GLuint modelMatrixLocation,
      GLuint modelViewProjMatrixLocation, GLuint modelViewMatrixLocation,
      GLuint normalMatrixLocation)
  {
    const auto mvMatrix = viewMatrix * modelMatrix;
    const auto mvpMatrix = projMatrix * mvMatrix;
    const auto normalMatrix = glm::transpose(glm::inverse(mvMatrix));
    glUniformMatrix4fv(
        modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(
        modelViewProjMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv(
        modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
    glUniformMatrix4fv(
        normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

private:
  // Builds the quad data
  void build(GLfloat width, GLfloat height)
  {
    GLfloat halfWidth = width / 2.f;
    GLfloat halfHeight = height / 2.f;

    // Define the vertices for a quad (two triangles forming a rectangle)
    QuadVertex vertices[] = {
        {{-halfWidth, 0.f, -halfHeight}, {0.f, 1.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, 0.f, -halfHeight}, {0.f, 1.f, 0.f},
            {1.f, 0.f}}, // Bottom-right
        {{halfWidth, 0.f, halfHeight}, {0.f, 1.f, 0.f},
            {1.f, 1.f}}, // Top-right

        {{-halfWidth, 0.f, -halfHeight}, {0.f, 1.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, 0.f, halfHeight}, {0.f, 1.f, 0.f},
            {1.f, 1.f}}, // Top-right
        {{-halfWidth, 0.f, halfHeight}, {0.f, 1.f, 0.f}, {0.f, 1.f}} // Top-left
    };

    m_Vertices.assign(vertices, vertices + 6); // Add all vertices to the vector
    m_nVertexCount = 6; // Two triangles (6 vertices total)
  }

  // Initializes VAO pointers for position, normal, and texture coordinates
  void initVaoPointer(GLuint vPos, GLuint vNorm, GLuint vTex)
  {
    std::cout << "quad vao init" << std::endl;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(vPos);
    glEnableVertexAttribArray(vNorm);
    glEnableVertexAttribArray(vTex);
    glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(QuadVertex, position));
    glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(QuadVertex, normal));
    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(QuadVertex, texCoords));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void initVboPointer()
  {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, getVertexCount() * getVertexSize(),
        getDataPointer(), GL_STATIC_DRAW);
  }

  std::vector<QuadVertex> m_Vertices;
  GLsizei m_nVertexCount; // Number of vertices
  GLuint vbo;
  GLuint vao;
};