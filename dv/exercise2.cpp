#include <iostream>
#include "VExercise2.h"

// Function to calculate the next LFSR value based on the current state
uint16_t next_lfsr_value(uint16_t current) {
    // Shift left and calculate the feedback bit
    return (current << 1) | ((current >> 10) ^ (current >> 8) ^ (current >> 3) ^ (current >> 1)) & 1;
}

// Function to test the LFSR functionality
bool test_lfsr(uint16_t initial_value) {
    VExercise2 model;
    
    // Initialize the model
    model.clk = 0;
    model.reset = 1;  // Apply reset
    model.init = initial_value;
    model.eval();

    // Remove reset and start clocking
    uint16_t expected_state = ~initial_value;
    for (int i = 0; i < 100; ++i) {
        model.clk = 1;  // Rising edge
        model.reset = 0;  // Clear reset
        model.eval();

        model.clk = 0;  // Falling edge
        model.eval();

        // Compare model output to expected LFSR output
        if (model.out != expected_state) {
            std::cerr << "Mismatch at cycle " << i << ": expected " << std::hex << expected_state
                      << " but got " << model.out << std::endl;
            return true;  // Return true if there is an error
        }

        // Calculate the next expected state
        expected_state = next_lfsr_value(expected_state);
    }
    return false;  // Return false if no errors found
}

int main() {
    bool failed = false;
    failed |= test_lfsr(0xACE1);  // Use a common non-trivial seed
    failed |= test_lfsr(0x1);     // Test with minimal seed
    failed |= test_lfsr(0xFFFF);  // Test with all bits set

    if (failed) {
        std::cout << "Tests failed." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }
    return failed ? 1 : 0;
}
