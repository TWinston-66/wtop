#include "cpu.hpp"
#include "memory.hpp"
#include <chrono>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <unistd.h>

using namespace ftxui;

constexpr auto LOOP_TIME = std::chrono::milliseconds(1000);

struct History {
  std::size_t cap = 512;
  std::deque<float> v;
  void push(float x) {
    v.push_back(x);
    if (v.size() > cap)
      v.pop_front();
  }
};

struct AppState {
  History cpu_total;
  std::vector<History> cpu_cores;
  History mem_used;
  CpuInfo cpu;
  MemInfo mem;
};

int main() {

  auto screen = ScreenInteractive::Fullscreen();

  CpuInfo cpu_info{};
  MemInfo mem_info{};

  auto ui = Renderer([&] {
    float mem_fraction =
        mem_info.total > 0 ? static_cast<float>(mem_info.used) / mem_info.total
                           : 0.0f;
    return vbox({
               text("wtop") | bold | center,
               separator(),
               hbox({text("cpu "), gauge(cpu_info.totalUsage / 100.0) | flex}),
               hbox({text("mem "), gauge(mem_fraction) | flex}),
           }) |
           border;
  });

  std::atomic<bool> running = true;
  std::thread tick([&] {
    auto prev_ticks = parse_proc_stat();
    auto next = std::chrono::steady_clock::now();
    while (running) {
      next += LOOP_TIME;
      std::this_thread::sleep_until(next);

      auto cur_ticks = parse_proc_stat();
      CpuInfo new_cpu = calculate_cpu(prev_ticks, cur_ticks);
      MemInfo new_mem = calculate_ram(parse_proc_meminfo());
      prev_ticks = std::move(cur_ticks);

      screen.Post(
          [&cpu_info, &mem_info, new_cpu = std::move(new_cpu), new_mem] {
            cpu_info = new_cpu;
            mem_info = new_mem;
          });
      screen.PostEvent(Event::Custom);
    }
  });

  screen.Loop(ui);
  running = false;
  tick.join();
  return 0;
}
