#include "memory.hpp"
#include <fstream>
#include <print>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> parse_proc_meminfo() {
  std::string path = "/proc/meminfo";

  std::ifstream proc_mem_file(path);
  std::vector<std::string> proc_mem_lines;

  if (!proc_mem_file.is_open()) {
    std::println("could not open /proc/meminfo");
    return std::vector<std::string>();
  }

  std::string line;
  while (std::getline(proc_mem_file, line)) {
    proc_mem_lines.push_back(line);
  };

  if (proc_mem_file.eof()) {
    proc_mem_file.close();
  } else {
    std::println("could not read /proc/meminfo");
    return std::vector<std::string>();
  }

  return proc_mem_lines;
}

MemInfo calculate_ram(const std::vector<std::string> &proc_mem_lines) {

  std::string target_values[] = {"MemTotal", "MemAvailable", "Buffers",
                                 "Cached",   "SReclaimable", "MemFree"};

  int found_values[6] = {};

  for (std::string mem_line : proc_mem_lines) {
    std::stringstream strm(mem_line);
    std::vector<std::string> sections;

    while (getline(strm, mem_line, ':')) {
      sections.push_back(mem_line);
    }

    if (sections.size() != 2) {
      std::println("found incorrect number of mem line sections");
      return MemInfo{};
    }

    std::string value_name;
    std::string value_measurement;

    for (int i = 0; i < 2; i++) {
      for (char ch : sections.at(i)) {
        if (ch != ' ') {
          if (i == 0) {
            value_name += ch;
          } else {
            value_measurement += ch;
          }
        }
      }
    }

    for (int i = 0; i < 6; i++) {
      if (value_name == target_values[i]) {
        found_values[i] = stoi(value_measurement.erase(
            value_measurement.length() - 2, value_measurement.length() - 1));
      }
    }
  }

  return {
      .used = found_values[0] - found_values[1],
      .buffered = found_values[2],
      .cached = found_values[3] + found_values[4],
      .free = found_values[5],
  };
}
