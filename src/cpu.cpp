#include "cpu.hpp"
#include <cstddef>
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

std::vector<CpuTicks> parse_proc_stat() {

  std::ifstream proc_stat_file("/proc/stat");

  if (!proc_stat_file.is_open()) {
    std::println("could not open /proc/stat");
    return std::vector<CpuTicks>();
  }

  std::vector<CpuTicks> ticks;

  std::string line;
  while (std::getline(proc_stat_file, line)) {
    std::stringstream line_stream(line);
    std::string part;
    std::vector<std::string> parts;

    while (line_stream >> part) {
      parts.push_back(part);
    }

    if (parts.size() < 9 || !parts[0].starts_with("cpu")) {
      continue;
    }

    unsigned long long values[8];
    for (int i = 0; i < 8; i++) {
      values[i] = std::stoull(parts[i + 1]);
    }

    unsigned long long total = 0;
    for (unsigned long long value : values) {
      total += value;
    }

    ticks.push_back({
        .idle = values[3] + values[4],
        .total = total,
    });
  }

  return ticks;
}

static double usage_between(const CpuTicks &previous, const CpuTicks &current) {
  unsigned long long total_delta = current.total - previous.total;
  unsigned long long idle_delta = current.idle - previous.idle;

  if (total_delta == 0) {
    return 0.0;
  }

  return 100.0 * (1.0 - static_cast<double>(idle_delta) / total_delta);
}

CpuInfo calculate_cpu(const std::vector<CpuTicks> &previous_ticks,
                      const std::vector<CpuTicks> &current_ticks) {
  if (previous_ticks.empty() || current_ticks.empty()) {
    return CpuInfo{};
  }

  CpuInfo cpu_info = {
      .coreCount = static_cast<int>(current_ticks.size()) - 1,
      .totalUsage = usage_between(previous_ticks[0], current_ticks[0]),
      .coreUsage = {},
  };

  for (std::size_t i = 1; i < current_ticks.size() && i < previous_ticks.size();
       i++) {
    cpu_info.coreUsage.push_back(
        usage_between(previous_ticks[i], current_ticks[i]));
  }

  return cpu_info;
}

void print_cpu_info(const CpuInfo &cpu_info) {
  std::println("CPU: {:.1f}% ({} cores)", cpu_info.totalUsage,
               cpu_info.coreCount);
  for (std::size_t i = 0; i < cpu_info.coreUsage.size(); i++) {
    std::println("  cpu{}: {:.1f}%", i, cpu_info.coreUsage[i]);
  }
  std::println("");
}
