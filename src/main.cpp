#include "memory.hpp"
#include <iostream>

int main() {

  std::vector<std::string> proc_meminfo = parse_proc_meminfo();
  MemInfo mem_info = calculate_ram(proc_meminfo);

  std::cout << "Used: " << mem_info.used << "kb\n"
            << "Buffers: " << mem_info.buffered << "kb\n"
            << "Cached: " << mem_info.cached << "kb\n"
            << "Free: " << mem_info.free << "kb\n";
  return 0;
}
