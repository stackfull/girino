// Copyright (c) 2018 Paul Thomas.
//
// This file is part of stackfull/scope.
//
#ifndef SCOPE_COPYOUTPUT_HPP
#define SCOPE_COPYOUTPUT_HPP

#include <vector>
#include <stdint.h>
#include <Output.hpp>


class CopyOutput: public Output {

public:
  std::vector<char> captured;
  size_t len = SIZE_T_MAX;

  void println(const char* message) override {
  }
  void report(const char* parameter, long value) override {
  }
  void binary(size_t l) override {
    len = l;
  }
  void text() override {}
  void write(const char* data, size_t len) override {
    for (const char* ptr = data; ptr - data < len; ptr++) {
      captured.push_back(*ptr);
    }
  }
  void running() override {};
  void stopped() override {};
  void latest(int value) override {};
};



#endif //SCOPE_COPYOUTPUT_HPP
