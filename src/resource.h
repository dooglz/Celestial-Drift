#pragma once
#include "log.h"
#include <string>
#include <unordered_map>
#include <vector>

template <class T> class Storage {
private:
  static std::unordered_map<std::string, T *> _container;
  static bool constructed;

public:
  Storage() {}

  // Manually Store an item, return false if key already stored.
  static bool Store(const std::string &name, T *item) {
    std::pair<typename std::unordered_map<std::string, T *>::iterator, bool> pr;
    pr = _container.insert(std::make_pair(name, item));
    return pr.second;
  }

  // Checks to see if key is already stored
  static bool Contains(const std::string &name) {
    typename std::unordered_map<std::string, T *>::iterator got = _container.find(name);
    if (got == _container.end()) {
      return false;
    } else {
      return true;
    }
  }

  static T *Get(const std::string &name) {
    // printf("Lookign for: %s ... ", name.c_str());
    typename std::unordered_map<std::string, T *>::iterator got = _container.find(name);
    if (got == _container.end()) {
      // Not loaded
      // printf("Not found, Loading \n");
      T *item = T::Load(name);
      _container.insert(std::make_pair(name, item));
      return item;
    } else {
      // printf("In warehouse\n");
      return got->second;
    }

    return NULL;
  }

  void _Shutdown() {
    typename std::unordered_map<std::string, T *>::iterator itt = _container.begin();
    while (itt != _container.end()) {
      delete itt->second;
      ++itt;
    }
    _container.clear();
  };
};

template <class T> std::unordered_map<std::string, T *> Storage<T>::_container;
template <class T> bool Storage<T>::constructed = false;