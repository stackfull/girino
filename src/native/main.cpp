// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#include "AnalogInput.hpp"
#include "Control.hpp"
#include "CopyOutput.hpp"
#include "debug.hpp"

#include <iostream>

#if DEBUG == 1

void debugPrint(const char* message) {
  std::cout << message << std::endl;
}

template<typename T>
void printKV(const char* key, T value) {
  std::cout << "# " << key << ": " << value << std::endl;
}

void debugValue(const char* key, long value) {
  printKV(key, value);
}

void debugValue(const char* key, const char* value) {
  printKV(key, value);
}


#endif

#include <ctype.h>
#include <stdio.h>

void hexdump(const char *buf, int buflen) {
  int i, j;
  for (i=0; i<buflen; i+=16) {
    printf("%06x: ", i);
    for (j=0; j<16; j++)
      if (i+j < buflen)
        printf("%02x ", buf[i+j]);
      else
        printf("   ");
    printf(" ");
    for (j=0; j<16; j++)
      if (i+j < buflen)
        printf("%c", isprint(buf[i+j]) ? buf[i+j] : '.');
    printf("\n");
  }
}

void AnalogInput::setup() {}

void AnalogInput::prescaler(int t) { }

void AnalogInput::start(Callback &cb) {}

void AnalogInput::stop() {}

void Control::setup() {}
Control::Control(Output &output, Scope &scope) :_output(output), _scope(scope) {}
void Control::report() {}
void Control::poll() {}
void Control::ready() {
  std::cout << "Girino Ready" << std::endl;
}

#ifndef UNIT_TEST

int main( int argc, char **argv) {

  Output output;
  AnalogInput adc;
  Scope scope(adc, output);
  Control control(output, scope);

  control.setup();
  scope.setup();

  control.report();
  control.ready();

  while(1) {
    scope.loop();
    control.poll();
  }
}

#endif
