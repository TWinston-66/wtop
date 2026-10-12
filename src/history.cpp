#include "history.hpp"
#include <cmath>
#include <vector>

void History::push(float value) {
  samples.push_back(value);
  if (samples.size() > cap) {
    samples.pop_front();
  }
}

float History::latest() const {
  return samples.empty() ? 0.0f : samples.back();
}

ftxui::GraphFunction as_graph(const History &h) {
  return [&h](int width, int height) {
    std::vector<int> columns(width, -1);
    auto sample = h.samples.rbegin();
    for (int x = width - 1; x >= 0 && sample != h.samples.rend(); x--, sample++) {
      int y = static_cast<int>(std::lround(*sample * height));
      if (y > 0) {
        columns[x] = y;
      }
    }
    return columns;
  };
}
