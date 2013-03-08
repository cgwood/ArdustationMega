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

/// @file       beep.h
/// @brief      audio feedback

/// @name       Beep codes
//@{
#define BEEP_CRITICAL   0
#define BEEP_NOTICE     1
#define BEEP_CONNECTED  2
// 3-5 spare
#define BEEP_WAYPOINT	3
#define BEEP_LAND		4
#define BEEP_RTL		5
//
#define BEEP_BADKEY     6
#define BEEP_KEY        7
#define BEEP_PACKET     8
#define BEEP_STARTUP    9
//@}

#define BEEP_NONE       255     ///< Sentinel value in _playing to indicate no tune currently playing

#define BEEP_TUNES_MAX  10      ///< maximum number of tunes
#define BEEP_TUNE_MAX   4       ///< maximum notes in a tune

#define BEEP_MUTE_MIN   0       ///< codes less than this cannot be muted

class Beep {
public:
  Beep(uint8_t tonePin) : 
  _pin     (tonePin),
  _playing (BEEP_NONE) {
  };

  void            play(uint8_t code);
  void            stop(void);
  void            update(void);
private:
  void            _playNote();

  uint8_t         _pin;
  uint8_t         _playing;
  uint8_t         _position;
  unsigned long   _noteStarted;
  uint16_t        _duration;

  struct note {
    uint16_t        pitch;          ///< frequency for tone
    uint16_t        duration;       ///< duration of tone
  };
  static const struct note  _tune[BEEP_TUNES_MAX][BEEP_TUNE_MAX];
};


