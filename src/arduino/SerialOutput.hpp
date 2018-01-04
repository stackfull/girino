// Copyright (c) 2018 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#ifndef SCOPE_SERIALOUTPUT_HPP
#define SCOPE_SERIALOUTPUT_HPP

#include <Output.hpp>

class SerialOutput: public Output {
public:
  void println(const char *message) override;
  void report(const char *parameter, long value) override;
  void binary(size_t len) override;
  void text() override;
  void write(const char *data, size_t len) override;
  void running() override;
  void stopped() override;
  void latest(int value) override;
};

#endif //SCOPE_SERIALOUTPUT_HPP
