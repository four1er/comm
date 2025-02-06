#pragma once

#include <cstdint>
enum class EMessageType : uint32_t {
  NONE = 0,
  LOG_ENTRY = 1,
  REQUEST_VOTE_REQUEST = 2,       // request vote request
  REQUEST_VOTE_RESPONSE = 3,      // request vote response
  APPEND_ENTRIES_REQUEST = 4,     // append entries request
  APPEND_ENTRIES_RESPONSE = 5,    // append entries response
  INSTALL_SNAPSHOT_REQUEST = 6,   // TODO: not implemented it
  INSTALL_SNAPSHOT_RESPONSE = 7,  // TODO: not implemented it
  COMMAND_REQUEST = 8,            // command request
  COMMAND_RESPONSE = 9,           // command response
};

struct TMessage {
  static constexpr EMessageType MessageType = EMessageType::NONE;
  uint32_t Type;
  uint32_t Len;
  char Value[0];
};

static_assert(sizeof(TMessage) == 8);

struct TLogEntry : public TMessage {};

struct TMessageEX : public TMessage {};

template <typename T>
class TMessageHolder {};
