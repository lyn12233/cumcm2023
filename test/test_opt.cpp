#include "coord_alg.hpp"
#include "opt_alg.hpp"

#include <iostream>

using std::cout;
using std::endl;
using std::vector;

int main() {

  // option(1): optimize in the whole space

  vector<float> min_s = //
      {
          0.5, 0.2, 0.0,                // r_max, d, yc
          0.8, 0.8, 0.8, 0.8, 0.8, 0.8, // pa
          0.8, 0.8, 0.8, 0.8, 0.8, 0.8, // pb
          0.0, 0.0, 0.0, 0.0, 0.0, 0.0  // ph
      };
  vector<float> max_s = {
      1, 1, 1,          //
      1, 1, 1, 1, 1, 1, //
      1, 1, 1, 1, 1, 1, //
      1, 1, 1, 1, 1, 1  //
  };

  // option(2): restrict r_max, pa,pb

  min_s = {
      1,    0.2,  0.0,                    // r_max, d, yc
      0.99, 0.99, 0.99, 0.99, 0.99, 0.99, // pa
      0.99, 0.99, 0.99, 0.99, 0.99, 0.99, // pb
      0.0,  0.0,  0.0,  0.0,  0.0,  0.0   // ph
  };

  // option(3): restrict range of d,yc,ph

  min_s = {
      0.99, 0.4,  0.65,                 // r_max, d, yc
      0.8,  0.8,  0.8,  0.8, 0.8,  0.8, // pa
      0.8,  0.8,  0.8,  0.8, 0.8,  0.8, // pb
      0.0, 0.0, 0.0,  0.0, 0.0, 0.0  // ph
  };
  max_s = {
      1.0, 0.5,  0.75,                //
      1,   1,    1,    1,   1,    1,  //
      1,   1,    1,    1,   1,    1,  //
      1, 1, 1,  1, 1, 1 //
  };

  int nb_swarm, nb_iter;
  nb_swarm = 40, nb_iter = 40;

  auto opt = ParticleOpt(fitness_v2, // func
                         nb_swarm,   // swarm size
                         21,         // dim
                         nb_iter,    // iter
                         2, 2,     // c1c2
                         0.9, 0.99,  // w1w2
                         0.2,        // v_max
                         min_s, max_s);

  cout << "main: start optimize" << endl;

  opt.optimize(1, 1);
}