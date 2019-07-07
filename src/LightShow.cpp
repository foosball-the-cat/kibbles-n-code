/**
 * Author: Maria Kazandjieva, <mariakaz.phd@gmail.com>
 * Date: July 6, 2019
 * 
 * Turn the three button lights in sequence over time (L > M > R > M > L)
 * to produce a flashing effect across the Cleverpet hub.
 * 
 * This code does not include sound or the ability to respond to pad presses.
 * For demo purposes only.s
 */

#include <hackerpet.h>

// Set this to the name of your player (dog, cat, etc.)
const char playerName[] = "Foosball";
/**
 * Global variables and constants
 * ------------------------------
 */
const unsigned long SOUND_FOODTREAT_DELAY = 1200;  // (ms) delay for reward sound
const unsigned long LIGHT_TIMING = 1000;  // (ms) delay for reward sound

// access to hub functionality (lights, foodtreats, etc.)
HubInterface hub;

// enables simultaneous execution of application and system thread
SYSTEM_THREAD(ENABLED);

/**
 * Helper functions
 * ----------------
 */

/// This will be called on every iteration of the main loop.
bool flashLights() {
  yield_begin();

  // before starting interaction, wait until:
  //  1. device layer is ready (in a good state)
  //  2. foodmachine is "idle", meaning it is not spinning or dispensing and
  //  tray is retracted (see FOODMACHINE_... constants)
  //  3. no button is currently pressed
  yield_wait_for(
      (hub.IsReady() && hub.FoodmachineState() == hub.FOODMACHINE_IDLE &&
       not hub.AnyButtonPressed()),
      false);

  // DI reset occurs if, for example, device layer detects that touchpads need
  // re-calibration
  hub.SetDIResetLock(true);

  // play "reward" sound
  //hub.PlayAudio(hub.AUDIO_POSITIVE, 20);
  // give the Hub a moment to finish playing the reward sound
  yield_sleep_ms(SOUND_FOODTREAT_DELAY, false);

  hub.SetLights(hub.LIGHT_LEFT, 0, 99, 0);
  yield_sleep_ms(LIGHT_TIMING, false); 
  hub.SetLights(hub.LIGHT_MIDDLE, 0, 99, 0);
  yield_sleep_ms(LIGHT_TIMING, false); 
  hub.SetLights(hub.LIGHT_RIGHT, 0, 99, 0);
  yield_sleep_ms(LIGHT_TIMING, false);
  hub.SetLights(hub.LIGHT_RIGHT, 0, 0, 0);
  yield_sleep_ms(LIGHT_TIMING, false); 
  hub.SetLights(hub.LIGHT_MIDDLE, 0, 0, 0);
  yield_sleep_ms(LIGHT_TIMING, false); 
  hub.SetLights(hub.LIGHT_LEFT, 0, 0, 0);
  yield_sleep_ms(LIGHT_TIMING, false); 


  hub.SetDIResetLock(false);  // allow DI board to reset if needed between
                              // interactions
  yield_finish();
  return true;
}

/**
 * Setup function
 * --------------
 */
void setup() {
  hub.Initialize("Light Show");
}


void loop() {
  // Advance the device layer state machine, but with 20 ms max time
  // spent per loop cycle.
  hub.Run(20);

  // Play 1 level of the Eating The Food challenge
  flashLights();  // Will return true if level is done

}