// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#include "Scope.hpp"


Scope::Scope(AnalogInput &adc, Output &output)
    :_adc(adc),
     _output(output),
     _leading(32),
     _trigger(TRIGGER_RISING),
     _threshold(127),
     _prescaler(128) {
}

void Scope::setup() {
  _sampler.reset();
  _adc.setup();
}

void Scope::receive(sample_t sample) {
  _sampler.feed(sample);
  if (_sampler.full()) {
    _adc.stop();
  } else if (!_sampler.buffering()) {
    if (triggers(sample)) {
      _sampler.sweep(SAMPLE_BUFFER_SIZE - _leading);
    }
  }

}

bool Scope::triggers(sample_t sample) {
  return sample > _threshold;
}

void Scope::loop() {
  if (_sampler.full()) {
    _sampler.write(_output);
    _sampler.reset();
  }
}

void Scope::run() {
  _sampler.reset();
  _adc.start(*this);
}

void Scope::stop() {
  _adc.stop();
}

void Scope::prescaler(int newValue) {
  _prescaler = newValue;
  _adc.prescaler(_prescaler);
}

void Scope::threshold(sample_t newValue) {
  _threshold = newValue;
}

void Scope::trigger(Trigger newValue) {
  _trigger = newValue;
}

void Scope::leading(size_t newValue) {
  _leading = newValue;
}

void Scope::report() {
  _output.report("Wait duration", _leading);
  _output.report("Prescaler", _prescaler);
  _output.report("Trigger event", _trigger);
  _output.report("Threshold",_threshold);
}

