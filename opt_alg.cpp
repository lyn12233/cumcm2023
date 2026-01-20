#include "opt_alg.hpp"

#include "coord_alg.hpp"

#include <cstdio>
#include <ctime>
#include <functional>
#include <iostream>
#include <random>
#include <vector>

using std::cout;
using std::vector;

Particle::Particle(int dim)
    : pos(dim, 0.f), v(dim, 0.f), best_pos(dim, 0.f), best_val(1e8),
      cur_val(1e8) {}

ParticleOpt::ParticleOpt(fitness_func_f func, int nb_swarm, int dim,
                         int nb_iter, float c1, float c2, float w1, float w2,
                         float v_max)
    : func(func), nb_swarm(nb_swarm), dim(dim), nb_iter(nb_iter), c1(c1),
      c2(c2), w1(w1), w2(w2), v_min(-v_max), v_max(v_max), rand_gen(1145),
      rand_dist(0.f, 1.f) {

  // prepare best value
  gbest_val = 1e8;
  gbest_pos.resize(dim, 0.5);

  // init swarm

  swarm.clear();
  swarm.reserve(nb_swarm);
  for (int i = 0; i < nb_swarm; i++) {
    Particle p(dim);
    for (int j = 0; j < dim; j++) {
      p.pos[j] = rand_dist(rand_gen);
      p.v[j] = v_min + rand_dist(rand_gen) * (v_max - v_min);
    }
    p.cur_val = func(p.pos);
    p.best_pos = p.pos, p.best_val = p.cur_val;
    if (p.cur_val < gbest_val) {
      gbest_val = p.cur_val, gbest_pos = p.pos;
    }
    swarm.push_back(p);
  }
}

static float clip(float x, float min_, float max_) {
  return std::min(std::max(x, min_), max_);
}

void ParticleOpt::update_particle(Particle &p) {

  // update v
  for (int i = 0; i < dim; i++) {
    float r1 = rand_dist(rand_gen);
    float r2 = rand_dist(rand_gen);
    p.v[i] = w1 * p.v[i] + c1 * r1 * (p.best_pos[i] - p.pos[i]) +
             c2 * r2 * (gbest_pos[i] - p.pos[i]);
    p.v[i] = clip(p.v[i], v_min, v_max);
  }
  // update pos
  for (int i = 0; i < dim; i++) {
    p.pos[i] += p.v[i];
    if (p.pos[i] < 0.f) {
      p.pos[i] = 1e-6;
      p.v[i] *= -0.5;
    } else if (p.pos[i] > 1.f) {
      p.pos[i] = 1 - 1e-6;
      p.v[i] *= -0.5;
    }
  }

  // update fitness
  p.cur_val = func(p.pos);
  if (p.cur_val < p.best_val) {
    p.best_pos = p.pos, p.best_val = p.cur_val;
  }
  if (p.cur_val < gbest_val) {
    gbest_pos = p.pos, gbest_val = p.cur_val;
  }
}

void ParticleOpt::optimize() {
  printf("start PSO(nb_swarm=%d, nb_iter=%d)\n", nb_swarm, nb_iter);

  for (int ep = 0; ep < nb_iter; ep++) {
    clock_t t0 = clock();

    w1 *= w2;
    for (auto &p : swarm) {
      update_particle(p);
    }

    clock_t t1 = clock();
    printf("epoch %d takes %ld ms, best result %f at (", ep, t1 - t0,
           gbest_val);
    for (int i = 0; i < dim; i++) {
      printf("%.2f, ", gbest_pos[i]);
    }
    printf(")\n");
  }
}