#include "coord_alg.hpp"

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
  float D, ST, alti;
  cout << "input: date from spring, the hour, latitude\n";
  cin >> D >> ST >> alti;
  alti = alti * 3.14 / 180;
  float phi, theta;
  get_sun_info(D, ST, alti, phi, theta);
  cout << "phi: " << phi / 3.14 * 180 << ", theta: " << theta / 3.14 * 180
       << endl;
  cout << "input height: " << endl;
  float height;
  cin >> height;
  cout << "DNI: " << get_dni(phi, height) << endl;
}