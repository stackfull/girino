// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//

#include <Arduino.h>
#include "Output.hpp"

void Output::println(const char* message) {
  Serial.println(message);
}

void Output::binary(size_t len) {
  Serial.print("L");
  Serial.println(len);
}

void Output::text() {
  Serial.flush();
}

void Output::write(const char *data, size_t len) {
  Serial.write(data, len);
}

void Output::report(const char *parameter, long value) {
  Serial.print(parameter);
  Serial.print(": ");
  Serial.println(value);
}

