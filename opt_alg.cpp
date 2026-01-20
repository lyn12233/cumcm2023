#include "opt_alg.hpp"

#include "coord_alg.hpp"

#include <cstdio>
#include <ctime>
#include <functional>
#include <iostream>
#include <random>
#include <stdio.h>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

Particle::Particle(int dim)
    : pos(dim, 0.f), v(dim, 0.f), best_pos(dim, 0.f), best_val(1e8),
      cur_val(1e8) {}

static float stretch(float x, float min_, float max_) {
  return min_ + x * (max_ - min_);
}

ParticleOpt::ParticleOpt(fitness_func_f func_, int nb_swarm_, int dim_,
                         int nb_iter_, float c1_, float c2_, float w1_,
                         float w2_, float v_max_,
                         const vector<float> &pos_mins_,
                         const vector<float> &pos_maxs_)
    : func(func_), nb_swarm(nb_swarm_), dim(dim_), nb_iter(nb_iter_), c1(c1_),
      c2(c2_), w1(w1_), w2(w2_), v_min(-v_max_), v_max(v_max_), rand_gen(1145),
      rand_dist(0.f, 1.f), pos_mins(pos_mins_), pos_maxs(pos_maxs_) {

  cout << "optimizer construction..." << endl;

  // prepare best value
  gbest_val = 1e8;
  gbest_pos.resize(dim_, 0.5);

  // prepare threshold
  for (int i = pos_mins.size(); i < dim; i++) {
    pos_mins.push_back(0);
  }
  for (int i = pos_maxs.size(); i < dim; i++) {
    pos_maxs.push_back(1);
  }
  cout << "parameter thresholds: " << endl;
  for (int i = 0; i < dim; i++) {
    cout << "(" << pos_mins[i] << ", " << pos_maxs[i] << ")";
  }
  cout << endl;

  // init swarm

  swarm.clear();
  swarm.reserve(nb_swarm_);
  for (int i = 0; i < nb_swarm_; i++) {
    Particle p(dim_);
    for (int j = 0; j < dim_; j++) {
      p.pos[j] = stretch(rand_dist(rand_gen), pos_mins[j], pos_maxs[j]);
      p.v[j] = stretch(rand_dist(rand_gen), v_min, v_max);
    }
    p.cur_val = func_(p.pos);
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

float ParticleOptPenalty = 1;

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
    if (p.pos[i] < pos_mins[i]) {
      p.pos[i] = pos_mins[i] + 1e-6;
      p.v[i] *= -0.5;
    } else if (p.pos[i] > pos_maxs[i]) {
      p.pos[i] = pos_maxs[i] - 1e-6;
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

void ParticleOpt::optimize(float init_penalty, float penalty_scale) {
  printf("start PSO(nb_swarm=%d, nb_iter=%d)\n", nb_swarm, nb_iter);
  FILE *fp;
  fopen_s(&fp, "opt_track.csv", "wb");
  ParticleOptPenalty = init_penalty;

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
    printf(")\n\n\n");

    fprintf(fp, "%f, ", gbest_val);
    for (int i = 0; i < dim; i++) {
      fprintf(fp, "%f, ", gbest_pos[i]);
    }
    fprintf(fp, "\n");
    fflush(fp);

    ParticleOptPenalty *= penalty_scale;
  }
  fclose(fp);
}