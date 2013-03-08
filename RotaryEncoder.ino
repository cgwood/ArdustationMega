// -*- Mode: C++; c-basic-offset: 8; indent-tabs-mode: nil -*-
//-
// Copyright (c) 2010 Michael Smith. All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.

/// @file       RotaryEncoder.pde
/// @brief      rotary encoder class

RotaryEncoder::RotaryEncoder(int pinA, int pinB, int gndPin) :
_pinA(pinA),
_pinB(pinB)
{
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, HIGH);
  if (-1 != gndPin) { 
    pinMode(gndPin, OUTPUT);
    digitalWrite(gndPin, LOW);
  }
}

void
RotaryEncoder::configure(volatile int *variable, int upperBound, int lowerBound, int prescaler, void (* callback)(int value))
{
  // save parameters
  _variable = variable;
  _upperBound = upperBound;
  _lowerBound = lowerBound;
  _prescaler = prescaler;
  _callback = callback;

  // initialise
  _accumulator = 0;
  _variable_old = *_variable;
  _prevPins = (digitalRead(_pinA) ? 2 : 0) | (digitalRead(_pinB) ? 1 : 0);


  // if we have no variable but we have a callback, use our private variable
  if ((NULL == _variable) && (NULL != _callback))
    _variable = &_value;
}

uint8_t
RotaryEncoder::haschanged(void)
{
  if (_variable_old != *_variable)
  {
    _variable_old = *_variable;
    return 1;//Page::ENCODER;
  }
  else
  {
    return 0;
  }
}

void
RotaryEncoder::update(void)
{
  uint8_t pa1, pa2, pb1, pb2;
  uint8_t pins, index;

  // rudimentary debounce
  do {
    pa1 = digitalRead(_pinA);
    pb1 = digitalRead(_pinB);
    pa2 = digitalRead(_pinA);
    pb2 = digitalRead(_pinB);
  } 
  while ((pa1 != pa2) || (pb1 != pb2));

  // compute LUT and save new pin values
  pins = (pa1 ? 2 : 0) | (pb1 ? 1 : 0);
  index = (pins << 2) | _prevPins;
  _prevPins = pins;
  //        Serial.print("pins ");
  //        Serial.println((int)pins);


  // is the transition valid?
  if (_validCode & (1 << index)) {


    // what is the direction?
    if (_directionCode & (1 << index)) {
      _accumulator++;
    } 
    else {
      _accumulator--;
    }

    //                Serial.print("valid ");
    //                Serial.print((int)index);
    //                Serial.print(" pins ");
    //                Serial.print((int)pins);
    //                Serial.print(" accumulator ");
    //                Serial.println(_accumulator);
    //                Serial.print(" variable ");
    //                Serial.println(*_variable);

    // handle prescaling
    if (0 == (_prescaler - _accumulator)) {
      if (*_variable < _upperBound)
        (*_variable)++;
      _accumulator = 0;
    } 
    else if (0 == (-_prescaler - _accumulator)) {
      if (*_variable > _lowerBound)
        (*_variable)--;
      _accumulator = 0;
    } 
    else {
      // no change to value, nothing more to do
      return;
    }
    //                Serial.print(" variable ");
    //                Serial.println(*_variable);

    // handle callback
    if (NULL != _callback)
      _callback(*_variable);
  }
}

