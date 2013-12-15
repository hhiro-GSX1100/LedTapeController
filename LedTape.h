//*************************************************************************************************
//* Author: hiro 2013/12/13
//* スイッチサイエンスのシリアルLED用のクラス
//* 送信データのタイミングの問題からPINに直接アサインする。
//* よって使用するPINは　#define LED_PIN_NO, #define LED_PIN で定義 
//* インスタンス化後には必ず LedTape::setup() を実行すること。
//*
//* 以下の関数のCopyrightは「Kazuyuki Eguchi」氏
//* send24bit, wait_T0H, wait_T0L, wait_T1H, wait_T1L, test 
//*************************************************************************************************
#ifndef _LEDTAPE_H_
#define _LEDTAPE_H_

#include <RLduino78.h>
#include "iodefine.h"

//*************************************************************
// データを出力する（アクセスする）ピンの定義
#define LED_PIN_0		(P1.BIT.bit1)	// P11
#define LED_PIN_1		(P1.BIT.bit2)	// P12
#define LED_PIN_2		(P3.BIT.bit0)	// P30
#define LED_PIN_3		(P1.BIT.bit6)	// P16
#define LED_PIN_4		(P3.BIT.bit1)	// P31
#define LED_PIN_5		(P1.BIT.bit5)	// P15
#define LED_PIN_6		(P1.BIT.bit0)	// P10
#define LED_PIN_7		(P0.BIT.bit0)	// P00
#define LED_PIN_8		(P0.BIT.bit1)	// P01
#define LED_PIN_9		(P1.BIT.bit3)	// P13
#define LED_PIN_10		(P1.BIT.bit4)	// P14
#define LED_PIN_11		(P7.BIT.bit2)	// P72
#define LED_PIN_12		(P7.BIT.bit1)	// P71
#define LED_PIN_13		(P7.BIT.bit0)	// P70
#define LED_PIN_14		(P2.BIT.bit2)	// P22
#define LED_PIN_15		(P2.BIT.bit3)	// P23
#define LED_PIN_16		(P2.BIT.bit4)	// P24
#define LED_PIN_17		(P2.BIT.bit5)	// P25
#define LED_PIN_18		(P2.BIT.bit6)	// P26
#define LED_PIN_19		(P2.BIT.bit7)	// P27
#define LED_PIN_20		(P14.BIT.bit7)	// P147
#define LED_PIN_21		(P12.BIT.bit0)	// P120
#define LED_PIN_22		(P1.BIT.bit7)	// P17
#define LED_PIN_23		(P5.BIT.bit1)	// P51
#define LED_PIN_24		(P5.BIT.bit0)	// P50
//
//*************************************************************

#define COLOR_RGB       (3)             // 色数（RGB３色）※変更しないこと
#define LED_PIN_NO      (6)             // ピン番号(0 - 24)
#define LED_PIN         LED_PIN_6       // シリアルLEDのポート(LED_PIN_0 - LED_PIN_24) ピン番号と同じ場所
#define LED_BRIGHTNESS  120             // 初期輝度
#define INIT_LEDS       1              // 初期LED個数

struct TColor{
	byte red;
	byte green;
	byte blue;
};

//***************************************************************************************
//* 【重要】スケッチの　setup　実行前のにインスタンス化しコンストラクタでハードウェア処理を記述するとエラーが生じる
//*   可能性を考慮してコンストラクタではほぼ何も実行しない。　
//*   よってインスタンス化後には必ず LedTape::setup() を実行すること。
//***************************************************************************************
class LedTape
{
    public:
		byte m_brightness = LED_BRIGHTNESS;	//デフォルト輝度の設定（0～255）
		enum ExecMode{ AutoSend, ClearAll, ClearAllAndAutoSend, NoClearAndNoSend };	//実行モードの列挙型
		//コンストラクタ
		LedTape();
		//デストラクタ
		~LedTape();
		void setup(byte LEDs = INIT_LEDS, byte brightness = LED_BRIGHTNESS);
		void setLEDs(byte LEDs);
		byte getLEDs();
		void send(void);
		void sendOriginalValue(void);
		void setAllColors(byte r, byte g, byte b, ExecMode m = AutoSend);
		void setAllColors(TColor c, ExecMode m = AutoSend);
		void setColor(int pos, byte r, byte g, byte b, ExecMode m = AutoSend);
		void setColor(int pos, TColor c, ExecMode m = AutoSend);
		void getColor(int pos, TColor *c);
		void clearAllColors();
		void setAllBrightness(byte br);
		void setBrightness(int pos, byte br);
		
    private:
		TColor m_tcolor;	//色の受け渡しに使用する構造体
		byte numLEDs;
		unsigned int numBytes;
		byte *p_pixels;
		byte *p_epixels;
		byte *p_brightness;
		void AllocMemorys();
		void FreeMemorys();
		void ClearAllPixels();
		volatile void send24bit(byte r, byte g, byte b);
        void wait_T0H(void);
        void wait_T0L(void);
        void wait_T1H(void);
        void wait_T1L(void);
        void test(void);
};

#endif // _LEDTAPE_H_