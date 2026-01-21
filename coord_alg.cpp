#include "coord_alg.hpp"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

constexpr static float pi = 3.14159263537989;

void gen_coord(float R_min, float R_max, float D,        //
               float yc,                                 //
               std::function<bool(float, float)> within, //
               vector<vec3> &result                      //
) {
  //
  result.clear();
  float r = R_min + D / 2;
  float theta = asin(D / 2 / r);
  bool odd = true;
  while (r + D < R_max) {

    // cout << "collecting ring " << r << endl;
    int dbg_cnt = 0;

    // push points
    if (odd) {
      for (int N = 0; (2 * N + 1) * theta < pi; N++) {
        float x = r * sin((2 * N + 1) * theta);
        float y = r * cos((2 * N + 1) * theta) + yc;
        if (within(x, y)) {
          result.push_back({x, y});
          result.push_back({-x, y});
          dbg_cnt += 2;
        } else {
          break;
        }
      }
    } else {
      if (within(0, r + yc)) {
        result.push_back({0, r + yc});
        dbg_cnt++;
      }
      for (int N = 1; 2 * N * theta < pi; N++) {
        float x = r * sin(2 * N * theta);
        float y = r * cos(2 * N * theta) + yc;
        if (within(x, y)) {
          result.push_back({x, y});
          result.push_back({-x, y});
          dbg_cnt += 2;
        } else {
          break;
        }
      }
    } // if(odd)

    // update oddness
    odd = !odd;

    // cout << "collected count " << dbg_cnt << endl;

    // update r, theta
    float r2 = r * cos(theta) + sqrt(D * D - pow(r * sin(theta), 2));
    if (r2 - r < D / 2) {
      r = r + D;
      theta = asin(D / 2 / r);
    } else {
      r = r2;
    }

    // cout << "update r to " << r << endl;
  }
}

void repr_vec2s(const vector<vec3> &vecs) {
  cout << "coords (3) " << vecs.size() << endl;
  for (const auto &v : vecs) {
    cout << "(" << v.x << ", " << v.y << ", " << v.z << "), ";
  }
  cout << endl;
}

static float gen_interp(float x,                      //
                        float x0, float x1, float x2, //
                        float y0, float y1, float y2  //
) {
  // return y0 * (x - x1) * (x - x2) / (x0 - x1) / (x0 - x2) +
  //        y1 * (x - x0) * (x - x2) / (x1 - x0) / (x1 - x2) +
  //        y2 * (x - x0) * (x - x1) / (x2 - x0) / (x2 - x1);
  // now only uses 2 point to interp
  if (x == x0)
    return y0;
  if (x == x2)
    return y2;
  return y0 * (x - x2) / (x0 - x2) + y2 * (x - x0) / (x2 - x0);
}
static float clip(float x, float min_, float max_) {
  return std::min(std::max(x, min_), max_);
}

void gen_abh(vector<vec3> &coords, vector<vec2> &ab,      //
             float yc, float R_min, float R_max, float D, //
             float pa0, float pa1, float pa2,             //
             float pa3, float pa4, float pa5,             //
             float pb0, float pb1, float pb2,             //
             float pb3, float pb4, float pb5,             //
             float ph0, float ph1, float ph2,             //
             float ph3, float ph4, float ph5              //
) {
  const float phi0 = 0, phi1 = pi / 2, phi2 = pi;
  float r0 = R_min, r1 = (R_max + R_min) / 2, r2 = R_max;
  ab.resize(coords.size());
  for (int i = 0; i < coords.size(); i++) {
    vec3 &pos = coords[i];
    vec2 &rect = ab[i];

    float r = sqrt(pow(pos.x, 2) + pow(pos.y - yc, 2));
    float phi = pi / 2 - atan((pos.y - yc) / pos.x);

    // heuristically, make them ordered
    if (pa0 > pa1)
      std::swap(pa0, pa1);
    if (pa1 > pa2)
      std::swap(pa1, pa2);
    if (pa0 > pa1)
      std::swap(pa0, pa1);

    // h

    phi = pos.x >= 0 ? phi : pi - phi;
    pos.z = gen_interp(r, r0, r1, r2, ph0, ph1, ph2) *
            gen_interp(phi, phi0, phi1, phi2, ph3, ph4, ph5);
    // de-normalize h
    pos.z = clip(2 + pos.z * (6 - 2), 2, 6);

    // a, b

    float a = gen_interp(r, r0, r1, r2, pa0, pa1, pa2) *
              gen_interp(phi, phi0, phi1, phi2, pa3, pa4, pa5);
    float b = gen_interp(r, r0, r1, r2, pb0, pb1, pb2) *
              gen_interp(phi, phi0, phi1, phi2, pb3, pb4, pb5);
    // demornalize a,b
    rect.x = clip((D - 5) * a, 2, 8);
    rect.y = clip(rect.x * b, 2, 8);
    rect.y = std::min(rect.y, 2 * pos.z);
  }
}

void get_sun_info(float D, float ST, float latitude, //
                  float &phi, float &theta           //
) {
  float delta = asin(sin(2 * pi * D / 365) * sin(23.45 * pi / 180));
  float omega = pi / 12 * (ST - 12);
  phi = asin(cos(delta) * cos(latitude) * cos(omega) +
             sin(delta) * sin(latitude));
  float cos_theta =
      (sin(delta) - sin(phi) * sin(latitude)) / (cos(phi) * cos(latitude));
  if (cos_theta <= -1 && cos_theta > -1 - 1e-3) {
    theta = -pi;
  } else if (cos_theta >= 1 && cos_theta < 1 + 1e-3) {
    theta = pi;
  } else if (cos_theta > -1 && cos_theta < 1) {
    theta = acos(cos_theta);
  } else {
    cout << "\033[38;5;9mfatal: cos_theta invalid:" << cos_theta << "\033[0m\n";
    exit(-1);
  }
}

float get_dni(float phi, float height) {
  const float g0 = 1.366;
  float a = 0.4237 - 0.00821 * pow(6 - height, 2);
  float b = 0.5055 + 0.00595 * pow(6.5 - height, 2);
  float c = 0.2711 + 0.01858 * pow(2.5 - height, 2);
  return g0 * (a + b * exp(-c / sin(phi)));
}

void prepare_nears(const std::vector<vec3> &coords,       //
                   float R_effect, float D,               //
                   std::vector<std::vector<int>> &groups, //
                   std::vector<int> &id2group             //
) {
  // (1) get range, logN
  float x_min = 1e8, x_max = -1e8;
  float y_min = 1e8, y_max = -1e8;
  for (const auto &c : coords) {
    x_min = std::min(x_min, c.x);
    x_max = std::max(x_max, c.x);
    y_min = std::min(y_min, c.y);
    y_max = std::max(y_max, c.y);
  }
  int Nx = (x_max - x_min) / R_effect / 4;
  int Ny = (y_max - y_min) / R_effect / 4;

  cout << "Nx,Ny: " << Nx << ", " << Ny << endl;

  // (2) prepare groups and id2group
  groups.resize(Nx * Ny); // ix,iy at ix*Ny+iy
  for (int i = 0; i < Nx * Ny; i++) {
    groups[i].clear();
  }
  id2group.resize(coords.size());

  // (3) put each coord to some groups and a lookup group(id2group)
  for (int i = 0; i < coords.size(); i++) {
    const auto &c = coords[i];
    float fx = (c.x - x_min) / (x_max - x_min);
    float fy = (c.y - y_min) / (y_max - y_min);

    // each coord belongs to a group
    int ix = clip(round(fx), 0, Nx - 1), iy = clip(round(fy), 0, Ny - 1);
    id2group[i] = ix * Ny + iy;

    // a coord may be effective in 4 groups
    ix = floor(fx), iy = floor(fy);
    if (ix >= 0 && ix < Nx && iy >= 0 && iy < Ny) {
      groups[ix * Ny + iy].push_back(i);
    }
    ix++;
    if (ix >= 0 && ix < Nx && iy >= 0 && iy < Ny) {
      groups[ix * Ny + iy].push_back(i);
    }
    iy++;
    if (ix >= 0 && ix < Nx && iy >= 0 && iy < Ny) {
      groups[ix * Ny + iy].push_back(i);
    }
    ix--;
    if (ix >= 0 && ix < Nx && iy >= 0 && iy < Ny) {
      groups[ix * Ny + iy].push_back(i);
    }
  }
}

void calcetas(const vector<vec3> &coords,                            //
              const vector<vec2> &ab,                                //
              int at, int nb_near, const int *nears, float R_effect, //
              float y_c, float h_c1, float h_c2, float rc,           //
              float theta, float phi, float Omega,                   //
              float &eta_c, float &eta_a, float &eta_t, float &eta_s //
) {
  // collect coordinate info
  vec3 pos = coords[at];
  vec3 d = vec3(0, y_c, h_c1 + h_c2 / 2) - pos;
  vec3 L = -vec3(sin(theta) * cos(phi), cos(theta) * cos(phi), sin(phi));
  vec3 N = vec3::norm(d.norm() - L);
  vec3 n2 = vec3::norm({d.x, d.y, 0});
  vec3 n3 = N.cross(n2);

  // make randoms
  float rands[16];
  for (int i = 0; i < 16; i++) {
    rands[i] = (float)(rand() & 0xff) / 255.;
  }

  // (1) eta_c
  eta_c = std::max(-N.dot(L), 0.f);

  // (2) eta_a
  float d_hr = d.dist();
  eta_a = 0.99321 - 0.0001176 * d_hr + 1.97e-8 * d_hr * d_hr;

  // (3) eta_t sampling
  float a = ab[at].x, b = ab[at].y;
  float R_diff = sin(0.5 * Omega) * d_hr + 0.5 * (a / 4 + b / 4);
  float h_2 = h_c2 / 2 * sqrt(d.x * d.x + d.y * d.y) / d_hr;
  float x_samples[] = {0,                               //
                       0.67,  0.67 * 0.5,  -0.67 * 0.5, //
                       -0.67, -0.67 * 0.5, 0.67 * 0.5};
  float y_samples[] = {0,                              //
                       0, 0.67 * 0.866,  0.67 * 0.866, //
                       0, -0.67 * 0.866, -0.67 * 0.866};
  float eta_truncs[4][4];
  for (int n = 0; n < 4; n++) {
    for (int m = 0; m < 4; m++) {
      float xo = (0.25 * n - 0.375) * a;
      float yo = (0.25 * m - 0.375) * b * eta_c;
      int cnt = 0;
      for (int i_samp = 0; i_samp < 7; i_samp++) {
        float xs =
            xo + R_diff * (x_samples[i_samp] - 0.125 + rands[n + i_samp] / 4);
        float ys =
            xo + R_diff * (y_samples[i_samp] - 0.125 + rands[m + i_samp] / 4);
        if (abs(xs) <= rc && abs(ys) <= h_2)
          cnt++;
      }
      eta_truncs[n][m] = (float)cnt / 7.f;
    }
  }

  // (4) eta_s sampling
  float eta_shadows[4][4];
  for (int n = 0; n < 4; n++) {
    for (int m = 0; m < 4; m++) {
      eta_shadows[n][m] = 1.f;
    }
  }
  for (int i = 0; i < nb_near; i++) {
    vec3 pos2 = coords[nears[i]];
    if (abs((pos - pos2).dist()) > R_effect)
      continue;
    vec3 relpos2 = {
        (pos2 - pos).dot(N),
        (pos2 - pos).dot(n2),
        (pos2 - pos).dot(n3),
    };
    if (relpos2.x <= 0)
      continue;
    float a2 = ab[nears[i]].x;
    float b2 = ab[nears[i]].y;
    float delta_b = relpos2.x * sqrt(1 - eta_c * eta_c);
    for (int n = 0; n < 4; n++) {
      for (int m = 0; m < 4; m++) {
        float xo = (0.25 * n - 0.5 + rands[n * 4 + m] / 4) * a;
        float yo = (0.25 * m - 0.5 + rands[m * 4 + n] / 4) * b * eta_c;
        if ((abs(xo - relpos2.y - delta_b) < a2 / 2 ||
             abs(xo - relpos2.y + delta_b) < a2 / 2) &&
            abs(yo - relpos2.z) < b2 / 2) {
          eta_shadows[n][m] = 0.f;
        }
      }
    }
  }

  // sum up eta_t and eta_s
  eta_t = 0, eta_s = 0;
  for (int n = 0; n < 4; n++) {
    for (int m = 0; m < 4; m++) {
      eta_s += eta_shadows[n][m];
      eta_t += eta_shadows[n][m] * eta_truncs[n][m];
    }
  }
  eta_s /= 16;
  eta_t /= 16;
  eta_t /= (eta_s != 0 ? eta_s : 1.f);
}

void calcflow(const std::vector<vec2> &ab,     //
              const std::vector<float> &eta_c, //
              const std::vector<float> &eta_a, //
              const std::vector<float> &eta_t, //
              const std::vector<float> &eta_s, //
              float dni,                       //
              float &tot, float &per           //
) {
  float areas = 0;
  tot = 0;
  for (int i = 0; i < ab.size(); i++) {
    float area = ab[i].x * ab[i].y;
    areas += area;
    tot += area * dni * eta_c[i] * eta_a[i] * eta_t[i] * eta_s[i];
  }
  per = tot / areas;
}

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
    const std::string &save_fn                        //
) {
  const float days[12] = {0,   30,  60,  90,  120, 150,
                          180, 210, 240, 270, 300, 330};
  const float hours[5] = {9, 10.5, 12, 13.5, 15};
  const float lat = 39.4 * pi / 180;
  const float height = 3;
  const float hc1 = 80, hc2 = 8, rc = 3.5;
  const float Omega = 0.53 * pi / 180;

  vector<int> nears;
  vector<float> eta_c, eta_a, eta_t, eta_s;
  vector<float> avg_c, avg_a, avg_t, avg_s;

  tot.resize(60);
  per.resize(60);

  vector<vec3> coords;
  gen_coord(r_min, r_max, D, yc, within, coords);
  if (!save_fn.empty()) {
    avg_c.resize(coords.size(), 0.f), avg_a.resize(coords.size(), 0.f);
    avg_t.resize(coords.size(), 0.f), avg_s.resize(coords.size(), 0.f);
  }
  vector<vec2> rects;
  gen_abh(coords, rects, yc, r_min, r_max, D, pa0, pa1, pa2, pa3, pa4, pa5, pb0,
          pb1, pb2, pb3, pb4, pb5, ph0, ph1, ph2, ph3, ph4, ph5);

  for (int i_day = 0; i_day < 12; i_day++) {
    for (int i_hour = 0; i_hour < 5; i_hour++) {

      float phi, theta;
      get_sun_info(days[i_day], hours[i_hour], lat, phi, theta);
      float dni = get_dni(phi, height);

      // cout << "phi,theta,dni: " << phi << ", " << theta << ", " << dni <<
      // endl;

      nears.resize(coords.size());
      for (int i = 0; i < coords.size(); i++)
        nears[i] = i;

      eta_c.resize(coords.size());
      eta_a.resize(coords.size());
      eta_t.resize(coords.size());
      eta_s.resize(coords.size());

      for (int i_coord = 0; i_coord < coords.size(); i_coord++) {
        calcetas(coords, rects, i_coord, nears.size(), nears.data(), R_effect,
                 yc, hc1, hc2, rc, theta, phi, Omega, eta_c[i_coord],
                 eta_a[i_coord], eta_t[i_coord], eta_s[i_coord]);
      }

      if (!save_fn.empty()) {
        for (int i = 0; i < coords.size(); i++) {
          avg_c[i] += eta_c[i] / 60, avg_a[i] += eta_a[i] / 60;
          avg_t[i] += eta_t[i] / 60, avg_s[i] += eta_s[i] / 60;
        }
      }

      calcflow(rects, eta_c, eta_a, eta_t, eta_s, dni, tot[i_day * 5 + i_hour],
               per[i_day * 5 + i_hour]);
    } // for hours
  } // for days

  if (!save_fn.empty()) {
    FILE *fp;
    fopen_s(&fp, save_fn.c_str(), "wb");
    for (int i = 0; i < coords.size(); i++) {
      fprintf(fp, "%f, %f, %f, %f, %f, %f, %f, %f, %f\n", coords[i].x,
              coords[i].y, coords[i].z, rects[i].x, rects[i].y, avg_c[i],
              avg_a[i], avg_t[i], avg_s[i]);
    }
    fflush(fp);
    fclose(fp);
  }
}

static bool def_within(float x, float y) {
  return x * x + y * y < 350.f * 350.f;
}

extern float ParticleOptPenalty;

float fitness_v2(const std::vector<float> &p) {
  const float r_min = 100;
  const float r_effect = 30;
  const float f_thre = 60000;

  if (p.size() < 21) {
    cout << "\033[38;5;9mtoo less parm\0330m\n";
    exit(-1);
  }
  // trim r_max to 300-700
  float r_max = clip(300 + p[0] * 400, 300, 700);
  // trim D to 2+5 - 8+5
  float D = clip(7 + p[1] * 6, 7, 13);
  // trim yc to - (0 - 350)
  float yc = -clip(p[2] * 350, 0, 350);

  vector<float> tot, per;

  printf("v2 input: R_max=%f, D=%f, yc=%f, penalty=%f, parms=", r_max, D, yc,
         ParticleOptPenalty);
  for (int i = 0; i < p.size(); i++) {
    printf("%.2f, ", p[i]);
  }
  cout << ")" << endl;

  fitness_v1(r_min, r_max, D, yc, r_effect, def_within, p[3], p[4], p[5], p[6],
             p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15], p[16],
             p[17], p[18], p[19], p[20], tot, per);

  float ftot = 0, ftot_real = 0;
  for (int i = 0; i < tot.size(); i++) {
    ftot += tot[i], ftot_real += tot[i] / per[i];
  }
  float fper = ftot / ftot_real;
  ftot /= tot.size();

  cout << "v2: tot,per: " << ftot << " " << fper << endl;

  return -fper + std::max(f_thre - ftot, 0.f) * ParticleOptPenalty;
}

void dump_args(const std::vector<float> &p, const std::string &fn) {
  const float r_min = 100;
  const float r_effect = 30;

  float r_max = clip(300 + p[0] * 400, 300, 700);
  float D = clip(7 + p[1] * 6, 7, 13);
  float yc = -clip(p[2] * 350, 0, 350);
  vector<float> tot, per;
  fitness_v1(r_min, r_max, D, yc, r_effect, def_within, p[3], p[4], p[5], p[6],
             p[7], p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15], p[16],
             p[17], p[18], p[19], p[20], tot, per, fn);
}
