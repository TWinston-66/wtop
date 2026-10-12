#include "cpu.hpp"
#include "memory.hpp"
#include "state.hpp"
#include <atomic>
#include <chrono>
#include <cmath>
#include <string>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <thread>
#include <unistd.h>

using namespace ftxui;

constexpr auto LOOP_TIME = std::chrono::milliseconds(1000);

int main() {

  auto screen = ScreenInteractive::Fullscreen();

  AppState state{};

  const auto heat = LinearGradient()
                        .Angle(90)
                        .Stop(Color::Red, 0.0f)
                        .Stop(Color::Yellow, 0.5f)
                        .Stop(Color::Green, 1.0f);

  auto pct = [](float f) {
    return std::to_string(static_cast<int>(std::lround(f * 100))) + "%";
  };

  auto ui = Renderer([&] {
              float cpu = state.cpu_total.latest();
              float mem = state.mem_used.latest();

              Elements cores;
              for (std::size_t i = 0; i < state.cpu_cores.size(); i++) {
                float c = state.cpu_cores[i].latest();
                cores.push_back(hbox({
                    text("C" + std::to_string(i)) | size(WIDTH, EQUAL, 4),
                    gauge(c) | color(heat) | size(WIDTH, EQUAL, 10),
                    text(" " + pct(c)) | size(WIDTH, EQUAL, 5),
                }));
              }

              auto cpu_box = window(
                  text(" cpu " + pct(cpu) + " ") | bold,
                  hbox({
                      graph(as_graph(state.cpu_total)) | color(heat) | flex,
                      separator(),
                      vbox(std::move(cores)),
                  }));

              auto mem_box =
                  window(text(" mem " + pct(mem) + " ") | bold,
                         graph(as_graph(state.mem_used)) | color(heat));

              return vbox({
                         text("wtop") | bold | center,
                         separator(),
                         cpu_box | size(HEIGHT, GREATER_THAN, 12) | flex,
                         mem_box | flex,
                     }) |
                     border;
            }) |
            CatchEvent([&](Event e) {
              if (e == Event::Character('q')) {
                screen.Exit();
                return true;
              }
              return false;
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

      screen.Post([&state, new_cpu = std::move(new_cpu), new_mem] {
        state.update(new_cpu, new_mem);
      });
      screen.PostEvent(Event::Custom);
    }
  });

  screen.Loop(ui);
  running = false;
  tick.join();
  return 0;
}
