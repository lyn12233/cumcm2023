#include "vec_types.hpp"

#include <cmath>

vec3 vec3::cross(const vec3 &v) const {
  return {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x};
}

float vec3::dist() const { return vec3::dist(*this); }
float vec3::dist(const vec3 &v) {
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float vec3::dot(const vec3 &v) const { return x * v.x + y * v.y + z * v.z; }

vec3 vec3::norm() const { return vec3::norm(*this); }

vec3 vec3::norm(const vec3 &v) {
  float dist = v.dist();
  if (dist < 1e-6)
    return {0};
  return v / dist;
}

vec3 vec3::operator/(float f) const { return {x / f, y / f, z / f}; }

vec3 vec3::operator+(const vec3 &v) const {
  return {x + v.x, y + v.y, z + v.z};
}
vec3 vec3::operator-(const vec3 &v) const {
  return {x - v.x, y - v.y, z - v.z};
}
vec3 vec3::operator-() const { return {-x, -y, -z}; }