/** \brief This little sketch is part of the Smart Footwell Light Project.
 *  It is supposed to run on a Digispark that is equipped with an IR receiver.
 *  
 *  The Digisparks role in this project is to receive IR signals from a remote, 
 *  find out to which button the signals belong and send the according button number 
 *  to the LED-Controller using the non-interrupt-based JBus protocol.
 *  
 *  To see the whole project head over to \link Instructables http://www.instructables.com/member/Basement+Engineering/ \endlink
 *  or \link Youtube https://www.youtube.com/channel/UCOITWYehgOXzxSZx-iEaULQ \endlink
 *  
 * \version 1.0 \date 10.04.2018
 * \author J. Kettler aka. Basement Engineering
 */

/** \brief Button signals for a simple 24 Button IR LED Remote as well as the repeat signal.
 */
const uint32_t BUTTON_SIGNALS[25] =
{
  //first row
  0xF700FF, // -> 1. UP 
  0xF7807F, // -> 2. DOWN
  0xF740BF, // -> 3. OFF
  0xF7C03F, // -> 4. ON
  //second row
  0xF720DF, // -> 5. Red
  0xF7A05F, // -> 6. Green
  0xF7609F, // -> 7. Blue
  0xF7E01F, // -> 8. White
  //third row
  0xF710EF, // -> 9. Red1
  0xF7906F, // -> 10. Green1
  0xF750AF, // -> 11. Blue1
  0xF7D02F, // -> 12. Flash
  //fifth row
  0xF730CF, // -> 13. Red2
  0xF7B04F, // -> 14. Green2
  0xF7708F, // -> 15. Blue2
  0xF7F00F, // -> 16. Strobe
  //sixth row
  0xF708F7, // -> 17. Red3
  0xF78877, // -> 18. Green3
  0xF748B7, // -> 19. Blue3
  0xF7C837, // -> 20. Fade
  //seventh row
  0xF728D7, // -> 21. Red4
  0xF7A857, // -> 22. Green4
  0xF76897, // -> 23. Blue4
  0xF7E817, // -> 24. Smooth

  0xFFFFFFFF, // -> 25. Repeat Message
};


/** \brief Holds the durations of the last IR Signals HIGH Pulses in microseconds.
 *  
 *  This is only used for debugging and can be printed via Digikeyboard using the printRawBuffer function.
 */
unsigned long rawBuffer[32];

//Pin declarations

/**The Pins 0 to 2 where choosen to still be able to use USB Debugging.
 * To use Pin 1 you have to remove the onboard LED from the Digispark.
 
const int REQUEST_PIN = 0;
const int DATA_PIN = 1; //Important!!! Remove the onboard LED to use this pin.
const int RECEIVER_PIN = 2;

//#define DIGIKEYBOARD_DEBUG //Uncomment this line if you want to receive debugging code via DigiKeyboard

#ifdef DIGIKEYBOARD_DEBUG
#include "DigiKeyboard.h"
#endif
*/

const int REQUEST_PIN = 3;
const int DATA_PIN = 4; 
const int RECEIVER_PIN = 2;

#include <JBus.h>

JBus jBus(REQUEST_PIN, DATA_PIN);

//functions
uint32_t getInput(void);
uint32_t decodeNECMessage(void);
byte toButtonNumber(uint32_t irInput);
bool isValidButton(byte buttonNumber);
void printRawBuffer(void);

void setup() {
pinMode(RECEIVER_PIN, INPUT);
  Serial.begin(9600);
  Serial.println("Ready!");
  #ifdef DIGIKEYBOARD_DEBUG
  DigiKeyboard.println("Digispark is ready");
  #endif
}

void loop() {

  uint32_t irSignal = getInput();

if(irSignal != 0)
{
  Serial.println(irSignal, HEX);
#ifdef DIGIKEYBOARD_DEBUG
    DigiKeyboard.println(irSignal, HEX);
    //printRawBuffer();
#endif

 byte buttonNumber = toButtonNumber(irSignal);

  if(isValidButton(buttonNumber))
  {
#ifdef DIGIKEYBOARD_DEBUG
    DigiKeyboard.println("It is a valid button");
    DigiKeyboard.print("Sending button number: ");
    DigiKeyboard.println((int)buttonNumber);
#endif

jBus.write(buttonNumber);
  }
}

}

/** \brief Waits until there is an input on the receiver pin and returns the decoded result.
 *  
 *  Returns 0 if the operation was unsuccessfull.
 */
uint32_t getInput(void) 
{
  while(pulseIn(RECEIVER_PIN, LOW) < 6000)
  {
    //wait
  }
  return decodeNECMessage();
}

/** \brief Decodes the 32 Bit message from the NEC coded receiver signal.
 *  
 *  Returns 0 if the operation was unsuccessfull.
 */
uint32_t decodeNECMessage(){
  uint32_t result = 0;

  for (int i = 0; i < 32; i++) {
      rawBuffer[i] = pulseIn(RECEIVER_PIN, HIGH,10000);

      if( (rawBuffer[i] > 1600) && (rawBuffer[i] < 1800) )
      {
          result = (result << 1) | 1;
      }
      else if( (rawBuffer[i] > 400) && (rawBuffer[i] < 600) )
        {
          result = (result << 1) | 0;
        }
       /*else if ( (rawBuffer[i] == 0) && (i == 0) ) //Repeat Signal (Only a start bit and no further message bits => pulseIn times out)
      {
        return 0xFFFFFFFF;
      }*/
      else
      {
        return 0;
      }
      }
     pulseIn(RECEIVER_PIN, HIGH,10000);

    return result;
}

/** \brief Compares the received input value with a list of known values for each button and returns the Button that fits the current signal.
 *  
 *  Returns 0 if the operation was unsuccessfull.
 */
byte toButtonNumber(uint32_t irInput)
{
  
 for(byte i = 0; i < 25; i++)
 { 
if(irInput == BUTTON_SIGNALS[i])
{
  return (i + 1);
}
 }
    return 0;
}

/** \brief An extra check to see if a valid Button was assigned.
 *  
 *  Returns false if the operation was unsuccessfull.
 */
 bool isValidButton(byte buttonNumber) 
{
  return (buttonNumber > 0 && buttonNumber <= 25);
}

/** \brief Prints out the RawBuffer using DigiKeyboard if debugging is enabled.
 */
void printRawBuffer(void)
{
 #ifdef DIGIKEYBOARD_DEBUG
  DigiKeyboard.print("The latest raw message is: ");
  DigiKeyboard.print("(");
   for (int i = 0; i < 32; i++) 
  {
    DigiKeyboard.print(rawBuffer[i]);
    DigiKeyboard.print("|");
  }
  DigiKeyboard.print(")");
  DigiKeyboard.println();
  #endif
}
