#pragma once

#include "cube.hpp"
#include "shaders.hpp"

#include <stb_image.h>

class Skybox
{
public:
  Skybox(const std::vector<std::string> &faces, CubeCustom &_cube,
      const fs::path &m_ShadersRootPath) :
      cube{_cube}
  {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
      unsigned char *data =
          stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
      if (data) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width,
            height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
      } else {
        std::cout << "Cubemap tex failed to load at path: " << faces[i]
                  << std::endl;
        stbi_image_free(data);
      }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    program = compileProgram({m_ShadersRootPath / "skybox.vs.glsl",
        m_ShadersRootPath / "skybox.fs.glsl"});

    cube.initObj(0, 1, 2);
  }

  // Function that draws a cube and apply the skybox on it
  void draw(const glm::mat4 &modelMatrix, const glm::mat4 &viewMatrix,
      const glm::mat4 &projMatrix, GLuint modelMatrixLocation,
      GLuint modelViewProjMatrixLocation, GLuint modelViewMatrixLocation,
      GLuint normalMatrixLocation)
  {
    glDepthMask(GL_FALSE);
    program.use();
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    const auto skyViewMatrix = glm::mat4(glm::mat3(viewMatrix));
    cube.draw(modelMatrix, skyViewMatrix, projMatrix, modelMatrixLocation,
        modelViewProjMatrixLocation, modelViewMatrixLocation,
        normalMatrixLocation, program.glId());
    glDepthMask(GL_TRUE);
  }

private:
  CubeCustom &cube;
  GLuint textureID;
  GLProgram program;
};