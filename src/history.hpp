#pragma once

#include <cstddef>
#include <deque>
#include <ftxui/dom/elements.hpp>

struct History {
  std::size_t cap = 512;
  std::deque<float> samples;

  void push(float value);
  float latest() const;
};

ftxui::GraphFunction as_graph(const History &h);
