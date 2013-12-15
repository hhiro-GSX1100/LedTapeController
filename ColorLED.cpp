#include "ColorLED.h"

ColorLED::ColorLED(int On_Value){
	LED_ON = LOW;
	LED_OFF = HIGH;
	m_RED_enableAnalog = false;
	m_GREEN_enableAnalog = false;
	m_BLUE_enableAnalog = false;	
	m_delay = 0;
	if(On_Value == LOW){
		LED_ON = LOW;
		LED_OFF = HIGH;
	}else{
		LED_ON = HIGH;
		LED_OFF = LOW;	
	}
};
ColorLED::~ColorLED(){
	pinMode(LED_RED_PIN, INPUT);
	pinMode(LED_GREEN_PIN, INPUT);
	pinMode(LED_BLUE_PIN, INPUT);
};
void ColorLED::setup(int RedPin, int GreenPin, int BluePin, int On_Value, unsigned long delay_ms){
	if(chkEnablePin(RedPin)) LED_RED_PIN = RedPin;
	if(chkEnablePin(GreenPin)) LED_GREEN_PIN = GreenPin;
	if(chkEnablePin(BluePin)) LED_BLUE_PIN = BluePin;
	if(enableAnalogWrite(LED_RED_PIN)) m_RED_enableAnalog = true;
	if(enableAnalogWrite(LED_GREEN_PIN)) m_GREEN_enableAnalog = true;
	if(enableAnalogWrite(LED_BLUE_PIN)) m_BLUE_enableAnalog = true;
	setOnValue(On_Value);
	setDelay(delay_ms);
	//**********************************************************************
	// KURUMI LED 初期設定
	pinMode(LED_RED_PIN, OUTPUT);
	pinMode(LED_GREEN_PIN, OUTPUT);
	pinMode(LED_BLUE_PIN, OUTPUT);
	digitalWrite(LED_RED_PIN, LED_OFF);
	digitalWrite(LED_GREEN_PIN, LED_OFF);
	digitalWrite(LED_BLUE_PIN, LED_OFF);
};
void ColorLED::setOnValue(int On_Value){
	if(On_Value == LOW){
		LED_ON = LOW;
		LED_OFF = HIGH;
	}else{
		LED_ON = HIGH;
		LED_OFF = LOW;	
	}
};
void ColorLED::setDelay(unsigned long ms){
	m_delay = ms;
};
void ColorLED::Red(){
	digitalWrite(LED_RED_PIN, LED_ON);
	digitalWrite(LED_GREEN_PIN, LED_OFF);
	digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Lime(){
	digitalWrite(LED_RED_PIN, LED_OFF);
	digitalWrite(LED_GREEN_PIN, LED_ON);
	digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Blue(){
	digitalWrite(LED_RED_PIN, LED_OFF);
	digitalWrite(LED_GREEN_PIN, LED_OFF);
	digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Yellow(){
	digitalWrite(LED_RED_PIN, LED_ON);
	digitalWrite(LED_GREEN_PIN, LED_ON);
	digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Aqua(){
	digitalWrite(LED_RED_PIN, LED_OFF);
	digitalWrite(LED_GREEN_PIN, LED_ON);
	digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Fechsia(){
	digitalWrite(LED_RED_PIN, LED_ON);
	digitalWrite(LED_GREEN_PIN, LED_OFF);
	digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::White(){
	digitalWrite(LED_RED_PIN, LED_ON);
	digitalWrite(LED_GREEN_PIN, LED_ON);
	digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Black(){
	digitalWrite(LED_RED_PIN, LED_OFF);
	digitalWrite(LED_GREEN_PIN, LED_OFF);
	digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Silver(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0xC0);
	else digitalWrite(LED_RED_PIN, LED_ON);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0xC0);
	else digitalWrite(LED_GREEN_PIN, LED_ON);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0xC0);
	else digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Gray(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x80);
	else digitalWrite(LED_RED_PIN, LED_ON);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x80);
	else digitalWrite(LED_GREEN_PIN, LED_ON);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x80);
	else digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Maroon(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x80);
	else digitalWrite(LED_RED_PIN, LED_ON);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x00);
	else digitalWrite(LED_GREEN_PIN, LED_OFF);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x00);
	else digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Purple(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x80);
	else digitalWrite(LED_RED_PIN, LED_ON);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x00);
	else digitalWrite(LED_GREEN_PIN, LED_OFF);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x80);
	else digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Green(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x00);
	else digitalWrite(LED_RED_PIN, LED_OFF);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x80);
	else digitalWrite(LED_GREEN_PIN, LED_ON);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x00);
	else digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Olive(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x80);
	else digitalWrite(LED_RED_PIN, LED_ON);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x80);
	else digitalWrite(LED_GREEN_PIN, LED_ON);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x00);
	else digitalWrite(LED_BLUE_PIN, LED_OFF);
	MyDelay();
};
void ColorLED::Navy(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x00);
	else digitalWrite(LED_RED_PIN, LED_OFF);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x00);
	else digitalWrite(LED_GREEN_PIN, LED_OFF);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x80);
	else digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
void ColorLED::Teal(){
	if(m_RED_enableAnalog) analogWrite(LED_RED_PIN, 0x00);
	else digitalWrite(LED_RED_PIN, LED_OFF);
	if(m_GREEN_enableAnalog) analogWrite(LED_GREEN_PIN, 0x80);
	else digitalWrite(LED_GREEN_PIN, LED_ON);
	if(m_BLUE_enableAnalog) analogWrite(LED_BLUE_PIN, 0x80);
	else digitalWrite(LED_BLUE_PIN, LED_ON);
	MyDelay();
};
//使用可能なPin（0～24）をチェック
bool ColorLED::chkEnablePin(int ChkPin){
	if(MIN_PIN_NO <= ChkPin && ChkPin <= MAX_PIN_NO) return true;
	return false;
};
//analogWriteが使用可能なPinかチェック
bool ColorLED::enableAnalogWrite(int ChkPin){
	bool bChk = false;
	for(int i = 0; i < enableAnalogWritePinCount; ++i){
		if(ChkPin == enableAnalogWritePinNo[i]){
			bChk = true;
			break;
		}
	}
	return bChk;
};
//
void ColorLED::LightingLED(int red_value, int green_value, int blue_value){
	int r, g, b;
	r = get8BitValue(red_value);
	g = get8BitValue(green_value);
	b = get8BitValue(blue_value); 
	if(m_RED_enableAnalog){
		analogWrite(LED_RED_PIN, r);
	}else{
		if(r==0) digitalWrite(LED_RED_PIN, LED_OFF);
		else digitalWrite(LED_RED_PIN, LED_ON);
	}
	if(m_GREEN_enableAnalog){
		analogWrite(LED_GREEN_PIN, g);
	}else{
		if(g==0) digitalWrite(LED_GREEN_PIN, LED_OFF);
		else digitalWrite(LED_GREEN_PIN, LED_ON);
	}
	if(m_BLUE_enableAnalog){
		analogWrite(LED_BLUE_PIN, b);
	}else{
		if(b==0) digitalWrite(LED_BLUE_PIN, LED_OFF);
		else digitalWrite(LED_BLUE_PIN, LED_ON);
	}
	MyDelay();
};
int ColorLED::get8BitValue(int value){
	if(value < 0){
		return 0;
	}else if(0 <= value && value <= 255){
		return value;
	}else{
		return 255;
	} 
};
void ColorLED::MyDelay(){
	if(m_delay > 0){
		delay(m_delay);
	}
};



