// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#ifndef SCOPE_OUTPUT_HPP
#define SCOPE_OUTPUT_HPP


class Output {
public:

  virtual void println(const char* message);
  virtual void report(const char* parameter, long value);
  virtual void binary(size_t len);
  virtual void text();
  virtual void write(const char* data, size_t len);

};


#endif //SCOPE_OUTPUT_HPP
