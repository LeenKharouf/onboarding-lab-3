#include <cstdint>
#include <catch2/catch_test_macros.hpp>
#include "VExercise1.h" // Make sure the header file is correctly named after your Verilog to C++ translation

// A lambda function to execute the model and perform the check
auto test_op = [](VExercise1& model, uint8_t code, uint8_t (*op)(uint8_t, uint8_t)) {
  model.op = code;
  model.a = 0;
  model.b = 0;

  do {
    do {
      model.eval(); // Simulate the behavior of the Verilog module
      uint8_t result = op(model.a, model.b);
      REQUIRE(result == model.out); // Check if the result matches the module output
    } while (++model.b != 0); // This will wrap around from 255 to 0, exiting the loop
  } while (++model.a != 0); // This will wrap around from 255 to 0, exiting the loop
};

// Test cases for each opcode
TEST_CASE("Opcode 0, XOR") {
  VExercise1 model;
  test_op(model, 0, [](uint8_t a, uint8_t b) -> uint8_t { return a ^ b; });
}

TEST_CASE("Opcode 1, Shift Left") {
  VExercise1 model;
  test_op(model, 1, [](uint8_t a, uint8_t b) -> uint8_t { return a << b; });
}

TEST_CASE("Opcode 2, Modulus") {
  VExercise1 model;
  test_op(model, 2, [](uint8_t a, uint8_t b) -> uint8_t { return b == 0 ? 0 : a % b; }); // Handling divide by zero
}

TEST_CASE("Opcode 3, NAND") {
  VExercise1 model;
  test_op(model, 3, [](uint8_t a, uint8_t b) -> uint8_t { return ~(a & b); });
}
