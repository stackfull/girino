// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#ifndef SCOPE_CONTROL_HPP
#define SCOPE_CONTROL_HPP


#include "Scope.hpp"

class Control {

public:
  Control(Output& output, Scope& scope);

  void setup();
  void ready();
  void poll();
  void report();

private:
  Scope& _scope;
  Output& _output;

  int readArg(const char* name);
};


#endif //SCOPE_CONTROL_HPP
