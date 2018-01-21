// Copyright (c) 2018 Paul Thomas.
//
// This file is part of stackfull/scope.
//
#ifndef SCOPE_OLEDDISPLAY_HPP
#define SCOPE_OLEDDISPLAY_HPP

#include <U8g2lib.h>
#include <Output.hpp>
#include "RunningAvg.hpp"

class OLEDDisplay : public Output {
public:
  explicit OLEDDisplay(Output &next);

  void setup();
  void loop();
  void println(const char *message) override;
  void report(const char *parameter, long value) override;
  void binary(size_t len) override;
  void text() override;
  void write(const char *data, size_t len) override;
  void running() override;
  void stopped() override;
  void latest(int value) override;

private:
  Output& _next;
  U8G2_SSD1306_128X64_NONAME_1_HW_I2C _u8g2;
  RunningAvg _level;
  volatile int _counter;
  int _point;
};


#endif //SCOPE_OLEDDISPLAY_HPP
