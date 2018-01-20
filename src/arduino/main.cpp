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

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void setup (void) {
  control.setup();
  scope.setup();
  oled.setup();
  serial.report("free", freeRam());
  control.ready();
}

void loop (void) {
  scope.loop();
  control.poll();
  oled.loop();
}

#endif
