#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <vector>

#include "common/geometry/plane/mesh.h"

namespace opengl::gui {
class FontFactory {
 public:
  struct TypeFace {
    geometry::plane::Mesh mesh;
    float advance{};
  };

  explicit FontFactory(const char *font_file, int arc_precision);
  ~FontFactory();

  std::vector<glm::vec2> GetCharMesh(char32_t c, float height, float *advance);
  std::vector<glm::vec2> GetStringMesh(const std::u32string &str,
                                       float height,
                                       float *advance);
  float GetStringAdvance(const std::u32string &str);

 private:
  const TypeFace &LoadCharMesh(char32_t c);

  FT_Library ft_library_{};
  FT_Face ft_face_{};
  int arc_precision_{};
  std::map<char32_t, TypeFace> char_mesh_;
};
}  // namespace opengl::gui
