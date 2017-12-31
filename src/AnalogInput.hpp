// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#ifndef SCOPE_ANALOG_INPUT_HPP
#define SCOPE_ANALOG_INPUT_HPP

#include <stdint.h>

class AnalogInput {
public:

  struct Callback {
    virtual void receive(uint8_t sample) = 0;
  };

  void setup();
  void prescaler(int t);
  void start(Callback& cb);
  void stop();
};


#endif //SCOPE_ANALOG_INPUT_HPP
