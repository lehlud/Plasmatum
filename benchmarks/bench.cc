#include "list.hh"

#include <ctime>
#include <chrono>
#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>

inline void plsm_list_test(int rounds, int data[]) {
  auto lst = create_plsm_list<int>();
  for (int i = 0; i < rounds; i++) {
    plsm_list_append(&lst, data[i]);
    if (plsm_list_get(&lst, i) != data[i]) {
      std::exit(420);
    }
  }
}

inline void vector_test(int rounds, int data[]) {
  auto vec = std::vector<int>();
  for (int i = 0; i < rounds; i++) {
    vec.push_back(data[i]);
    if (vec.at(i) != data[i]) {
      std::exit(421);
    }
  }
}

int pow(int base, int exp) {
  if (exp == 0) return 1;
  
  int result = base;
  for (int i = 1; i < exp; i++) {
    result *= base;
  }
  return result;
}

using namespace std::chrono;

template <class c, class d>
void print_result(const std::string& name, const time_point<c, d> &start, const time_point<c, d> &end) {
        const auto res = end - start;
        std::cout << name << res.count() << " µs" << std::endl;
}

int main() {
  auto lst = create_plsm_list<int>();

  int rounds = pow(10, 5);

  int data[rounds];
  const int data_max = pow(10, 9);
  for (int i = 0; i < rounds; i++) {
    std::srand(std::time(nullptr));
    data[i] = rand() % data_max;
  }

  auto plsm_start = high_resolution_clock::now();
  plsm_list_test(rounds, data);
  auto plsm_end = high_resolution_clock::now();

  auto cxx_start = high_resolution_clock::now();
  vector_test(rounds, data);
  auto cxx_end = high_resolution_clock::now(); 

  print_result("Plasmatum List:\t", plsm_start, plsm_end);
  print_result("C++ Vector:\t", cxx_start, cxx_end);
}
