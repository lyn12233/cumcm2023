#pragma once

struct vec2 {
  float x, y;
};
typedef struct vec2 vec2_t;

struct vec3 {
  float x, y, z;
  vec3(float x = 0, float y = 0, float z = 0);
  vec3 cross(const vec3 &v) const;
  float dist() const;
  static float dist(const vec3 &v);
  float dot(const vec3 &v) const;
  vec3 norm() const;
  static vec3 norm(const vec3 &v);
  vec3 operator/(float) const;
  vec3 operator+(const vec3 &v) const;
  vec3 operator-(const vec3 &v) const;
  vec3 operator-() const;
};
typedef struct vec3 vec3_t;