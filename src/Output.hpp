// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#ifndef SCOPE_OUTPUT_HPP
#define SCOPE_OUTPUT_HPP


class Output {
public:
  virtual void println(const char* message) = 0;
  virtual void report(const char* parameter, long value) = 0;
  virtual void binary(size_t len) = 0;
  virtual void text() = 0;
  virtual void write(const char* data, size_t len) = 0;
  virtual void running() = 0;
  virtual void stopped() = 0;
  virtual void latest(int value) = 0;
};


#endif //SCOPE_OUTPUT_HPP
