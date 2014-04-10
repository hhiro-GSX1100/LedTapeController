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
	m_RED_enableAnalog = enableAnalogWrite(LED_RED_PIN);
	m_GREEN_enableAnalog = enableAnalogWrite(LED_GREEN_PIN);
	m_BLUE_enableAnalog = enableAnalogWrite(LED_BLUE_PIN);
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
void ColorLED::setBrightness(byte brightness){
	m_brightness = brightness;
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
void ColorLED::LightingLED(byte red_value, byte green_value, byte blue_value){
	int r = (int)(((unsigned int)(red_value) * (unsigned int)(m_brightness)) / 255.0);
	int g = (int)(((unsigned int)(green_value) * (unsigned int)(m_brightness)) / 255.0);
	int b = (int)(((unsigned int)(blue_value) * (unsigned int)(m_brightness)) / 255.0);
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
//
void ColorLED::LightingLED(byte red_value, byte green_value, byte blue_value, byte brightness){
	m_brightness = brightness;
	LightingLED(red_value, green_value, blue_value);
};
void ColorLED::MyDelay(){
	if(m_delay > 0){
		delay(m_delay);
	}
};
int ColorLED::byte2int(byte b){
	return (int)((unsigned int)(b));
}



