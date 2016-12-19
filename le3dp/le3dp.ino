/* Simplified Logitech Extreme 3D Pro Joystick Report Parser 
 * Ejemplo modificado por SETISAEDU para el control de salidas digitales y servomotores
 */
#include <hid.h>
#include <hiduniversal.h>
#include <usbhub.h>


#include "le3dp_rptparser.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB                                             Usb;
USBHub                                          Hub(&Usb);
HIDUniversal                                    Hid(&Usb);
JoystickEvents                                  JoyEvents;
JoystickReportParser                            Joy(&JoyEvents);



void setup()
{
  
  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

  if (Usb.Init() == -1)
      Serial.println("OSC did not start.");
  //Declaracion de los pines de los LED como salidas
  for(int j=22;j<42;j++)
  {
    pinMode(j,OUTPUT);
  }
  delay( 200 );

  if (!Hid.SetReportParser(0, &Joy))
      ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1  );
}

void loop()
{
    Usb.Task();
}

