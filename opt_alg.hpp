#pragma once

#include <functional>
#include <random>
#include <vector>

struct Particle {
  std::vector<float> pos, v, best_pos;
  float best_val, cur_val;
  Particle(int dim = 21);
};

extern float ParticleOptPenalty;

class ParticleOpt {
public:
  int nb_swarm;
  int dim;
  int nb_iter;
  float c1, c2, w1, w2, v_max, v_min;
  std::vector<Particle> swarm;
  std::vector<float> gbest_pos;
  float gbest_val;
  std::vector<float> pos_mins, pos_maxs;

  // random generator
  std::mt19937 rand_gen;
  std::uniform_real_distribution<float> rand_dist;

  // fitness functino
  using fitness_func_f = std::function<float(const std::vector<float> &)>;
  fitness_func_f func;
  ParticleOpt(fitness_func_f func_, int nb_swarm_ = 10, int dim_ = 21,
              int nb_iter_ = 40, float c1_ = 2, float c2_ = 2, float w1_ = 0.9,
              float w2_ = 0.99,   //
              float v_max_ = 0.2, //
              const std::vector<float> &pos_mins_ = {},
              const std::vector<float> &pos_maxs_ = {});
  void update_particle(Particle &p);
  void optimize(float init_pentaly = 1e-3, float penlty_scale = 1.45);
};