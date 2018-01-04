// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#include <Arduino.h>
#include "Control.hpp"

#define BAUDRATE	115200	// Baud rate of UART in bps
#define COMBUFFERSIZE	4	// Size of buffer for incoming numbers
#define COMMANDDELAY	20	// ms to wait for the filling of Serial buffer

//-----------------------------------------------------------------------------
// fillBuffer
//-----------------------------------------------------------------------------
// Fills the given buffer with bufferSize chars from a Serial object

namespace {

char commandBuffer[COMBUFFERSIZE+1];

void fillBuffer( char *buffer, int bufferSize, Stream* serial )
{
  // Clean buffer
  memset( (void *)buffer, 0, bufferSize );

  int limit = min( bufferSize, serial->available() );

  // Fill buffer
  for ( int i = 0; i < limit; i++ ) {
    buffer[i] = serial->read();
  }
}

}

Control::Control(Output& output, Scope& scope)
    :_scope(scope),
     _output(output){

}

void Control::setup() {
  Serial.begin(BAUDRATE);
  memset( (void *)commandBuffer, 0, sizeof(commandBuffer) );
}

void Control::ready() {
  _output.println("Girino ready");
}

void Control::poll() {

  if ( Serial.available() > 0 ) {
    int theChar = Serial.read();
    switch (theChar) {
      case 's':      // 's' for starting ADC conversions
        _scope.run();
        break;

      case 'S':      // 'S' for stopping ADC conversions
        _scope.stop();
        break;

      case 'p':      // 'p' for new prescaler setting
      case 'P':
        _scope.prescaler(readArg("prescaler"));
        break;

      case 'e':      // 'e' for new trigger event setting
      case 'E':
        _scope.trigger(static_cast<enum Trigger>(readArg("trigger event")));
        break;

      case 'w':      // 'w' for new wait setting
      case 'W':
        _scope.leading(readArg("waitDuration"));
        break;

      case 't':      // 'w' for new threshold setting
      case 'T':
        _scope.threshold(readArg("threshold"));
        break;

      case 'd':      // 'd' for display status
      case 'D':
        report();
        break;

      default:
        // Display error message
        _output.println((String(F("ERROR: Command not found, it was: ")) + theChar).c_str());
    }
  }
}

void Control::report() {
  _output.report("Buffer size", SAMPLE_BUFFER_SIZE);
  _output.report("Baud rate", BAUDRATE);
  _scope.report();
}

int Control::readArg(const char* name) {
  delay(COMMANDDELAY);
  fillBuffer(commandBuffer, COMBUFFERSIZE, &Serial);
  return atoi(commandBuffer);
}

