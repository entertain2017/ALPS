#include <Arduino.h>
#include <alps.h>
#include <Print.h>
#include <CapacitiveSensor.h>

int pwm_pin = 0;
int up_pin = 0;
int down_pin = 0;
int lastv = 255;
int cycle_val = 0;
int ecv = 0;
void touch(void);

static CapacitiveSensor cp = CapacitiveSensor(0,0);

int Alps::Read(int pin, int mode)
{
	int val = 0;
	if ( mode == 0 ) {
		// if mode 0 output as int raw adc
		return analogRead(pin);
	} 
	if ( mode == 1 ) {
		// if mode 1 output as int percent value
		val = analogRead(pin); 
		return (int)val/1023.0*100;
	}
	return 0;
};


void Alps::MotorBegin(int pwm, int up, int down, int cycle , long freq, int et = 300) 
{
	// set output pins for L293D PWM, UP, DOWN (FW,RW)
	pinMode(pwm,OUTPUT);
	pinMode(up,OUTPUT);
	pinMode(down,OUTPUT);
	analogWriteFrequency(pwm, freq);
	pwm_pin = pwm;
	up_pin = up;
	down_pin = down;
	cycle_val = cycle;
	ecv = et;

	
}

void Alps::MotorUp(){
	// set pins. Pot will move up
	digitalWrite(up_pin,HIGH);
	digitalWrite(down_pin,LOW);
	analogWrite(pwm_pin,cycle_val);
};

void Alps::MotorDown(){
	// set pins. Pot will move down
	digitalWrite(up_pin,LOW);
	digitalWrite(down_pin,HIGH);
	analogWrite(pwm_pin,cycle_val);
};

void Alps::MotorStop(){
	// set pins. Pot will stop move
	digitalWrite(up_pin,LOW);
	digitalWrite(down_pin,LOW);
	analogWrite(pwm_pin,0);
};


int Alps::Move(int val)
{
	// Move until set postition... 0-100
	int aval = analogRead(A0) / 1023.0 * 100;
	//int a = aval;
	int ec = 0;
	
	//int pos = int(val * 1023.0 / 100)+1;

	if ( lastv != val ){
  		if ( val < aval ) {
     			MotorDown();
      			while(int(analogRead(A0)/ 1023.0 * 100) >= val ) {
        			delay(1);
        			ec++;
        			if ( ec == ecv ) {break;}
			}
                	MotorStop();
		}
		else if ( val > aval ) {
			MotorUp();
			while(int(analogRead(A0)/ 1023.0 * 100) <= val ) {
				delay(1);
				ec++;
				if ( ec == ecv ) {break;}
			}
			MotorStop();
		}
      	ec = 0;
      	lastv = val;
	}
	return 0;
};

void Alps::TouchBegin(int tpin1, int tpin2)
{
	// Set Pin for Touch Sense
	cp = CapacitiveSensor(tpin1,tpin2);
	
};

int Alps::Touched( int ms)
{
	// Return Touch sense value
	long val =  cp.capacitiveSensor(ms);
	return val;
	
};


