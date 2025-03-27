#pragma once
#include "Vertex.hpp"
#include "shaders.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <vector>

// Représente un cylindre discretisé centré en (0, 0, 0) (dans son repère local)
// Son axe vertical est (0, 1, 0) et ses axes transversaux sont (1, 0, 0) et (0,
// 0, 1)
class Cylinder
{
  // Alloue et construit les données (implantation dans le .cpp)
  void build(GLfloat r, GLfloat height, GLsizei discLat, GLsizei discLong)
  {
    GLfloat rcpLat = 1.f / discLat, rcpLong = 1.f / discLong;
    GLfloat dPhi = 2 * glm::pi<float>() * rcpLat, dHeight = height * rcpLong;

    std::vector<Vertex> data;

    for (GLsizei j = 0; j <= discLong; ++j) {
      GLfloat z = -height / 2 + j * dHeight;

      for (GLsizei i = 0; i <= discLat; ++i) {
        Vertex vertex;

        // Coordonnées de texture
        vertex.m_TexCoords.x = i * rcpLat;
        vertex.m_TexCoords.y = 1.f - j * rcpLong;

        // Normales (unité) de la surface du cylindre
        vertex.m_Normal.x = cos(i * dPhi);
        vertex.m_Normal.y = 0.f; // Pas de composante verticale
        vertex.m_Normal.z = sin(i * dPhi);

        // Position (le long de l'axe Z et autour du cercle)
        vertex.m_Position.x = r * vertex.m_Normal.x;
        vertex.m_Position.y = z;
        vertex.m_Position.z = r * vertex.m_Normal.z;

        data.push_back(vertex);
      }
    }

    m_nVertexCount = discLat * discLong * 6;

    GLuint idx = 0;
    // Construit les vertex finaux en regroupant les données en triangles:
    for (GLsizei j = 0; j < discLong; ++j) {
      GLsizei offset = j * (discLat + 1);
      for (GLsizei i = 0; i < discLat; ++i) {
        m_VertexBuffer.push_back(data[offset + i]);
        m_VertexBuffer.push_back(data[offset + (i + 1)]);
        m_VertexBuffer.push_back(data[offset + discLat + 1 + (i + 1)]);
        m_VertexBuffer.push_back(data[offset + i]);
        m_VertexBuffer.push_back(data[offset + discLat + 1 + (i + 1)]);
        m_VertexBuffer.push_back(data[offset + i + discLat + 1]);
      }
    }

    // Init VBO
    glGenBuffers(1, &this->_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
    glBufferData(GL_ARRAY_BUFFER, this->m_VertexBuffer.size() * sizeof(Vertex),
        this->getVertexBuffer(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }

public:
  Cylinder() {}

  Cylinder(GLfloat radius, GLfloat height, GLsizei discLat, GLsizei discLong) :
      m_nVertexCount(0)
  {
    build(radius, height, discLat, discLong);
  }

  const Vertex *getDataPointer() const { return &this->m_VertexBuffer[0]; }

  GLsizei getVertexCount() const { return m_nVertexCount; }

  const unsigned long getVertexSize() const { return Vertex::sizeOfVertex(); }

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

  void initObj(GLuint vPos, GLuint vNorm, GLuint vTex)
  {
    initVboPointer();
    initVaoPointer(vPos, vNorm, vTex);
  }

  void initVaoPointer(GLuint vPos, GLuint vNorm, GLuint vTex) const
  {

    glEnableVertexAttribArray(vPos);
    glEnableVertexAttribArray(vNorm);
    glEnableVertexAttribArray(vTex);
    glVertexAttribPointer(vPos, 3, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(Vertex, position));
    glVertexAttribPointer(vNorm, 3, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(Vertex, normal));
    glVertexAttribPointer(vTex, 2, GL_FLOAT, GL_FALSE, getVertexSize(),
        (GLvoid *)offsetof(Vertex, texCoords));
  }

  void initVboPointer()
  {
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, getVertexCount() * getVertexSize(),
        getDataPointer(), GL_STATIC_DRAW);
  }

private:
  GLsizei m_nVertexCount;
  std::vector<Vertex> m_Vertices;
  GLuint vbo;
  GLuint vao;
};