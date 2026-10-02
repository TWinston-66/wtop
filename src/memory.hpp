#pragma once

#include <string>
#include <vector>

struct MemInfo {
  int used;
  int buffered;
  int cached;
  int free;
};

std::vector<std::string> parse_proc_meminfo();
MemInfo calculate_ram(const std::vector<std::string> &proc_mem_lines);
