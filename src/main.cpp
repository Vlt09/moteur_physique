#include "ViewerApplication.hpp"
#include "utils/GLFWHandle.hpp"
#include "utils/filesystem.hpp"

#include <args.hxx>

std::vector<std::string> split(
    const std::string &str, const std::string &delim);

int main(int argc, char **argv)
{
  auto returnCode = 0;

  uint32_t width = 1280;
  uint32_t height = 720;

  ViewerApplication app{fs::path{argv[0]}, width, height};
  returnCode = app.run();
  return returnCode;
}

std::vector<std::string> split(const std::string &str, const std::string &delim)
{
  std::vector<std::string> tokens;
  size_t prev = 0, pos = 0;
  do {
    pos = str.find(delim, prev);
    if (pos == std::string::npos)
      pos = str.length();
    std::string token = str.substr(prev, pos - prev);
    if (!token.empty())
      tokens.push_back(token);
    prev = pos + delim.length();
  } while (pos < str.length() && prev < str.length());
  return tokens;
}