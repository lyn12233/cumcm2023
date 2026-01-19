#include "coord_alg.hpp"

#include <ctime>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

bool within(float x, float y) { return x * x + y * y < 300 * 300; }

int main() {

  float yc = 0.f;
  float r_min = 100, r_max = 300;
  float d = 11, r_effect = 30;

  vector<float> tot, per;

  clock_t t0 = clock();

  fittness_v1(r_min, r_max, d, yc, r_effect, within, 1, 1, 1, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1, 1, 1, 1, 1, tot, per);

  clock_t t1 = clock();
  cout << "fitness uses time(ms): " << t1 - t0 << endl;
  cout << "tot: " << endl;
  for (int i = 0; i < tot.size(); i++) {
    cout << tot[i] << " (" << per[i] << ", " << i << "th)" << endl;
  }
}