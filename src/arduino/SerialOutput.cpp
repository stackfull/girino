// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#include <Arduino.h>
#include "SerialOutput.hpp"

void SerialOutput::println(const char* message) {
  Serial.println(message);
}

void SerialOutput::binary(size_t len) {
  Serial.print("L");
  Serial.println(len);
}

void SerialOutput::text() {
  Serial.flush();
}

void SerialOutput::write(const char *data, size_t len) {
  Serial.write(data, len);
}

void SerialOutput::report(const char *parameter, long value) {
  Serial.print(parameter);
  Serial.print(": ");
  Serial.println(value);
}

