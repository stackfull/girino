// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#include "Scope.hpp"


namespace {
const char *const PARAM_KEY_PRESCALER = "Prescaler";
const char *const PARAM_KEY_LEADING = "Wait duration";
const char *const PARAM_KEY_TRIGGER = "Trigger event";
const char *const PARAM_KEY_THRESHOLD = "Threshold";

const int DEFAULT_LEADING = 32;
const Trigger DEFAULT_TRIGGER = TRIGGER_RISING;
const int DEFAULT_THRESHOLD = 127;
const int DEFAULT_PRESCALER = 128;
}

Scope::Scope(AnalogInput &adc, Output &output)
    :_adc(adc),
     _output(output),
     _leading(DEFAULT_LEADING),
     _prescaler(DEFAULT_PRESCALER),
     _trigger(DEFAULT_TRIGGER),
     _threshold(DEFAULT_THRESHOLD) {
}

void Scope::setup() {
  _sampler.reset();
  _adc.setup();
}

void Scope::receive(sample_t sample) {
  if (_sampler.full()) {
    return;
  }
  _sampler.feed(sample);
  if (_sampler.free_running()) {
    if (triggers(sample)) {
      _sampler.sweep(SAMPLE_BUFFER_SIZE - _leading);
    }
  }
  _last = sample;
}

bool Scope::triggers(sample_t sample) {
  switch (_trigger) {
    case TRIGGER_TOGGLE:
      return (sample > _threshold) xor (_last > _threshold);
    case TRIGGER_FALLING:
      return sample < _threshold;
    case TRIGGER_RISING:
      return sample > _threshold;
  }
  return true;
}

void Scope::loop() {
  _output.latest(_last);
  if (_sampler.full()) {
    _sampler.write(_output);
    _sampler.reset();
  }
}

void Scope::run() {
  _sampler.reset();
  _adc.start(*this);
  _output.running();
}

void Scope::stop() {
  _adc.stop();
  _output.stopped();
}

void Scope::prescaler(int newValue) {
  _prescaler = newValue;
  _adc.prescaler(_prescaler);
  _output.report(PARAM_KEY_PRESCALER, _prescaler);
}

void Scope::threshold(sample_t newValue) {
  _threshold = newValue;
  _output.report(PARAM_KEY_THRESHOLD, _threshold);
}

void Scope::trigger(Trigger newValue) {
  _trigger = newValue;
  _output.report(PARAM_KEY_TRIGGER, _trigger);
}

void Scope::leading(size_t newValue) {
  _leading = newValue;
  _output.report(PARAM_KEY_LEADING, _leading);
}

void Scope::report() {
  _output.report(PARAM_KEY_LEADING, _leading);
  _output.report(PARAM_KEY_PRESCALER, _prescaler);
  _output.report(PARAM_KEY_TRIGGER, _trigger);
  _output.report(PARAM_KEY_THRESHOLD, _threshold);
}

