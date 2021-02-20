#pragma once

#define fast_calc_precision 11
#include <cmath>
#include <functional>
#include <random>

#ifdef _WIN32
#include <Windows.h>
#elif __unix__
#include <unistd.h>
#endif

struct iv2;
struct iv3;
struct iv4;
struct v2
{
  float x, y;
  v2()
  {
    this->x = 0;
    this->y = 0;
  }
  v2(float x, float y)
  {
    this->x = x;
    this->y = y;
  }
  v2(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  v2(float d)
  {
    this->x = d;
    this->y = 0;
  }
  v2(const iv2& base);
  v2 operator+(const v2 a) { return v2(x + a.x, y + a.y); }
  v2 operator-(const v2 a) { return v2(x - a.x, y - a.y); }
  void operator+=(const v2 a)
  {
    x += a.x;
    y += a.y;
  }
  void operator-=(const v2 a)
  {
    x -= a.x;
    y -= a.y;
  }
  v2 operator+(const float a) { return v2(x + a, y + a); }
  v2 operator-(const float a) { return v2(x - a, y - a); }
  v2 operator*(const float a) { return v2(x * a, y * a); }
  v2 operator/(const float a) { return v2(x / a, y / a); }
  void operator+=(const float a)
  {
    x += a;
    y += a;
  }
  void operator-=(const float a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const float a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const float a)
  {
    x /= a;
    y /= a;
  }
};
struct v3
{
  float x, y, z;
  v3()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  v3(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  v3(const v2 &vector2)
  {
    this->x = vector2.x;
    this->y = vector2.y;
    this->z = 0;
  }
  v3(float d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
  }
  v3(const iv3&base);
  v3 operator+(const v3 a) { return v3(x + a.x, y + a.y, z + a.z); }
  v3 operator-(const v3 a) { return v3(x - a.x, y - a.y, z - a.z); }
  void operator+=(const v3 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
  }
  void operator-=(const v3 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
  }
  v3 operator+(const float a) { return v3(x + a, y + a, z + a); }
  v3 operator-(const float a) { return v3(x - a, y - a, z - a); }
  v3 operator*(const float a) { return v3(x * a, y * a, z * a); }
  v3 operator/(const float a) { return v3(x / a, y / a, z / a); }
  void operator+=(const float a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const float a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const float a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const float a)
  {
    x /= a;
    y /= a;
    z /= a;
  }
  v3 &operator=(const v2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  v2 toV2() { return v2(x, y); }
};
struct v4
{
  float x, y, z, w;
  v4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  v4(float x, float y, float z, float w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  v4(const v3 &vector3)
  {
    this->x = vector3.x;
    this->y = vector3.y;
    this->z = vector3.z;
    this->w = 0;
  }
  v4(const v2 &vector2)
  {
    this->x = vector2.x;
    this->y = vector2.y;
    this->z = 0;
    this->w = 0;
  }
  v4(float d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  v4(const iv4&base);
  v4 operator+(const v4 a) { return v4(x + a.x, y + a.y, z + a.z, w + a.w); }
  v4 operator-(const v4 a) { return v4(x - a.x, y - a.y, z - a.z, w - a.w); }
  void operator+=(const v4 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
  }
  void operator-=(const v4 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
  }
  v4 operator+(const float a) { return v4(x + a, y + a, z + a, w + a); }
  v4 operator-(const float a) { return v4(x - a, y - a, z - a, w - a); }
  v4 operator*(const float a) { return v4(x * a, y * a, z * a, w * a); }
  v4 operator/(const float a) { return v4(x / a, y / a, z / a, w / a); }
  void operator+=(const float a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const float a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const float a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const float a)
  {
    x /= a;
    y /= a;
    z /= a;
    z /= a;
  }
  v4 &operator=(const v2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  v4 &operator=(const v3 &a)
  {
    this->x = a.x;
    this->y = a.y;
    this->z = a.z;
    return *this;
  }
  v2 toV2() { return v2(x, y); }
  v3 toV3() { return v3(x, y, z); }
};

struct iv2
{
  int x, y;
  iv2()
  {
    this->x = 0;
    this->y = 0;
  }
  iv2(int x, int y)
  {
    this->x = x;
    this->y = y;
  }
  iv2(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  iv2(int d)
  {
    this->x = d;
    this->y = 0;
  }
  iv2(const v2 &base)
  {
    this->x = std::floor(base.x);
    this->y = std::floor(base.y);
  }
  bool operator==(const iv2 &a) { return x == a.x && y == a.y; }
  bool operator!=(const iv2 &a) { return x != a.x || y != a.y; }
  iv2 operator+(const iv2 a) { return iv2(x + a.x, y + a.y); }
  iv2 operator-(const iv2 a) { return iv2(x - a.x, y - a.y); }
  void operator+=(const iv2 a)
  {
    x += a.x;
    y += a.y;
  }
  void operator-=(const iv2 a)
  {
    x -= a.x;
    y -= a.y;
  }
  iv2 operator+(const int a) { return iv2(x + a, y + a); }
  iv2 operator-(const int a) { return iv2(x - a, y - a); }
  iv2 operator*(const int a) { return iv2(x * a, y * a); }
  iv2 operator/(const int a) { return iv2(x / a, y / a); }
  void operator+=(const int a)
  {
    x += a;
    y += a;
  }
  void operator-=(const int a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const int a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const int a)
  {
    x /= a;
    y /= a;
  }
};
struct iv3
{
  int x, y, z;
  iv3()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  iv3(int x, int y, int z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  iv3(const iv2 &vector2)
  {
    this->x = vector2.x;
    this->y = vector2.y;
    this->z = 0;
  }
  iv3(int d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
  }
  iv3(const v3&base){
    this->x=std::floor(base.x);
    this->y=std::floor(base.y);
    this->z = std::floor(base.z);
  }
  bool operator==(const iv3 &a) { return x == a.x && y == a.y && z == a.z; }
  bool operator!=(const iv3 &a) { return x != a.x || y != a.y || z != a.z; }
  iv3 operator+(const iv3 a) { return iv3(x + a.x, y + a.y, z + a.z); }
  iv3 operator-(const iv3 a) { return iv3(x - a.x, y - a.y, z - a.z); }
  void operator+=(const iv3 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
  }
  void operator-=(const iv3 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
  }
  iv3 operator+(const int a) { return iv3(x + a, y + a, z + a); }
  iv3 operator-(const int a) { return iv3(x - a, y - a, z - a); }
  iv3 operator*(const int a) { return iv3(x * a, y * a, z * a); }
  iv3 operator/(const int a) { return iv3(x / a, y / a, z / a); }
  void operator+=(const int a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const int a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const int a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const int a)
  {
    x /= a;
    y /= a;
    z /= a;
  }
  iv3 &operator=(const iv2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  iv2 toIV2() { return iv2(x, y); }
};
struct iv4
{
  int x, y, z, w;
  iv4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  iv4(int x, int y, int z, int w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  iv4(const iv3 &vector3)
  {
    this->x = vector3.x;
    this->y = vector3.y;
    this->z = vector3.z;
    this->w = 0;
  }
  iv4(const iv2 &vector2)
  {
    this->x = vector2.x;
    this->y = vector2.y;
    this->z = 0;
    this->w = 0;
  }
  iv4(int d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  iv4(const v4&base)
  {
    this->x = std::floor(base.x);
    this->y = std::floor(base.y);
    this->z = std::floor(base.z);
  }
  bool operator==(const iv4 &a)
  {
    return x == a.x && y == a.y && z == a.z && w == a.w;
  }
  bool operator!=(const iv4 &a)
  {
    return x != a.x || y != a.y || z != a.z || w != a.w;
  }
  iv4 operator+(const iv4 a) { return iv4(x + a.x, y + a.y, z + a.z, w + a.w); }
  iv4 operator-(const iv4 a) { return iv4(x - a.x, y - a.y, z - a.z, w - a.w); }
  void operator+=(const iv4 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
  }
  void operator-=(const iv4 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
  }
  iv4 operator+(const int a) { return iv4(x + a, y + a, z + a, w + a); }
  iv4 operator-(const int a) { return iv4(x - a, y - a, z - a, w - a); }
  iv4 operator*(const int a) { return iv4(x * a, y * a, z * a, w * a); }
  iv4 operator/(const int a) { return iv4(x / a, y / a, z / a, w / a); }
  void operator+=(const int a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const int a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const int a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const int a)
  {
    x /= a;
    y /= a;
    z /= a;
    z /= a;
  }
  iv4 &operator=(const iv2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  iv4 &operator=(const iv3 &a)
  {
    this->x = a.x;
    this->y = a.y;
    this->z = a.z;
    return *this;
  }
  iv2 toIV2() { return iv2(x, y); }
  iv3 toIV3() { return iv3(x, y, z); }
};
inline v2::v2(const iv2 &base)
{
  this->x = base.x;
  this->y = base.y;
}
inline v3::v3(const iv3 &base)
{
  this->x = base.x;
  this->y = base.y;
  this->z=base.z;
}
inline v4::v4(const iv4 &base)
{
  this->x = base.x;
  this->y = base.y;
  this->z=base.z;
  this->w=base.w;
}
#define v2ToArray(v) {v.x,v.y}
#define v3ToArray(v) {v.x,v.y,v.z}
#define v4ToArray(v) {v.x,v.y,v.z,v.w} 

inline float randf()// randome float between 0-1
{
  return ((float)std::rand() / (RAND_MAX));
}

struct color
{
  unsigned char r, g, b, a;
  color()
  {
    r = 0;
    g = 0;
    b = 0;
    a = 0;
  }
  color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
  {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
  }
  color(const color &base)
  {
    this->r = base.r;
    this->g = base.g;
    this->b = base.b;
    this->a = base.a;
  }
};

enum Key
{
  SPACE,
  APOSTROPHE,
  COMMA,
  MINUS,
  PERIOD,
  SLASH,
  KEY_0,
  KEY_1,
  KEY_2,
  KEY_3,
  KEY_4,
  KEY_5,
  KEY_6,
  KEY_7,
  KEY_8,
  KEY_9,
  SEMICOLON,
  EQUAL,
  KEY_A,
  KEY_B,
  KEY_C,
  KEY_D,
  KEY_E,
  KEY_F,
  KEY_G,
  KEY_H,
  KEY_I,
  KEY_J,
  KEY_K,
  KEY_L,
  KEY_M,
  KEY_N,
  KEY_O,
  KEY_P,
  KEY_Q,
  KEY_R,
  KEY_S,
  KEY_T,
  KEY_U,
  KEY_V,
  KEY_W,
  KEY_X,
  KEY_Y,
  KEY_Z,
  LEFT_BRACKET,
  BACKSLASH,
  BRACKET,
  GRAVE_ACCENT,
  ESCAPE,
  ENTER,
  TAB,
  BACKSPACE,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_DOWN,
  KEY_UP,
  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
  F13,
  F14,
  F15,
  F16,
  F17,
  F18,
  F19,
  F20,
  F21,
  F22,
  F23,
  F24,
  F25,
  LEFT_SHIFT,
  LEFT_CONTROL,
  LEFT_ALT,
  LEFT_SUPER,
  RIGHT_SHIFT,
  RIGHT_CONTROL,
  RIGHT_ALT,
  UNREGISTERED_KEY,
  KEY_LAST = UNREGISTERED_KEY,
  MOUSE1,
  MOUSE_LEFT = MOUSE1,
  MOUSE2,
  MOUSE_RIGHT = MOUSE2,
  MOUSE3,
  MOUSE_MIDDLE = MOUSE3,
  MOUSE4,
  MOUSE5,
  MOUSE6,
  MOUSE7,
  MOUSE8,
  MOUSE_LAST = MOUSE8
};

inline void sleepMillis(unsigned int millis)
{
#ifdef _WIN32
  Sleep(millis);
#elif __unix__
  usleep(millis * 1000);
#endif
}

void threadDetach(std::function<void(void)> func);
std::function<void(void)> threadLaunch(std::function<void(void)> func);
#ifdef _WIN32
#elif __unix__ | __APPLE__
#include <thread>
inline void threadDetach(std::function<void(void)> func)
{
  std::thread t = std::thread(func);
  t.detach();
}

inline std::function<void(void)> threadLaunch(std::function<void(void)> func)
{
  std::thread *t = new std::thread(func);
  return [t]() {t->join();delete t; };
}
#endif
class threaded_job final
{ // use "fflush(stdout)" if printing
private:
  bool jobStart = false, jobEnd = true, alive = true;
  std::function<void(void)> threadJoin;
  std::function<void(void)> job;

public:
  threaded_job(std::function<void(void)> job)
  {
    this->job = job;
    threadJoin = threadLaunch([this]() {
      while (alive)
      {
        if (jobStart)
        {
          jobStart = false;
          jobEnd = false;
          this->job();
          jobEnd = true;
        }
      }
    });
  }
  threaded_job(const threaded_job &base) : threaded_job(base.job)
  {
  }
  ~threaded_job()
  {
    this->alive = false;
    threadJoin();
  }
  void start() // start job
  {
    if (jobEnd)
    {
      jobStart = true;
      jobEnd = false;
    }
  }
  void join() // wait until job has finished executing
  {
    while (!jobEnd)
    {
    }
  }
  void stop() //use with caution, if used from inside thread, will cause hang loop
  {
    this->alive = false;
    threadJoin();
  }
  void detach() //stops thread but will only join when object destroyed
  {
    alive = false;
  }
  void changeJob(std::function<void(void)> newJob) // changes the function called on job.start(), not tested
  {
    this->job = newJob;
  }
};

namespace kNgine
{
  struct mapper
  {
    v4 min, max, targetMin, targetMax;
    mapper()
    {
      this->min = v4();
      this->max = v4();
      this->targetMin = v4();
      this->targetMax = v4();
    }
    mapper(float min, float max, float targetMin, float targetMax)
    {
      this->min = v4(min);
      this->max = v4(max);
      this->targetMin = v4(targetMin);
      this->targetMax = v4(targetMax);
    }
    mapper(v2 min, v2 max, v2 targetMin, v2 targetMax)
    {
      this->min = v4(min);
      this->max = v4(max);
      this->targetMin = v4(targetMin);
      this->targetMax = v4(targetMax);
    }
    mapper(v3 min, v3 max, v3 targetMin, v3 targetMax)
    {
      this->min = v4(min);
      this->max = v4(max);
      this->targetMin = v4(targetMin);
      this->targetMax = v4(targetMax);
    }
    mapper(v4 min, v4 max, v4 targetMin, v4 targetMax)
    {
      this->min = v4(min);
      this->max = v4(max);
      this->targetMin = v4(targetMin);
      this->targetMax = v4(targetMax);
    }
    float map(float n)
    {
      float scale = (v4(n) - min).x / (max - min).x;
      return scale * (targetMax - targetMin).x + targetMin.x;
    }
    v2 map(v2 n)
    {
      float xScale = (v4(n) - min).x / (max - min).x;
      float yScale = (v4(n) - min).y / (max - min).y;
      return v2(xScale * (targetMax - targetMin).x + targetMin.x,
                yScale * (targetMax - targetMin).y + targetMin.y);
    }
    v3 map(v3 n)
    {
      float xScale = (v4(n) - min).x / (max - min).x;
      float yScale = (v4(n) - min).y / (max - min).y;
      float zScale = (v4(n) - min).z / (max - min).z;
      return v3(xScale * (targetMax - targetMin).x + targetMin.x,
                yScale * (targetMax - targetMin).y + targetMin.y,
                zScale * (targetMax - targetMin).z + targetMin.z);
    }
    v4 map(v4 n)
    {
      float xScale = (v4(n) - min).x / (max - min).x;
      float yScale = (v4(n) - min).y / (max - min).y;
      float zScale = (v4(n) - min).z / (max - min).z;
      float wScale = (v4(n) - min).w / (max - min).w;
      return v4(xScale * (targetMax - targetMin).x + targetMin.x,
                yScale * (targetMax - targetMin).y + targetMin.y,
                zScale * (targetMax - targetMin).z + targetMin.z,
                wScale * (targetMax - targetMin).w + targetMin.w);
    }
  };

  inline bool lineCross(v2 s1, v2 end1, v2 s2, v2 end2)
  {
    float uA =
        ((end2.x - s2.x) * (s1.y - s2.y) - (end2.y - s2.y) * (s1.x - s2.x)) /
        ((end2.y - s2.y) * (end1.x - s1.x) - (end2.x - s2.x) * (end1.y - s1.y));
    float uB =
        ((end1.x - s1.x) * (s1.y - s2.y) - (end1.y - s1.y) * (s1.x - s2.x)) /
        ((end2.y - s2.y) * (end1.x - s1.x) - (end2.x - s2.x) * (end1.y - s1.y));
    return uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1;
  }
  inline v2 collisionLine(v2 s1, v2 end1, v2 s2, v2 end2)
  {
    float uA =
        ((end2.x - s2.x) * (s1.y - s2.y) - (end2.y - s2.y) * (s1.x - s2.x)) /
        ((end2.y - s2.y) * (end1.x - s1.x) - (end2.x - s2.x) * (end1.y - s1.y));
    float uB =
        ((end1.x - s1.x) * (s1.y - s2.y) - (end1.y - s1.y) * (s1.x - s2.x)) /
        ((end2.y - s2.y) * (end1.x - s1.x) - (end2.x - s2.x) * (end1.y - s1.y));
    return v2(s1.x + (uA * (end1.x - s1.x)), s1.y + (uA * (end1.y - s1.y)));
  }
  // square of distance
  inline float optimizedLineLength(v4 start, v4 end)
  {
    v4 distance = end - start;
    return distance.x * distance.x + distance.y * distance.y +
           distance.z * distance.z + distance.w * distance.w;
  }
  inline float lineLength(v4 start, v4 end)
  {
    return std::sqrt(optimizedLineLength(start, end));
  }
  inline float regularizeDegree(float angle)
  {
    float degree = std::fmod(angle, 360);
    if (degree < 0)
      degree += 360;
    return degree;
  }
  inline float regularizeRad(float angle)
  {
    float rad = std::fmod(angle, 2 * M_PI);
    if (rad < 0)
      rad += 2 * M_PI;
    return rad;
  }
  inline float degreeToRad(float degree) { return degree / 180.0 * M_PI; }
  inline float radToDegree(float rad) { return rad / M_PI * 180; }
  // in radians
  inline float fast_cos(float angle)
  {
    float t_angle = regularizeRad(angle) - M_PI;
    float res = 0;
    float x = 1;
    int precision = fast_calc_precision;
    const int factorialPrecision = 20;
    unsigned long long factorials[factorialPrecision] = {1,
                                                         1,
                                                         2,
                                                         6,
                                                         24,
                                                         120,
                                                         720,
                                                         5040,
                                                         40320,
                                                         362880,
                                                         3628800,
                                                         39916800,
                                                         479001600,
                                                         6227020800,
                                                         87178291200,
                                                         1307674368000,
                                                         20922789888000,
                                                         355687428096000,
                                                         6402373705728000,
                                                         121645100408832000};
    for (int i = 0; i < precision && i < factorialPrecision; i += 4)
    {
      res += x / factorials[i];
      x *= t_angle * t_angle;
      res -= x / factorials[i + 2];
      x *= t_angle * t_angle;
    }
    return -res;
  }
  inline float fast_sin(float angle)
  {
    float t_angle = regularizeRad(angle) - M_PI;
    float res = 0;
    float x = t_angle;
    int precision = fast_calc_precision;
    const int factorialPrecision = 21;
    unsigned long long factorials[factorialPrecision] = {1,
                                                         1,
                                                         2,
                                                         6,
                                                         24,
                                                         120,
                                                         720,
                                                         5040,
                                                         40320,
                                                         362880,
                                                         3628800,
                                                         39916800,
                                                         479001600,
                                                         6227020800,
                                                         87178291200,
                                                         1307674368000,
                                                         20922789888000,
                                                         355687428096000,
                                                         6402373705728000,
                                                         121645100408832000,
                                                         2432902008176640000};
    for (int i = 1; i < precision; i += 4)
    {
      res += x / factorials[i];
      x *= t_angle * t_angle;
      res -= x / factorials[i + 2];
      x *= t_angle * t_angle;
    }
    return -res;
  }
  inline float fast_atan(float a)
  {
    if ((a > -1.5 && a < -0.7) || (a < 1.4 && a >= 0.6))
    {
      return std::atan(a);
    }
    if (a > -1 && a < 1)
    {
      float res = 0;
      float x = a;
      int precision = fast_calc_precision;
      for (int i = 1; i < precision * 2; i += 2)
      {
        res += x / (4 * i - 3);
        x *= a * a;
        res -= x / (4 * i - 1);
        x *= a * a;
      }
      return res;
    }
    else
    {
      float res = 0;
      float x = -a;
      int precision = 11;
      for (int i = 1; i < precision * 2; i += 2)
      {
        res += 1.0 / ((4 * i - 3) * x);
        x *= a * a;
        res -= 1.0 / ((4 * i - 1) * x);
        x *= a * a;
      }
      if (x < 0)
      {
        res += M_PI / 2;
      }
      else
      {
        res -= M_PI / 2;
      }
      return res;
    }
  }
  inline v2 rotateAroundPointRadian(v2 center, v2 point, float angle)
  {
    float c = fast_cos(angle);
    float s = fast_sin(angle);
    v2 rPoint = point - center;
    v2 res = v2();
    res.x = rPoint.x * c - rPoint.y * s;
    res.y = rPoint.x * s + rPoint.y * c;
    res += center;
    return res;
  }
  inline v2 rotateAroundPointDegree(v2 center, v2 point, float angle)
  {
    return rotateAroundPointRadian(center, point, degreeToRad(angle));
  }
  inline v2 getPointAroundCircleRadian(float angle)
  {
    return v2(fast_cos(angle), fast_sin(angle));
  }
  inline v2 getPointAroundCircleDegree(float angle)
  {
    return getPointAroundCircleRadian(degreeToRad(angle));
  }
} // namespace kNgine
