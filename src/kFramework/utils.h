#pragma once

// general utils functions, can work with both c and c++

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#ifdef utils_print
#ifndef __cplusplus
#include <stdio.h>
#define println(x) printf("%s\n",x);
#define printv2(v) printf("{%f,%f}",v.x,v.y);
#define printv3(v) printf("{%f,%f,%f}", v.x, v.y,v.z);
#define printv4(v) printf("{%f,%f,%f,%f}", v.x, v.y,v.z,v.w);
#else
#include <iostream>
#define print(x) std::cout<<x;
#define println(x) std::cout << x << std::endl;
#define printv2(v) std::cout << "{" << v.x << "," << v.y << "}" << std::endl;
#define printv3(v) std::cout << "{" << v.x << "," << v.y << "," << v.z << "}" << std::endl;
#define printv4(v) std::cout << "{" << v.x << "," << v.y << "," << v.z << "," << v.w << "}" << std::endl;
#endif
#endif

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define PIf 3.1415926535897f
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PI 0.3989422804
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PIf 0.3989422804f
#define EULERS_NUMBER 2.7182818284590452353602874713527
#define EULERS_NUMBERf 2.7182818284590452353602874713527f

#ifndef utils_utils_FastCalcPrecision
#define utils_FastCalcPrecision 9
#endif

#ifndef utils_NoTypedefs
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef u8 b8;
#ifndef __cplusplus
#define bool u8
#define true 1
#define false 0

#endif
#endif


#ifdef __cplusplus
union v2
{
  struct
  {
    float x;
    float y;
  };
  float elements[2];
  v2 operator+(const v2& a) { return {x + a.x, y + a.y}; }
  v2 operator-(const v2& a) { return {x - a.x, y - a.y}; }
  void operator+=(const v2& a)
  {
    x += a.x;
    y += a.y;
  }
  void operator-=(const v2& a)
  {
    x -= a.x;
    y -= a.y;
  }
  v2 operator+(const float& a) { return {x + a, y + a}; }
  v2 operator-(const float& a) { return {x - a, y - a}; }
  v2 operator*(const float& a) { return {x * a, y * a}; }
  v2 operator/(const float& a) { return {x / a, y / a}; }
  void operator+=(const float& a)
  {
    x += a;
    y += a;
  }
  void operator-=(const float& a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const float& a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const float& a)
  {
    x /= a;
    y /= a;
  }
};
union v3
{
  struct
  {
    float x;
    float y;
    float z;
  };
  struct
  {
    float r;
    float g;
    float b;
  };
  float elements[3];
  v3 operator+(const v3& a) { return {x + a.x, y + a.y, z + a.z}; }
  v3 operator-(const v3& a) { return {x - a.x, y - a.y, z - a.z}; }
  void operator+=(const v3& a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
  }
  void operator-=(const v3& a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
  }
  v3 operator+(const float& a) { return {x + a, y + a, z + a}; }
  v3 operator-(const float& a) { return {x - a, y - a, z - a}; }
  v3 operator*(const float& a) { return {x * a, y * a, z * a}; }
  v3 operator/(const float& a) { return {x / a, y / a, z / a}; }
  void operator+=(const float& a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const float& a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const float& a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const float& a)
  {
    x /= a;
    y /= a;
    z /= a;
  }
};
union v4
{
  struct
  {
    float x;
    float y;
    union
    {
      struct
      {
        float z;

        union
        {
          float w;
          float radius;
        };
      };
      struct
      {
        float width;
        float height;
      };
    };
  };
  struct
  {
    v3 xyz;
    float _unused;
  };
  struct
  {
    float r;
    float g;
    float b;
    float a;
  };
  float elements[4];
  v4 operator+(const v4& a) { return {x + a.x, y + a.y, z + a.z, w + a.w}; }
  v4 operator-(const v4& a) { return {x - a.x, y - a.y, z - a.z, w - a.w}; }
  void operator+=(const v4& a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
  }
  void operator-=(const v4& a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
  }
  v4 operator+(const float& a) { return {x + a, y + a, z + a, w + a}; }
  v4 operator-(const float& a) { return {x - a, y - a, z - a, w - a}; }
  v4 operator*(const float& a) { return {x * a, y * a, z * a, w * a}; }
  v4 operator/(const float& a) { return {x / a, y / a, z / a, w / a}; }
  void operator+=(const float& a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const float& a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const float& a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const float& a)
  {
    x /= a;
    y /= a;
    z /= a;
    z /= a;
  }
};

static v2 V2Init(float x, float y)
{
  v2 v = {x, y};
  return v;
}
#define v2(x, y) V2Init(x, y)
static v3 V3Init(float x, float y, float z)
{
  v3 v = {x, y, z};
  return v;
}
#define v3(x, y, z) V3Init(x, y, z)
static v4 V4Init(float x, float y, float z, float w)
{
  v4 v = {x, y, z, w};
  return v;
}
#define v4(x, y, z, w) V4Init(x, y, z, w)
#define v4u(x) v4(x, x, x, x)

union iv2
{
  struct
  {
    int32_t x;
    int32_t y;
  };
  int32_t elements[2];
  iv2 operator+(const iv2 &a) { return {x + a.x, y + a.y}; }
  iv2 operator-(const iv2 &a) { return {x - a.x, y - a.y}; }
  void operator+=(const iv2 &a)
  {
    x += a.x;
    y += a.y;
  }
  void operator-=(const iv2 &a)
  {
    x -= a.x;
    y -= a.y;
  }
  iv2 operator+(const int32_t& a) { return {x + a, y + a}; }
  iv2 operator-(const int32_t& a) { return {x - a, y - a}; }
  iv2 operator*(const int32_t& a) { return {x * a, y * a}; }
  iv2 operator/(const int32_t& a) { return {x / a, y / a}; }
  void operator+=(const int32_t &a)
  {
    x += a;
    y += a;
  }
  void operator-=(const int32_t &a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const int32_t &a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const int32_t &a)
  {
    x /= a;
    y /= a;
  }
  bool operator==(const iv2& a){return x==a.x&&y==a.y;}
  bool operator!=(const iv2& a){return !(x==a.x&&y==a.y);}
};
union iv3
{
  struct
  {
    int32_t x;
    int32_t y;
    int32_t z;
  };
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
  };
  int32_t elements[3];
  iv3 operator+(const iv3 &a) { return {x + a.x, y + a.y, z + a.z}; }
  iv3 operator-(const iv3 &a) { return {x - a.x, y - a.y, z - a.z}; }
  void operator+=(const iv3 &a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
  }
  void operator-=(const iv3 &a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
  }
  iv3 operator+(const int32_t &a) { return {x + a, y + a, z + a}; }
  iv3 operator-(const int32_t &a) { return {x - a, y - a, z - a}; }
  iv3 operator*(const int32_t &a) { return {x * a, y * a, z * a}; }
  iv3 operator/(const int32_t &a) { return {x / a, y / a, z / a}; }
  void operator+=(const int32_t &a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const int32_t &a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const int32_t &a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const int32_t &a)
  {
    x /= a;
    y /= a;
    z /= a;
  }
  bool operator==(const iv3 &a) { return x == a.x && y == a.y&&z==a.z; }
  bool operator!=(const iv3 &a) { return !(x == a.x && y == a.y&&z==a.z); }
};
union iv4
{
  struct
  {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
  };
  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t a;
  };
  int32_t elements[4];
  iv4 operator+(const iv4 &a) { return {x + a.x, y + a.y, z + a.z, w + a.w}; }
  iv4 operator-(const iv4 &a) { return {x - a.x, y - a.y, z - a.z, w - a.w}; }
  void operator+=(const iv4 &a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
  }
  void operator-=(const iv4 &a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
  }
  iv4 operator+(const int32_t &a) { return {x + a, y + a, z + a, w + a}; }
  iv4 operator-(const int32_t &a) { return {x - a, y - a, z - a, w - a}; }
  iv4 operator*(const int32_t &a) { return {x * a, y * a, z * a, w * a}; }
  iv4 operator/(const int32_t &a) { return {x / a, y / a, z / a, w / a}; }
  void operator+=(const int32_t &a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const int32_t &a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const int32_t &a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const int32_t &a)
  {
    x /= a;
    y /= a;
    z /= a;
    z /= a;
  }
  bool operator==(const iv4 &a) { return x == a.x && y == a.y && z == a.z && w==a.w; }
  bool operator!=(const iv4 &a) { return !(x == a.x && y == a.y && z == a.z && w==a.w); }
};
static iv2 IV2Init(int32_t x, int32_t y)
{
  iv2 v = {x, y};
  return v;
}
#define iv2(x, y) IV2Init(x, y)
static iv3 IV3Init(int32_t x, int32_t y, int32_t z)
{
  iv3 v = {x, y, z};
  return v;
}
#define iv3(x, y, z) IV3Init(x, y, z)
static iv4 IV4Init(int32_t x, int32_t y, int32_t z, int32_t w)
{
  iv4 v = {x, y, z, w};
  return v;
}
#define iv4(x, y, z, w) IV4Init(x, y, z, w)

#else
typedef union v2
{
  struct{
    float x;
    float y;
  };
  float elements[2];
} v2;
typedef union v3
{
  struct
  {
    float x;
    float y;
    float z;
  };

  struct
  {
    float r;
    float g;
    float b;
  };

  float elements[3];
} v3;
typedef union v4
{
  struct
  {
    float x;
    float y;
    union
    {
      struct
      {
        float z;

        union
        {
          float w;
          float radius;
        };
      };
      struct
      {
        float width;
        float height;
      };
    };
  };

  struct
  {
    v3 xyz;
    float _unused;
  };

  struct
  {
    float r;
    float g;
    float b;
    float a;
  };

  float elements[4];
} v4;

static v2 V2Init(float x, float y)
{
  v2 v = {x, y};
  return v;
}
#define v2(x, y) V2Init(x, y)
static v3 V3Init(float x, float y, float z)
{
  v3 v = {x, y, z};
  return v;
}
#define v3(x, y, z) V3Init(x, y, z)
static v4 V4Init(float x, float y, float z, float w)
{
  v4 v = {x, y, z, w};
  return v;
}
#define v4(x, y, z, w) V4Init(x, y, z, w)
#define v4u(x) v4(x, x, x, x)

typedef union iv2
{
  struct{
    int32_t x;
    int32_t y;
  };
  int32_t elements[2];
} iv2;
typedef union iv3
{
  struct
  {
    int32_t x;
    int32_t y;
    int32_t z;
  };

  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
  };

  int32_t elements[3];
} iv3;
typedef union iv4
{
  struct
  {
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
  };

  struct
  {
    int32_t r;
    int32_t g;
    int32_t b;
    int32_t a;
  };

  int32_t elements[4];
} iv4;
static iv2 IV2Init(int32_t x, int32_t y)
{
  iv2 v = {x, y};
  return v;
}
#define iv2(x, y) IV2Init(x, y)
static iv3 IV3Init(int32_t x, int32_t y, int32_t z)
{
  iv3 v = {x, y, z};
  return v;
}
#define iv3(x, y, z) IV3Init(x, y, z)
static iv4 IV4Init(int32_t x, int32_t y, int32_t z, int32_t w)
{
  iv4 v = {x, y, z, w};
  return v;
}
#define iv4(x, y, z, w) IV4Init(x, y, z, w)
#endif

#define utils_VectorObjects
#if defined(utils_VectorObjects) && defined(__cplusplus)
struct kv2;
struct kv3;
struct kv4;
struct kiv2;
struct kiv3;
struct kiv4;
struct kv2
{
  float x;
  float y;
  kv2()
  {
    this->x = 0;
    this->y = 0;
  }
  kv2(float x, float y)
  {
    this->x = x;
    this->y = y;
  }
  kv2(float d)
  {
    this->x = d;
    this->y = 0;
  }

  kv2(const v2&base)
  {
    this->x=base.x;
    this->y=base.y;
  }
  kv2(const v3 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kv2(const v4 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kv2(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kv2(const iv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kv2(const iv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }

  kv2(const kv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kv2(const kiv2 &base);
  kv2(const kv3 &base);
  kv2(const kiv3 &base);
  kv2(const kv4 &base);
  kv2(const kiv4 &base);
  kv2 operator+(const kv2 a) { return kv2(x + a.x, y + a.y); }
  kv2 operator-(const kv2 a) { return kv2(x - a.x, y - a.y); }
  void operator+=(const kv2 a)
  {
    x += a.x;
    y += a.y;
  }
  void operator-=(const kv2 a)
  {
    x -= a.x;
    y -= a.y;
  }
  kv2 operator+(const float a) { return kv2(x + a, y + a); }
  kv2 operator-(const float a) { return kv2(x - a, y - a); }
  kv2 operator*(const float a) { return kv2(x * a, y * a); }
  kv2 operator/(const float a) { return kv2(x / a, y / a); }
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
struct kv3
{
  union
  {
    struct
    {
      float x;
      float y;
      float z;
    };
    struct
    {
      float r;
      float g;
      float b;
    };
    float elements[3];
  };
  kv3()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  kv3(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  kv3(float d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
  }

  kv3(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=0;
  }
  kv3(const v3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=base.z;
  }
  kv3(const v4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=base.z;
  }
  kv3(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=0;
  }
  kv3(const iv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=base.z;
  }
  kv3(const iv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=base.z;
  }

  kv3(const kv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z=0;
  }
  kv3(const kiv2 &base);
  kv3(const kv3 &base)
  {
    this->x=base.x;
    this->y=base.y;
    this->z=base.z;
  }
  kv3(const kiv3 &base);
  kv3(const kv4 &base);
  kv3(const kiv4 &base);
  kv3 operator+(const kv3 a) { return kv3(x + a.x, y + a.y, z + a.z); }
  kv3 operator-(const kv3 a) { return kv3(x - a.x, y - a.y, z - a.z); }
  void operator+=(const kv3 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
  }
  void operator-=(const kv3 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
  }
  kv3 operator+(const float a) { return kv3(x + a, y + a, z + a); }
  kv3 operator-(const float a) { return kv3(x - a, y - a, z - a); }
  kv3 operator*(const float a) { return kv3(x * a, y * a, z * a); }
  kv3 operator/(const float a) { return kv3(x / a, y / a, z / a); }
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
  kv3 &operator=(const kv2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
};
struct kv4
{
  union
  {
    struct
    {
      float x;
      float y;
      union
      {
        struct
        {
          float z;
          float w;
        };
        struct
        {
          float width;
          float height;
        };
      };
    };
    struct
    {
      float r;
      float g;
      float b;
      float a;
    };
    float elements[4];
  };
  kv4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  kv4(float x, float y, float z, float w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  kv4(float d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }

  kv4(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
    this->w=0;
  }
  kv4(const v3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w=0;
  }
  kv4(const v4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w=base.w;
  }
  kv4(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
    this->w=0;
  }
  kv4(const iv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = 0;
  }
  kv4(const iv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = base.w;
  }

  kv4(const kv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
    this->w = 0;
  }
  kv4(const kiv2 &base);
  kv4(const kv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = 0;
  }
  kv4(const kiv3 &base);
  kv4(const kv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = base.w;
  }
  kv4(const kiv4 &base);
  kv4 operator+(const kv4 a) { return kv4(x + a.x, y + a.y, z + a.z, w + a.w); }
  kv4 operator-(const kv4 a) { return kv4(x - a.x, y - a.y, z - a.z, w - a.w); }
  void operator+=(const kv4 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
  }
  void operator-=(const kv4 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
  }
  kv4 operator+(const float a) { return kv4(x + a, y + a, z + a, w + a); }
  kv4 operator-(const float a) { return kv4(x - a, y - a, z - a, w - a); }
  kv4 operator*(const float a) { return kv4(x * a, y * a, z * a, w * a); }
  kv4 operator/(const float a) { return kv4(x / a, y / a, z / a, w / a); }
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
  kv4 &operator=(const kv2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  kv4 &operator=(const kv3 &a)
  {
    this->x = a.x;
    this->y = a.y;
    this->z = a.z;
    return *this;
  }
};
struct kiv2
{
  int32_t x, y;
  kiv2()
  {
    this->x = 0;
    this->y = 0;
  }
  kiv2(int32_t x, int32_t y)
  {
    this->x = x;
    this->y = y;
  }
  kiv2(int32_t d)
  {
    this->x = d;
    this->y = 0;
  }
  
  kiv2(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kiv2(const v3 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kiv2(const v4 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kiv2(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kiv2(const iv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kiv2(const iv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }

  kiv2(const kv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  kiv2(const kiv2 &base){
    this->x=base.x;
    this->y=base.y;
  }
  kiv2(const kv3 &base){
    this->x=base.x;
    this->y=base.y;
  }
  kiv2(const kiv3 &base);
  kiv2(const kv4 &base){
    this->x=base.x;
    this->y=base.y;
  }
  kiv2(const kiv4 &base);
  bool operator==(const kiv2 &a) { return x == a.x && y == a.y; }
  bool operator!=(const kiv2 &a) { return x != a.x || y != a.y; }
  kiv2 operator+(const kiv2 a) { return kiv2(x + a.x, y + a.y); }
  kiv2 operator-(const kiv2 a) { return kiv2(x - a.x, y - a.y); }
  void operator+=(const kiv2 a)
  {
    x += a.x;
    y += a.y;
  }
  void operator-=(const kiv2 a)
  {
    x -= a.x;
    y -= a.y;
  }
  kiv2 operator+(const int32_t a) { return kiv2(x + a, y + a); }
  kiv2 operator-(const int32_t a) { return kiv2(x - a, y - a); }
  kiv2 operator*(const int32_t a) { return kiv2(x * a, y * a); }
  kiv2 operator/(const int32_t a) { return kiv2(x / a, y / a); }
  void operator+=(const int32_t a)
  {
    x += a;
    y += a;
  }
  void operator-=(const int32_t a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const int32_t a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const int32_t a)
  {
    x /= a;
    y /= a;
  }
};
struct kiv3
{
  int32_t x, y, z;
  kiv3()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  kiv3(int32_t x, int32_t y, int32_t z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  kiv3(int32_t d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
  }

  kiv3(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
  }
  kiv3(const v3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
  }
  kiv3(const v4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
  }
  kiv3(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
  }
  kiv3(const iv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
  }
  kiv3(const iv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
  }

  kiv3(const kv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
  }
  kiv3(const kiv2 &base){
    this->x=base.x;
    this->y=base.y;
    this->z=0;
  }
  kiv3(const kv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
  }
  kiv3(const kiv3 &base){
    this->x=base.x;
    this->y=base.y;
    this->z=base.z;
  }
  kiv3(const kv4 &base){
    this->x=base.x;
    this->y=base.y;
    this->z=base.z;
  }
  kiv3(const kiv4 &base);
  bool operator==(const kiv3 &a) { return x == a.x && y == a.y && z == a.z; }
  bool operator!=(const kiv3 &a) { return x != a.x || y != a.y || z != a.z; }
  kiv3 operator+(const kiv3 a) { return kiv3(x + a.x, y + a.y, z + a.z); }
  kiv3 operator-(const kiv3 a) { return kiv3(x - a.x, y - a.y, z - a.z); }
  void operator+=(const kiv3 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
  }
  void operator-=(const kiv3 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
  }
  kiv3 operator+(const int32_t a) { return kiv3(x + a, y + a, z + a); }
  kiv3 operator-(const int32_t a) { return kiv3(x - a, y - a, z - a); }
  kiv3 operator*(const int32_t a) { return kiv3(x * a, y * a, z * a); }
  kiv3 operator/(const int32_t a) { return kiv3(x / a, y / a, z / a); }
  void operator+=(const int32_t a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const int32_t a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const int32_t a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const int32_t a)
  {
    x /= a;
    y /= a;
    z /= a;
  }
  kiv3 &operator=(const kiv2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  kiv2 toIV2() { return kiv2(x, y); }
};
struct kiv4
{
  int32_t x, y, z, w;
  kiv4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  kiv4(int32_t x, int32_t y, int32_t z, int32_t w)
  {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
  }
  kiv4(int32_t d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }

  kiv4(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
    this->w = 0;
  }
  kiv4(const v3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = 0;
  }
  kiv4(const v4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = base.w;
  }
  kiv4(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
    this->w = 0;
  }
  kiv4(const iv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = 0;
  }
  kiv4(const iv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = base.w;
  }

  kiv4(const kv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = 0;
    this->w = 0;
  }
  kiv4(const kiv2 &base){
    this->x=base.x;
    this->y=base.y;
    this->z=0;
    this->w=0;
  }
  kiv4(const kv3 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = 0;
  }
  kiv4(const kiv3 &base){
    this->x=base.x;
    this->y=base.y;
    this->z=base.z;
    this->w=0;
  }
  kiv4(const kv4 &base)
  {
    this->x = base.x;
    this->y = base.y;
    this->z = base.z;
    this->w = base.w;
  }
  kiv4(const kv4 &base)
  {
    this->x = (int32_t)floor(base.x);
    this->y = (int32_t)floor(base.y);
    this->z = (int32_t)floor(base.z);
    this->w = (int32_t)floor(base.w);
  }
  bool operator==(const kiv4 &a)
  {
    return x == a.x && y == a.y && z == a.z && w == a.w;
  }
  bool operator!=(const kiv4 &a)
  {
    return x != a.x || y != a.y || z != a.z || w != a.w;
  }
  kiv4 operator+(const kiv4 a) { return kiv4(x + a.x, y + a.y, z + a.z, w + a.w); }
  kiv4 operator-(const kiv4 a) { return kiv4(x - a.x, y - a.y, z - a.z, w - a.w); }
  void operator+=(const kiv4 a)
  {
    x += a.x;
    y += a.y;
    z += a.z;
    w += a.w;
  }
  void operator-=(const kiv4 a)
  {
    x -= a.x;
    y -= a.y;
    z -= a.z;
    w -= a.w;
  }
  kiv4 operator+(const int32_t a) { return kiv4(x + a, y + a, z + a, w + a); }
  kiv4 operator-(const int32_t a) { return kiv4(x - a, y - a, z - a, w - a); }
  kiv4 operator*(const int32_t a) { return kiv4(x * a, y * a, z * a, w * a); }
  kiv4 operator/(const int32_t a) { return kiv4(x / a, y / a, z / a, w / a); }
  void operator+=(const int32_t a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const int32_t a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const int32_t a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const int32_t a)
  {
    x /= a;
    y /= a;
    z /= a;
    z /= a;
  }
  kiv4 &operator=(const kiv2 &a)
  {
    this->x = a.x;
    this->y = a.y;
    return *this;
  }
  kiv4 &operator=(const kiv3 &a)
  {
    this->x = a.x;
    this->y = a.y;
    this->z = a.z;
    return *this;
  }
  kiv2 toIV2() { return kiv2(x, y); }
  kiv3 toIV3() { return kiv3(x, y, z); }
};

inline kv2::kv2(const kiv2 &base){
  this->x=base.x;
  this->y=base.y;
}
inline kv2::kv2(const kv3 &base){
  this->x=base.x;
  this->y=base.y;
}
inline kv2::kv2(const kiv3 &base){
  this->x=base.x;
  this->y=base.y;
}
inline kv2::kv2(const kv4 &base){
  this->x=base.x;
  this->y=base.y;
}
inline kv2::kv2(const kiv4 &base){
  this->x=base.x;
  this->y=base.y;
}
inline kv2::kv2(const kv4 &base)
{
  this->x = base.x;
  this->y = base.y;
}

inline kv3::kv3(const kiv2 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=0;
}
inline kv3::kv3(const kiv3 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=base.z;
}
inline kv3::kv3(const kv4 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=base.z;
}
inline kv3::kv3(const kiv4 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=base.z;
}

inline kv4::kv4(const kiv2 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=0;
  this->w=0;

}
inline kv4::kv4(const kiv3 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=base.y;
  this->w=0;

}
inline kv4::kv4(const kiv4 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=base.y;
  this->w=base.w;

}

inline kiv2::kiv2(const kiv3 &base){
  this->x=base.x;
  this->y=base.y;
}
inline kiv2::kiv2(const kiv4 &base){
  this->x=base.x;
  this->y=base.y;
}

inline kiv3::kiv3(const kiv4 &base){
  this->x=base.x;
  this->y=base.y;
  this->z=base.z;
}

#define V2ToArray(v) \
  {                  \
    v.x, v.y         \
  }
#define V3ToArray(v) \
  {                  \
    v.x, v.y, v.z    \
  }
#define V4ToArray(v)   \
  {                    \
    v.x, v.y, v.z, v.w \
  }

#endif

#define toV2(v) v2((v).x, (v).y)
#define toV3(v) v3((v).x, (v).y, (v).z)
#define toV4(v) v4((v).x, (v).y, (v).z,(v).w)
#define toIV2(v) iv2((v).x, (v).y)
#define toIV3(v) iv3((v).x, (v).y, (v).z)
#define toIV4(v) iv3((v).x, (v).y, (v).z, (v).w)

static inline float V2Dot(v2 a, v2 b)
{
  return a.x * b.x + a.y * b.y;
}
static inline float V2LengthSquared(v2 v)
{
  return v.x * v.x + v.y * v.y;
}
static inline v2 V2Normalize(v2 v)
{
  float length = sqrtf(V2LengthSquared(v));
  v.x /= length;
  v.y /= length;
  return v;
}
static inline v2 V2AddV2(v2 a, v2 b)
{
  v2 v = v2(a.x + b.x, a.y + b.y);
  return v;
}
static inline v2 V2MinusV2(v2 a, v2 b)
{
  v2 v = v2(a.x - b.x, a.y - b.y);
  return v;
}
static inline v2 V2MultiplyF32(v2 v, float f)
{
  v.x *= f;
  v.y *= f;
  return v;
}
static inline v3 V3AddV3(v3 a, v3 b)
{
  v3 c = v3(a.x + b.x, a.y + b.y, a.z + b.z);
  return c;
}
static inline v3 V3MinusV3(v3 a, v3 b)
{
  v3 c = v3(a.x - b.x, a.y - b.y, a.z - b.z);
  return c;
}
static inline v3 V3MultiplyF32(v3 v, float f)
{
  v.x *= f;
  v.y *= f;
  v.z *= f;
  return v;
}
static inline float V3LengthSquared(v3 a)
{
  return a.x * a.x + a.y * a.y + a.z * a.z;
}
static inline float V3Length(v3 a)
{
  return sqrtf(V3LengthSquared(a));
}
static inline v3 V3Normalize(v3 v)
{
  float length = V3Length(v);
  v3 result = v3(
      v.x / length,
      v.y / length,
      v.z / length);
  return result;
}
static inline float V3Dot(v3 a, v3 b)
{
  float dot =
      a.x * b.x +
      a.y * b.y +
      a.z * b.z;
  return dot;
}
static inline v3 V3Cross(v3 a, v3 b)
{
  v3 result = v3(
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x);
  return result;
}
static inline float minInV3(v3 v)
{
  float minimum = v.x;
  if (v.y < minimum)
  {
    minimum = v.y;
  }
  if (v.z < minimum)
  {
    minimum = v.z;
  }
  return minimum;
}
static inline float maxInV3(v3 v)
{
  float maximum = v.x;
  if (v.y > maximum)
  {
    maximum = v.y;
  }
  if (v.z > maximum)
  {
    maximum = v.z;
  }
  return maximum;
}
static inline bool V4RectHasPoint(v4 v, v2 p)
{
  return (p.x >= v.x && p.x <= v.x + v.width &&
          p.y >= v.y && p.y <= v.y + v.height);
}

typedef struct m4
{
  float elements[4][4];
} m4;
static m4 M4InitFill(float val)
{
  m4 m = {
      {
          {val, val, val, val},
          {val, val, val, val},
          {val, val, val, val},
          {val, val, val, val},
      }};
  return m;
}
static m4 M4InitDiagonal(float diagonal)
{
  m4 m = {
      {
          {diagonal, 0.f, 0.f, 0.f},
          {0.f, diagonal, 0.f, 0.f},
          {0.f, 0.f, diagonal, 0.f},
          {0.f, 0.f, 0.f, diagonal},
      }};
  return m;
}
static inline m4 M4MultiplyM4(m4 a, m4 b)
{
  m4 c = {0};

  for (int32_t j = 0; j < 4; ++j)
  {
    for (int32_t i = 0; i < 4; ++i)
    {
      c.elements[i][j] = (a.elements[0][j] * b.elements[i][0] +
                          a.elements[1][j] * b.elements[i][1] +
                          a.elements[2][j] * b.elements[i][2] +
                          a.elements[3][j] * b.elements[i][3]);
    }
  }

  return c;
}
static inline m4 M4MultiplyF32(m4 a, float b)
{
  for (int32_t j = 0; j < 4; ++j)
  {
    for (int32_t i = 0; i < 4; ++i)
    {
      a.elements[i][j] *= b;
    }
  }

  return a;
}
static inline float V4Dot(v4 a, v4 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
static inline v4 V4AddV4(v4 a, v4 b)
{
  v4 c = v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
  return c;
}
static inline v4 V4MinusV4(v4 a, v4 b)
{
  v4 c = v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
  return c;
}
static inline v4 V4MultiplyF32(v4 a, float f)
{
  v4 c = v4(a.x * f, a.y * f, a.z * f, a.w * f);
  return c;
}
static inline v4 V4MultiplyV4(v4 a, v4 b)
{
  v4 c = v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
  return c;
}
static inline v4 V4MultiplyM4(v4 v, m4 m)
{
  v4 result = v4(0, 0, 0, 0);
  for (int32_t i = 0; i < 4; ++i)
  {
    result.elements[i] = (v.elements[0] * m.elements[0][i] +
                          v.elements[1] * m.elements[1][i] +
                          v.elements[2] * m.elements[2][i] +
                          v.elements[3] * m.elements[3][i]);
  }
  return result;
}
static inline m4 M4TranslateV3(v3 translation)
{
  m4 result = M4InitDiagonal(1.f);
  result.elements[3][0] = translation.x;
  result.elements[3][1] = translation.y;
  result.elements[3][2] = translation.z;
  return result;
}
static inline m4 M4ScaleV3(v3 scale)
{
  m4 result = M4InitDiagonal(1.f);
  result.elements[0][0] = scale.x;
  result.elements[1][1] = scale.y;
  result.elements[2][2] = scale.z;
  return result;
}
static inline m4 M4Perspective(float fov, float aspect_ratio, float near_z, float far_z)
{
  m4 result = {0};
  float tan_theta_over_2 = tanf(fov * (PIf / 360.f));
  result.elements[0][0] = 1.f / tan_theta_over_2;
  result.elements[1][1] = aspect_ratio / tan_theta_over_2;
  result.elements[2][3] = -1.f;
  result.elements[2][2] = (near_z + far_z) / (near_z - far_z);
  result.elements[3][2] = (2.f * near_z * far_z) / (near_z - far_z);
  result.elements[3][3] = 0.f;
  return result;
}
static inline m4 M4Orthographic(float left, float right, float bottom, float top, float near_depth, float far_depth)
{
  m4 result = {0};

  result.elements[0][0] = 2.f / (right - left);
  result.elements[1][1] = 2.f / (top - bottom);
  result.elements[2][2] = -2.f / (far_depth - near_depth);
  result.elements[3][3] = 1.f;
  result.elements[3][0] = (left + right) / (left - right);
  result.elements[3][1] = (bottom + top) / (bottom - top);
  result.elements[3][2] = (far_depth + near_depth) / (near_depth - far_depth);

  return result;
}
static inline m4 M4LookAt(v3 eye, v3 center, v3 up)
{
  m4 result;

  v3 f = V3Normalize(V3MinusV3(center, eye));
  v3 s = V3Normalize(V3Cross(f, up));
  v3 u = V3Cross(s, f);

  result.elements[0][0] = s.x;
  result.elements[0][1] = u.x;
  result.elements[0][2] = -f.x;
  result.elements[0][3] = 0.f;

  result.elements[1][0] = s.y;
  result.elements[1][1] = u.y;
  result.elements[1][2] = -f.y;
  result.elements[1][3] = 0.f;

  result.elements[2][0] = s.z;
  result.elements[2][1] = u.z;
  result.elements[2][2] = -f.z;
  result.elements[2][3] = 0.f;

  result.elements[3][0] = -V3Dot(s, eye);
  result.elements[3][1] = -V3Dot(u, eye);
  result.elements[3][2] = V3Dot(f, eye);
  result.elements[3][3] = 1.f;

  return result;
}
static inline m4 M4Inverse(m4 m)
{
  float coef00 = m.elements[2][2] * m.elements[3][3] - m.elements[3][2] * m.elements[2][3];
  float coef02 = m.elements[1][2] * m.elements[3][3] - m.elements[3][2] * m.elements[1][3];
  float coef03 = m.elements[1][2] * m.elements[2][3] - m.elements[2][2] * m.elements[1][3];
  float coef04 = m.elements[2][1] * m.elements[3][3] - m.elements[3][1] * m.elements[2][3];
  float coef06 = m.elements[1][1] * m.elements[3][3] - m.elements[3][1] * m.elements[1][3];
  float coef07 = m.elements[1][1] * m.elements[2][3] - m.elements[2][1] * m.elements[1][3];
  float coef08 = m.elements[2][1] * m.elements[3][2] - m.elements[3][1] * m.elements[2][2];
  float coef10 = m.elements[1][1] * m.elements[3][2] - m.elements[3][1] * m.elements[1][2];
  float coef11 = m.elements[1][1] * m.elements[2][2] - m.elements[2][1] * m.elements[1][2];
  float coef12 = m.elements[2][0] * m.elements[3][3] - m.elements[3][0] * m.elements[2][3];
  float coef14 = m.elements[1][0] * m.elements[3][3] - m.elements[3][0] * m.elements[1][3];
  float coef15 = m.elements[1][0] * m.elements[2][3] - m.elements[2][0] * m.elements[1][3];
  float coef16 = m.elements[2][0] * m.elements[3][2] - m.elements[3][0] * m.elements[2][2];
  float coef18 = m.elements[1][0] * m.elements[3][2] - m.elements[3][0] * m.elements[1][2];
  float coef19 = m.elements[1][0] * m.elements[2][2] - m.elements[2][0] * m.elements[1][2];
  float coef20 = m.elements[2][0] * m.elements[3][1] - m.elements[3][0] * m.elements[2][1];
  float coef22 = m.elements[1][0] * m.elements[3][1] - m.elements[3][0] * m.elements[1][1];
  float coef23 = m.elements[1][0] * m.elements[2][1] - m.elements[2][0] * m.elements[1][1];

  v4 fac0 = v4(coef00, coef00, coef02, coef03);
  v4 fac1 = v4(coef04, coef04, coef06, coef07);
  v4 fac2 = v4(coef08, coef08, coef10, coef11);
  v4 fac3 = v4(coef12, coef12, coef14, coef15);
  v4 fac4 = v4(coef16, coef16, coef18, coef19);
  v4 fac5 = v4(coef20, coef20, coef22, coef23);

  v4 vec0 = v4(m.elements[1][0], m.elements[0][0], m.elements[0][0], m.elements[0][0]);
  v4 vec1 = v4(m.elements[1][1], m.elements[0][1], m.elements[0][1], m.elements[0][1]);
  v4 vec2 = v4(m.elements[1][2], m.elements[0][2], m.elements[0][2], m.elements[0][2]);
  v4 vec3 = v4(m.elements[1][3], m.elements[0][3], m.elements[0][3], m.elements[0][3]);

  v4 inv0 = V4AddV4(V4MinusV4(V4MultiplyV4(vec1, fac0), V4MultiplyV4(vec2, fac1)), V4MultiplyV4(vec3, fac2));
  v4 inv1 = V4AddV4(V4MinusV4(V4MultiplyV4(vec0, fac0), V4MultiplyV4(vec2, fac3)), V4MultiplyV4(vec3, fac4));
  v4 inv2 = V4AddV4(V4MinusV4(V4MultiplyV4(vec0, fac1), V4MultiplyV4(vec1, fac3)), V4MultiplyV4(vec3, fac5));
  v4 inv3 = V4AddV4(V4MinusV4(V4MultiplyV4(vec0, fac2), V4MultiplyV4(vec1, fac4)), V4MultiplyV4(vec2, fac5));

  v4 sign_a = v4(+1, -1, +1, -1);
  v4 sign_b = v4(-1, +1, -1, +1);

  m4 inverse;
  for (uint32_t i = 0; i < 4; ++i)
  {
    inverse.elements[0][i] = inv0.elements[i] * sign_a.elements[i];
    inverse.elements[1][i] = inv1.elements[i] * sign_b.elements[i];
    inverse.elements[2][i] = inv2.elements[i] * sign_a.elements[i];
    inverse.elements[3][i] = inv3.elements[i] * sign_b.elements[i];
  }

  v4 row0 = v4(inverse.elements[0][0], inverse.elements[1][0], inverse.elements[2][0], inverse.elements[3][0]);
  v4 m0 = v4(m.elements[0][0], m.elements[0][1], m.elements[0][2], m.elements[0][3]);
  v4 dot0 = V4MultiplyV4(m0, row0);
  float dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

  float one_over_det = 1 / dot1;

  return M4MultiplyF32(inverse, one_over_det);
}
static inline m4 M4RemoveRotation(m4 mat)
{
  v3 scale = v3(
      V3Length(v3(mat.elements[0][0], mat.elements[0][1], mat.elements[0][2])),
      V3Length(v3(mat.elements[1][0], mat.elements[1][1], mat.elements[1][2])),
      V3Length(v3(mat.elements[2][0], mat.elements[2][1], mat.elements[2][2])));

  mat.elements[0][0] = scale.x;
  mat.elements[1][0] = 0.f;
  mat.elements[2][0] = 0.f;

  mat.elements[0][1] = 0.f;
  mat.elements[1][1] = scale.y;
  mat.elements[2][1] = 0.f;

  mat.elements[0][2] = 0.f;
  mat.elements[1][2] = 0.f;
  mat.elements[2][2] = scale.z;

  return mat;
}
static inline m4 M4Rotate(float angle, v3 axis)
{
  m4 result = M4InitDiagonal(1.f);

  axis = V3Normalize(axis);

  float sin_theta = sinf(angle);
  float cos_theta = cosf(angle);
  float cos_value = 1.f - cos_theta;

  result.elements[0][0] = (axis.x * axis.x * cos_value) + cos_theta;
  result.elements[0][1] = (axis.x * axis.y * cos_value) + (axis.z * sin_theta);
  result.elements[0][2] = (axis.x * axis.z * cos_value) - (axis.y * sin_theta);

  result.elements[1][0] = (axis.y * axis.x * cos_value) - (axis.z * sin_theta);
  result.elements[1][1] = (axis.y * axis.y * cos_value) + cos_theta;
  result.elements[1][2] = (axis.y * axis.z * cos_value) + (axis.x * sin_theta);

  result.elements[2][0] = (axis.z * axis.x * cos_value) + (axis.y * sin_theta);
  result.elements[2][1] = (axis.z * axis.y * cos_value) - (axis.x * sin_theta);
  result.elements[2][2] = (axis.z * axis.z * cos_value) + cos_theta;

  return result;
}
static inline v3 calculateTriangleNormal(v3 p1, v3 p2, v3 p3)
{
  v3 p1_to_p2 = v3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
  v3 p1_to_p3 = v3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
  v3 normal = V3Cross(p1_to_p3, p1_to_p2);
  normal.y *= -1;
  return normal;
}
static inline v3 calculateTriangleNormalNormalized(v3 p1, v3 p2, v3 p3)
{
  v3 normal = calculateTriangleNormal(p1, p2, p3);
  normal = V3Normalize(normal);
  return normal;
}
static inline m4 M4Mapper(v3 min, v3 max, v3 targetMin, v3 targetMax)
{
  // float scale = (n - min.x) / (max.x - min.x) * (targetMax.x - targetMin.x) + targetMin.x;

  m4 transform = M4InitDiagonal(1.f);
  transform = M4MultiplyM4(transform, M4TranslateV3(V3MultiplyF32(targetMin, 1.f)));
  transform = M4MultiplyM4(transform, M4ScaleV3(v3(targetMax.x - targetMin.x, targetMax.y - targetMin.y, targetMax.z - targetMin.z)));
  transform = M4MultiplyM4(transform, M4ScaleV3(v3(1.f / (max.x - min.x), 1.f / (max.y - min.y), 1.f / (max.z - min.z))));
  transform = M4MultiplyM4(transform, M4TranslateV3(V3MultiplyF32(min, -1.f)));

  return transform;
}


#define fCompare(a,b) (fabsf(a - b) < 0.0001)
#define fCompareN(a,b,n) (fabsf(a - b) < n)

static inline bool CharIsSpace(char c)
{
  return c <= 32;
}
static inline bool CharIsAlpha(char c)
{
  return ((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z'));
}
static inline bool CharIsDigit(char c)
{
  return (c >= '0' && c <= '9');
}
static inline int32_t CharToLower(int32_t c)
{
  if (c >= 'A' && c <= 'Z')
  {
    c += 32;
  }
  return c;
}
static inline int32_t CharToUpper(int32_t c)
{
  if (c >= 'a' && c <= 'z')
  {
    c -= 32;
  }
  return c;
}
static inline bool CStringMatchCaseInsensitive(const char *str1, const char *str2)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (uint32_t i = 0;; ++i)
    {
      if (CharToLower(str1[i]) != CharToLower(str2[i]))
      {
        result = 0;
        break;
      }
      if (str1[i] == 0 && str2[i] == 0)
      {
        break;
      }
    }
  }
  else if (str1 || str2)
  {
    result = 0;
  }

  return result;
}
static inline bool CStringMatchCaseSensitive(const char *str1, const char *str2)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (uint32_t i = 0;; ++i)
    {
      if (str1[i] != str2[i])
      {
        result = 0;
        break;
      }
      if (str1[i] == 0 && str2[i] == 0)
      {
        break;
      }
    }
  }
  else if (str1 || str2)
  {
    result = 0;
  }

  return result;
}
#ifdef utils_StrManipulation
#define stringCopy strcpy
#define stringCopyN strncpy
#define calculateCStringLength(s) ((uint32_t)strlen(s))
#define CStringToint32_t(s) ((int32_t)atoi(s))
#define CStringToI16(s) ((int16_t)atoi(s))
#define CStringToF32(s) ((float)atof(s))
static inline uint32_t HashCString(char *string)
{
  uint32_t hash = 5381;
  int32_t c;
  while ((c = *string++))
  {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}
static inline bool CStringMatchCaseInsensitiveN(const char *str1, const char *str2, uint32_t n)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (uint32_t i = 0; i < n; ++i)
    {
      if (CharToLower(str1[i]) != CharToLower(str2[i]))
      {
        result = 0;
        break;
      }
      if (str1[i] == 0 && str2[i] == 0)
      {
        break;
      }
    }
  }
  else if (str1 || str2)
  {
    result = 0;
  }

  return result;
}
static inline bool CStringMatchCaseSensitiveN(const char *str1, const char *str2, uint32_t n)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (uint32_t i = 0; i < n; ++i)
    {
      if (str1[i] != str2[i])
      {
        result = 0;
        break;
      }
      if (str1[i] == 0 && str2[i] == 0)
      {
        break;
      }
    }
  }
  else if (str1 || str2)
  {
    result = 0;
  }

  return result;
}
static inline bool CStringContains(const char *str, const char *substr)
{
  bool result = 0;

  if (str && substr)
  {
    for (uint32_t i = 0; str[i]; ++i)
    {
      if (str[i] == substr[0])
      {
        result = 1;
        for (uint32_t j = i + 1; str[j] && substr[j - i]; ++j)
        {
          if (str[j] != substr[j - i] && substr[j - i] != 0)
          {
            result = 0;
            break;
          }
        }

        if (result)
        {
          break;
        }
      }
    }
  }

  return result;
}
static inline char *CStringPointerAfterSubstring(char *str, char *substr)
{
  char *result = 0;

  if (str && substr)
  {
    for (uint32_t i = 0; str[i]; ++i)
    {
      result = str + i;
      if (str[i] == substr[0])
      {
        for (uint32_t j = 0;; ++j)
        {
          if (str[i + j] == substr[j])
          {
            ++result;
            if (str[i + j] == 0 && substr[j] == 0)
            {
              break;
            }
          }
          else if (substr[j] == 0)
          {
            break;
          }
          else if (str[i + j] == 0)
          {
            result = 0;
            break;
          }
          else
          {
            result = 0;
            break;
          }
        }

        if (result)
        {
          break;
        }
      }
    }
  }

  return result;
}
static inline uint32_t CStringIndexAfterSubstring(char *str, char *substr)
{
  uint32_t result = 0;
  for (uint32_t i = 0; str[i]; ++i)
  {
    if (str[i] == substr[0])
    {
      if (CStringMatchCaseInsensitiveN(str + i, substr, calculateCStringLength(substr)))
      {
        result = i + calculateCStringLength(substr);
      }
    }
  }
  return result;
}
static inline uint32_t CStringFirstIndexAfterSubstring(char *str, char *substr)
{
  uint32_t result = 0;
  for (uint32_t i = 0; str[i]; ++i)
  {
    if (str[i] == substr[0])
    {
      if (CStringMatchCaseInsensitiveN(str + i, substr, calculateCStringLength(substr)))
      {
        result = i + calculateCStringLength(substr);
        break;
      }
    }
  }
  return result;
}
static inline char *CStringFindSubstring(char *str, char *substr)
{
  char *result = 0;

  if (str && substr)
  {
    for (uint32_t i = 0; str[i]; ++i)
    {
      if (str[i] == substr[0])
      {
        char *possible_result = str + i;
        bool match = 1;
        for (uint32_t j = i + 1; str[j] && substr[j - i]; ++j)
        {
          if (str[j] != substr[j - i] && substr[j - i] != 0)
          {
            match = 0;
            break;
          }
        }

        if (match)
        {
          result = possible_result;
          break;
        }
      }
    }
  }

  return result;
}
static inline void CopyCStringToFixedSizeBuffer(char *destination, uint32_t destination_max, char *source)
{
  for (uint32_t i = 0; i < destination_max; ++i)
  {
    destination[i] = source[i];
    if (source[i] == 0)
    {
      break;
    }
  }
  destination[destination_max - 1] = 0;
}
static inline void CopySubstringToStringUntilCharN(char *str1, uint32_t str1_max, char *str2, char str2_term)
{
  uint32_t write_pos = 0;
  while (1)
  {
    if (str2[write_pos] == str2_term || write_pos == str1_max - 1)
    {
      str1[write_pos++] = 0;
      break;
    }
    else
    {
      str1[write_pos] = str2[write_pos];
      ++write_pos;
    }
  }
}
static inline int32_t GetFirstint32_tFromCString(char *str)
{
  int32_t result = 0;

  int32_t last_digit = -1;

  for (int32_t i = 0; str[i]; ++i)
  {
    if (CharIsDigit(str[i]))
    {
      last_digit = i;
    }
    else if (last_digit != -1)
    {
      break;
    }
  }

  if (last_digit >= 0)
  {
    int32_t digit_multiplier = 1;

    for (int32_t i = last_digit; i >= 0; --i)
    {
      if (CharIsDigit(str[i]))
      {
        int32_t digit = str[i] - '0';
        result += digit_multiplier * digit;
        digit_multiplier *= 10;
      }
      else
      {
        if (str[i] == '-')
        {
          result *= -1;
        }
        break;
      }
    }
  }

  return result;
}
static inline float GetFirstF32FromCString(char *str)
{
  float result = 0;
  bool found_first_digit = 0;
  uint32_t float_write_pos = 0;
  char float_str[64] = {0};
  uint32_t read_pos = 0;
  for (;; ++read_pos)
  {
    if (str[read_pos] == 0)
    {
      break;
    }
    if (found_first_digit)
    {
      if (float_write_pos == sizeof(float_str))
      {
        float_str[sizeof(float_str) - 1] = 0;
        break;
      }
      if (CharIsDigit(str[read_pos]) || str[read_pos] == '.' || str[read_pos] == '-')
      {
        float_str[float_write_pos++] = str[read_pos];
      }
      else
      {
        float_str[float_write_pos++] = 0;
        break;
      }
    }
    else
    {
      if (CharIsDigit(str[read_pos]) || str[read_pos] == '.' || str[read_pos] == '-')
      {
        float_str[float_write_pos++] = str[read_pos];
        found_first_digit = 1;
      }
    }
  }
  result = CStringToF32(float_str);
  return result;
}
#endif

typedef struct rgbcolor
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} rgbcolor;
static rgbcolor RGBColorInit(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
  rgbcolor c = {r, g, b, a};
  return c;
}
#define rgbcolor(r, g, b, a) RGBColorInit(r, g, b, a)
static inline v3 RGBToHSV(v3 rgb)
{
  float c_max = maxInV3(rgb);
  float c_min = minInV3(rgb);
  float delta = c_max - c_min;
  bool c_max_is_r = rgb.r > rgb.g && rgb.r > rgb.b;
  bool c_max_is_g = rgb.g > rgb.r && rgb.g > rgb.b;
  bool c_max_is_b = rgb.b > rgb.r && rgb.b > rgb.g;
  float h = (c_max_is_r ? (rgb.g - rgb.b) / delta + 0 : c_max_is_g ? (rgb.b - rgb.r) / delta + 2
                                                  : c_max_is_b   ? (rgb.r - rgb.g) / delta + 4
                                                                 : 0);
  float s = c_max == 0 ? 0 : (delta / c_max);
  float v = c_max;
  v3 hsv = v3(h / 6.f, s, v);
  return hsv;
}
static inline v3 RGBColorToHSV(rgbcolor rgba)
{
  v3 newColor = v3(rgba.r / 255.f, rgba.g / 255.f, rgba.b / 255.f);
  return RGBToHSV(newColor);
}
static inline v3 HSVToRGB(v3 hsv)
{
  float h = fmodf(hsv.x * 360.f, 360.f);
  float s = hsv.y;
  float v = hsv.z;

  float c = v * s;
  float x = c * (1 - fabsf(fmodf((h / 60.f), 2) - 1));
  float m = v - c;

  float r;
  float g;
  float b;

  if ((h >= 0.f && h < 60.f) || (h >= 360.f && h < 420.f))
  {
    r = c;
    g = x;
    b = 0;
  }
  else if (h >= 60.f && h < 120.f)
  {
    r = x;
    g = c;
    b = 0;
  }
  else if (h >= 120.f && h < 180.f)
  {
    r = 0;
    g = c;
    b = x;
  }
  else if (h >= 180.f && h < 240.f)
  {
    r = 0;
    g = x;
    b = c;
  }
  else if (h >= 240.f && h < 300.f)
  {
    r = x;
    g = 0;
    b = c;
  }
  else if ((h >= 300.f && h <= 360.f) ||
           (h >= -60.f && h <= 0.f))
  {
    r = c;
    g = 0;
    b = x;
  }

  v3 rgb = v3(r + m, g + m, b + m);
  return rgb;
}
static inline rgbcolor HSVToRGBColor(v3 hsv)
{
  v3 newColor = HSVToRGB(hsv);
  return rgbcolor((int32_t)floorf(newColor.r * 255) % 255, (int32_t)floorf(newColor.g * 255) % 255, (int32_t)floorf(newColor.b * 255) % 255, 1);
}

typedef enum Key
{
  KEY_SPACE,
  KEY_APOSTROPHE,
  KEY_COMMA,
  KEY_MINUS,
  KEY_PERIOD,
  KEY_SLASH,
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
  KEY_SEMICOLON,
  KEY_EQUAL,
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
  KEY_LEFT_BRACKET,
  KEY_BACKSLASH,
  KEY_RIGHT_BRACKET,
  KEY_GRAVE_ACCENT,
  KEY_ENTER,
  KEY_TAB,
  KEY_BACKSPACE,
  KEY_ESCAPE,
  KEY_DELETE,
  ASCII_KEY_LAST,
// end of ascii characters
  KEY_INSERT=ASCII_KEY_LAST,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_DOWN,
  KEY_UP,
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_F13,
  KEY_F14,
  KEY_F15,
  KEY_F16,
  KEY_F17,
  KEY_F18,
  KEY_F19,
  KEY_F20,
  KEY_F21,
  KEY_F22,
  KEY_F23,
  KEY_F24,
  KEY_F25,
  KEY_LEFT_SHIFT,
  KEY_LEFT_CONTROL,
  KEY_LEFT_ALT,
  KEY_LEFT_SUPER,
  KEY_RIGHT_SHIFT,
  KEY_RIGHT_CONTROL,
  KEY_RIGHT_ALT,
  UNREGISTERED_KEY,
  KEY_LAST,
  // mouse buttons
  MOUSE_BUTTON1=KEY_LAST,
  MOUSE_LEFT = MOUSE_BUTTON1,
  MOUSE_BUTTON2,
  MOUSE_RIGHT = MOUSE_BUTTON2,
  MOUSE_BUTTON3,
  MOUSE_MIDDLE = MOUSE_BUTTON3,
  MOUSE_BUTTON4,
  MOUSE_BUTTON5,
  MOUSE_BUTTON6,
  MOUSE_BUTTON7,
  MOUSE_BUTTON8,
  MOUSE_LAST
} Key;
static inline char KeyToChar(Key k){
  switch (k)
  {
  case KEY_SPACE:
    return ' ';
  case KEY_APOSTROPHE:
    return '\'' /* ' */;
  case KEY_COMMA:
    return ',' /* , */;
  case KEY_MINUS:
    return '-' /* - */;
  case KEY_PERIOD:
    return '.' /* . */;
  case KEY_SLASH:
    return '/' /* / */;
  case KEY_0:
    return '0';
  case KEY_1:
    return '1';
  case KEY_2:
    return '2';
  case KEY_3:
    return '3';
  case KEY_4:
    return '4';
  case KEY_5:
    return '5';
  case KEY_6:
    return '6';
  case KEY_7:
    return '7';
  case KEY_8:
    return '8';
  case KEY_9:
    return '9';
  case KEY_SEMICOLON:
    return ';' /* ; */;
  case KEY_EQUAL:
    return '=' /* = */;
  case KEY_A:
    return 'a';
  case KEY_B:
    return 'b';
  case KEY_C:
    return 'c';
  case KEY_D:
    return 'd';
  case KEY_E:
    return 'e';
  case KEY_F:
    return 'f';
  case KEY_G:
    return 'g';
  case KEY_H:
    return 'h';
  case KEY_I:
    return 'i';
  case KEY_J:
    return 'j';
  case KEY_K:
    return 'k';
  case KEY_L:
    return 'l';
  case KEY_M:
    return 'm';
  case KEY_N:
    return 'n';
  case KEY_O:
    return 'o';
  case KEY_P:
    return 'p';
  case KEY_Q:
    return 'q';
  case KEY_R:
    return 'r';
  case KEY_S:
    return 's';
  case KEY_T:
    return 't';
  case KEY_U:
    return 'u';
  case KEY_V:
    return 'v';
  case KEY_W:
    return 'w';
  case KEY_X:
    return 'x';
  case KEY_Y:
    return 'y';
  case KEY_Z:
    return 'z';
  case KEY_LEFT_BRACKET:
    return '[' /* [ */;
  case KEY_BACKSLASH:
    return '\\' /* \ */;
  case KEY_RIGHT_BRACKET:
    return ']' /* ] */;
  case KEY_GRAVE_ACCENT:
    return '`' /* ` */;
  case KEY_ESCAPE:
    return 27;
  case KEY_BACKSPACE:
    return 8;
  case KEY_DELETE:
    return 127;
  case KEY_TAB:
    return '\t';
  case KEY_ENTER:
    return '\n';
  default:
    return 0;
  }
}
static inline Key CharToKey(char c){
  switch (c)
  {
    case ' ':
      return KEY_SPACE;
    case '\'':
      return KEY_APOSTROPHE /* ' */;
    case ',':
      return KEY_COMMA /* , */;
    case '-':
      return KEY_MINUS /* - */;
    case '.':
      return KEY_PERIOD /* . */;
    case '/':
      return KEY_SLASH /* / */;
    case '0':
      return KEY_0;
    case '1':
      return KEY_1;
    case '2':
      return KEY_2;
    case '3':
      return KEY_3;
    case '4':
      return KEY_4;
    case '5':
      return KEY_5;
    case '6':
      return KEY_6;
    case '7':
      return KEY_7;
    case '8':
      return KEY_8;
    case '9':
      return KEY_9;
    case ';':
      return KEY_SEMICOLON /* ; */;
    case '=':
      return KEY_EQUAL /* = */;
    case 'a':
      return KEY_A;
    case 'b':
      return KEY_B;
    case 'c':
      return KEY_C;
    case 'd':
      return KEY_D;
    case 'e':
      return KEY_E;
    case 'f':
      return KEY_F;
    case 'g':
      return KEY_G;
    case 'h':
      return KEY_H;
    case 'i':
      return KEY_I;
    case 'J':
      return KEY_J;
    case 'k':
      return KEY_K;
    case 'l':
      return KEY_L;
    case 'm':
      return KEY_M;
    case 'n':
      return KEY_N;
    case 'o':
      return KEY_O;
    case 'p':
      return KEY_P;
    case 'q':
      return KEY_Q;
    case 'r':
      return KEY_R;
    case 's':
      return KEY_S;
    case 't':
      return KEY_T;
    case 'u':
      return KEY_U;
    case 'v':
      return KEY_V;
    case 'w':
      return KEY_W;
    case 'x':
      return KEY_X;
    case 'y':
      return KEY_Y;
    case 'z':
      return KEY_Z;
    case '[':
      return KEY_LEFT_BRACKET /* [ */;
    case '\\':
      return KEY_BACKSLASH /* \ */;
    case ']':
      return KEY_RIGHT_BRACKET /* ] */;
    case '`':
      return KEY_GRAVE_ACCENT /* ` */;
    case 27:
      return KEY_ESCAPE;
    case 8:
      return KEY_BACKSPACE;
    case 127:
      return KEY_DELETE;
    case '\t':
      return KEY_TAB;
    case '\n':
      return KEY_ENTER;
    default:
      return UNREGISTERED_KEY;
  }
}
static inline u64 KeyBitmap(Key k){
  if(k<ASCII_KEY_LAST){
    return 1ULL<<k;
  }else{
    return 1ULL<<(k-ASCII_KEY_LAST);
  }
}

typedef enum cardinal8dir
{
  NORTH,
  SOUTH,
  EAST,
  WEST,
  NORTH_EAST,
  NORTH_WEST,
  SOUTH_EAST,
  SOUTH_WEST,
  CENTER
} cardinal8dir;

#ifndef utils_NoRand
static inline void seedRandomNumberGenerator(void)
{
  srand((uint32_t)time(0));
}
static inline float randf()
{
  return ((float)rand() / (RAND_MAX));
}
#endif

#ifdef utils_PerlinNoise
static int32_t global_perlin_noise_seed = 0;
static int32_t global_perlin_noise_hash[] = {
    208, 34, 231, 213, 32, 248, 233, 56, 161, 78, 24, 140, 71, 48, 140, 254, 245, 255, 247, 247, 40,
    185, 248, 251, 245, 28, 124, 204, 204, 76, 36, 1, 107, 28, 234, 163, 202, 224, 245, 128, 167, 204,
    9, 92, 217, 54, 239, 174, 173, 102, 193, 189, 190, 121, 100, 108, 167, 44, 43, 77, 180, 204, 8, 81,
    70, 223, 11, 38, 24, 254, 210, 210, 177, 32, 81, 195, 243, 125, 8, 169, 112, 32, 97, 53, 195, 13,
    203, 9, 47, 104, 125, 117, 114, 124, 165, 203, 181, 235, 193, 206, 70, 180, 174, 0, 167, 181, 41,
    164, 30, 116, 127, 198, 245, 146, 87, 224, 149, 206, 57, 4, 192, 210, 65, 210, 129, 240, 178, 105,
    228, 108, 245, 148, 140, 40, 35, 195, 38, 58, 65, 207, 215, 253, 65, 85, 208, 76, 62, 3, 237, 55, 89,
    232, 50, 217, 64, 244, 157, 199, 121, 252, 90, 17, 212, 203, 149, 152, 140, 187, 234, 177, 73, 174,
    193, 100, 192, 143, 97, 53, 145, 135, 19, 103, 13, 90, 135, 151, 199, 91, 239, 247, 33, 39, 145,
    101, 120, 99, 3, 186, 86, 99, 41, 237, 203, 111, 79, 220, 135, 158, 42, 30, 154, 120, 67, 87, 167,
    135, 176, 183, 191, 253, 115, 184, 21, 233, 58, 129, 233, 142, 39, 128, 211, 118, 137, 139, 255,
    114, 20, 218, 113, 154, 27, 127, 246, 250, 1, 8, 198, 250, 209, 92, 222, 173, 21, 88, 102, 219};

static inline int32_t PerlinNoise2(int32_t x, int32_t y)
{
  int32_t tmp = global_perlin_noise_hash[(y + global_perlin_noise_seed) % 256];
  return global_perlin_noise_hash[(tmp + x) % 256];
}
static inline float PerlinLinearlyInterpolate(float x, float y, float s)
{
  return x + s * (y - x);
}
static inline float PerlinSmoothlyInterpolate(float x, float y, float s)
{
  return PerlinLinearlyInterpolate(x, y, s * s * (3 - 2 * s));
}
static inline float PerlinNoise2D(float x, float y)
{
  int32_t x_int = (int32_t)x;
  int32_t y_int = (int32_t)y;
  float x_frac = x - x_int;
  float y_frac = y - y_int;
  int32_t s = PerlinNoise2(x_int, y_int);
  int32_t t = PerlinNoise2(x_int + 1, y_int);
  int32_t u = PerlinNoise2(x_int, y_int + 1);
  int32_t v = PerlinNoise2(x_int + 1, y_int + 1);
  float low = PerlinSmoothlyInterpolate((float)s, (float)t, x_frac);
  float high = PerlinSmoothlyInterpolate((float)u, (float)v, x_frac);
  return PerlinSmoothlyInterpolate(low, high, y_frac);
}
static inline float Perlin2D(float x, float y, float freq, int32_t depth)
{
  float xa = x * freq;
  float ya = y * freq;
  float amp = 1.0;
  float fin = 0;
  float div = 0.0;

  for (int32_t i = 0; i < depth; i++)
  {
    div += 256 * amp;
    fin += PerlinNoise2D(xa, ya) * amp;
    amp /= 2;
    xa *= 2;
    ya *= 2;
  }

  return fin / div;
}
#endif

static inline float interpolateLinear(float t)
{
  float result = 0;
  if (t < 0)
  {
    result = 0;
  }
  else if (t > 1)
  {
    result = 1;
  }
  else
  {
    result = t;
  }
  return result;
}
static inline float interpolateSmooth(float t)
{
  float result = 0;
  if (t < 0)
  {
    result = 0;
  }
  else if (t > 1)
  {
    result = 1;
  }
  else
  {
    float t2 = t * t;
    float t4 = t2 * t2;
    float t6 = t2 * t4;
    result = (4 / 9) * t6 - (17 / 9) * t4 + (22 / 9) * t2;
  }
  return result;
}
static inline float regularizeDegree(float angle)
{
  float degree = fmodf(angle, 360);
  if (degree < 0)
    degree += 360;
  return degree;
}
static inline float regularizeRad(float angle)
{
  float rad = fmodf(angle, 2 * PIf);
  if (rad < 0)
    rad += 2 * PIf;
  return rad;
}
static inline float degreeToRad(float degree) { return degree / 180.f * PIf; }
static inline float radToDegree(float rad) { return rad / PIf * 180; }
static inline float fast_cos(float angle)
{
  float t_angle = regularizeRad(angle) - PIf;
  float res = 0;
  float x = 1;
  int32_t precision = utils_FastCalcPrecision;
  int32_t factorialPrecision = 20;
  unsigned long long factorials[] = {0,
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
  for (int32_t i = 0; i <= precision && i < factorialPrecision; i += 4)
  {
    res += x / factorials[i];
    x *= t_angle * t_angle;
    res -= x / factorials[i + 2];
    x *= t_angle * t_angle;
  }
  return -res;
}
static inline float fast_sin(float angle)
{
  float t_angle = regularizeRad(angle) - PIf;
  float res = 0;
  float x = t_angle;
  int32_t precision = utils_FastCalcPrecision;
  int32_t factorialPrecision = 21;
  unsigned long long factorials[] = {0,
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
  for (int32_t i = 1; i <= precision && i < factorialPrecision; i += 4)
  {
    res += x / factorials[i];
    x *= t_angle * t_angle;
    res -= x / factorials[i + 2];
    x *= t_angle * t_angle;
  }
  return -res;
}
static inline float fast_atan(float a)
{
  if ((a > -1.5 && a < -0.7) || (a < 1.4 && a >= 0.6))
  {
    return atanf(a);
  }
  if (a > -1 && a < 1)
  {
    float res = 0;
    float x = a;
    int32_t precision = utils_FastCalcPrecision;
    for (int32_t i = 1; i <= precision; i += 2)
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
    int32_t precision = 11;
    for (int32_t i = 1; i < precision * 2; i += 2)
    {
      res += 1.f / ((4 * i - 3) * x);
      x *= a * a;
      res -= 1.f / ((4 * i - 1) * x);
      x *= a * a;
    }
    if (x < 0)
    {
      res += PIf / 2;
    }
    else
    {
      res -= PIf / 2;
    }
    return res;
  }
}

static inline void sleepMillis(uint32_t millis)
{
#ifdef _WIN32
  Sleep(millis);
#elif defined(__unix__) || defined(__APPLE__)
  usleep(millis * 1000);
#endif
}

#ifdef __cplusplus
class mapper
{
public:
  v4 min, max, targetMin, targetMax;
  mapper()
  {
    this->min = v4(0, 0, 0, 0);
    this->max = v4(0, 0, 0, 0);
    this->targetMin = v4(0, 0, 0, 0);
    this->targetMax = v4(0, 0, 0, 0);
  }
  mapper(float min, float max, float targetMin, float targetMax)
  {
    this->min = v4(min, 0, 0, 0);
    this->max = v4(max, 0, 0, 0);
    this->targetMin = v4(targetMin, 0, 0, 0);
    this->targetMax = v4(targetMax, 0, 0, 0);
  }
  mapper(v2 min, v2 max, v2 targetMin, v2 targetMax)
  {
    this->min = v4(min.x, min.y, 0, 0);
    this->max = v4(max.x, max.y, 0, 0);
    this->targetMin = v4(targetMin.x, targetMin.y, 0, 0);
    this->targetMax = v4(targetMax.x, targetMax.y, 0, 0);
  }
  mapper(v3 min, v3 max, v3 targetMin, v3 targetMax)
  {
    this->min = v4(min.x, min.y, min.z, 0);
    this->max = v4(max.x, max.y, max.z, 0);
    this->targetMin = v4(targetMin.x, targetMin.y, targetMin.z, 0);
    this->targetMax = v4(targetMax.x, targetMax.y, targetMax.z, 0);
  }
  mapper(v4 min, v4 max, v4 targetMin, v4 targetMax)
  {
    this->min = min;
    this->max = max;
    this->targetMin = targetMin;
    this->targetMax = targetMax;
  }
  float map(float n)
  {
    float scale = (n - min.x) / (max.x - min.x);
    return scale * (targetMax.x - targetMin.x) + targetMin.x;
  }
  v2 map(v2 n)
  {
    float xScale = V4MinusV4(v4(n.x, n.y, 0, 0), min).x / (V4MinusV4(max, min)).x;
    float yScale = V4MinusV4(v4(n.x, n.y, 0, 0), min).y / (V4MinusV4(max, min)).y;
    return v2(xScale * V4MinusV4(targetMax, targetMin).x + targetMin.x,
              yScale * V4MinusV4(targetMax, targetMin).y + targetMin.y);
  }
  v3 map(v3 n)
  {
    float xScale = V4MinusV4(v4(n.x, n.y, n.z, 0), min).x / (V4MinusV4(max, min)).x;
    float yScale = V4MinusV4(v4(n.x, n.y, n.z, 0), min).y / (V4MinusV4(max, min)).y;
    float zScale = V4MinusV4(v4(n.x, n.y, n.z, 0), min).z / (V4MinusV4(max, min)).z;
    return v3(xScale * V4MinusV4(targetMax, targetMin).x + targetMin.x,
              yScale * V4MinusV4(targetMax, targetMin).y + targetMin.y,
              zScale * V4MinusV4(targetMax, targetMin).z + targetMin.z);
  }
  v4 map(v4 n)
  {
    float xScale = V4MinusV4(n, min).x / (V4MinusV4(max, min)).x;
    float yScale = V4MinusV4(n, min).y / (V4MinusV4(max, min)).y;
    float zScale = V4MinusV4(n, min).z / (V4MinusV4(max, min)).z;
    float wScale = V4MinusV4(n, min).w / (V4MinusV4(max, min)).w;
    return v4(xScale * V4MinusV4(targetMax, targetMin).x + targetMin.x,
              yScale * V4MinusV4(targetMax, targetMin).y + targetMin.y,
              zScale * V4MinusV4(targetMax, targetMin).z + targetMin.z,
              wScale * V4MinusV4(targetMax, targetMin).w + targetMin.w);
  }
  m4 toM4(){
    m4 mat=M4Mapper(toV3(min),toV3(max),toV3(targetMin),toV3(targetMax));
    return mat;
  }
};
#endif

