#pragma once

#include <functional>
#include <random>
#include <vector>

struct Particle {
  std::vector<float> pos, v, best_pos;
  float best_val, cur_val;
  Particle(int dim = 21);
};

class ParticleOpt {
public:
  int nb_swarm;
  int dim;
  int nb_iter;
  float c1, c2, w1, w2, v_max, v_min;
  std::vector<Particle> swarm;
  std::vector<float> gbest_pos;
  float gbest_val;

  // random generator
  std::mt19937 rand_gen;
  std::uniform_real_distribution<float> rand_dist;

  // fitness functino
  using fitness_func_f = std::function<float(const std::vector<float> &)>;
  fitness_func_f func;
  ParticleOpt(fitness_func_f func, int nb_swarm = 20, int dim = 21,
              int nb_iter = 100, float c1 = 2, float c2 = 2, float w1 = 0.9,
              float w2 = 0.99, float v_max = 0.2);
  void update_particle(Particle &p);
  void optimize();
};