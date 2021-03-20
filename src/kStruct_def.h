#pragma once

#include "utils.h"

#define arg_def_max_length 4

typedef enum kTypes
{
  kTYPE_BOOL = 1 << 0,
  kTYPE_CHAR = 1 << 0,
  kTYPE_INT8 = 1 << 0,
  kTYPE_UINT8 = 1 << 0,
  kTYPE_INT16 = 1 << 1,
  kTYPE_UINT16 = 1 << 1,
  kTYPE_INT32 = 1 << 2,
  kTYPE_UINT32 = 1 << 2,
  kTYPE_FLOAT32 = 1 << 2,
  kTYPE_INT64 = 1 << 3,
  kTYPE_UINT64 = 1 << 3,
  kTYPE_FLOAT64 = 1 << 3,
  kTYPE_PTR = 1 << 3,
  kTYPE_v2 = (1 << 2) * 2,
  kTYPE_v3 = (1 << 2) * 3,
  kTYPE_v4 = (1 << 2) * 4,
  kTYPE_iv2 = (1 << 2) * 2,
  kTYPE_iv3 = (1 << 2) * 3,
  kTYPE_iv4 = (1 << 2) * 4,
} kTypes;
typedef struct kStruct_StructDef
{
  size_t length;
  kTypes args[arg_def_max_length];
} kStruct_StructDef;
static void *getElementOfObjectAtIndex(u32 index, kStruct_StructDef def, void *obj)
{
  u8 *ptr = (u8 *)obj;
  u32 distance;
  for (u32 i = 0; i < index; i++)
  {
    distance += def.args[i];
  }
  return ptr + distance;
}
