// Copyright (c) 2018 Paul Thomas.
//
// This file is part of stackfull/scope.
//
#include <Wire.h>
#include <binary.h>
#include "OLEDDisplay.hpp"

namespace {

#define SSD1306_ADDR                 B01111000
inline uint8_t iicAddr(uint8_t SA0) {
  return SSD1306_ADDR | SA0;
}

const int I2C_ADDR = iicAddr(0);

}


OLEDDisplay::OLEDDisplay(Output &next)
    : _next(next),
      _u8g2(U8G2_R0),
      _level(0.05){
}

void OLEDDisplay::setup() {
  _u8g2.setI2CAddress(I2C_ADDR);
  _u8g2.begin();
  _u8g2.firstPage();
  do {
    _u8g2.setFont(u8g2_font_chroma48medium8_8r);
    _u8g2.drawStr(10,40,"Hello World!");
  } while ( _u8g2.nextPage() );
}

void OLEDDisplay::loop() {
  String mean(_level.mean(), 2);
  String stdev(_level.standard_deviation(), 4);

  _u8g2.firstPage();
  do {
    _u8g2.setFont(u8g2_font_chroma48medium8_8r);
    _u8g2.drawStr(10,25,mean.c_str());
    _u8g2.drawStr(10,40,stdev.c_str());
  } while ( _u8g2.nextPage() );
}

void OLEDDisplay::println(const char *message) {
  _next.println(message);
}

void OLEDDisplay::report(const char *parameter, long value) {
  _next.report(parameter, value);
}

void OLEDDisplay::binary(size_t len) {
  _next.binary(len);
}

void OLEDDisplay::text() {
  _next.text();
}

void OLEDDisplay::write(const char *data, size_t len) {
  _next.write(data, len);
}

void OLEDDisplay::running() {
  _next.running();
}

void OLEDDisplay::stopped() {
  _next.stopped();
}

void OLEDDisplay::latest(int value) {
  _level.sample(value);
  _counter++;
  _next.latest(value);
}



