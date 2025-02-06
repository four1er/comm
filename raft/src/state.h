#pragma once
#include <cstdint>
#include <vector>

#include "message.h"

struct TState {
  uint64_t CurrentTerm = 1;
  uint32_t VotedFor = 0;
  std::vector<TMessageHolder<TLogEntry>> Log;
};
