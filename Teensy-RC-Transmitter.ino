#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


const uint64_t pipeOut = 0xE8E8F0F0E1LL;

//Initialize the radio module
RF24 radio(9, 10);

struct MyData
{
	byte throttle;
	byte yaw;
	byte pitch;
	byte roll;
	byte mode;
	byte collective;
};

MyData data;

void setup()
{
	radio.begin();
	radio.setAutoAck(false);
	radio.setDataRate(RF24_250KBPS);

	radio.openWritingPipe(pipeOut);

	resetData();

}


void loop()
{
	// The calibration numbers used here should be measured 
	// for your joysticks using the TestJoysticks sketch.
	data.throttle = mapJoystickValues(analogRead(0), 13, 524, 1015, true);
	data.yaw = mapJoystickValues(analogRead(1), 1, 505, 1020, true);
	data.pitch = mapJoystickValues(analogRead(2), 12, 544, 1021, true);
	data.roll = mapJoystickValues(analogRead(3), 34, 522, 1020, true);

	radio.write(&data, sizeof(MyData));
}


void resetData()
{
	data.throttle = 0;
	data.pitch = 127;
	data.roll = 127;
	data.yaw = 127;
}

// Returns a corrected value for a joystick position that takes into account
// the values of the outer extents and the middle of the joystick range.
int mapJoystickValues(int val, int lower, int middle, int upper, bool reverse)
{
	val = constrain(val, lower, upper);
	if (val < middle)
		val = map(val, lower, middle, 0, 128);
	else
		val = map(val, middle, upper, 128, 255);
	return (reverse ? 255 - val : val);
}
