#pragma once

#include "glad/glad.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <vector>

// Represents a single vertex of a cube
struct CubeVertex
{

  CubeVertex(
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

class CubeCustom
{
public:
  // Constructor: Allocates data and builds the vertex attributes.
  CubeCustom(GLfloat width, GLfloat height, GLfloat depth) : m_nVertexCount(0)
  {
    build(width, height, depth); // Build method (implementation in the .cpp)
  }

  // Returns a pointer to the data
  const CubeVertex *getDataPointer() const { return &m_Vertices[0]; }

  // Returns the number of vertices
  GLsizei getVertexCount() const { return m_nVertexCount; }

  const unsigned long getVertexSize() const
  {
    return CubeVertex::sizeOfVertex();
  }

  void initObj(GLuint vPos, GLuint vNorm, GLuint vTex)
  {
    initVboPointer();
    initVaoPointer(vPos, vNorm, vTex);
  }

  void draw(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix,
      const glm::mat4 &projMatrix, GLuint modelMatrixLocation,
      GLuint modelViewProjMatrixLocation, GLuint modelViewMatrixLocation,
      GLuint normalMatrixLocation, GLuint glId)
  // TO DO use a struct to pass all args
  {
    getUniform(glId, modelMatrixLocation, modelViewProjMatrixLocation,
        modelViewMatrixLocation, normalMatrixLocation);
    const auto mvMatrix = viewMatrix * modelMatrix;
    const auto mvpMatrix = projMatrix * mvMatrix;
    const auto normalMatrix = glm::transpose(glm::inverse(mvMatrix));
    glUniformMatrix4fv(
        modelViewProjMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvpMatrix));
    glUniformMatrix4fv(
        modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(mvMatrix));
    // glUniformMatrix4fv(
    //     normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    //     Error here don't uncomment
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

  void getUniform(GLuint glId, GLuint modelMatrixLocation,
      GLuint modelViewProjMatrixLocation, GLuint modelViewMatrixLocation,
      GLuint normalMatrixLocation)
  {
    modelMatrixLocation = glGetUniformLocation(glId, "uModelMatrix");
    modelViewProjMatrixLocation =
        glGetUniformLocation(glId, "uModelViewProjMatrix");
    modelViewMatrixLocation = glGetUniformLocation(glId, "uModelViewMatrix");
    normalMatrixLocation = glGetUniformLocation(glId, "uNormalMatrix");
  }

private:
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
        (GLvoid *)offsetof(CubeVertex, position));
    glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(CubeVertex, normal));
    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(CubeVertex, texCoords));
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
  // Builds the cube data
  void build(GLfloat width, GLfloat height, GLfloat depth)
  {
    GLfloat halfWidth = width / 2.f;
    GLfloat halfHeight = height / 2.f;
    GLfloat halfDepth = depth / 2.f;

    // Define the vertices for a cube (12 triangles forming 6 rectangle faces)
    CubeVertex vertices[] = {
        // Front face
        {{-halfWidth, -halfHeight, halfDepth}, {0.f, 0.f, 1.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, -halfHeight, halfDepth}, {0.f, 0.f, 1.f},
            {1.f, 0.f}}, // Bottom-right
        {{halfWidth, halfHeight, halfDepth}, {0.f, 0.f, 1.f},
            {1.f, 1.f}}, // Top-right

        {{-halfWidth, -halfHeight, halfDepth}, {0.f, 0.f, 1.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, halfHeight, halfDepth}, {0.f, 0.f, 1.f},
            {1.f, 1.f}}, // Top-right
        {{-halfWidth, halfHeight, halfDepth}, {0.f, 0.f, 1.f},
            {0.f, 1.f}}, // Top-left

        // Back face
        {{halfWidth, -halfHeight, -halfDepth}, {0.f, 0.f, -1.f},
            {0.f, 0.f}}, // Bottom-left
        {{-halfWidth, -halfHeight, -halfDepth}, {0.f, 0.f, -1.f},
            {1.f, 0.f}}, // Bottom-right
        {{-halfWidth, halfHeight, -halfDepth}, {0.f, 0.f, -1.f},
            {1.f, 1.f}}, // Top-right

        {{halfWidth, -halfHeight, -halfDepth}, {0.f, 0.f, -1.f},
            {0.f, 0.f}}, // Bottom-left
        {{-halfWidth, halfHeight, -halfDepth}, {0.f, 0.f, -1.f},
            {1.f, 1.f}}, // Top-right
        {{halfWidth, halfHeight, -halfDepth}, {0.f, 0.f, -1.f},
            {0.f, 1.f}}, // Top-left

        // Left face
        {{-halfWidth, -halfHeight, -halfDepth}, {-1.f, 0.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{-halfWidth, -halfHeight, halfDepth}, {-1.f, 0.f, 0.f},
            {1.f, 0.f}}, // Bottom-right
        {{-halfWidth, halfHeight, halfDepth}, {-1.f, 0.f, 0.f},
            {1.f, 1.f}}, // Top-right

        {{-halfWidth, -halfHeight, -halfDepth}, {-1.f, 0.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{-halfWidth, halfHeight, halfDepth}, {-1.f, 0.f, 0.f},
            {1.f, 1.f}}, // Top-right
        {{-halfWidth, halfHeight, -halfDepth}, {-1.f, 0.f, 0.f},
            {0.f, 1.f}}, // Top-left

        // Right face
        {{halfWidth, -halfHeight, halfDepth}, {1.f, 0.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, -halfHeight, -halfDepth}, {1.f, 0.f, 0.f},
            {1.f, 0.f}}, // Bottom-right
        {{halfWidth, halfHeight, -halfDepth}, {1.f, 0.f, 0.f},
            {1.f, 1.f}}, // Top-right

        {{halfWidth, -halfHeight, halfDepth}, {1.f, 0.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, halfHeight, -halfDepth}, {1.f, 0.f, 0.f},
            {1.f, 1.f}}, // Top-right
        {{halfWidth, halfHeight, halfDepth}, {1.f, 0.f, 0.f},
            {0.f, 1.f}}, // Top-left

        // Top face
        {{-halfWidth, halfHeight, halfDepth}, {0.f, 1.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, halfHeight, halfDepth}, {0.f, 1.f, 0.f},
            {1.f, 0.f}}, // Bottom-right
        {{halfWidth, halfHeight, -halfDepth}, {0.f, 1.f, 0.f},
            {1.f, 1.f}}, // Top-right

        {{-halfWidth, halfHeight, halfDepth}, {0.f, 1.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, halfHeight, -halfDepth}, {0.f, 1.f, 0.f},
            {1.f, 1.f}}, // Top-right
        {{-halfWidth, halfHeight, -halfDepth}, {0.f, 1.f, 0.f},
            {0.f, 1.f}}, // Top-left

        // Bottom face
        {{-halfWidth, -halfHeight, -halfDepth}, {0.f, -1.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, -halfHeight, -halfDepth}, {0.f, -1.f, 0.f},
            {1.f, 0.f}}, // Bottom-right
        {{halfWidth, -halfHeight, halfDepth}, {0.f, -1.f, 0.f},
            {1.f, 1.f}}, // Top-right

        {{-halfWidth, -halfHeight, -halfDepth}, {0.f, -1.f, 0.f},
            {0.f, 0.f}}, // Bottom-left
        {{halfWidth, -halfHeight, halfDepth}, {0.f, -1.f, 0.f},
            {1.f, 1.f}}, // Top-right
        {{-halfWidth, -halfHeight, halfDepth}, {0.f, -1.f, 0.f},
            {0.f, 1.f}} // Top-left
    };

    m_Vertices.assign(
        vertices, vertices + 36); // Add all vertices to the vector
    m_nVertexCount =
        36; // Six faces * 2 triangles per face * 3 vertices per triangle
  }

  std::vector<CubeVertex> m_Vertices;
  GLsizei m_nVertexCount; // Number of vertices
  GLuint vao, vbo;
};