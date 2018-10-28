/*   This sketch allows you to emulate multiple Somfy RTS or Simu HZ remotes.
   If you want to learn more about the Somfy RTS protocol, check out https://pushstack.wordpress.com/somfy-rts-protocol/
   
   The rolling code will be stored in EEPROM, so that you can power the Arduino off.
   
   Easiest way to make it work for you:
    - Choose a remote name
    - Choose a remote number
    - Choose a starting point for the rolling code. Any unsigned int works, 1 is a good start
    - Choose the used module
    - Upload the sketch
    - Long-press the program button of YOUR ACTUAL REMOTE until your blind goes up and down slightly
    - send 'yourRemoteName/p' to the serial terminal
  To make a group command, just repeat the last two steps with another blind (one by one)
  
  Then:
    - U will make it go up
    - D will make it go down
    - M for MY command
    - P for PROGRAM command
*/

#include <Arduino.h>
#include <Somfy_Remote.h>
#include <EEPROM.h>

#define EEPROM_SIZE 64

// Number of remotes to store
const uint remoteCount = 2;

// Array storing the multiple remotes
SomfyRemote remotes[remoteCount] = {
    SomfyRemote("remote1", 0x131171, 1, 2), // <- Change remote name, remote code, rolling code and module here!
    SomfyRemote("remote2", 0x089501, 1, 2)  // <- Change remote name, remote code, rolling code and module here!
};

void setup()
{
    // Setup Serial and EEPROM
    Serial.begin(115200);
    EEPROM.begin(EEPROM_SIZE);
}

void loop()
{
    // check if input is available
    if (Serial.available() > 0)
    {
        // Get string from serial input and divide it into remote name and command 
        String serialInput = Serial.readString();
        uint divider = serialInput.indexOf("/");
        String remoteName = serialInput.substring(0, divider);
        char command = ((serialInput.substring(divider+1)).c_str())[0];

        // Send the command via the corresponding remote
        for (int i = 0; i < remoteCount; i++)
        {
            if (remotes[i].getName() == remoteName)
            {
                remotes[i].move(command);
            }
        }
        
        // Commit the rolling codes
        EEPROM.commit();
    }
}