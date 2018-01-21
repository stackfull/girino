// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#ifndef SCOPE_SCOPE_HPP
#define SCOPE_SCOPE_HPP

#include <stdint.h>

#include "SampleBuffer.hpp"
#include "AnalogInput.hpp"
#include "Output.hpp"

#define SAMPLE_BUFFER_SIZE 128

enum Trigger {
  TRIGGER_TOGGLE = 1,
  TRIGGER_FALLING,
  TRIGGER_RISING,
};

class Scope :public AnalogInput::Callback {

public:
  typedef uint8_t sample_t;
  const sample_t MAX_SAMPLE = (sample_t)-1;

  Scope(AnalogInput &adc, Output &output);

  void setup();
  void run();
  void stop();
  void loop();
  void report();
  void receive(sample_t sample) override;
  void threshold(sample_t newValue);
  void trigger(Trigger newValue);
  void prescaler(int newValue);
  void leading(size_t newValue);

private:
  SampleBuffer<sample_t, SAMPLE_BUFFER_SIZE> _sampler;
  AnalogInput& _adc;
  Output& _output;
  size_t _leading;
  int _prescaler;
  Trigger _trigger;
  sample_t _threshold;
  volatile sample_t _last;
  bool _searching;

  bool triggers(sample_t sample);
};

#endif //SCOPE_SCOPE_HPP
