#ifndef SETUP_R2B0XX_H_ZF4EWBIG
#define SETUP_R2B0XX_H_ZF4EWBIG

#include "pinout_r2b0xx.h"

#include "core/CommunicationBackend.h"
#include "core/InputMode.h"
#include "core/socd.h"
#include "core/state.h"
#include "comms/DInputBackend.h"
#include "comms/GamecubeBackend.h"
#include "comms/N64Backend.h"
#include "modes/Melee20Button.h"

CommunicationBackend *gCurrentBackend;
InputMode *gCurrentMode;
state::InputState gInputState;

void initialise() {
  pinMode(pinout::SWITCH, OUTPUT);

  // Hold Mod X + A on plugin for Brook board mode.
  if (gInputState.mod_x && gInputState.a)
    digitalWrite(pinout::SWITCH, LOW);
  else
    digitalWrite(pinout::SWITCH, HIGH);

  /* Choose communication backend. */
  if (gInputState.c_down) {
    // Hold C-Down on plugin for GameCube mode.
    gCurrentBackend = new GamecubeBackend(125, pinout::GCC_DATA);
  } else if (gInputState.c_up) {
    // Hold C-Up on plugin for official GameCube adapter mode.
    gCurrentBackend = new GamecubeBackend(0, pinout::GCC_DATA);
  } else if (gInputState.c_left) {
    // Hold C-Left on plugin for N64 mode.
    gCurrentBackend = new N64Backend(60, pinout::GCC_DATA);
  } else {
    // Default to DInput mode.
    gCurrentBackend = new DInputBackend();
    // Input viewer only used when connected to PC i.e. when using DInput mode.
    Serial.begin(115200, SERIAL_8N1);
  }

  /* Always start in Melee mode. Must set mode only after initialising the
     backend. */
  gCurrentMode =
      new Melee20Button(socd::SOCD_2IP_NO_REAC, gInputState, gCurrentBackend);
}

#endif /* end of include guard: SETUP_R2B0XX_H_ZF4EWBIG */