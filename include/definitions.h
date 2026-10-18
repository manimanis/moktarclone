#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <stdint.h>
#include <unistd.h>

typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;

#ifndef __cplusplus
#if !defined(__bool_true_false_are_defined) && (!defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L)
#include <stdbool.h>
#endif
#endif

#endif
