#include <chrono>
#include <functional>

long function_duration(const std::function<void()> &function) {
  auto start = std::chrono::high_resolution_clock::now();
  function();
  auto stop = std::chrono::high_resolution_clock::now();
  return std::chrono::duration_cast<std::chrono::microseconds>(stop - start)
      .count();
}
