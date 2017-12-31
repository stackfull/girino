// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
#include <Arduino.h>
#include "Scope.hpp"
#include "Control.hpp"

#ifndef UNIT_TEST

namespace {
Output output;
AnalogInput adc;
Scope scope(adc, output);
Control control(output, scope);
}

void setup (void) {
	control.setup();
  scope.setup();

  control.ready();
}

void loop (void) {
  scope.loop();
  control.poll();
}

#endif
