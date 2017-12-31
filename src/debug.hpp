// Copyright (c) 2017 Paul Thomas.
//
// This file is part of stackfull/scope.
//
// This file contains code originally from Girino
//   https://github.com/supacyan/girino.
// Copyright 2012 Cristiano Lino Fontana.
//

#ifndef SCOPE_DEBUG_HPP
#define SCOPE_DEBUG_HPP

#define DEBUG		0

#if DEBUG == 1
extern void debugPrint(const char* message);
extern void debugValue(const char* key, long value);
extern void debugValue(const char* key, const char* value);
#define dv(expression) debugValue( #expression, expression )
#define dp(expression) debugPrint( expression )
#else
#define dv(expression)
#define dp(expression)
#endif

#endif //SCOPE_DEBUG_HPP
