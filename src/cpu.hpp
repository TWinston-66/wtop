#pragma once

#include <vector>

struct CpuInfo {
  int coreCount;
  double totalUsage;
  std::vector<double> coreUsage;
};

struct CpuTicks {
  unsigned long long idle;
  unsigned long long total;
};

std::vector<CpuTicks> parse_proc_stat();

CpuInfo calculate_cpu(const std::vector<CpuTicks> &previous_ticks,
                      const std::vector<CpuTicks> &current_ticks);

void print_cpu_info(const CpuInfo &cpu_info);
