#include "LedTape.h"

//コンストラクタ
LedTape::LedTape(){};
//デストラクタ
LedTape::~LedTape(){
	//メモリ解放
	if(p_brightness) free(p_brightness);
	if(p_pixels) free(p_pixels);
	if(p_epixels) free(p_epixels);
	// 利用するシリアルLEDの端子を入力ポートにする
	pinMode(LED_PIN_NO, INPUT);
};
//セットアップ関数、インスタンス化後に必ず呼ぶ必要がある。
void LedTape::setup(byte LEDs, byte brightness){
	// 利用するシリアルLEDの端子を出力ポートにする
	pinMode(LED_PIN_NO, OUTPUT);
	//Color保持用の配列を動的に生成
	numLEDs = LEDs;
	numBytes = numLEDs * COLOR_RGB;
	m_brightness = brightness;
	//メモリ割り当て※必ず他のパラメータ（numLEDs, numBytes）設定後に実行のこと。
	//輝度メモリ割り当て＆値設定　【LEDの個数】分割り当て
	if((p_brightness = (byte *)malloc(numLEDs))){
		if(p_brightness) memset(p_brightness, m_brightness, numLEDs);
	}
	//LEDデータメモリ割り当て＆値クリア　【LEDの個数×３】割り当て
	if((p_pixels = (byte *)malloc(numBytes))){
		if(p_pixels) memset(p_pixels, 0x00, numBytes);
	}
	if((p_epixels = (byte *)malloc(numBytes))) {
		if(p_epixels) memset(p_epixels, 0x00, numBytes);
	}
};
//コントロールするLEDの数を変更する。
void LedTape::setLEDs(byte LEDs){
	numLEDs = LEDs;
	numBytes = numLEDs * COLOR_RGB;
	//現在のメモリ領域を解放
	if(p_brightness) free(p_brightness);
	if(p_pixels) free(p_pixels);
	if(p_epixels) free(p_epixels);
	//輝度メモリ割り当て＆値設定　【LEDの個数】分割り当て
	if((p_brightness = (byte *)malloc(numLEDs))){
		if(p_brightness) memset(p_brightness, m_brightness, numLEDs);
	}
	//LEDデータメモリ割り当て＆値クリア　【LEDの個数×３】割り当て
	if((p_pixels = (byte *)malloc(numBytes))){
		if(p_pixels) memset(p_pixels, 0x00, numBytes);
	}
	if((p_epixels = (byte *)malloc(numBytes))) {
		if(p_epixels) memset(p_epixels, 0x00, numBytes);
	}
};

byte LedTape::getLEDs(){
	return numLEDs;
}

//全ての色をクリアする。
void LedTape::ClearAllPixels(){
	if(p_pixels) memset(p_pixels, 0x00, numBytes);
	if(p_epixels) memset(p_epixels, 0x00, numBytes);
};
//メモリを取得する
void LedTape::AllocMemorys(){
	FreeMemorys();
	//輝度メモリ割り当て＆値設定　【LEDの個数】分割り当て
	if((p_brightness = (byte *)malloc(numLEDs))){
		if(p_brightness) memset(p_brightness, m_brightness, numLEDs);
	}
	//LEDデータメモリ割り当て＆値クリア　【LEDの個数×３】割り当て
	if((p_pixels = (byte *)malloc(numBytes))){
		if(p_pixels) memset(p_pixels, 0x00, numBytes);
	}
	if((p_epixels = (byte *)malloc(numBytes))) {
		if(p_epixels) memset(p_epixels, 0x00, numBytes);
	}
};
//メモリを解放する
void LedTape::FreeMemorys(){
	if(p_brightness) free(p_brightness);
	if(p_pixels) free(p_pixels);
	if(p_epixels) free(p_epixels);
};

// 全LEDの色を更新する
void LedTape::setAllColors(byte r, byte g, byte b, ExecMode m)
{
    // 指定色ですべてのLED情報を更新する。
    for(int i = 0 ; i < numLEDs ; i++){
        p_pixels[COLOR_RGB * i + 0] = r;
        p_pixels[COLOR_RGB * i + 1] = g;
        p_pixels[COLOR_RGB * i + 2] = b;
    }
	if(m == AutoSend || m == ClearAllAndAutoSend) send();
};
// 全LEDの色を更新する
void LedTape::setAllColors(TColor c, ExecMode m)
{
    // 指定色ですべてのLED情報を更新する。
    for(int i = 0 ; i < numLEDs ; i++){
        p_pixels[COLOR_RGB * i + 0] = c.red;
        p_pixels[COLOR_RGB * i + 1] = c.green;
        p_pixels[COLOR_RGB * i + 2] = c.blue;
    }
	if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

// １つのLEDのColorをセットする。
void LedTape::setColor(int pos, byte r, byte g, byte b, ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
		// 全背景をクリアする。
		ClearAllPixels();
	}
	if(pos >= 0 && pos < numLEDs){
		// 指定色で該当のLED情報を更新する。
		p_pixels[COLOR_RGB * pos + 0] = r;
		p_pixels[COLOR_RGB * pos + 1] = g;
		p_pixels[COLOR_RGB * pos + 2] = b;
	}
	if(m == AutoSend || m == ClearAllAndAutoSend) send();
};
void LedTape::setColor(int pos, TColor c, ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
		// 全背景をクリアする。
		ClearAllPixels();
	}
	if(pos >= 0 && pos < numLEDs){
		// 指定色で該当のLED情報を更新する。
		p_pixels[COLOR_RGB * pos + 0] = c.red;
		p_pixels[COLOR_RGB * pos + 1] = c.green;
		p_pixels[COLOR_RGB * pos + 2] = c.blue;
	}
	if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

//セットされている色の取得
void LedTape::getColor(int pos, TColor *c){
	if(pos >= 0 && pos < numLEDs){
		c->red = p_pixels[COLOR_RGB * pos + 0];
		c->green = p_pixels[COLOR_RGB * pos + 1];
		c->blue = p_pixels[COLOR_RGB * pos + 2];
	}else{
		c->red = 0x00;
		c->green = 0x00;
		c->blue = 0x00;
	}
};

void LedTape::clearAllColors(){
    // 全背景をクリアする。
    ClearAllPixels();
};
//全輝度設定
void LedTape::setAllBrightness(byte br){
	m_brightness = br;
	if(p_brightness) memset(p_brightness, m_brightness, numLEDs);
}
//輝度個別設定
void LedTape::setBrightness(int pos, byte br){
	if(pos >= 0 && pos < numLEDs){
		p_brightness[pos] = br;
	}
};

// 利用する全LEDの情報をシリアルで送信する。
void LedTape::send(void){
	//輝度で送信する数値を計算し格納
	for(int i = 0 ; i < numLEDs ; ++i){
		int itmp0 = (p_pixels[COLOR_RGB * i + 0] * p_brightness[i]) / 255.0;
		int itmp1 = (p_pixels[COLOR_RGB * i + 1] * p_brightness[i]) / 255.0;
		int itmp2 = (p_pixels[COLOR_RGB * i + 2] * p_brightness[i]) / 255.0;
		p_epixels[COLOR_RGB * i + 0] = (byte) itmp0 & 0xff;
		p_epixels[COLOR_RGB * i + 1] = (byte) itmp1 & 0xff;
		p_epixels[COLOR_RGB * i + 2] = (byte) itmp2 & 0xff;
	}
	//送信
    for(int i = 0 ; i < numLEDs ; ++i){
        send24bit(p_epixels[COLOR_RGB * i + 0], p_epixels[COLOR_RGB * i + 1], p_epixels[COLOR_RGB * i + 2]);
		//send24bit(p_pixels[COLOR_RGB * i + 0], p_pixels[COLOR_RGB * i + 1], p_pixels[COLOR_RGB * i + 2]);
    }
};

// 利用する全LEDの情報をシリアルで送信する。
//　オリジナル輝度
void LedTape::sendOriginalValue(void){
	//送信
    for(int i = 0 ; i < numLEDs ; ++i){
        send24bit(p_pixels[COLOR_RGB * i + 0], p_pixels[COLOR_RGB * i + 1], p_pixels[COLOR_RGB * i + 2]);
    }
};

void LedTape::wait_T0H(void){
    NOP();
};

void LedTape::wait_T0L(void){
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
};

void LedTape::wait_T1H(void){
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
};

void LedTape::wait_T1L(void){
    NOP();
};

void LedTape::test(void){
    while(1)
    {
        LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
    }
};

volatile void LedTape::send24bit(byte r, byte g, byte b){
    ////////////////////////////////////////////////////////////////////////////
    // ◇割り込み禁止
    ////////////////////////////////////////////////////////////////////////////
    noInterrupts();

	  if(g & 0x80){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(g & 0x40){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(g & 0x20){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }
	  
	  if(g & 0x10){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(g & 0x08){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(g & 0x04){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(g & 0x02){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(g & 0x01){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x80){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x40){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x20){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }
	  
	  if(r & 0x10){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x08){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x04){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x02){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(r & 0x01){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x80){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x40){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x20){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }
	  
	  if(b & 0x10){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x08){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x04){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x02){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

	  if(b & 0x01){
		  LED_PIN = 1;wait_T1H();LED_PIN = 0;wait_T1L();
	  }else{
		  LED_PIN = 1;wait_T0H();LED_PIN = 0;wait_T0L();
	  }

    ////////////////////////////////////////////////////////////////////////////
    // ◇割り込み許可
    ////////////////////////////////////////////////////////////////////////////
    interrupts();
};
