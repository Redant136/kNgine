#pragma once

#include "utils.h"

#ifndef kStruct_arg_def_max_length
#define kStruct_arg_def_max_length 4
#endif

#if _WIN32 || _WIN64
#if _WIN64
#define kTypePointerSize 8
#else
#define kTypePointerSize 4
#endif
#elif __GNUC__
#if __x86_64__ || __ppc64__
#define kTypePointerSize 8
#else
#define kTypePointerSize 4
#endif
#endif

typedef enum kTypes
{
  kTYPE_bool,
  kTYPE_char,
  kTYPE_i8,
  kTYPE_u8,
  kTYPE_i16,
  kTYPE_u16,
  kTYPE_i32,
  kTYPE_u32,
  kTYPE_f32,
  kTYPE_i64,
  kTYPE_u64,
  kTYPE_f64,
  kTYPE_PTR,
  kTYPE_v2,
  kTYPE_v3,
  kTYPE_v4,
  kTYPE_iv2,
  kTYPE_iv3,
  kTYPE_iv4
} kTypes;

static u8 kTypeSizeByte(kTypes t)
{
#ifndef kType_lookupTable
  switch (t)
  {
  case kTYPE_bool:
    return 1 << 0;
  case kTYPE_char:
    return 1 << 0;
  case kTYPE_i8:
    return 1 << 0;
  case kTYPE_u8:
    return 1 << 0;
  case kTYPE_i16:
    return 1 << 1;
  case kTYPE_u16:
    return 1 << 1;
  case kTYPE_i32:
    return 1 << 2;
  case kTYPE_u32:
    return 1 << 2;
  case kTYPE_f32:
    return 1 << 2;
  case kTYPE_i64:
    return 1 << 3;
  case kTYPE_u64:
    return 1 << 3;
  case kTYPE_f64:
    return 1 << 3;
  case kTYPE_PTR:
    return kTypePointerSize;
  case kTYPE_v2:
    return ((1 << 2) * 2);
  case kTYPE_v3:
    return ((1 << 2) * 3);
  case kTYPE_v4:
    return ((1 << 2) * 4);
  case kTYPE_iv2:
    return ((1 << 2) * 2);
  case kTYPE_iv3:
    return ((1 << 2) * 3);
  case kTYPE_iv4:
    return ((1 << 2) * 4);
  default:
    return 0;
  }
#else
  if (t >= 18)
  {
    return 0;
  }
  static const u8 table[18] = {
      1 << 0,
      1 << 0,
      1 << 0,
      1 << 0,
      1 << 1,
      1 << 1,
      1 << 2,
      1 << 2,
      1 << 2,
      1 << 3,
      1 << 3,
      1 << 3,
      kTypePointerSize,
      (1 << 2) * 2,
      (1 << 2) * 3,
      (1 << 2) * 4,
      (1 << 2) * 2,
      (1 << 2) * 3,
      (1 << 2) * 4};
  return table[t];
#endif
}
typedef struct kStruct_StructDef
{
  size_t length;
  kTypes args[kStruct_arg_def_max_length];
} kStruct_StructDef;
static void *getElementOfObjectAtIndex(u32 index, kStruct_StructDef def, void *obj)
{
  u8 *ptr = (u8 *)obj;
  u32 distance;
  for (u32 i = 0; i < index; i++)
  {
    distance += kTypeSizeByte(def.args[i]);
  }
  return ptr + distance;
}

#undef kTypePointerSize


// for casting from void with kType to var
#define KType(t) kTYPE_##t

// data, var and type are accessible inside expression
#define kCastingConditionExpression(var, t, data, type, expression) \
  if (t == KType(type))                                             \
  {                                                                 \
    expression;                                                     \
  } // yes there will be semicolons extra, but this is to make sure user doesn't forget

// wont work for pointer
#define kCastingCondition(var, t, data, type) kCastingConditionExpression(var, t, data, type, {var = *(type *)data;})

// if var is castable ie basic type, cast it. macro to save code typing
#define castableKTypePointerCasting(varType, var, data, ktype)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             \
  {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        \
    if (ktype == kTYPE_bool)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
      var = (*(bool *)data) ? 1 : 0;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       \
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      \
    else kCastingCondition(var, ktype, data, char) else kCastingCondition(var, ktype, data, i8) else kCastingCondition(var, ktype, data, u8) else kCastingCondition(var, ktype, data, i16) else kCastingCondition(var, ktype, data, u16) else kCastingCondition(var, ktype, data, i32) else kCastingCondition(var, ktype, data, u32) else kCastingCondition(var, ktype, data, f32) else kCastingCondition(var, ktype, data, i64) else kCastingCondition(var, ktype, data, u64) else kCastingCondition(var, ktype, data, f64) else if (ktype == kTYPE_PTR) \
      {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
        var = **(varType **)data;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          \
      }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    else kCastingConditionExpression(var, ktype, data, v2, {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
      var = (*(v2 *)data).x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
    }) else kCastingConditionExpression(var, ktype, data, v3, {\
      var = (*(v3*)data).x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    \
    }) else kCastingConditionExpression(var, ktype, data, v4, {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 \
      var = (*(v4 *)data).x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               \
    }) else kCastingConditionExpression(var, ktype, data, iv2, {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
      var = (*(iv2 *)data).x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              \
    }) else kCastingConditionExpression(var, ktype, data, iv3, {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
      var = (*(iv3 *)data).x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              \
    }) else kCastingConditionExpression(var, ktype, data, iv4, {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                \
      var = (*(iv4 *)data).x;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              \
    })                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     \
  }

#define kCastType_bool(var, data, type)                                                                       \
  {                                                                                                           \
    if (type == kTYPE_bool)                                                                       \
    {                                                                                                         \
      var = *(bool *)data;                                                                                    \
    }                                                                                                         \
    else if (type == kTYPE_char || type == kTYPE_i8 ||                                \
             type == kTYPE_u8 || type == kTYPE_i16 ||                                 \
             type == kTYPE_u16 || type == kTYPE_i32 ||                                \
             type == kTYPE_u32 || type == kTYPE_f32 ||                                \
             type == kTYPE_i64 || type == kTYPE_u64 ||                                \
             type == kTYPE_f64)                                                                   \
    {                                                                                                         \
      var = *(u64 *)data;                                                                                     \
    }                                                                                                         \
    else if (type == kTYPE_PTR)                                                                   \
    {                                                                                                         \
      var = data != NULL;                                                                                     \
    }                                                                                                         \
    else if (type == kTYPE_v2 || type == kTYPE_v3 || type == kTYPE_v4 ||  \
             type == kTYPE_iv2 || type == kTYPE_iv3 || type == kTYPE_iv4) \
    {                                                                                                         \
      assert(0 && "cannot cast type");                                                                        \
    }                                                                                                         \
  }
#define kCastType_char(var, data, type) castableKTypePointerCasting(char, var, data, type);
#define kCastType_i8(var, data, type) castableKTypePointerCasting(i8, var, data, type);
#define kCastType_u8(var, data, type) castableKTypePointerCasting(u8, var, data, type);
#define kCastType_i16(var, data, type) castableKTypePointerCasting(i16, var, data, type);
#define kCastType_u16(var, data, type) castableKTypePointerCasting(u16, var, data, type);
#define kCastType_i32(var, data, type) castableKTypePointerCasting(i32, var, data, type);
#define kCastType_u32(var, data, type) castableKTypePointerCasting(u32, var, data, type);
#define kCastType_f32(var, data, type) castableKTypePointerCasting(f32, var, data, type);
#define kCastType_i64(var, data, type) castableKTypePointerCasting(i64, var, data, type);
#define kCastType_u64(var, data, type) castableKTypePointerCasting(u64, var, data, type);
#define kCastType_f64(var, data, type) castableKTypePointerCasting(f64, var, data, type);
#define kCastType_v2(var, data, type)                                         \
  {                                                                           \
    if (type == kTYPE_bool)                                       \
    {                                                                         \
      var = v2((*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0);             \
    }                                                                         \
    else                                                                      \
      kCastingConditionExpression(var, type, data, char, {        \
        var = v2(*(u8 *)data, 0);                                             \
      }) else kCastingConditionExpression(var, type, data, i8, {  \
        var = v2(*(i8 *)data, 0);                                             \
      }) else kCastingConditionExpression(var, type, data, u8, {  \
        var = v2(*(u8 *)data, 0);                                             \
      }) else kCastingConditionExpression(var, type, data, i16, { \
        var = v2(*(i16 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, u16, { \
        var = v2(*(u16 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, i32, { \
        var = v2(*(i32 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, u32, { \
        var = v2(*(u32 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, f32, { \
        var = v2(*(f32 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, i64, { \
        var = v2(*(i64 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, u64, { \
        var = v2(*(u64 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, f64, { \
        var = v2(*(f64 *)data, 0);                                            \
      }) else if (type == kTYPE_PTR)                              \
      {                                                                       \
        var = **(v2 **)data;                                                  \
      }                                                                       \
    else kCastingConditionExpression(var, type, data, v2, {       \
      var = *(v2 *)data;                                                      \
    }) else kCastingConditionExpression(var, type, data, v3, {    \
      var = toV2(*(v3 *)data);                                                \
    }) else kCastingConditionExpression(var, type, data, v4, {    \
      var = toV2(*(v4 *)data);                                                \
    }) else kCastingConditionExpression(var, type, data, iv2, {   \
      var = toV2(*(iv2 *)data);                                               \
    }) else kCastingConditionExpression(var, type, data, iv3, {   \
      var = toV2(*(iv3 *)data);                                               \
    }) else kCastingConditionExpression(var, type, data, iv4, {   \
      var = toV2(*(iv4 *)data);                                               \
    })                                                                        \
  }
#define kCastType_v3(var, data, type)                                                      \
  {                                                                                        \
    if (type == kTYPE_bool)                                                    \
    {                                                                                      \
      var = v3((*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0); \
    }                                                                                      \
    else                                                                                   \
      kCastingConditionExpression(var, type, data, char, {                     \
        var = v3(*(u8 *)data, 0, 0);                                                       \
      }) else kCastingConditionExpression(var, type, data, i8, {               \
        var = v3(*(i8 *)data, 0, 0);                                                       \
      }) else kCastingConditionExpression(var, type, data, u8, {               \
        var = v3(*(u8 *)data, 0, 0);                                                       \
      }) else kCastingConditionExpression(var, type, data, i16, {              \
        var = v3(*(i16 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, u16, {              \
        var = v3(*(u16 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, i32, {              \
        var = v3(*(i32 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, u32, {              \
        var = v3(*(u32 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, f32, {              \
        var = v3(*(f32 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, i64, {              \
        var = v3(*(i64 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, u64, {              \
        var = v3(*(u64 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, f64, {              \
        var = v3(*(f64 *)data, 0, 0);                                                      \
      }) else if (type == kTYPE_PTR)                                           \
      {                                                                                    \
        var = **(v3 **)data;                                                               \
      }                                                                                    \
    else kCastingConditionExpression(var, type, data, v2, {                    \
      var = v3((*(v2 *)data).x, (*(v2 *)data).y, 0);                                       \
    }) else kCastingConditionExpression(var, type, data, v3, {                 \
      var = *(v3 *)data;                                                                   \
    }) else kCastingConditionExpression(var, type, data, v4, {                 \
      var = toV3(*(v4 *)data);                                                             \
    }) else kCastingConditionExpression(var, type, data, iv2, {                \
      var = v3((*(iv2 *)data).x, (*(iv2 *)data).y, 0);                                     \
    }) else kCastingConditionExpression(var, type, data, iv3, {                \
      var = toV3(*(iv3 *)data);                                                            \
    }) else kCastingConditionExpression(var, type, data, iv4, {                \
      var = toV3(*(iv4 *)data);                                                            \
    })                                                                                     \
  }
#define kCastType_v4(var, data, type)                                                                               \
  {                                                                                                                 \
    if (type == kTYPE_bool)                                                                             \
    {                                                                                                               \
      var = v4((*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0); \
    }                                                                                                               \
    else                                                                                                            \
      kCastingConditionExpression(var, type, data, char, {                                              \
        var = v4(*(u8 *)data, 0, 0, 0);                                                                             \
      }) else kCastingConditionExpression(var, type, data, i8, {                                        \
        var = v4(*(i8 *)data, 0, 0, 0);                                                                             \
      }) else kCastingConditionExpression(var, type, data, u8, {                                        \
        var = v4(*(u8 *)data, 0, 0, 0);                                                                             \
      }) else kCastingConditionExpression(var, type, data, i16, {                                       \
        var = v4(*(i16 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, u16, {                                       \
        var = v4(*(u16 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, i32, {                                       \
        var = v4(*(i32 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, u32, {                                       \
        var = v4(*(u32 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, f32, {                                       \
        var = v4(*(f32 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, i64, {                                       \
        var = v4(*(i64 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, u64, {                                       \
        var = v4(*(u64 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, f64, {                                       \
        var = v4(*(f64 *)data, 0, 0, 0);                                                                            \
      }) else if (type == kTYPE_PTR)                                                                    \
      {                                                                                                             \
        var = **(v4 **)data;                                                                                        \
      }                                                                                                             \
    else kCastingConditionExpression(var, type, data, v2, {                                             \
      var = v4((*(v2 *)data).x, (*(v2 *)data).y, 0, 0);                                                             \
    }) else kCastingConditionExpression(var, type, data, v3, {                                          \
      var = v4((*(v3 *)data).x, (*(v3 *)data).y, (*(v3 *)data).z, 0);                                               \
    }) else kCastingConditionExpression(var, type, data, v4, {                                          \
      var = *(v4 *)data;                                                                                            \
    }) else kCastingConditionExpression(var, type, data, iv2, {                                         \
      var = v4((*(iv2 *)data).x, (*(iv2 *)data).y, 0, 0);                                                           \
    }) else kCastingConditionExpression(var, type, data, iv3, {                                         \
      var = v4((*(iv3 *)data).x, (*(iv3 *)data).y, (*(iv3 *)data).z, 0);                                            \
    }) else kCastingConditionExpression(var, type, data, iv4, {                                         \
      var = toV4(*(iv4 *)data);                                                                                     \
    })                                                                                                              \
  }
#define kCastType_iv2(var, data, type)                                        \
  {                                                                           \
    if (type == kTYPE_bool)                                       \
    {                                                                         \
      var = iv2((*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0);            \
    }                                                                         \
    else                                                                      \
      kCastingConditionExpression(var, type, data, char, {        \
        var = iv2(*(u8 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, i8, {  \
        var = iv2(*(i8 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, u8, {  \
        var = iv2(*(u8 *)data, 0);                                            \
      }) else kCastingConditionExpression(var, type, data, i16, { \
        var = iv2(*(i16 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, u16, { \
        var = iv2(*(u16 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, i32, { \
        var = iv2(*(i32 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, u32, { \
        var = iv2(*(u32 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, f32, { \
        var = iv2(*(f32 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, i64, { \
        var = iv2(*(i64 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, u64, { \
        var = iv2(*(u64 *)data, 0);                                           \
      }) else kCastingConditionExpression(var, type, data, f64, { \
        var = iv2(*(f64 *)data, 0);                                           \
      }) else if (type == kTYPE_PTR)                              \
      {                                                                       \
        var = **(iv2 **)data;                                                 \
      }                                                                       \
    else kCastingConditionExpression(var, type, data, v2, {       \
      var = toIV2(*(v2 *)data);                                               \
    }) else kCastingConditionExpression(var, type, data, v3, {    \
      var = toIV2(*(v3 *)data);                                               \
    }) else kCastingConditionExpression(var, type, data, v4, {    \
      var = toIV2(*(v4 *)data);                                               \
    }) else kCastingConditionExpression(var, type, data, iv2, {   \
      var = *(iv2 *)data;                                                     \
    }) else kCastingConditionExpression(var, type, data, iv3, {   \
      var = toIV2(*(iv3 *)data);                                              \
    }) else kCastingConditionExpression(var, type, data, iv4, {   \
      var = toIV2(*(iv4 *)data);                                              \
    })                                                                        \
  }
#define kCastType_iv3(var, data, type)                                                      \
  {                                                                                         \
    if (type == kTYPE_bool)                                                     \
    {                                                                                       \
      var = iv3((*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0); \
    }                                                                                       \
    else                                                                                    \
      kCastingConditionExpression(var, type, data, char, {                      \
        var = iv3(*(u8 *)data, 0, 0);                                                       \
      }) else kCastingConditionExpression(var, type, data, i8, {                \
        var = iv3(*(i8 *)data, 0, 0);                                                       \
      }) else kCastingConditionExpression(var, type, data, u8, {                \
        var = iv3(*(u8 *)data, 0, 0);                                                       \
      }) else kCastingConditionExpression(var, type, data, i16, {               \
        var = iv3(*(i16 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, u16, {               \
        var = iv3(*(u16 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, i32, {               \
        var = iv3(*(i32 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, u32, {               \
        var = iv3(*(u32 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, f32, {               \
        var = iv3(*(f32 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, i64, {               \
        var = iv3(*(i64 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, u64, {               \
        var = iv3(*(u64 *)data, 0, 0);                                                      \
      }) else kCastingConditionExpression(var, type, data, f64, {               \
        var = iv3(*(f64 *)data, 0, 0);                                                      \
      }) else if (type == kTYPE_PTR)                                            \
      {                                                                                     \
        var = **(iv3 **)data;                                                               \
      }                                                                                     \
    else kCastingConditionExpression(var, type, data, v2, {                     \
      var = iv3((*(v2 *)data).x, (*(v2 *)data).y, 0);                                       \
    }) else kCastingConditionExpression(var, type, data, v3, {                  \
      var = toIV3(*(v3 *)data);                                                             \
    }) else kCastingConditionExpression(var, type, data, v4, {                  \
      var = toIV3(*(v4 *)data);                                                             \
    }) else kCastingConditionExpression(var, type, data, iv2, {                 \
      var = iv3((*(iv2 *)data).x, (*(iv2 *)data).y, 0);                                     \
    }) else kCastingConditionExpression(var, type, data, iv3, {                 \
      var = *(iv3 *)data;                                                                   \
    }) else kCastingConditionExpression(var, type, data, iv4, {                 \
      var = toIV3(*(iv4 *)data);                                                            \
    })                                                                                      \
  }
#define kCastType_iv4(var, data, type)                                                                               \
  {                                                                                                                  \
    if (type == kTYPE_bool)                                                                              \
    {                                                                                                                \
      var = iv4((*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0, (*(bool *)data) ? 1 : 0); \
    }                                                                                                                \
    else                                                                                                             \
      kCastingConditionExpression(var, type, data, char, {                                               \
        var = iv4(*(u8 *)data, 0, 0, 0);                                                                             \
      }) else kCastingConditionExpression(var, type, data, i8, {                                         \
        var = iv4(*(i8 *)data, 0, 0, 0);                                                                             \
      }) else kCastingConditionExpression(var, type, data, u8, {                                         \
        var = iv4(*(u8 *)data, 0, 0, 0);                                                                             \
      }) else kCastingConditionExpression(var, type, data, i16, {                                        \
        var = iv4(*(i16 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, u16, {                                        \
        var = iv4(*(u16 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, i32, {                                        \
        var = iv4(*(i32 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, u32, {                                        \
        var = iv4(*(u32 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, f32, {                                        \
        var = iv4(*(f32 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, i64, {                                        \
        var = iv4(*(i64 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, u64, {                                        \
        var = iv4(*(u64 *)data, 0, 0, 0);                                                                            \
      }) else kCastingConditionExpression(var, type, data, f64, {                                        \
        var = iv4(*(f64 *)data, 0, 0, 0);                                                                            \
      }) else if (type == kTYPE_PTR)                                                                     \
      {                                                                                                              \
        var = **(iv4 **)data;                                                                                        \
      }                                                                                                              \
    else kCastingConditionExpression(var, type, data, v2, {                                              \
      var = iv4((*(v2 *)data).x, (*(v2 *)data).y, 0, 0);                                                             \
    }) else kCastingConditionExpression(var, type, data, v3, {                                           \
      var = iv4((*(v3 *)data).x, (*(v3 *)data).y, (*(v3 *)data).z, 0);                                               \
    }) else kCastingConditionExpression(var, type, data, v4, {                                           \
      var = toIV4(*(v4 *)data);                                                                                      \
    }) else kCastingConditionExpression(var, type, data, iv2, {                                          \
      var = iv4((*(iv2 *)data).x, (*(iv2 *)data).y, 0, 0);                                                           \
    }) else kCastingConditionExpression(var, type, data, iv3, {                                          \
      var = iv4((*(iv3 *)data).x, (*(iv3 *)data).y, (*(iv3 *)data).z, 0);                                            \
    }) else kCastingConditionExpression(var, type, data, iv4, {                                          \
      var = *(iv4 *)data;                                                                                            \
    })                                                                                                               \
  }

#define kCastType(varType, var, data, type) kCastType_##varType(var, data, type);
#define kCastTypeCreate(varType, var, data, type) \
  varType var;\
  kCastType(varType,var,data,type);\