// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
#include <Arduino.h>
#include "Scope.hpp"
#include "Control.hpp"
#include "SerialOutput.hpp"
#include "OLEDDisplay.hpp"

#ifndef UNIT_TEST

namespace {
SerialOutput serial;
OLEDDisplay oled(serial);
AnalogInput adc;
Scope scope(adc, oled);
Control control(serial, scope);

}

void setup (void) {
  control.setup();
  scope.setup();
  oled.setup();
  control.ready();
}

void loop (void) {
  scope.loop();
  control.poll();
  oled.loop();
}

#endif
