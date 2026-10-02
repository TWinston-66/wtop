#include "memory.hpp"
#include <chrono>
#include <iostream>
#include <thread>

constexpr auto LOOP_TIME = std::chrono::milliseconds(1000);

int main() {
  auto next = std::chrono::steady_clock::now();

  while (true) {
    std::vector<std::string> proc_meminfo = parse_proc_meminfo();
    MemInfo mem_info = calculate_ram(proc_meminfo);

    std::cout << "Used: " << mem_info.used << "kb\n"
              << "Buffers: " << mem_info.buffered << "kb\n"
              << "Cached: " << mem_info.cached << "kb\n"
              << "Free: " << mem_info.free << "kb\n\n";

    next += LOOP_TIME;
    std::this_thread::sleep_until(next);
  }

  return 0;
}
