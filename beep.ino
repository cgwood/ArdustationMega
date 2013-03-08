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

/// @file       beep.pde
/// @brief      audio feedback

const Beep::note Beep::_tune[BEEP_TUNES_MAX][BEEP_TUNE_MAX] = {
        {{2300, 300}, {0, 0}},                          // BEEP_CRITICAL
        {{500, 100}, {0, 0}},                           // BEEP_NOTICE
        {{659, 100}, {659, 100}, {880, 300}, {0, 0}},   // BEEP_CONNECTED
        {{800, 100}, {800, 100}, {0, 0}},               // BEEP_WAYPOINT
        {{1200, 250}, {750, 250}, {300, 250}, {0, 0}},  // BEEP_LAND
        {{831, 100}, {880, 100}, {880, 100}, {0, 0}},   // BEEP_RTL
        {{300, 200}, {0, 0}},                           // BEEP_BADKEY
        {{1000, 50}, {0, 0}},                           // BEEP_KEY
        {{4000, 5}, {0, 0}},                            // BEEP_PACKET
        {{880, 100}, {831, 100}, {880, 100}, {0, 0}},   // BEEP_STARTUP
};

void
Beep::play(uint8_t code)
{
  // does the new tune have priority?
  if (code > _playing)
    return;

  //        // is the code muted?
  //        if ((code >= BEEP_MUTE_MIN) && nvram.nv.muted)
  //                return;

  // start the new tune
  _playing = code;
  _position = 0;
  _playNote();
}

void
Beep::stop(void)
{
  _playing = BEEP_NONE;
  noTone(_pin);
}

void
Beep::update(void)
{
  // if playing nothing, do nothing
  if (BEEP_NONE == _playing)
    return;

  // if the current note is done, play the next
  if ((millis() - _noteStarted) > _duration) {
    _position++;
    _playNote();
  }
}

void
Beep::_playNote(void)
{
  uint16_t        frequency;
  const prog_char *pp;

  // if we are still inside the tunt
  if (_position < BEEP_TUNE_MAX) {

    // get the frequency and duration
    frequency = _tune[_playing][_position].pitch;
    _duration = _tune[_playing][_position].duration;

    // if this isn't a terminal note
    if (0 < _duration) {

      // stop the previous tone and start the new tone
      noTone(_pin);
      tone(_pin, frequency, _duration);
      _noteStarted = millis();
      return;
    }
  }

  // tune has ended
  stop();
}


