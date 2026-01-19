#include "coord_alg.hpp"

#include <cstdio>
#include <ctime>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;

bool within(float x, float y) { return x * x + y * y < 300 * 300; }

int main() {

  float yc = 0.f;

  clock_t t0 = clock();

  vector<vec3> coords;
  gen_coord(100, 300, 12, yc, within, coords);
  vector<vec2> rects;
  gen_abh(coords, rects, -100, 100, 300, 12, //
          1, 1, 1, 1, 1, 1,                  //
          1, 1, 1, 1, 1, 1,                  //
          1, 1, 1, 1, 1, 1);

  clock_t t1 = clock();

  auto nears = vector<int>(coords.size());
  for (int i = 0; i < nears.size(); i++) {
    nears[i] = i;
  }
  auto eta_cs = vector<float>(coords.size());
  auto eta_as = vector<float>(coords.size());
  auto eta_ts = vector<float>(coords.size());
  auto eta_ss = vector<float>(coords.size());

  for (int i = 0; i < coords.size(); i++) {
    calcetas(coords, rects, i, nears.size(), nears.data(), //
             30,                                           // Reffect
             yc, 80, 8, 3.5,                               // yc,hc1,hc2,rc
             180 * 3.14 / 180, 52 * 3.14 / 180,
             0.53 * 3.14 / 180,                         // theta,phi,Omega
             eta_cs[i], eta_as[i], eta_ts[i], eta_ss[i] //
    );
    // cout << "step " << i << endl;
  }

  clock_t t2 = clock();

  float tot, per;
  float dni = 1;
  calcflow(rects, eta_cs, eta_as, eta_ts, eta_ss, dni, tot, per);

  clock_t t3 = clock();

  cout << "generation use time(ms): " << t1 - t0
       << ", calculation use time(ms): " << t2 - t1
       << ", summation use time(ms): " << t3 - t2 << endl;

  cout << "total input walt(kW): " << tot << endl;
  cout << "inpul walt per reflector: " << per << endl;

  // dump these in csv: (x,y,h,a,b,cos,atmosphere,truncate,shadow)
  FILE *fp;
  fopen_s(&fp, "tmp.csv", "wb");
  for (int i = 0; i < coords.size(); i++) {
    fprintf(fp, "%f, %f, %f, %f, %f, %f, %f, %f, %f\n", //
            coords[i].x, coords[i].y, coords[i].z,      //
            rects[i].x, rects[i].y,                     //
            eta_cs[i], eta_as[i], eta_ts[i], eta_ss[i]  //
    );
  }
  fclose(fp);
}