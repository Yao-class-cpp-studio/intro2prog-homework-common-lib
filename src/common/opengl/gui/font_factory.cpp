#include "common/opengl/gui/font_factory.h"

#include "common/geometry/plane/line.h"
#include "common/geometry/plane/mesh.h"
#include "common/opengl/gui/interpolation.h"
#include "common/opengl/util.h"

namespace opengl::gui {

namespace {
void FTErrorCheck(FT_Error error,
                  const char *file,
                  int line,
                  const char *code) {
  if (error) {
    LOG_ERROR("({}:{}) Freetype.{} : {}", file, line, error, code);
  }
}

#define FTCall(x) opengl::gui::FTErrorCheck(x, __FILE__, __LINE__, #x)

template <class Ty>
Ty BezierInterpolate(std::vector<Ty> control_points, float x) {
  for (size_t level = control_points.size() - 1; level; level--) {
    for (size_t i = 0; i < level; i++) {
      control_points[i] = Mix(control_points[i], control_points[i + 1], x);
    }
  }
  return control_points[0];
}
}  // namespace

FontFactory::FontFactory(const char *font_file, int arc_precision) {
  FTCall(FT_Init_FreeType(&ft_library_));
  FTCall(FT_New_Face(ft_library_, font_file, 0, &ft_face_));
  FTCall(FT_Set_Pixel_Sizes(ft_face_, 0, 1000));
  arc_precision_ = arc_precision;
}

FontFactory::~FontFactory() {
  FTCall(FT_Done_Face(ft_face_));
  FTCall(FT_Done_FreeType(ft_library_));
}

std::vector<glm::vec2> FontFactory::GetCharMesh(char32_t c,
                                                float height,
                                                float *advance) {
  height *= 1e-3;
  std::vector<glm::vec2> vpos;

  const TypeFace &type_face = LoadCharMesh(c);
  for (auto triangle : type_face.mesh.GetTriangles()) {
    vpos.emplace_back(triangle.v0 * height);
    vpos.emplace_back(triangle.v1 * height);
    vpos.emplace_back(triangle.v2 * height);
  }

  if (advance) {
    *advance = type_face.advance;
  }

  return vpos;
}

std::vector<glm::vec2> FontFactory::GetStringMesh(const std::u32string &str,
                                                  float height,
                                                  float *advance) {
  height *= 1e-3;
  std::vector<glm::vec2> vpos;
  float adv = 0.0;
  for (auto c : str) {
    const TypeFace &type_face = LoadCharMesh(c);
    for (auto triangle : type_face.mesh.GetTriangles()) {
      vpos.emplace_back(triangle.v0 * height + glm::vec2(adv, 0.0));
      vpos.emplace_back(triangle.v1 * height + glm::vec2(adv, 0.0));
      vpos.emplace_back(triangle.v2 * height + glm::vec2(adv, 0.0));
    }
    adv += type_face.advance * height;
  }

  if (advance) {
    *advance = adv;
  }

  return vpos;
}

const FontFactory::TypeFace &FontFactory::LoadCharMesh(char32_t c) {
  if (!char_mesh_.count(c)) {
    FTCall(FT_Load_Char(ft_face_, c, FT_LOAD_DEFAULT));
    FT_Outline outline = ft_face_->glyph->outline;

    //            LOG_INFO("n_points {}", outline.n_points);
    //            LOG_INFO("n_contours {}", outline.n_contours);

    //            for (int i = 0; i < outline.n_contours; i++) {
    //                LOG_INFO("{}", outline.contours[i]);
    //            }
    //
    //            for (int i = 0; i < outline.n_points; i++) {
    //                LOG_INFO("{} {} {:#x}", outline.points[i].x,
    //                outline.points[i].y, outline.tags[i]);
    //            }

    int start = 0;

    auto mesh = geometry::plane::Mesh{};
    std::vector<std::vector<glm::vec2>> contours;
    for (int i = 0; i < outline.n_contours;
         start = outline.contours[i] + 1, i++) {
      std::vector<glm::vec2> contour_vecs;
      std::vector<char> contour_tags;
      for (int j = start; j <= outline.contours[i]; j++) {
        auto point = outline.points[j];
        contour_vecs.emplace_back(float(point.x) / 64.0f,
                                  float(point.y) / 64.0f);
        contour_tags.push_back(outline.tags[j]);
      }

      int first_on = 0;
      while (!(contour_tags[first_on] & 1)) {
        first_on++;
      }

      if (first_on) {
        auto size = contour_tags.size();
        for (int j = 0; j < first_on; j++) {
          contour_tags.push_back(contour_tags[j]);
          contour_vecs.push_back(contour_vecs[j]);
        }
        for (int j = 0; j < size; j++) {
          contour_tags[j] = contour_tags[j + first_on];
          contour_vecs[j] = contour_vecs[j + first_on];
        }
        contour_tags.resize(size);
        contour_vecs.resize(size);
      }

      std::vector<glm::vec2> stacked_vecs{contour_vecs[0]};

      std::vector<glm::vec2> outline_vecs;
      for (int j = 1; j <= contour_vecs.size(); j++) {
        int true_j = ((j == contour_vecs.size()) ? 0 : j);
        auto vec = contour_vecs[true_j];
        if (contour_tags[true_j] & 1) {
          if (stacked_vecs.size() == 1) {
            // vpos.push_back(glm::vec3{stacked_vecs[0], 1.0});
            outline_vecs.push_back(vec);
          } else {
            stacked_vecs.push_back(vec);
            int precision = arc_precision_;
            auto last_v = stacked_vecs[0];
            for (int k = 1; k <= precision; k++) {
              // vpos.push_back(glm::vec3{last_v, 1.0});
              last_v =
                  BezierInterpolate(stacked_vecs, float(k) / float(precision));
              outline_vecs.push_back(last_v);
            }
            stacked_vecs.resize(1);
          }
          stacked_vecs[0] = vec;
        } else {
          stacked_vecs.push_back(vec);
        }
      }

      contours.push_back(std::move(outline_vecs));
    }

    auto cross = [](const glm::vec2 &v0, const glm::vec2 &v1) {
      return v0.x * v1.y - v0.y * v1.x;
    };

    auto signal = [](float x) {
      if (x < 0.0)
        return -1;
      else if (x > 0.0)
        return 1;
      return 0;
    };

    auto directional_angle = [cross, signal](const glm::vec2 &v0,
                                             const glm::vec2 &v1) {
      auto lv0 = glm::length(v0);
      auto lv1 = glm::length(v1);
      if (lv0 < 1e-8 || lv1 < 1e-8) {
        return 0.0f;
      }
      return std::acos(
                 std::max(std::min(dot(v0, v1) / (lv0 * lv1), 1.0f), -1.0f)) *
             ((signal(cross(v0, v1)) == -1) ? -1.0f : 1.0f);
    };

    auto contour_area = [cross](const std::vector<glm::vec2> &contour) {
      float area = 0.0;
      int sz = int(contour.size());
      for (int i = 1; i < sz - 1; i++) {
        area += cross(contour[i] - contour[0], contour[i + 1] - contour[0]);
      }
      return area;
    };

    auto rotate_vector = [](std::vector<glm::vec2> &contour, int offset) {
      int sz = int(contour.size());
      for (int i = 0; i < offset; i++) {
        contour.push_back(contour[i]);
      }
      for (int i = 0; i < sz; i++) {
        contour[i] = contour[i + offset];
      }
      contour.resize(sz);
    };

    auto surround_pos_check = [directional_angle](
                                  const std::vector<glm::vec2> &contour,
                                  const glm::vec2 &pos) {
      float angle = 0.0;
      int sz = int(contour.size());
      for (int i = 0; i < sz; i++) {
        int j = (i + 1) % sz;
        angle += directional_angle(contour[i] - pos, contour[j] - pos);
      }
      // LOG_INFO("{}", std::abs(angle));
      return std::abs(std::abs(angle) - float(LAND_PI * 2.0f)) <
             std::abs(std::abs(angle));
    };

    auto surround_check = [surround_pos_check](
                              const std::vector<glm::vec2> &outer,
                              const std::vector<glm::vec2> &inner) {
      return surround_pos_check(outer, inner[0]);
    };

    using namespace geometry::plane;

    auto is_intersect = [signal](const Segment &seg0, const Segment &seg1) {
      Line line0{seg0};
      Line line1{seg1};
      return (std::abs(signal(line0(seg1.u)) - signal(line0(seg1.v))) == 2) &&
             (std::abs(signal(line1(seg0.u)) - signal(line1(seg0.v))) == 2);
    };

    auto merge_contours = [contour_area, rotate_vector, is_intersect, &contours,
                           signal,
                           cross](std::vector<glm::vec2> &outer_contour,
                                  std::vector<glm::vec2> &inner_contour) {
      auto outer_area = contour_area(outer_contour);
      auto inner_area = contour_area(inner_contour);
      if (outer_area * inner_area > 0.0) {
        std::reverse(inner_contour.begin(), inner_contour.end());
      }

      int outer_index = -1;
      int inner_index = -1;
      float dist = 1e30f;

      for (int i = 0; i < outer_contour.size(); i++) {
        int i_1 =
            (i - 1 + int(outer_contour.size())) % int(outer_contour.size());
        int i1 = (i + 1) % int(outer_contour.size());
        for (int j = 0; j < inner_contour.size(); j++) {
          if (std::abs(signal(cross(outer_contour[i] - outer_contour[i_1],
                                    inner_contour[j] - outer_contour[i_1])) -
                       signal(outer_area)) == 2)
            continue;
          if (std::abs(signal(cross(outer_contour[i] - inner_contour[j],
                                    outer_contour[i1] - inner_contour[j])) -
                       signal(outer_area)) == 2)
            continue;
          Segment seg{outer_contour[i], inner_contour[j]};
          bool pass = true;
          for (auto &contour : contours) {
            for (int k = 0; k < contour.size(); k++) {
              int k1 = (k + 1) % int(contour.size());
              if (is_intersect(seg, Segment{contour[k], contour[k1]})) {
                pass = false;
                break;
              }
            }
            if (!pass)
              break;
          }
          if (pass) {
            float this_dist = glm::length(seg.u - seg.v);
            if (this_dist < dist) {
              dist = this_dist;
              outer_index = i;
              inner_index = j;
            }
          }
        }
      }

      rotate_vector(outer_contour, outer_index);
      rotate_vector(inner_contour, inner_index);
      outer_contour.push_back(outer_contour[0]);
      inner_contour.push_back(inner_contour[0]);
      for (auto &v : inner_contour) {
        outer_contour.push_back(v);
      }
    };

    int inner_index, outer_index;
    while (true) {
      inner_index = -1;
      int sz = int(contours.size());
      for (int i = 0; i < sz; i++) {
        int surrounded_by = -1;
        int surrounded_cnt = 0;
        for (int j = 0; j < sz; j++) {
          if (i != j) {
            if (surround_check(contours[j], contours[i])) {
              // LOG_INFO("{}<-{}", j, i);
              surrounded_cnt++;
              surrounded_by = j;
            }
          }
        }
        if (surrounded_cnt == 1) {
          outer_index = surrounded_by;
          inner_index = i;
        }
      }
      if (inner_index != -1) {
        // LOG_INFO("{}<={}", outer_index, inner_index);
        merge_contours(contours[outer_index], contours[inner_index]);
        contours[inner_index] = contours[contours.size() - 1];
        contours.pop_back();
      } else {
        break;
      }
    }

    for (auto &contour : contours) {
      mesh += geometry::plane::Mesh{contour};
    }

    char_mesh_[c].mesh = std::move(mesh);
    char_mesh_[c].advance = float(ft_face_->glyph->advance.x) / 64.0f;
  }
  return char_mesh_.at(c);
}

}  // namespace opengl::gui
