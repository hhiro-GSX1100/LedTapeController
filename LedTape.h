//*************************************************************************************************
//* Author: hiro 2014/02/10 V2.0
//* スイッチサイエンスのシリアルLED用のクラス
//* 送信データのタイミングの問題からPINに直接アサインする。
//* コンストラクタでPINを指定すること。　【例】 LedTape t = LedTape(3);
//* インスタンス化後には必ず LedTape::setup() を実行すること。
//*************************************************************************************************
#ifndef _LEDTAPE_H_
#define _LEDTAPE_H_

#include <RLduino78.h>
#include "iodefine.h"

//*************************************************************

#define COLOR_RGB       (3)             // 色数（RGB３色）※変更しないこと
#define LED_BRIGHTNESS  120             // 初期輝度
#define INIT_LEDS       1              // 初期LED個数
#define MAX_OF_LED_NUMBERS 255            //使用可能な最大LED数

//!< 入植ポート・レジスタ・ベースアドレス
#define getPortOutputRegisterAddr(port)		((volatile uint8_t*)(ADDR_PORT_REG + port))	

// ピン変換テーブル
const static uint8_t	g_au8DigitalPortTable[NUM_DIGITAL_PINS] = {
	DIGITAL_PIN_0,  DIGITAL_PIN_1,  DIGITAL_PIN_2,  DIGITAL_PIN_3,
	DIGITAL_PIN_4,  DIGITAL_PIN_5,  DIGITAL_PIN_6,  DIGITAL_PIN_7,
	DIGITAL_PIN_8,  DIGITAL_PIN_9,  DIGITAL_PIN_10, DIGITAL_PIN_11,
	DIGITAL_PIN_12, DIGITAL_PIN_13, DIGITAL_PIN_14, DIGITAL_PIN_15,
	DIGITAL_PIN_16, DIGITAL_PIN_17, DIGITAL_PIN_18, DIGITAL_PIN_19,
#if defined(REL_GR_KURUMI)
	DIGITAL_PIN_20, DIGITAL_PIN_21, DIGITAL_PIN_22, DIGITAL_PIN_23,
	DIGITAL_PIN_24,
#endif
};

const static uint8_t	g_au8DigitalPinMaskTable[NUM_DIGITAL_PINS] = {
	DIGITAL_PIN_MASK_0,  DIGITAL_PIN_MASK_1,  DIGITAL_PIN_MASK_2,  DIGITAL_PIN_MASK_3, 
	DIGITAL_PIN_MASK_4,  DIGITAL_PIN_MASK_5,  DIGITAL_PIN_MASK_6,  DIGITAL_PIN_MASK_7, 
	DIGITAL_PIN_MASK_8,  DIGITAL_PIN_MASK_9,  DIGITAL_PIN_MASK_10, DIGITAL_PIN_MASK_11, 
	DIGITAL_PIN_MASK_12, DIGITAL_PIN_MASK_13, DIGITAL_PIN_MASK_14, DIGITAL_PIN_MASK_15,
	DIGITAL_PIN_MASK_16, DIGITAL_PIN_MASK_17, DIGITAL_PIN_MASK_18, DIGITAL_PIN_MASK_19,
#if defined(REL_GR_KURUMI)
	DIGITAL_PIN_MASK_20, DIGITAL_PIN_MASK_21, DIGITAL_PIN_MASK_22, DIGITAL_PIN_MASK_23,
	DIGITAL_PIN_MASK_24,
#endif
};

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
        byte m_brightness = LED_BRIGHTNESS;    //デフォルト輝度の設定（0～255）
        enum ExecMode{ AutoSend, ClearAll, ClearAllAndAutoSend, NoClearAndNoSend };    //実行モードの列挙型
        //コンストラクタ
        LedTape(byte Pin = 6);
        //デストラクタ
        ~LedTape();
        void setup(byte LEDs = INIT_LEDS, byte brightness = LED_BRIGHTNESS);
        void setLEDs(byte LEDs);
        byte getLEDs();
        volatile void send(void);
        void setAllColors(byte r, byte g, byte b, ExecMode m = AutoSend);
        void setAllColors(TColor c, ExecMode m = AutoSend);
        void setColor(int pos, byte r, byte g, byte b, ExecMode m);
        void setColor(int pos, TColor c, ExecMode m);
        void getColor(int pos, TColor *c);
        void push(TColor c, ExecMode m = AutoSend);
        void pull(TColor c, ExecMode m = AutoSend);
        void reverse(byte led_pos = 0, ExecMode m = AutoSend);    //反転
        void copy(byte led_pos, bool common = false);            //先頭から led_pos - 1 までの色を led_pos 以降にコピーする。　common は先頭色を共有する場合にtrue
        void reverseCopy(byte led_pos, bool common = false);        //先頭から led_pos - 1 までの色を led_pos 以降に反転コピーする。　common は先頭色を共有する場合にtrue
        void segment(byte led_counts, ExecMode m = AutoSend);    //led_counts 個でセグメントに分けて後はコピーを使用。
        void segmentReverse(byte led_counts, bool common = true, ExecMode m = AutoSend);    //led_counts 個でセグメントに分けて後は反転コピーを使用。
        void mirror(ExecMode m = AutoSend);
        void brightnessReverse(byte led_pos = 0);    //輝度反転
        void brightnessCopy(byte led_pos, bool common = false);    
        void brightnessReverseCopy(byte led_pos = 0, bool common = false);    //輝度反転
        void brightnessMirror();
        void clearAllColors();
        void setAllBrightness(byte br);
        void clearAllBrightness();
        void setBrightness(int pos, byte br);
        byte getBrightness(int pos);
        
    private:
		byte m_u8Port;		//pin port
		byte m_On_u8Bit;	//PinMask
		byte m_Off_u8Bit;	//PinMask		
		byte m_pin;			//pin no
		volatile byte *m_Px;	//memory address
        TColor m_tcolor;    //色の受け渡しに使用する構造体
        byte numLEDs;
        unsigned int numBytes;
        byte *p_pixels;
        byte *p_epixels;
        byte *p_brightness;
        uint32_t endTime;       // Latch timing reference
        void AllocMemories();
        void FreeMemories();
        void ClearAllPixels();
        void setColor(int pos, byte r, byte g, byte b);
        void setColor(int pos, TColor c);
        void getColor(int pos, byte *r, byte *g, byte *b);
		void code_1();
		void code_0();
        void wait_T0H(void);
        void wait_T0L(void);
        void wait_T1H(void);
        void wait_T1L(void);
        void test(void);
};

#endif // _LEDTAPE_H_