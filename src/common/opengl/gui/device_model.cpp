#include "common/opengl/gui/device_model.h"

namespace opengl::gui {

DeviceModel::DeviceModel(Model *model) {
  auto vertices = model->GetVertices();
  auto indices = model->GetIndices();
  vertex_buffer_ = std::make_unique<opengl::Buffer>(
      vertices.size() * sizeof(ColorVertex),
      vertices.empty() ? nullptr : vertices.data());
  index_buffer_ = std::make_unique<opengl::Buffer>(
      indices.size() * sizeof(uint32_t),
      indices.empty() ? nullptr : indices.data());
  vertex_array_ = std::make_unique<opengl::VertexArray>(
      vertex_buffer_.get(), std::vector<uint32_t>{GL_RGB32F, GL_RGB32F},
      index_buffer_.get());
  num_index_ = indices.size();
}

void DeviceModel::Update(Model *model) {
  auto vertices = model->GetVertices();
  auto indices = model->GetIndices();
  if (!vertices.empty()) {
    vertex_buffer_->UploadData(0, vertices.size() * sizeof(ColorVertex),
                               vertices.data());
  }
  if (!indices.empty()) {
    index_buffer_->UploadData(0, indices.size() * sizeof(uint32_t),
                              indices.data());
  }
  num_index_ = indices.size();
}

void DeviceModel::Draw(uint32_t draw_type) {
  OpenGLCall(glBindVertexArray, vertex_array_->GetHandle());
  OpenGLCall(glDrawElements, draw_type, static_cast<int>(num_index_),
             GL_UNSIGNED_INT, nullptr);
  OpenGLCall(glBindVertexArray, 0);
}
}  // namespace opengl::gui
