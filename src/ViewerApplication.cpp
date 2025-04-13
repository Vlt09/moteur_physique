#include "ViewerApplication.hpp"

#include <iostream>
#include <numeric>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>

#include "Link.hpp"
#include "PMat.hpp"
#include "utils/Cylinder.hpp"
#include "utils/Sphere.hpp"
#include "utils/cameras.hpp"
#include "utils/cube.hpp"
#include "utils/quad.hpp"
#include "utils/skybox.hpp"

#include <stb_image.h>
#include <stb_image_write.h>
#include <tiny_gltf.h>

#define FLAG3D true

GLuint modelMatrixLocation, modelViewProjMatrixLocation,
    modelViewMatrixLocation, normalMatrixLocation, uLightDirection,
    uLightIntensity, uBaseColorTexture, uBaseColorFactor, uMetallicFactor,
    uMetallicRoughnessTexture, uRoughnessFactor, uEmissiveFactor,
    uEmissiveTexture, uApplyOcclusion, uOcclusionFactor, uOcclusionTexture,
    uNormalTexture;

static GFLvector g{0.0f, 0.0f};
static glm::vec2 g_f{0.0f, 0.0f};

void anim(std::vector<std::shared_ptr<PMat>> &pmats, std::vector<Link> &links,
    std::vector<Sphere> &spheres, float h)
{
  for (int i = 0; i < links.size(); i++) {
    links[i].update();
  }

  for (int i = 0; i < pmats.size(); i++) {
    pmats[i]->update(h);

    auto pos = pmats[i]->position();
    spheres[i].addTranslation(glm::vec3(pos.x, pos.y, pos.z));
  }
}

void moveParticle(std::vector<std::shared_ptr<PMat>> &pmats)
{
  GFLvector t = (GFLvector){-9.5f, 0.0f};
  pmats[0]->addForce(t);
}

void initFlag(float k, float z, std::vector<std::shared_ptr<PMat>> &pmats,
    std::vector<Link> &links, std::vector<Sphere> &spheres)
{
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      if (j == 0) {
        GFLpoint pos{0.005, 0.005 - (i * 0.05), 0.0};

        auto pmat = std::make_shared<PMat>(PMat(pos, FLAG3D));
        pmats.emplace_back(pmat);
        links.emplace_back(Link::Extern_Force(*pmat, &g, FLAG3D));

        // Particle
        Sphere sphere(0.005f, 16, 16);
        sphere.addTranslation(glm::vec3(pos.x, pos.y, pos.z));
        spheres.emplace_back(sphere);

      } else {
        GFLpoint pos{0.005 + (j * 0.05), 0.005 - (i * 0.05), 0.0};

        auto pmat = std::make_shared<PMat>(PMat(
            1., pos, (GFLvector){0.f, 0.f}, PMat::Model::LEAP_FROG, FLAG3D));
        pmats.emplace_back(pmat);
        links.emplace_back(Link::Hook_Spring(
            *pmats[(i * 10) + (j - 1)], *pmats[(i * 10) + j], k, z));
        links.emplace_back(Link::Extern_Force(*pmat, &g, FLAG3D));

        Sphere sphere(0.005f, 16, 16);
        sphere.addTranslation(glm::vec3(pos.x, pos.y, pos.z));
        spheres.emplace_back(sphere);
      }
    }
  }
  std::cout << "links.size = " << links.size() << std::endl;
}

void keyCallback(
    GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE) {
    glfwSetWindowShouldClose(window, 1);
  }
}

std::vector<GLuint> ViewerApplication::createBufferObjects(
    const tinygltf::Model &model)
{
  std::vector<GLuint> bufferObjects(
      model.buffers.size(), 0); // Assuming buffers is a std::vector of Buffer
  glGenBuffers(model.buffers.size(), bufferObjects.data());
  for (size_t i = 0; i < model.buffers.size(); ++i) {
    glBindBuffer(GL_ARRAY_BUFFER, bufferObjects[i]);
    glBufferStorage(GL_ARRAY_BUFFER,
        model.buffers[i].data.size(), // Assume a Buffer has a data member
                                      // variable of type std::vector
        model.buffers[i].data.data(), 0);
  }
  glBindBuffer(
      GL_ARRAY_BUFFER, 0); // Cleanup the binding point after the loop only

  return bufferObjects;
}

void getUniform(const GLProgram &glslProgram)
{
  modelMatrixLocation =
      glGetUniformLocation(glslProgram.glId(), "uModelMatrix");
  modelViewProjMatrixLocation =
      glGetUniformLocation(glslProgram.glId(), "uModelViewProjMatrix");
  modelViewMatrixLocation =
      glGetUniformLocation(glslProgram.glId(), "uModelViewMatrix");
  normalMatrixLocation =
      glGetUniformLocation(glslProgram.glId(), "uNormalMatrix");
}

int ViewerApplication::run()
{
  // Loader shaders
  auto glslProgram = compileProgram({m_ShadersRootPath / m_vertexShader,
      m_ShadersRootPath / m_fragmentShader});

  // Build projection matrix
  const auto diag = glm::vec3(1., 1., 1);
  auto maxDistance = glm::length(diag);
  const auto projMatrix =
      glm::perspective(70.f, float(m_nWindowWidth) / m_nWindowHeight,
          0.001f * maxDistance, 1.5f * maxDistance);

  std::unique_ptr<CameraController> cameraController =
      std::make_unique<FirstPersonCameraController>(
          m_GLFWHandle.window(), 0.5f * maxDistance);
  if (m_hasUserCamera) {
    cameraController->setCamera(m_userCamera);
  } else {
    const auto center = glm::vec3(0.f, 0.f, 0.f);
    const auto up = glm::vec3(0, 1, 0);
    const auto eye = glm::vec3(1.f, 1.f, 1.f);
    cameraController->setCamera(
        Camera{glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0)});
  }

  // const auto textureObjects = createTextureObjects(model);

  // Gen default texture for object
  float white[] = {1., 1., 1., 1.};
  GLuint whiteTexture;
  glGenTextures(1, &whiteTexture);
  glBindTexture(GL_TEXTURE_2D, whiteTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, white);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);

  // Setup OpenGL state for rendering
  glEnable(GL_DEPTH_TEST);
  glslProgram.use();

  getUniform(glslProgram);

  const auto pathToFaces = "assets/";

  const std::vector<std::string> faces{"../../assets/skybox/right.jpg",
      "../../assets/skybox/left.jpg", "../../assets/skybox/top.jpg",
      "../../assets/skybox/bottom.jpg", "../../assets/skybox/front.jpg",
      "../../assets/skybox/back.jpg"};

  float h, Fe, Fa, tempo;
  Fe = 1000;
  h = 1. / (float)Fe;

  float k = 0.000001 * SQR(Fe); // on suppose que m = 1
  float z = 0.00005 * (float)Fe;

  QuadCustom quad(1, 1);
  CubeCustom cube(1, 1, 1);
  Sphere sphere(0.005f, 16, 16);
  std::vector<std::shared_ptr<PMat>> pmats;
  std::vector<Sphere> spheres;
  std::vector<Link> links;

  Skybox skybox(faces, cube, m_ShadersRootPath);

  initFlag(k, z, pmats, links, spheres);
  quad.initObj(0, 1, 2);
  // cube.initObj(0, 1, 2);

  const auto drawScene = [&](const Camera &camera) {
    glViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const auto viewMatrix = camera.getViewMatrix();
    const auto modelMatrix = glm::mat4(1.0f);

    glslProgram.use();

    getUniform(glslProgram);

    for (auto &s : spheres) {
      s.draw(modelMatrix, viewMatrix, projMatrix, modelMatrixLocation,
          modelViewProjMatrixLocation, modelViewMatrixLocation,
          normalMatrixLocation);
    }
  };

  // Uniform variable for light
  glm::vec3 lightDirection(1.f, 1.f, 1.f);
  glm::vec3 lightIntensity(1.f, 1.f, 1.f);
  glm::vec3 color = {1.f, 1.f, 1.f};
  float theta = 1.f;
  float phi = 1.f;
  bool lightCam = false;

  // Uniform variable for occlusion
  bool occlusionState = true;

  // Loop until the user closes the window
  for (auto iterationCount = 0u; !m_GLFWHandle.shouldClose();
      ++iterationCount) {
    const auto seconds = glfwGetTime();

    const auto camera = cameraController->getCamera();

    drawScene(camera);
    anim(pmats, links, spheres, h);
    std::cout << "sphere 0 position = " << pmats[0]->position().x << std::endl;

    // GUI code:
    imguiNewFrame();

    {
      ImGui::Begin("GUI");
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
          1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("eye: %.3f %.3f %.3f", camera.eye().x, camera.eye().y,
            camera.eye().z);
        ImGui::Text("center: %.3f %.3f %.3f", camera.center().x,
            camera.center().y, camera.center().z);
        ImGui::Text(
            "up: %.3f %.3f %.3f", camera.up().x, camera.up().y, camera.up().z);

        ImGui::Text("front: %.3f %.3f %.3f", camera.front().x, camera.front().y,
            camera.front().z);
        ImGui::Text("left: %.3f %.3f %.3f", camera.left().x, camera.left().y,
            camera.left().z);

        ImGui::SliderFloat("Adjust Fa value", &Fa, 0.0f, 1.0f);
        ImGui::SliderFloat("Adjust tempo value", &tempo, 0.0f, 300.0f);
        if (ImGui::SliderFloat(
                "Adjust gravity x value", &(g_f.x), 0.0f, 9.0f)) {
          g.x = (float)g_f.x;
        }

        if (ImGui::SliderFloat(
                "Adjust gravity y value", &(g_f.y), 0.0f, 9.0f)) {
          g.y = (float)g_f.y;
        }

        if (ImGui::Button("Move Particle")) {
          moveParticle(pmats);
        }

        ImGui::End();
      }
    }

    imguiRenderFrame();

    glfwPollEvents(); // Poll for and process events

    auto ellapsedTime = glfwGetTime() - seconds;
    auto guiHasFocus =
        ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
    if (!guiHasFocus) {
      cameraController->update(float(ellapsedTime));
    }

    m_GLFWHandle.swapBuffers(); // Swap front and back buffers
  }

  return 0;
}

ViewerApplication::ViewerApplication(
    const fs::path &appPath, uint32_t width, uint32_t height) :
    m_nWindowWidth(width),
    m_nWindowHeight(height),
    m_AppPath{appPath},
    m_AppName{m_AppPath.stem().string()},
    m_ImGuiIniFilename{m_AppName + ".imgui.ini"},
    m_ShadersRootPath{m_AppPath.parent_path() / "shaders"}
{

  ImGui::GetIO().IniFilename =
      m_ImGuiIniFilename.c_str(); // At exit, ImGUI will store its windows
                                  // positions in this file

  glfwSetKeyCallback(m_GLFWHandle.window(), keyCallback);

  printGLVersion();
}