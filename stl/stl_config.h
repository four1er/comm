#pragma once

#define __USE_STDCPP11_TYPE_TRAITS__

#include <cstdio>
#include <cstdlib>

#define __THROW_BAD_ALLOC             \
  fprintf(stderr, "out of memory\n"); \
  exit(1)

#define __STL_REQUIRES(__type_var, __concept) \
  do {                                        \
  } while (0)
