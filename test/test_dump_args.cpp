#include "coord_alg.hpp"

#include <vector>

using std::vector;

int main() {
  vector<float> p = {
      1,    0.44, 0.7,                   //
      1,    1,    1,    1,    1,    1,   //
      1,    1,    1,    1,    1,    1,   //
      0.59, 0.26, 0.63, 0.51, 0.19, 0.34 //
  };
  p = {
      1,    0.46, 0.63,               //
      1,    1,    1,    1,    1, 1,   //
      1,    1,    1,    1,    1, 1,   //
      0.55, 1,    0.73, 0.73, 1, 0.45 //
  };
  dump_args(p, "tmp.csv");
}