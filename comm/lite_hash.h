#pragma once
#include <assert.h>

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
struct LRUHandle {
  void *value;
  void (*deleter)(const std::string, void *value);
  LRUHandle *next_hash;
  LRUHandle *next_;
  LRUHandle *prev;
  size_t charge;
  size_t key_length;
  bool in_cache;
  uint32_t refs;
  uint32_t hash;
  //   char key_data[1];
  std::string key;
};

class HandleTable {
 public:
  HandleTable() : length_(0), elems_(0), list_(nullptr) { Resize(); }
  ~HandleTable() { delete[] list_; };

  LRUHandle *Lookup(const std::string &key, uint32_t hash) {
    return *FindPointer(key, hash);
  }

  LRUHandle *Insert(LRUHandle *h) {
    LRUHandle **ptr = FindPointer(h->key, h->hash);
    LRUHandle *old = *ptr;
    h->next_hash = (old == nullptr ? nullptr : old->next_hash);
    *ptr = h;
    if (old == nullptr) {
      ++elems_;
      if (elems_ > length_) {
        Resize();
      }
    }
    return old;
  }

 private:
  uint32_t length_;
  uint32_t elems_;

  LRUHandle **list_;

  LRUHandle **FindPointer(const std::string &key, uint32_t hash) {
    LRUHandle **ptr = &list_[hash & (length_ - 1)];
    while (*ptr != nullptr && ((*ptr)->hash != hash || key != (*ptr)->key)) {
      ptr = &(*ptr)->next_hash;
    }
    return ptr;
  }

  void Resize() {
    uint32_t new_length = 4;
    while (new_length < elems_) {
      new_length *= 2;
    }
    LRUHandle **new_list = new LRUHandle *[new_length];
    memset(new_list, 0, sizeof(new_list[0]) * new_length);
    uint32_t count = 0;
    for (uint32_t i = 0; i < length_; i++) {
      LRUHandle *h = list_[i];
      while (h != nullptr) {
        LRUHandle *next = h->next_hash;
        uint32_t hash = h->hash;
        LRUHandle **ptr = &new_list[hash & (new_length - 1)];
        h->next_hash = *ptr;
        *ptr = h;
        h = next;
        count++;
      }
    }
    assert(count == elems_);
    delete[] list_;
    list_ = new_list;
    length_ = new_length;
  }
};
