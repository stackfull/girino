// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#ifndef SCOPE_SAMPLE_BUFFER_HPP
#define SCOPE_SAMPLE_BUFFER_HPP

#include <stddef.h>
#include "Output.hpp"
#include "debug.hpp"


template<typename T, size_t SIZE> class SampleBuffer {
public:
  static const size_t size = SIZE;
  typedef T sample_t;
  enum State {
    FREE_RUNNING,
    BUFFERING,
    FULL
  };

  SampleBuffer() {
      reset();
  }

  bool free_running() { return _state == FREE_RUNNING; };
  bool full() { return _state == FULL; };
  bool buffering() { return _state == BUFFERING; };

  void reset() {
    _ptr = _count = _end_mark = 0;
    _state = FREE_RUNNING;
  }

  void sweep(size_t sweep_length) {
    _end_mark = circular(_ptr + sweep_length);
    _state = BUFFERING;
  }

  void feed(T sample);
  void write(Output& output);

private:
  T _buffer[SIZE];
  size_t _ptr;
  size_t _count;
  size_t _end_mark;
  State _state;

  size_t circular(size_t idx) { return idx % SIZE; }
};



template<typename T, size_t SIZE>
void SampleBuffer<T, SIZE>::feed(T sample) {
  if (full()) return;

  _buffer[_ptr] = sample;
  _ptr = circular(_ptr + 1);
  if (_count < SIZE) {
    _count = _count + 1;
  }

  if (buffering()) {
    if (_ptr == _end_mark) {
      _state = FULL;
    }
  }
}

template<typename T, size_t SIZE>
void SampleBuffer<T, SIZE>::write(Output& output) {

  auto * cast = reinterpret_cast<char*>(_buffer);
  dv(_count);
  dv(_ptr);
  dv(_end_mark);
  dv(_state);
  output.binary(_count);
  if (_count < SIZE) {
    // Incomplete trace - find the beginning using _count
    size_t start = circular(_ptr + SIZE - _count);
    dv(start);
    if (start > _ptr) {
      output.write(cast + start, SIZE - start);
      output.write(cast, _ptr);
    } else {
      output.write(cast + start, _count);
    }
  } else {
    // Simplest case of a full buffer - write out full circular buffer in 2 steps.
    output.write(cast + _ptr, SIZE - _ptr);
    output.write(cast, _ptr);
  }
  output.text();
}


#endif //SCOPE_SAMPLE_BUFFER_HPP
