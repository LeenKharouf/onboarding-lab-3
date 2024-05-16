#include <VExercise4.h>

int main() {
  VExercise4 model;
  uint8_t alpha, beta, gamma;
  alpha = 3;
  beta = 1;
  gamma = 2;
  model.alpha = alpha;
  model.beta = beta;
  model.gamma = gamma;
  
  bool isOutputCorrect = true;

  model.cs = 0;
  for (int index = 0; index < 4; ++index) {
    model.sel = index;
    model.eval();
    isOutputCorrect &= (model.out == 0);
  }

  model.cs = 1;
  for (int index = 0; index < 4; ++index) {
    model.sel = index;
    model.eval();
    switch (index) {
      case 0:
        isOutputCorrect &= (model.out == alpha);
        break;
      case 1:
        isOutputCorrect &= (model.out == beta);
        break;
      case 2:
        isOutputCorrect &= (model.out == gamma);
        break;
      case 3:
        isOutputCorrect &= (model.out == (alpha & (beta | gamma)));
        break;
    }
  }
  return !isOutputCorrect; // Return 1 if output is incorrect, 0 if correct
}