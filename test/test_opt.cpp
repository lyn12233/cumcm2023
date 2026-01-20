#include "coord_alg.hpp"
#include "opt_alg.hpp"

#include <iostream>

using std::cout;
using std::endl;

int main() {
  auto opt = ParticleOpt(fitness_v2, 20, 21, 20, 2, 2, 0.9, 0.99, 0.2);

  cout << "main: start optimize" << endl;

  opt.optimize();
}