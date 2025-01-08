#pragma once

#define __USE_STDCPP11_TYPE_TRAITS__

#include <cstdio>
#include <cstdlib>

#define __THROW_BAD_ALLOC             \
  fprintf(stderr, "out of memory\n"); \
  exit(1)
