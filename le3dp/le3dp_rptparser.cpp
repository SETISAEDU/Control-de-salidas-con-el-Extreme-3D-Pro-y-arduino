#include "le3dp_rptparser.h"

JoystickReportParser::JoystickReportParser(JoystickEvents *evt) :
	joyEvents(evt)
{}

void JoystickReportParser::Parse(HID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf)
{
	bool match = true;

	// Checking if there are changes in report since the method was last called
	for (uint8_t i=0; i<RPT_GAMEPAD_LEN; i++) {
		if( buf[i] != oldPad[i] ) {
			match = false;
			break;
		}
  }
  	// Calling Game Pad event handler
	if (!match && joyEvents) {
		joyEvents->OnGamePadChanged((const GamePadEventData*)buf);

		for (uint8_t i=0; i<RPT_GAMEPAD_LEN; i++) oldPad[i] = buf[i];
	}
}

void JoystickEvents::OnGamePadChanged(const GamePadEventData *evt)
{
	Serial.print("X: ");
	PrintHex<uint16_t>(evt->x, 0x80);
	Serial.print(" Y: ");
	PrintHex<uint16_t>(evt->y, 0x80);
	Serial.print(" Hat Switch: ");
	PrintHex<uint8_t>(evt->hat, 0x80);
	Serial.print(" Twist: ");
	PrintHex<uint8_t>(evt->twist, 0x80);
	Serial.print(" Slider: ");
	PrintHex<uint8_t>(evt->slider, 0x80);
  Serial.print(" Buttons A: ");
	PrintHex<uint8_t>(evt->buttons_a, 0x80);
	Serial.print(" Buttons B: ");
	PrintHex<uint8_t>(evt->buttons_b, 0x80);
	Serial.println("");
 
 //Actualizacion de valores de las salidas
  
  servoX.attach(3);
  servoY.attach(5);//Configuracion de los pines de salida para cada servo
  servoS.attach(7);
  servoT.attach(9);
                                            
  short int mul[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; //Vector para generar la discriminacion
  //Impresion de los valores a los pines digitales
  for(int x=0;x<8;x++)
  {
    digitalWrite(22+x,evt->buttons_a&mul[x]);
    digitalWrite(34+x,evt->hat&mul[x]);
  }
  for(int x=0;x<4;x++)
  {
    digitalWrite(30+x,evt->buttons_b&mul[x]);
  }
  
  //Actualizacion de las posiciones de los servos
  servoX.write(map(evt->x,0,1023,15,180));
  servoY.write(map(evt->y,0,1023,15,180));
  servoS.write(map(evt->slider,0,255,15,180));
  servoT.write(map(evt->twist,255,0,15,180)); //Se invierte para que sea igual a el giro de la palanca

//  servoX.write(map(evt->x,1023,0,15,180));
//  servoY.write(map(evt->y,1023,0,15,180));
//  servoS.write(map(evt->slider,255,0,15,180));
//  servoT.write(map(evt->twist,255,0,15,180));
  
}
