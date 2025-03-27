#pragma once

#include "Vertex.hpp"
#include "shaders.hpp"

#include <vector>

class Sphere
{
public:
  // Constructeur: alloue le tableau de données et construit les attributs des
  // vertex
  Sphere(GLfloat radius, GLsizei discLat, GLsizei discLong) : m_nVertexCount(0)
  {
    build(radius, discLat, discLong); // Construction (voir le .cpp)
  }

  // Renvoit le pointeur vers les données
  const Vertex *getDataPointer() const { return &m_Vertices[0]; }

  // Renvoit le nombre de vertex
  GLsizei getVertexCount() const { return m_nVertexCount; }

  const unsigned long getVertexSize() const { return Vertex::sizeOfVertex(); }

  void initVaoPointer(GLuint vPos, GLuint vNorm, GLuint vTex) const
  {
    std::cout << "sphere vao init" << std::endl;
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
  // Alloue et construit les données (implantation dans le .cpp)
  void build(GLfloat radius, GLsizei discLat, GLsizei discLong)
  {
    // Equation paramétrique en (r, phi, theta) de la sphère
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= phi <= 2PI
    //
    // x(r, phi, theta) = r sin(phi) cos(theta)
    // y(r, phi, theta) = r sin(theta)
    // z(r, phi, theta) = r cos(phi) cos(theta)
    //
    // Discrétisation:
    // dPhi = 2PI / discLat, dTheta = PI / discLong
    //
    // x(r, i, j) = r * sin(i * dPhi) * cos(-PI / 2 + j * dTheta)
    // y(r, i, j) = r * sin(-PI / 2 + j * dTheta)
    // z(r, i, j) = r * cos(i * dPhi) * cos(-PI / 2 + j * dTheta)

    GLfloat rcpLat = 1.f / discLat, rcpLong = 1.f / discLong;
    GLfloat dPhi = 2 * glm::pi<float>() * rcpLat,
            dTheta = glm::pi<float>() * rcpLong;

    std::vector<SphereVertex> data;

    // Construit l'ensemble des vertex
    for (GLsizei j = 0; j <= discLong; ++j) {
      GLfloat cosTheta = cos(-glm::pi<float>() / 2 + j * dTheta);
      GLfloat sinTheta = sin(-glm::pi<float>() / 2 + j * dTheta);

      for (GLsizei i = 0; i <= discLat; ++i) {
        SphereVertex vertex;

        vertex.texCoords.x = i * rcpLat;
        vertex.texCoords.y = 1.f - j * rcpLong;

        vertex.normal.x = sin(i * dPhi) * cosTheta;
        vertex.normal.y = sinTheta;
        vertex.normal.z = cos(i * dPhi) * cosTheta;

        vertex.position = radius * vertex.normal;

        data.push_back(vertex);
      }
    }

    m_nVertexCount = discLat * discLong * 6;

    // Construit les vertex finaux en regroupant les données en triangles:
    // Pour une longitude donnée, les deux triangles formant une face sont de la
    // forme: (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for (GLsizei j = 0; j < discLong; ++j) {
      GLsizei offset = j * (discLat + 1);
      for (GLsizei i = 0; i < discLat; ++i) {
        m_Vertices.push_back(data[offset + i]);
        m_Vertices.push_back(data[offset + (i + 1)]);
        m_Vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
        m_Vertices.push_back(data[offset + i]);
        m_Vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
        m_Vertices.push_back(data[offset + i + discLat + 1]);
      }
    }
  }

  std::vector<Vertex> m_Vertices;
  GLsizei m_nVertexCount; // Nombre de sommets
  GLuint vbo;
  GLuint vao;
};