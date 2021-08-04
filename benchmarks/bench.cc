#include <vector>

#include "../include/vector.hh"

long plsm_vector_test(long rounds) {
  long result;

  plsm::vector<long> vec;
  for (long i = 0; i < rounds; i++) {
    vec.push(i);
    result += vec.at(i);
  }

  return result;
}

long std_vector_test(long rounds) {
  long result;

  std::vector<long> vec;
  for (long i = 0; i < rounds; i++) {
    vec.push_back(i);
    result += vec.at(i);
  }

  return result;
}

#include "bench.hh"
#include <iostream>

int main() {
  long test_rounds = 500000000;

  std::cout << "std::vector:  " << function_duration([=]() {
    std_vector_test(test_rounds);
  }) << " μs" << std::endl;

  std::cout << "plsm::vector: " << function_duration([=]() {
    plsm_vector_test(test_rounds);
  }) << " μs" << std::endl;
}
