//*************************************************************************************************
//* Author: hiro 2013/12/13
//* KurumiのフルカラーLED用コントロールクラス
//*  アナログ出力が可能なピン番号は（3, 5, 6, 9, 10, 11, 22, 23, 24）であることに注意すること。
//*  アナログ出力ができないピンでこれを使用するメソッドを実行した場合、値があればON、なければOFFとして扱う。
//* 【使用方法】
//*  スケッチのsetup内でInitializeを呼ぶ。引数に使用するPINの番号を渡す事ができる。順に赤、緑、青のPinを指定する。
//*  必要に応じてdelay時間をsetDelayを用いてセットする。setDelayは一度セットすれば同じ値なら何度も呼ぶ必要がなくなる。
//*  発光させるメソッドを呼ぶ
//*  LightingLEDはanalogWriteを使用して任意の色を出力する。引数は順に赤、緑、青のPWMのデューティ（0～255）
//* 【例】
//*  インスタンス:　KurumiLED myLED;
//*  setup内で: myLED.setup(22, 23, 24);
//*    必要に応じて: myLED.setDelay(1000);
//*  loop内で:  myLED.Red(); myLED.Purple();
//*    必要に応じて: myLED.setDelay(2000);
//*    必要に応じて: myLED.LightingLED(50, 100, 150); 
//*************************************************************************************************
#ifndef _COLORLED_H_
#define _COLORLED_H_

#include <RLduino78.h>

class ColorLED{
	public:
		//コンストラクタ
		ColorLED(int On_Value = LOW);
		~ColorLED();
		//setupはスケッチのsetup以降で呼ぶこと。
		void setup(int RedPin = 22, int GreenPin = 23, int BluePin = 24, int On_Value = LOW, unsigned long delay_ms = 0);
		void setOnValue(int On_Value = LOW);
		void setDelay(unsigned long ms);
		void setBrightness(byte brightness);
		//*******************************************************************************
		//* 以下は発光させるメソッド。
		//*******************************************************************************
		void Red();
		void Lime();
		void Blue();
		void Yellow();
		void Aqua();
		void Fechsia();
		void White();
		void Black();
		//*******************************************************************************
		//* 以下の８色及びLightLEDはanalogWriteを使用します。
		//* よって、analogWriteを使用できないPINを使用した場合はdigitalWriteで出力するので注意が必要です。
		//*******************************************************************************
		void Silver();
		void Gray();
		void Maroon();
		void Purple();
		void Green();
		void Olive();
		void Navy();
		void Teal();
		void LightingLED(byte red_value, byte green_value, byte blue_value);
		void LightingLED(byte red_value, byte green_value, byte blue_value, byte brightness);
	private:
		//*********************************************************************
		// Pin 22,23,24 are assigned to RGB LEDs.
		int LED_RED_PIN = 22;
		int LED_GREEN_PIN = 23;
		int LED_BLUE_PIN = 24;
		int LED_ON;
		int LED_OFF;
		bool m_RED_enableAnalog;
		bool m_GREEN_enableAnalog;
		bool m_BLUE_enableAnalog;		
		unsigned long m_delay = 0;
		byte m_brightness = 0xff;
		static const int MIN_PIN_NO = 0;
		static const int MAX_PIN_NO = 24;
		static const int enableAnalogWritePinCount = 6;
		//アナログ出力が可能なピン番号（3, 5, 6, 9, 10, 11, 22, 23, 24）
		const int enableAnalogWritePinNo[enableAnalogWritePinCount] = {3, 5, 6, 9, 10, 11};
		//使用可能なPin（0～24）をチェックする関数
		bool chkEnablePin(int ChkPin);
		//analogWriteが使用可能なPinかチェックする関数
		bool enableAnalogWrite(int ChkPin);
		void MyDelay();
		int byte2int(byte b);
};

#endif // _COLORLED_H_