#pragma once

#include "vec_types.hpp"

#include <functional>
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

void get_sun_info(float D, float ST, float altitude, //
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