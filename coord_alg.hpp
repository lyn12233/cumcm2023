#pragma once

#include "vec_types.hpp"

#include <functional>
#include <string>
#include <vector>

void gen_coord(float R_min, float R_max, float D,        //
               float yc,                                 //
               std::function<bool(float, float)> within, //
               std::vector<vec3> &result                 //
);

void repr_vec2s(const std::vector<vec3> &vecs);

void gen_abh(std::vector<vec3> &coords, std::vector<vec2> &ab, //
             float yc, float R_min, float R_max, float D,      //
             float pa0, float pa1, float pa2,                  //
             float pa3, float pa4, float pa5,                  //
             float pb0, float pb1, float pb2,                  //
             float pb3, float pb4, float pb5,                  //
             float ph0, float ph1, float ph2,                  //
             float ph3, float ph4, float ph5                   //
);

void get_sun_info(float D, float ST, float latitude, //
                  float &phi, float &theta           //
);

float get_dni(float phi, float height);

void prepare_nears(const std::vector<vec3> &coords,       //
                   float R_effect, float D,               //
                   std::vector<std::vector<int>> &groups, //
                   std::vector<int> &id2group             //
);

void calcetas(const std::vector<vec3> &coords,                       //
              const std::vector<vec2> &ab,                           //
              int at, int nb_near, const int *nears, float R_effect, //
              float y_c, float h_c1, float h_c2, float rc,           //
              float theta, float phi, float Omega,                   //
              float &eta_c, float &eta_a, float &eta_t, float &eta_s //
);

void calcflow(const std::vector<vec2> &ab,     //
              const std::vector<float> &eta_c, //
              const std::vector<float> &eta_a, //
              const std::vector<float> &eta_t, //
              const std::vector<float> &eta_s, //
              float dni,                       //
              float &tot, float &per           //
);

void fitness_v1(                                      //
    float r_min, float r_max,                         //
    float D,                                          //
    float yc,                                         //
    float R_effect,                                   // this not to opt
    std::function<bool(float, float)> within,         //
    float pa0, float pa1, float pa2,                  //
    float pa3, float pa4, float pa5,                  //
    float pb0, float pb1, float pb2,                  //
    float pb3, float pb4, float pb5,                  //
    float ph0, float ph1, float ph2,                  //
    float ph3, float ph4, float ph5,                  //
    std::vector<float> &tot, std::vector<float> &per, //
    const std::string &save_fn = ""                   //
);

///@param parm swarm position parameters, ordered in r_max in (300,700), D in
///(7,13), yc in (0,-350) (as input they are normalized to 0,1), pa0..pa5,
/// pb0..pb5, ph0..ph5
///@return fitness value, opt toward minimum, considers the penalty of total
/// energy flow > 60kW, via exterior point method
float fitness_v2(const std::vector<float> &parm);

void dump_args(const std::vector<float> &p, const std::string &fn = "tmp.csv");