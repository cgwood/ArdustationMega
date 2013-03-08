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

/// @file       RotaryEncoder.h
/// @brief      rotary encoder class

/// @class      RotaryEncoder
/// @brief      Driver for two-signal relative position rotary encoder
/// @discussion This driver supports low-cost quadrature encoders connected to
///             any two pins.  Optionally, the encoder's common may also be 
///             connected to a pin (this can help when exploring a new encoder).
///             The driver may be polled by calling the ::update method;
///             either periodically or from a pin change interrupt.
/// 
class RotaryEncoder {
public:
  RotaryEncoder(int pinA, int PinB, int gndPin = -1);

  /// Check the pins and update the variable as required
  ///
  void            update(void);

  /// Configure the driver to manage a variable.
  ///
  /// @param variable     The variable to be updated by the encoder.
  ///                     Note that the variable is not shadowed by
  ///                     the driver, it is updated based on the
  ///                     value it has at the time that ::update is
  ///                     called.  If this value is NULL, an internal
  ///                     variable is used and passed to the callback.
  /// @param upperBound   The driver will never increase the variable
  ///                     above this value.
  /// @param lowerBound   The driver will never decrease the variable
  ///                     below this value.
  /// @param prescaler    Scaling value for the encoder.  This many
  ///                     transitions from the encoder are required to
  ///                     cause the variable to change by one.  If the
  ///                     prescaler value is negative, the direction
  ///                     of rotation is reversed.
  /// @param callback     A function to call when the variable is
  ///                     changed by the driver.  If both the 
  ///                     variable and callback are NULL, the driver
  ///                     is disabled and will use very little CPU.
  ///
  void            configure(
  volatile int *variable, 
  int upperBound, 
  int lowerBound, 
  int prescaler = 1, 
  void (* callback)(int value) = NULL);
  uint8_t			haschanged(void);
private:
  volatile int    *_variable;                     ///< pointer to the current variable
  int             _prescaler;                     ///< prescaler
  int             _upperBound;                    ///< upper limit for variable
  int             _lowerBound;                    ///< lower limit for variable
  void            (* _callback)(int value);       ///< callback for variable change notification

  int             _accumulator;                   ///< fractional count
  int             _value;                         ///< internal scratch for _variable == NULL

  int             _pinA;                          ///< phase1 pin number
  int             _pinB;                          ///< phase2 pin number

  uint8_t         _prevPins;                      ///< previous pin state
  int             _variable_old;                  ///< For seeing if variable has changed

    // We use the previous and current state of the encoder to generate
  // a 4-bit address.  That address gives an offset into the _validCode
  // bitfield indicating whether this is a valid gray code transition
  // (thereby avoiding code skips) and into _directionCode indicating
  // whether the transition is positive or negative.
  //
  //  new old  valid direction
  // ------------------------
  //  0 0 0 0     0         -
  //  0 0 0 1     1         1
  //  0 0 1 0     1         0
  //  0 0 1 1     0 6       - 2
  //  0 1 0 0     1         0
  //  0 1 0 1     0         -
  //  0 1 1 0     0         -
  //  0 1 1 1     1 9       1 8
  //  1 0 0 0     1         1
  //  1 0 0 1     0         -
  //  1 0 1 0     0         -
  //  1 0 1 1     1 9       0 1
  //  1 1 0 0     0         -
  //  1 1 0 1     1         0
  //  1 1 1 0     1         1
  //  1 1 1 1     0 6       - 4
  //

  static const uint16_t _validCode     = 0x6996;  ///< valid/invalid gray codes
  static const uint16_t _directionCode = 0x4182;  ///< direction for valid transitions
};


