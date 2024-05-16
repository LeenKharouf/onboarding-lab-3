#include <bit>
#include <catch2/catch_test_macros.hpp>
#include <cstdint>
#include <VExercise3.h>

using namespace std;

struct SimulationEx3 {
  int alpha;
  uint16_t beta;
  uint16_t gamma;

  uint16_t result;

  SimulationEx3(int alpha, uint16_t beta, uint16_t gamma) : alpha(alpha), beta(beta), gamma(gamma) {
    result = (computeBInput() << 8) | computeAInput();
  }

private:
  int state = 4;

  uint8_t simulateMystery1(int alpha, uint8_t beta, uint8_t gamma) {
    switch (alpha) {
      case 0:
        return ((beta & 0b111) << 3) | (gamma & 0b111);
      case 1:
        return ((gamma & 0b111) << 3) | (beta & 0b111) | 0b1000000;
      case 2:
        return beta;
      default:
        return gamma;
    }
  }

  uint8_t computeAInput() {
    int shifted_alpha = alpha & 0b11;
    
    uint8_t masked_beta = beta & 0b11111111;
    uint8_t masked_gamma = gamma & 0b11111111;
    return simulateMystery1(shifted_alpha, masked_beta, masked_gamma);
  }

  uint8_t computeBInput() {
    int adjusted_alpha = (alpha & 0b1100) >> 2;
    uint8_t upper_beta = beta >> 8;
    uint8_t upper_gamma = gamma >> 8;

    return simulateMystery1(adjusted_alpha, upper_beta, upper_gamma);
  }

public:
  void advanceState() {
    uint8_t a_input = computeAInput();
    uint8_t b_input = computeBInput();

    state = (state + 1) % 5;
    switch (state) {
      case 0:
        result = (a_input << 8) | (result & 0b11111111);
        break;
      case 1:
        result = (result & 0b1111111100000000) | b_input;
        break;
      case 2:
        result = ((result & 0b1111111100000000) >> 8) | ((result & 0b11111111) << 8);
        break;
      case 3:
        result = ((result & 0b1111) << 12) | ((result & 0b11110000) << 4) | ((result & 0b111100000000) >> 4) | (result >> 12);
        break;
      default:
        result = popcount(result) & 1;
        break;
    }
  }
};

void executeStep(VExercise3& model) {
  model.clk = 1;
  model.eval();
  model.clk = 0;
  model.eval();
}

void validateModel(VExercise3& model) {
  SimulationEx3 simulation(model.a, model.b, model.c);

  model.reset = 1;
  executeStep(model);
  model.reset = 0;
  REQUIRE(model.result == simulation.result);

  for(size_t i = 0; i < 10; ++i) {
    executeStep(model);
    simulation.advanceState();
    REQUIRE(model.result == simulation.result);
  }
}

TEST_CASE("Exercise 3 - Test operation of Mystery1") {
  VExercise3 model;
  model.a = 0;
  model.b = 0;
  model.c = 0;

  for(size_t i = 0; i < 16; ++i) {
    model.a = i;
    for (size_t j = 0; j < 10; ++j) {
      model.b = rand() % ((uint16_t) ~0 + 1);
      for (size_t k = 0; k < 10; ++k) { 
        model.c = rand() % ((uint16_t) ~0 + 1);
        validateModel(model);
        
      }
    }
  } 
}

TEST_CASE("Exercise 3 - Validate reset behavior") {
  VExercise3 model;
  model.reset = 1;
  model.a = 0;
  model.b = 0;
  model.c = 0;

  executeStep(model);
  uint16_t expected_result = 0;
  REQUIRE(model.result == expected_result);

  executeStep(model);
  executeStep(model);
  executeStep(model);
  executeStep(model);
  REQUIRE(model.result == expected_result);
}
