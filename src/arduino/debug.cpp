// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#include "debug.hpp"

#if DEBUG == 1

#include <Arduino.h>

void debugPrint(const char* message) {
  Serial.println( message );
}

template<typename T>
void printKV(const char* key, T value) {
  Serial.print("# ");
  Serial.print( key );
  Serial.print( ": " );
  Serial.println( value );
}

void debugValue(const char* key, long value) {
  printKV(key, value);
}

void debugValue(const char* key, const char* value) {
  printKV(key, value);
}

#endif
