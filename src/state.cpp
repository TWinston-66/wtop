#include "state.hpp"
#include <cstddef>

void AppState::update(const CpuInfo &new_cpu, const MemInfo &new_mem) {
  cpu = new_cpu;
  mem = new_mem;

  cpu_total.push(static_cast<float>(new_cpu.totalUsage / 100.0));

  cpu_cores.resize(new_cpu.coreUsage.size());
  for (std::size_t i = 0; i < new_cpu.coreUsage.size(); i++) {
    cpu_cores[i].push(static_cast<float>(new_cpu.coreUsage[i] / 100.0));
  }

  mem_used.push(new_mem.total > 0
                    ? static_cast<float>(new_mem.used) / new_mem.total
                    : 0.0f);
}
