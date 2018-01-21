// Copyright (c) 2018 Paul Thomas.
//
// This file is part of stackfull/scope.
//
#ifndef SCOPE_RUNNINGAVG_HPP
#define SCOPE_RUNNINGAVG_HPP

class RunningAvg {
public:
  explicit RunningAvg(double a)
      :_a(a), _ainv(1.0-a), _mean(0.0L), _meansq(0.0L) {
  }
  void sample(double x) {
    _mean = _ainv * _mean + _a * x;
    _meansq = _ainv * _meansq + _a * x * x;
  }
  double mean() { return _mean; }
  double variance() { return _meansq - _mean * _mean; }
  double standard_deviation() { return sqrt(variance()); }
private:
  float _a;
  float _ainv;
  double _mean;
  double _meansq;
};

#endif //SCOPE_RUNNINGAVG_HPP
