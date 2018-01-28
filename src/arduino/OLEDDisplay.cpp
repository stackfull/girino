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
  double level = _level.mean();
  double dev = _level.standard_deviation();
  auto lev_height = static_cast<u8g2_uint_t>((level * 32.0 / 256.0));
  auto err_height = static_cast<u8g2_uint_t>((dev * 32.0 / 256.0));
  double range = 5.0;
  double voltage = level * range / 256.0;

  String mean(voltage, 2);
  double err_pcnt = dev * 100.0 / 256.0;
  String stdev(err_pcnt, err_pcnt >= 10.0 ? 0 : 1);

  _u8g2.firstPage();
  do {
    _u8g2.setFont(u8g2_font_logisoso32_tn);
    _u8g2.drawStr(16,16+32,mean.c_str());
    _u8g2.setFont(u8g2_font_chroma48medium8_8r);
    _u8g2.drawStr(100,16+8,stdev.c_str());
    _u8g2.setFont(u8g2_font_logisoso16_tf);
    _u8g2.drawStr(100,16+32,"V");
    //_u8g2.drawFrame(0, 16, 4, 32);
    _u8g2.drawHLine(0, 16, 6);
    _u8g2.drawHLine(0, 16+32, 6);
    for (int mark = 1; mark < 5; ++mark) {
      _u8g2.drawPixel(0, 16 + 32 - (mark * 32 / 5));
    }
    //_u8g2.drawFrame(5, 16, 3, 32);
    _u8g2.drawBox(1, 16+32-lev_height, 2, lev_height);
    _u8g2.drawBox(4, 16+32-err_height, 1, err_height);

    _u8g2.setFont(u8g2_font_profont10_tf);
    _u8g2.drawStr(0,64,"t1.4^");
    _u8g2.drawFrame(40, 64 - 4, 48, 4);
    _u8g2.drawBox(40, 64 - 4, _samples * 40 / 512, 4);
    _u8g2.drawStr(90,64,"23.4ms");

  } while ( _u8g2.nextPage() );
}

void OLEDDisplay::println(const char *message) {
  _next.println(message);
}

void OLEDDisplay::report(const char *parameter, long value) {
  _next.report(parameter, value);
}

void OLEDDisplay::binary(size_t len) {
  _samples = len;
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



