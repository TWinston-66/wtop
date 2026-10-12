#pragma once

#include "cpu.hpp"
#include "history.hpp"
#include "memory.hpp"
#include <vector>

struct AppState {
  History cpu_total;
  std::vector<History> cpu_cores;
  History mem_used;

  CpuInfo cpu{};
  MemInfo mem{};

  void update(const CpuInfo &new_cpu, const MemInfo &new_mem);
};
