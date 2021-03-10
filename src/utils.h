#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif

#define ArrayCount(a) (sizeof(a) / sizeof((a)[0]))

#define Bytes(n) (n)
#define Kilobytes(n) (Bytes(n) * 1024)
#define Megabytes(n) (Kilobytes(n) * 1024)

#define PIf 3.1415926535897f
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PI 0.3989422804
#define ONE_OVER_SQUARE_ROOT_OF_TWO_PIf 0.3989422804f
#define EULERS_NUMBER 2.7182818284590452353602874713527
#define EULERS_NUMBERf 2.7182818284590452353602874713527f

#ifndef fast_calc_precision
#define fast_calc_precision 9
#endif

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

#if defined(vectorObjects) && defined(__cplusplus)
struct v2;
struct v3;
struct v4;
struct iv2;
struct iv3;
struct iv4;
struct v2
{
  f32 x;
  f32 y;
  v2()
  {
    this->x = 0;
    this->y = 0;
  }
  v2(f32 x, f32 y)
  {
    this->x = x;
    this->y = y;
  }
  v2(const v2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  v2(f32 d)
  {
    this->x = d;
    this->y = 0;
  }
  v2(const iv2 &base);
  v2(const v3 &base);
  v2(const v4 &base);
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
  v2 operator+(const f32 a) { return v2(x + a, y + a); }
  v2 operator-(const f32 a) { return v2(x - a, y - a); }
  v2 operator*(const f32 a) { return v2(x * a, y * a); }
  v2 operator/(const f32 a) { return v2(x / a, y / a); }
  void operator+=(const f32 a)
  {
    x += a;
    y += a;
  }
  void operator-=(const f32 a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const f32 a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const f32 a)
  {
    x /= a;
    y /= a;
  }
};
struct v3
{
  union
  {
    struct
    {
      f32 x;
      f32 y;
      f32 z;
    };
    struct
    {
      f32 r;
      f32 g;
      f32 b;
    };
    f32 elements[3];
  };
  v3()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  v3(f32 x, f32 y, f32 z)
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
  v3(f32 d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
  }
  v3(const iv3 &base);
  v3(const v4 &base);
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
  v3 operator+(const f32 a) { return v3(x + a, y + a, z + a); }
  v3 operator-(const f32 a) { return v3(x - a, y - a, z - a); }
  v3 operator*(const f32 a) { return v3(x * a, y * a, z * a); }
  v3 operator/(const f32 a) { return v3(x / a, y / a, z / a); }
  void operator+=(const f32 a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const f32 a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const f32 a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const f32 a)
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
};
struct v4
{
  union
  {
    struct
    {
      f32 x;
      f32 y;
      union
      {
        struct
        {
          f32 z;
          f32 w;
        };
        struct
        {
          f32 width;
          f32 height;
        };
      };
    };
    struct
    {
      f32 r;
      f32 g;
      f32 b;
      f32 a;
    };
    f32 elements[4];
  };
  v4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  v4(f32 x, f32 y, f32 z, f32 w)
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
  v4(f32 d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  v4(const iv4 &base);
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
  v4 operator+(const f32 a) { return v4(x + a, y + a, z + a, w + a); }
  v4 operator-(const f32 a) { return v4(x - a, y - a, z - a, w - a); }
  v4 operator*(const f32 a) { return v4(x * a, y * a, z * a, w * a); }
  v4 operator/(const f32 a) { return v4(x / a, y / a, z / a, w / a); }
  void operator+=(const f32 a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const f32 a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const f32 a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const f32 a)
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
};
struct iv2
{
  i32 x, y;
  iv2()
  {
    this->x = 0;
    this->y = 0;
  }
  iv2(i32 x, i32 y)
  {
    this->x = x;
    this->y = y;
  }
  iv2(const iv2 &base)
  {
    this->x = base.x;
    this->y = base.y;
  }
  iv2(i32 d)
  {
    this->x = d;
    this->y = 0;
  }
  iv2(const v2 &base)
  {
    this->x = (i32 )floor(base.x);
    this->y = (i32 )floor(base.y);
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
  iv2 operator+(const i32 a) { return iv2(x + a, y + a); }
  iv2 operator-(const i32 a) { return iv2(x - a, y - a); }
  iv2 operator*(const i32 a) { return iv2(x * a, y * a); }
  iv2 operator/(const i32 a) { return iv2(x / a, y / a); }
  void operator+=(const i32 a)
  {
    x += a;
    y += a;
  }
  void operator-=(const i32 a)
  {
    x -= a;
    y -= a;
  }
  void operator*=(const i32 a)
  {
    x *= a;
    y *= a;
  }
  void operator/=(const i32 a)
  {
    x /= a;
    y /= a;
  }
};
struct iv3
{
  i32 x, y, z;
  iv3()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }
  iv3(i32 x, i32 y, i32 z)
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
  iv3(i32 d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
  }
  iv3(const v3 &base)
  {
    this->x = (i32 )floor(base.x);
    this->y = (i32 )floor(base.y);
    this->z = (i32 )floor(base.z);
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
  iv3 operator+(const i32 a) { return iv3(x + a, y + a, z + a); }
  iv3 operator-(const i32 a) { return iv3(x - a, y - a, z - a); }
  iv3 operator*(const i32 a) { return iv3(x * a, y * a, z * a); }
  iv3 operator/(const i32 a) { return iv3(x / a, y / a, z / a); }
  void operator+=(const i32 a)
  {
    x += a;
    y += a;
    z += a;
  }
  void operator-=(const i32 a)
  {
    x -= a;
    y -= a;
    z -= a;
  }
  void operator*=(const i32 a)
  {
    x *= a;
    y *= a;
    z *= a;
  }
  void operator/=(const i32 a)
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
  i32 x, y, z, w;
  iv4()
  {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  iv4(i32 x, i32 y, i32 z, i32 w)
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
  iv4(i32 d)
  {
    this->x = d;
    this->y = 0;
    this->z = 0;
    this->w = 0;
  }
  iv4(const v4 &base)
  {
    this->x = (i32 )floor(base.x);
    this->y = (i32 )floor(base.y);
    this->z = (i32 )floor(base.z);
    this->w = (i32 )floor(base.w);
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
  iv4 operator+(const i32 a) { return iv4(x + a, y + a, z + a, w + a); }
  iv4 operator-(const i32 a) { return iv4(x - a, y - a, z - a, w - a); }
  iv4 operator*(const i32 a) { return iv4(x * a, y * a, z * a, w * a); }
  iv4 operator/(const i32 a) { return iv4(x / a, y / a, z / a, w / a); }
  void operator+=(const i32 a)
  {
    x += a;
    y += a;
    z += a;
    z += a;
  }
  void operator-=(const i32 a)
  {
    x -= a;
    y -= a;
    z -= a;
    z -= a;
  }
  void operator*=(const i32 a)
  {
    x *= a;
    y *= a;
    z *= a;
    z *= a;
  }
  void operator/=(const i32 a)
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
  this->x = (f32)base.x;
  this->y = (f32)base.y;
}
inline v2::v2(const v3 &base)
{
  this->x = base.x;
  this->y = base.y;
}
inline v2::v2(const v4 &base)
{
  this->x = base.x;
  this->y = base.y;
}
inline v3::v3(const iv3 &base)
{
  this->x = (f32)base.x;
  this->y = (f32)base.y;
  this->z = (f32)base.z;
}
inline v3::v3(const v4 &base)
{
  this->x = base.x;
  this->y = base.y;
  this->z = base.z;
}
inline v4::v4(const iv4 &base)
{
  this->x = (f32)base.x;
  this->y = (f32)base.y;
  this->z = (f32)base.z;
  this->w = (f32)base.w;
}
#define v2ToArray(v) \
  {                  \
    v.x, v.y         \
  }
#define v3ToArray(v) \
  {                  \
    v.x, v.y, v.z    \
  }
#define v4ToArray(v)   \
  {                    \
    v.x, v.y, v.z, v.w \
  }

#else
typedef struct
{
  f32 x;
  f32 y;
} v2;
typedef union
{
  struct
  {
    f32 x;
    f32 y;
    f32 z;
  };

  struct
  {
    f32 r;
    f32 g;
    f32 b;
  };

  f32 elements[3];
} v3;
typedef union
{
  struct
  {
    f32 x;
    f32 y;
    union
    {
      struct
      {
        f32 z;

        union
        {
          f32 w;
          f32 radius;
        };
      };
      struct
      {
        f32 width;
        f32 height;
      };
    };
  };

  struct
  {
    v3 xyz;
    f32 _unused;
  };

  struct
  {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
  };

  f32 elements[4];
} v4;

static v2 V2Init(f32 x, f32 y)
{
  v2 v = {x, y};
  return v;
}
#define v2(x, y) V2Init(x, y)
static v3 V3Init(f32 x, f32 y, f32 z)
{
  v3 v = {x, y, z};
  return v;
}
#define v3(x, y, z) V3Init(x, y, z)
static v4 V4Init(f32 x, f32 y, f32 z, f32 w)
{
  v4 v = {x, y, z, w};
  return v;
}
#define v4(x, y, z, w) V4Init(x, y, z, w)
#define v4u(x) v4(x, x, x, x)

typedef struct iv2
{
  i32 x;
  i32 y;
} iv2;
typedef union iv3
{
  struct
  {
    i32 x;
    i32 y;
    i32 z;
  };

  struct
  {
    i32 r;
    i32 g;
    i32 b;
  };

  i32 elements[3];
} iv3;
typedef union iv4
{
  struct
  {
    i32 x;
    i32 y;
    i32 z;
    i32 w;
  };

  struct
  {
    i32 r;
    i32 g;
    i32 b;
    i32 a;
  };

  i32 elements[4];
} iv4;
static iv2 IV2Init(i32 x, i32 y)
{
  iv2 v = {x, y};
  return v;
}
#define iv2(x, y) IV2Init(x, y)
static iv3 IV3Init(i32 x, i32 y, i32 z)
{
  iv3 v = {x, y, z};
  return v;
}
#define iv3(x, y, z) IV3Init(x, y, z)
static iv4 IV4Init(i32 x, i32 y, i32 z, i32 w)
{
  iv4 v = {x, y, z, w};
  return v;
}
#define iv4(x, y, z, w) IV4Init(x, y, z, w)

#endif
#define toV2(v) v2(v.x,v.y)
#define toV3(v) v2(v.x,v.y,v.y)
#define toV4(v) v2(v.x, v.y)

static f32 V2Dot(v2 a, v2 b)
{
  return a.x * b.x + a.y * b.y;
}
static f32 V2LengthSquared(v2 v)
{
  return v.x * v.x + v.y * v.y;
}
static v2 V2Normalize(v2 v)
{
  f32 length = sqrtf(V2LengthSquared(v));
  v.x /= length;
  v.y /= length;
  return v;
}
static v2 V2AddV2(v2 a,v2 b)
{
  v2 v=v2(a.x+b.x,a.y+b.y);
  return v;
}
static v2 V2MinusV2(v2 a, v2 b)
{
  v2 v = v2(a.x - b.x, a.y - b.y);
  return v;
}
static v2 V2MultiplyF32(v2 v, f32 f)
{
  v.x *= f;
  v.y *= f;
  return v;
}
static v3 V3AddV3(v3 a, v3 b)
{
  v3 c = v3(a.x + b.x, a.y + b.y, a.z + b.z);
  return c;
}
static v3 V3MinusV3(v3 a, v3 b)
{
  v3 c = v3(a.x - b.x, a.y - b.y, a.z - b.z);
  return c;
}
static v3 V3MultiplyF32(v3 v, f32 f)
{
  v.x *= f;
  v.y *= f;
  v.z *= f;
  return v;
}
static f32 V3LengthSquared(v3 a)
{
  return a.x * a.x + a.y * a.y + a.z * a.z;
}
static f32 V3Length(v3 a)
{
  return sqrtf(V3LengthSquared(a));
}
static v3 V3Normalize(v3 v)
{
  f32 length = V3Length(v);
  v3 result = v3(
      v.x / length,
      v.y / length,
      v.z / length);
  return result;
}
static f32 V3Dot(v3 a, v3 b)
{
  f32 dot =
      a.x * b.x +
      a.y * b.y +
      a.z * b.z;
  return dot;
}
static v3 V3Cross(v3 a, v3 b)
{
  v3 result = v3(
      a.y * b.z - a.z * b.y,
      a.z * b.x - a.x * b.z,
      a.x * b.y - a.y * b.x);
  return result;
}
static f32 minInV3(v3 v)
{
  f32 minimum = v.x;
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
static f32 maxInV3(v3 v)
{
  f32 maximum = v.x;
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
static bool V4RectHasPoint(v4 v, v2 p)
{
  return (p.x >= v.x && p.x <= v.x + v.width &&
          p.y >= v.y && p.y <= v.y + v.height);
}

typedef struct m4
{
  f32 elements[4][4];
} m4;
static m4 M4InitD(f32 diagonal)
{
  m4 m = {
      {
          {diagonal},
          {0.f, diagonal},
          {0.f, 0.f, diagonal},
          {0.f, 0.f, 0.f, diagonal},
      }};
  return m;
}
static m4 M4MultiplyM4(m4 a, m4 b)
{
  m4 c = {0};

  for (i32 j = 0; j < 4; ++j)
  {
    for (i32 i = 0; i < 4; ++i)
    {
      c.elements[i][j] = (a.elements[0][j] * b.elements[i][0] +
                          a.elements[1][j] * b.elements[i][1] +
                          a.elements[2][j] * b.elements[i][2] +
                          a.elements[3][j] * b.elements[i][3]);
    }
  }

  return c;
}
static m4 M4MultiplyF32(m4 a, f32 b)
{
  for (i32 j = 0; j < 4; ++j)
  {
    for (i32 i = 0; i < 4; ++i)
    {
      a.elements[i][j] *= b;
    }
  }

  return a;
}
static f32 V4Dot(v4 a, v4 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
static v4 V4AddV4(v4 a, v4 b)
{
  v4 c = v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
  return c;
}
static v4 V4MinusV4(v4 a, v4 b)
{
  v4 c = v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
  return c;
}
static v4 V4MultiplyF32(v4 a, f32 f)
{
  v4 c = v4(a.x * f, a.y * f, a.z * f, a.w * f);
  return c;
}
static v4 V4MultiplyV4(v4 a, v4 b)
{
  v4 c = v4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
  return c;
}
static v4 V4MultiplyM4(v4 v, m4 m)
{
  v4 result = v4(0, 0, 0, 0);

  for (i32 i = 0; i < 4; ++i)
  {
    result.elements[i] = (v.elements[0] * m.elements[0][i] +
                          v.elements[1] * m.elements[1][i] +
                          v.elements[2] * m.elements[2][i] +
                          v.elements[3] * m.elements[3][i]);
  }

  return result;
}
static m4 M4TranslateV3(v3 translation)
{
  m4 result = M4InitD(1.f);
  result.elements[3][0] = translation.x;
  result.elements[3][1] = translation.y;
  result.elements[3][2] = translation.z;
  return result;
}
static m4 M4ScaleV3(v3 scale)
{
  m4 result = M4InitD(1.f);
  result.elements[0][0] = scale.x;
  result.elements[1][1] = scale.y;
  result.elements[2][2] = scale.z;
  return result;
}
static m4 M4Perspective(f32 fov, f32 aspect_ratio, f32 near_z, f32 far_z)
{
  m4 result = {0};
  f32 tan_theta_over_2 = tanf(fov * (PIf / 360.f));
  result.elements[0][0] = 1.f / tan_theta_over_2;
  result.elements[1][1] = aspect_ratio / tan_theta_over_2;
  result.elements[2][3] = -1.f;
  result.elements[2][2] = (near_z + far_z) / (near_z - far_z);
  result.elements[3][2] = (2.f * near_z * far_z) / (near_z - far_z);
  result.elements[3][3] = 0.f;
  return result;
}
static m4 M4Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 near_depth, f32 far_depth)
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
static m4 M4LookAt(v3 eye, v3 center, v3 up)
{
  m4 result;

  v3 f = V3Normalize(V3MinusV3(center, eye));
  v3 s = V3Normalize(V3Cross(f, up));
  v3 u = V3Cross(s, f);

  result.elements[0][0] = s.x;
  result.elements[0][1] = u.x;
  result.elements[0][2] = -f.x;
  result.elements[0][3] = 0.0f;

  result.elements[1][0] = s.y;
  result.elements[1][1] = u.y;
  result.elements[1][2] = -f.y;
  result.elements[1][3] = 0.0f;

  result.elements[2][0] = s.z;
  result.elements[2][1] = u.z;
  result.elements[2][2] = -f.z;
  result.elements[2][3] = 0.0f;

  result.elements[3][0] = -V3Dot(s, eye);
  result.elements[3][1] = -V3Dot(u, eye);
  result.elements[3][2] = V3Dot(f, eye);
  result.elements[3][3] = 1.0f;

  return result;
}
static m4 M4Inverse(m4 m)
{
  f32 coef00 = m.elements[2][2] * m.elements[3][3] - m.elements[3][2] * m.elements[2][3];
  f32 coef02 = m.elements[1][2] * m.elements[3][3] - m.elements[3][2] * m.elements[1][3];
  f32 coef03 = m.elements[1][2] * m.elements[2][3] - m.elements[2][2] * m.elements[1][3];
  f32 coef04 = m.elements[2][1] * m.elements[3][3] - m.elements[3][1] * m.elements[2][3];
  f32 coef06 = m.elements[1][1] * m.elements[3][3] - m.elements[3][1] * m.elements[1][3];
  f32 coef07 = m.elements[1][1] * m.elements[2][3] - m.elements[2][1] * m.elements[1][3];
  f32 coef08 = m.elements[2][1] * m.elements[3][2] - m.elements[3][1] * m.elements[2][2];
  f32 coef10 = m.elements[1][1] * m.elements[3][2] - m.elements[3][1] * m.elements[1][2];
  f32 coef11 = m.elements[1][1] * m.elements[2][2] - m.elements[2][1] * m.elements[1][2];
  f32 coef12 = m.elements[2][0] * m.elements[3][3] - m.elements[3][0] * m.elements[2][3];
  f32 coef14 = m.elements[1][0] * m.elements[3][3] - m.elements[3][0] * m.elements[1][3];
  f32 coef15 = m.elements[1][0] * m.elements[2][3] - m.elements[2][0] * m.elements[1][3];
  f32 coef16 = m.elements[2][0] * m.elements[3][2] - m.elements[3][0] * m.elements[2][2];
  f32 coef18 = m.elements[1][0] * m.elements[3][2] - m.elements[3][0] * m.elements[1][2];
  f32 coef19 = m.elements[1][0] * m.elements[2][2] - m.elements[2][0] * m.elements[1][2];
  f32 coef20 = m.elements[2][0] * m.elements[3][1] - m.elements[3][0] * m.elements[2][1];
  f32 coef22 = m.elements[1][0] * m.elements[3][1] - m.elements[3][0] * m.elements[1][1];
  f32 coef23 = m.elements[1][0] * m.elements[2][1] - m.elements[2][0] * m.elements[1][1];

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
  for (u32 i = 0; i < 4; ++i)
  {
    inverse.elements[0][i] = inv0.elements[i] * sign_a.elements[i];
    inverse.elements[1][i] = inv1.elements[i] * sign_b.elements[i];
    inverse.elements[2][i] = inv2.elements[i] * sign_a.elements[i];
    inverse.elements[3][i] = inv3.elements[i] * sign_b.elements[i];
  }

  v4 row0 = v4(inverse.elements[0][0], inverse.elements[1][0], inverse.elements[2][0], inverse.elements[3][0]);
  v4 m0 = v4(m.elements[0][0], m.elements[0][1], m.elements[0][2], m.elements[0][3]);
  v4 dot0 = V4MultiplyV4(m0, row0);
  f32 dot1 = (dot0.x + dot0.y) + (dot0.z + dot0.w);

  f32 one_over_det = 1 / dot1;

  return M4MultiplyF32(inverse, one_over_det);
}
static m4 M4RemoveRotation(m4 mat)
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
static m4 M4Rotate(f32 angle, v3 axis)
{
  m4 result = M4InitD(1.f);

  axis = V3Normalize(axis);

  f32 sin_theta = sinf(angle);
  f32 cos_theta = cosf(angle);
  f32 cos_value = 1.0f - cos_theta;

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
static v3 calculateTriangleNormal(v3 p1, v3 p2, v3 p3)
{
  v3 p1_to_p2 = v3(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
  v3 p1_to_p3 = v3(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
  v3 normal = V3Cross(p1_to_p3, p1_to_p2);
  normal.y *= -1;
  return normal;
}
static v3 calculateTriangleNormalNormalized(v3 p1, v3 p2, v3 p3)
{
  v3 normal = calculateTriangleNormal(p1, p2, p3);
  normal = V3Normalize(normal);
  return normal;
}

#ifdef strManipulation
#define StringCopy strcpy
#define StringCopyN strncpy
#define CalculateCStringLength(s) ((u32)strlen(s))
#define CStringToI32(s) ((i32)atoi(s))
#define CStringToI16(s) ((i16)atoi(s))
#define CStringToF32(s) ((f32)atof(s))
static bool CharIsSpace(char c)
{
  return c <= 32;
}
static bool CharIsAlpha(char c)
{
  return ((c >= 'a' && c <= 'z') ||
          (c >= 'A' && c <= 'Z'));
}
static bool CharIsDigit(char c)
{
  return (c >= '0' && c <= '9');
}
static i32 CharToLower(i32 c)
{
  if (c >= 'A' && c <= 'Z')
  {
    c += 32;
  }
  return c;
}
static i32 CharToUpper(i32 c)
{
  if (c >= 'a' && c <= 'z')
  {
    c -= 32;
  }
  return c;
}
static u32 HashCString(char *string)
{
  u32 hash = 5381;
  i32 c;
  while ((c = *string++))
  {
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}
static bool CStringMatchCaseInsensitiveN(const char *str1, const char *str2, u32 n)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (u32 i = 0; i < n; ++i)
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
static bool CStringMatchCaseInsensitive(const char *str1, const char *str2)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (u32 i = 0;; ++i)
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
static bool CStringMatchCaseSensitiveN(const char *str1, const char *str2, u32 n)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (u32 i = 0; i < n; ++i)
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
static bool CStringMatchCaseSensitive(const char *str1, const char *str2)
{
  bool result = 1;

  if (str1 && str2)
  {
    for (u32 i = 0;; ++i)
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
static bool CStringContains(const char *str, const char *substr)
{
  bool result = 0;

  if (str && substr)
  {
    for (u32 i = 0; str[i]; ++i)
    {
      if (str[i] == substr[0])
      {
        result = 1;
        for (u32 j = i + 1; str[j] && substr[j - i]; ++j)
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
static char *CStringPointerAfterSubstring(char *str, char *substr)
{
  char *result = 0;

  if (str && substr)
  {
    for (u32 i = 0; str[i]; ++i)
    {
      result = str + i;
      if (str[i] == substr[0])
      {
        for (u32 j = 0;; ++j)
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
static u32 CStringIndexAfterSubstring(char *str, char *substr)
{
  u32 result = 0;
  for (u32 i = 0; str[i]; ++i)
  {
    if (str[i] == substr[0])
    {
      if (CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
      {
        result = i + CalculateCStringLength(substr);
      }
    }
  }
  return result;
}
static u32 CStringFirstIndexAfterSubstring(char *str, char *substr)
{
  u32 result = 0;
  for (u32 i = 0; str[i]; ++i)
  {
    if (str[i] == substr[0])
    {
      if (CStringMatchCaseInsensitiveN(str + i, substr, CalculateCStringLength(substr)))
      {
        result = i + CalculateCStringLength(substr);
        break;
      }
    }
  }
  return result;
}
static char *CStringFindSubstring(char *str, char *substr)
{
  char *result = 0;

  if (str && substr)
  {
    for (u32 i = 0; str[i]; ++i)
    {
      if (str[i] == substr[0])
      {
        char *possible_result = str + i;
        bool match = 1;
        for (u32 j = i + 1; str[j] && substr[j - i]; ++j)
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
static void CopyCStringToFixedSizeBuffer(char *destination, u32 destination_max, char *source)
{
  for (u32 i = 0; i < destination_max; ++i)
  {
    destination[i] = source[i];
    if (source[i] == 0)
    {
      break;
    }
  }
  destination[destination_max - 1] = 0;
}
static void CopySubstringToStringUntilCharN(char *str1, u32 str1_max, char *str2, char str2_term)
{
  u32 write_pos = 0;
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
static i32 GetFirstI32FromCString(char *str)
{
  i32 result = 0;

  i32 last_digit = -1;

  for (i32 i = 0; str[i]; ++i)
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
    i32 digit_multiplier = 1;

    for (i32 i = last_digit; i >= 0; --i)
    {
      if (CharIsDigit(str[i]))
      {
        i32 digit = str[i] - '0';
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
static f32 GetFirstF32FromCString(char *str)
{
  f32 result = 0;
  bool found_first_digit = 0;
  u32 float_write_pos = 0;
  char float_str[64] = {0};
  u32 read_pos = 0;
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
  u8 r;
  u8 g;
  u8 b;
  u8 a;
} rgbcolor;
static rgbcolor RGBColorInit(u8 r, u8 g, u8 b, u8 a)
{
  rgbcolor c = {r, g, b, a};
  return c;
}
#define rgbcolor(r, g, b, a) RGBColorInit(r, g, b, a)
static v3 RGBToHSV(v3 rgb)
{
  f32 c_max = maxInV3(rgb);
  f32 c_min = minInV3(rgb);
  f32 delta = c_max - c_min;
  bool c_max_is_r = rgb.r > rgb.g && rgb.r > rgb.b;
  bool c_max_is_g = rgb.g > rgb.r && rgb.g > rgb.b;
  bool c_max_is_b = rgb.b > rgb.r && rgb.b > rgb.g;
  f32 h = (c_max_is_r ? (rgb.g - rgb.b) / delta + 0 : c_max_is_g ? (rgb.b - rgb.r) / delta + 2
                                                  : c_max_is_b   ? (rgb.r - rgb.g) / delta + 4
                                                                 : 0);
  f32 s = c_max == 0 ? 0 : (delta / c_max);
  f32 v = c_max;
  v3 hsv = v3(h / 6.f, s, v);
  return hsv;
}
static v3 RGBColorToHSV(rgbcolor rgba)
{
  v3 newColor = v3(rgba.r / 255.0f, rgba.g / 255.0f, rgba.b / 255.0f);
  return RGBToHSV(newColor);
}
static v3 HSVToRGB(v3 hsv)
{
  f32 h = fmodf(hsv.x * 360.f, 360.f);
  f32 s = hsv.y;
  f32 v = hsv.z;

  f32 c = v * s;
  f32 x = c * (1 - fabsf(fmodf((h / 60.f), 2) - 1));
  f32 m = v - c;

  f32 r;
  f32 g;
  f32 b;

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
static rgbcolor HSVToRGBColor(v3 hsv)
{
  v3 newColor = HSVToRGB(hsv);
  return rgbcolor((i32)floorf(newColor.r * 255) % 255, (i32)floorf(newColor.g * 255) % 255, (i32)floorf(newColor.b * 255) % 255, 1);
}

typedef enum Key
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
  RIGHT_BRACKET,
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
} Key;

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

static void seedRandomNumberGenerator(void)
{
  srand((u32 )time(0));
}
static f32 randf()
{
  return ((float)rand() / (RAND_MAX));
}
#ifdef perlinNoise
static i32 global_perlin_noise_seed = 0;
static i32 global_perlin_noise_hash[] = {
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

static i32 PerlinNoise2(i32 x, i32 y)
{
  i32 tmp = global_perlin_noise_hash[(y + global_perlin_noise_seed) % 256];
  return global_perlin_noise_hash[(tmp + x) % 256];
}
static f32 PerlinLinearlyInterpolate(f32 x, f32 y, f32 s)
{
  return x + s * (y - x);
}
static f32 PerlinSmoothlyInterpolate(f32 x, f32 y, f32 s)
{
  return PerlinLinearlyInterpolate(x, y, s * s * (3 - 2 * s));
}
static f32 PerlinNoise2D(f32 x, f32 y)
{
  i32 x_int = (i32 )x;
  i32 y_int = (i32 )y;
  f32 x_frac = x - x_int;
  f32 y_frac = y - y_int;
  i32 s = PerlinNoise2(x_int, y_int);
  i32 t = PerlinNoise2(x_int + 1, y_int);
  i32 u = PerlinNoise2(x_int, y_int + 1);
  i32 v = PerlinNoise2(x_int + 1, y_int + 1);
  f32 low = PerlinSmoothlyInterpolate((f32)s, (f32)t, x_frac);
  f32 high = PerlinSmoothlyInterpolate((f32)u, (f32)v, x_frac);
  return PerlinSmoothlyInterpolate(low, high, y_frac);
}
static f32 Perlin2D(f32 x, f32 y, f32 freq, i32 depth)
{
  f32 xa = x * freq;
  f32 ya = y * freq;
  f32 amp = 1.0;
  f32 fin = 0;
  f32 div = 0.0;

  for (i32 i = 0; i < depth; i++)
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

static f32 InterpolateLinear(f32 t)
{
  f32 result = 0;
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
static f32 InterpolateSmooth(f32 t)
{
  f32 result = 0;
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
    f32 t2 = t * t;
    f32 t4 = t2 * t2;
    f32 t6 = t2 * t4;
    result = (4 / 9) * t6 - (17 / 9) * t4 + (22 / 9) * t2;
  }
  return result;
}
static f32 regularizeDegree(f32 angle)
{
  f32 degree = fmodf(angle, 360);
  if (degree < 0)
    degree += 360;
  return degree;
}
static f32 regularizeRad(f32 angle)
{
  f32 rad = fmodf(angle, 2 * PIf);
  if (rad < 0)
    rad += 2 * PIf;
  return rad;
}
static f32 degreeToRad(f32 degree) { return degree / 180.0 * PIf; }
static f32 radToDegree(f32 rad) { return rad / PIf * 180; }
static f32 fast_cos(f32 angle)
{
  f32 t_angle = regularizeRad(angle) - PIf;
  f32 res = 0;
  f32 x = 1;
  i32 precision = fast_calc_precision;
  i32 factorialPrecision = 20;
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
  for (i32 i = 0; i <= precision && i < factorialPrecision; i += 4)
  {
    res += x / factorials[i];
    x *= t_angle * t_angle;
    res -= x / factorials[i + 2];
    x *= t_angle * t_angle;
  }
  return -res;
}
static f32 fast_sin(f32 angle)
{
  f32 t_angle = regularizeRad(angle) - PIf;
  f32 res = 0;
  f32 x = t_angle;
  i32 precision = fast_calc_precision;
  i32 factorialPrecision = 21;
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
  for (i32 i = 1; i <= precision && i < factorialPrecision; i += 4)
  {
    res += x / factorials[i];
    x *= t_angle * t_angle;
    res -= x / factorials[i + 2];
    x *= t_angle * t_angle;
  }
  return -res;
}
static f32 fast_atan(f32 a)
{
  if ((a > -1.5 && a < -0.7) || (a < 1.4 && a >= 0.6))
  {
    return atanf(a);
  }
  if (a > -1 && a < 1)
  {
    f32 res = 0;
    f32 x = a;
    i32 precision = fast_calc_precision;
    for (i32 i = 1; i <= precision; i += 2)
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
    f32 res = 0;
    f32 x = -a;
    i32 precision = 11;
    for (i32 i = 1; i < precision * 2; i += 2)
    {
      res += 1.0 / ((4 * i - 3) * x);
      x *= a * a;
      res -= 1.0 / ((4 * i - 1) * x);
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

static void sleepMillis(u32 millis)
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
    this->min = v4(0,0,0,0);
    this->max = v4(0,0,0,0);
    this->targetMin = v4(0, 0, 0, 0);
    this->targetMax = v4(0, 0, 0, 0);
  }
  mapper(f32 min, f32 max, f32 targetMin, f32 targetMax)
  {
    this->min = v4(min,0,0,0);
    this->max = v4(max,0,0,0);
    this->targetMin = v4(targetMin,0,0,0);
    this->targetMax = v4(targetMax,0,0,0);
  }
  mapper(v2 min, v2 max, v2 targetMin, v2 targetMax)
  {
    this->min = v4(min.x,min.y,0,0);
    this->max = v4(max.x,max.y,0,0);
    this->targetMin = v4(targetMin.x,targetMin.y,0,0);
    this->targetMax = v4(targetMax.x,targetMax.y,0,0);
  }
  mapper(v3 min, v3 max, v3 targetMin, v3 targetMax)
  {
    this->min = v4(min.x,min.y,min.z,0);
    this->max = v4(max.x,max.y,max.z,0);
    this->targetMin = v4(targetMin.x,targetMin.y,targetMin.z,0);
    this->targetMax = v4(targetMax.x, targetMax.y, targetMax.z,0);
  }
  mapper(v4 min, v4 max, v4 targetMin, v4 targetMax)
  {
    this->min = min;
    this->max = max;
    this->targetMin = targetMin;
    this->targetMax = targetMax;
  }
  f32 map(f32 n)
  {
    f32 scale = (n - min.x) / (max.x - min.x);
    return scale * (targetMax.x - targetMin.x) + targetMin.x;
  }
  v2 map(v2 n)
  {
    f32 xScale = V4MinusV4(v4(n.x,n.y,0,0),min).x/(V4MinusV4(max, min)).x;
    f32 yScale = V4MinusV4(v4(n.x, n.y, 0, 0), min).y / (V4MinusV4(max, min)).y;
    return v2(xScale * V4MinusV4(targetMax, targetMin).x + targetMin.x,
              yScale * V4MinusV4(targetMax, targetMin).y + targetMin.y);
  }
  v3 map(v3 n)
  {
    f32 xScale = V4MinusV4(v4(n.x, n.y, n.z, 0), min).x / (V4MinusV4(max, min)).x;
    f32 yScale = V4MinusV4(v4(n.x, n.y, n.z, 0), min).y / (V4MinusV4(max, min)).y;
    f32 zScale = V4MinusV4(v4(n.x, n.y, n.z, 0), min).z / (V4MinusV4(max, min)).z;
    return v3(xScale * V4MinusV4(targetMax, targetMin).x + targetMin.x,
              yScale * V4MinusV4(targetMax, targetMin).y + targetMin.y,
              zScale * V4MinusV4(targetMax, targetMin).z + targetMin.z);
  }
  v4 map(v4 n)
  {
    f32 xScale = V4MinusV4(n, min).x / (V4MinusV4(max, min)).x;
    f32 yScale = V4MinusV4(n, min).y / (V4MinusV4(max, min)).y;
    f32 zScale = V4MinusV4(n, min).z / (V4MinusV4(max, min)).z;
    f32 wScale = V4MinusV4(n, min).w / (V4MinusV4(max, min)).w;
    return v4(xScale * V4MinusV4(targetMax, targetMin).x + targetMin.x,
              yScale * V4MinusV4(targetMax, targetMin).y + targetMin.y,
              zScale * V4MinusV4(targetMax, targetMin).z + targetMin.z,
              wScale * V4MinusV4(targetMax, targetMin).w + targetMin.w);
  }
};
#endif