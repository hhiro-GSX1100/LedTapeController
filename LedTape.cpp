//*************************************************************************************************
//* Author: hiro 2014/02/10 V2.0  2017/09/09 Update for New KURUMI Library
//*************************************************************************************************
#include "LedTape.h"

//コンストラクタ
LedTape::LedTape(byte Pin){
    endTime = 0;
	m_pin = Pin;
	if (!(m_pin < NUM_DIGITAL_PINS)) {
		m_pin = 6;
	}
	m_u8Port = g_au8DigitalPortTable[m_pin];
	m_On_u8Bit  = g_au8DigitalPinMaskTable[m_pin];
	m_Off_u8Bit = ~m_On_u8Bit;
	m_Px = getPortOutputRegisterAddr(m_u8Port);
	//<-- 2017/09/09
	pinMode(Pin, OUTPUT);
	// 2017/09/09 -->
};
//デストラクタ
LedTape::~LedTape(){
    //メモリ解放
    if(p_brightness) free(p_brightness);
    if(p_pixels) free(p_pixels);
    if(p_epixels) free(p_epixels);
    // 利用するシリアルLEDの端子を入力ポートにする
    pinMode(m_pin, INPUT);
};
//セットアップ関数、インスタンス化後に必ず呼ぶ必要がある。
void LedTape::setup(byte LEDs, byte brightness){
    //LED NUMBER Limitter 
    if(LEDs > MAX_OF_LED_NUMBERS) LEDs = MAX_OF_LED_NUMBERS;
    // 利用するシリアルLEDの端子を出力ポートにする
    pinMode(m_pin, OUTPUT);
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
    //LED NUMBER Limitter 
    if(LEDs > MAX_OF_LED_NUMBERS) LEDs = MAX_OF_LED_NUMBERS;
    
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

//******************************************************************************
//private
//全ての色をクリアする。
void LedTape::ClearAllPixels(){
    if(p_pixels) memset(p_pixels, 0x00, numBytes);
    if(p_epixels) memset(p_epixels, 0x00, numBytes);
};
//メモリを取得する
void LedTape::AllocMemories(){
    FreeMemories();
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
void LedTape::FreeMemories(){
    if(p_brightness) free(p_brightness);
    if(p_pixels) free(p_pixels);
    if(p_epixels) free(p_epixels);
};

void LedTape::setColor(int pos, TColor c){
    if(pos < 0 || pos >= numLEDs) return;
    // 指定色で該当のLED情報を更新する。
    p_pixels[COLOR_RGB * pos + 0] = c.green;
    p_pixels[COLOR_RGB * pos + 1] = c.red;
    p_pixels[COLOR_RGB * pos + 2] = c.blue;
};

void LedTape::setColor(int pos, byte r, byte g, byte b){
    if(pos < 0 || pos >= numLEDs) return;
    // 指定色で該当のLED情報を更新する。
    p_pixels[COLOR_RGB * pos + 0] = g;
    p_pixels[COLOR_RGB * pos + 1] = r;
    p_pixels[COLOR_RGB * pos + 2] = b;
};

void LedTape::getColor(int pos, byte *r, byte *g, byte *b){
    if(pos >= 0 && pos < numLEDs){
        *g = p_pixels[COLOR_RGB * pos + 0];
        *r = p_pixels[COLOR_RGB * pos + 1];
        *b = p_pixels[COLOR_RGB * pos + 2];
    }else{
        *g = 0x00;
        *r = 0x00;
        *b = 0x00;
    }
};

//***********************************************************************************
//public
// 全LEDの色を更新する
void LedTape::setAllColors(byte r, byte g, byte b, ExecMode m)
{
    // 指定色ですべてのLED情報を更新する。
    for(int i = 0 ; i < numLEDs ; i++){
        setColor(i, r, g, b);
    }
    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};
// 全LEDの色を更新する
void LedTape::setAllColors(TColor c, ExecMode m)
{
    // 全背景をクリアする。
    ClearAllPixels();
    // 指定色ですべてのLED情報を更新する。
    for(int i = 0 ; i < numLEDs ; i++){
        setColor(i, c.red, c.green, c.blue);
    }
    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

// １つのLEDのColorをセットする。
void LedTape::setColor(int pos, byte r, byte g, byte b, ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
        // 全背景をクリアする。
        ClearAllPixels();
    }
    // 指定色で該当のLED情報を更新する。
    setColor(pos, r, g, b);
    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};
void LedTape::setColor(int pos, TColor c, ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
        // 全背景をクリアする。
        ClearAllPixels();
    }
    // 指定色で該当のLED情報を更新する。
    setColor(pos, c);
    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

//セットされている色の取得
void LedTape::getColor(int pos, TColor *c){
    if(pos >= 0 && pos < numLEDs){
        c->green = p_pixels[COLOR_RGB * pos + 0];
        c->red = p_pixels[COLOR_RGB * pos + 1];
        c->blue = p_pixels[COLOR_RGB * pos + 2];
    }else{
        c->green = 0x00;
        c->red = 0x00;
        c->blue = 0x00;
    }
};

//【Push】ひとつ後ろへずらして新しい色を先頭に配置する
//追加色の輝度は前の値を使用するので必要があれば「setBrightness」で設定すること。
void LedTape::push(TColor c, ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
        //何もしない。
    }
    //1色を後方へずらす。
    TColor tc;
    for(int i = (numLEDs - 1); i > 0; --i){
        getColor(i - 1, &tc);
        setColor(i, tc);
        setBrightness(i, getBrightness( i - 1 ));
    }
    // 先頭の色を指定色で更新する。
    setColor(0, c);
    //p_brightnessは元の値を保持
    if(m == AutoSend || m == ClearAllAndAutoSend) send();    
};

//【Pull】ひとつ前へずらして新しい色を最後に配置する
//追加色の輝度は前の値を使用するので必要があれば「setBrightness」で設定すること。
void LedTape::pull(TColor c, ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
        //何もしない
    }
    //1色を前方へずらす。
    TColor tc;
    for(int i = 0; i < (numLEDs - 1); ++i){
        getColor(i + 1, &tc);
        setColor(i, tc);
        setBrightness(i, getBrightness( i + 1 ));
    }
    // 末尾の色を指定色で更新する。
    setColor(numLEDs - 1, c);
    //p_brightnessは元の値を保持
    if(m == AutoSend || m == ClearAllAndAutoSend) send();    
};

//反転する。
void LedTape::reverse(byte led_pos, ExecMode m){
   if(m == ClearAll || m == ClearAllAndAutoSend){
        // 何もしない。
    }
    //反転する。数が少ないので愚直に置き換える
    if(led_pos == 0 || led_pos > numLEDs) led_pos = numLEDs;
    byte tmp_br;
    int rem = led_pos % 2;
    int loopNums = led_pos / 2;    //ｉｎｔ変換で端数は切り捨てとなるので、商について入れ替えれば良い。
    TColor tc1, tc2;
    for(int i = 0; i < loopNums; ++i){
        int mypos = led_pos - 1 - i;
        getColor(i, &tc1);
        getColor(mypos, &tc2);
        setColor(i, tc2);
        setColor(mypos, tc1);
        tmp_br = getBrightness(i);
        setBrightness(i, getBrightness(mypos));
        setBrightness(mypos, tmp_br);
    }

    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

void LedTape::copy(byte led_pos, bool common){
    if(led_pos == 0 || led_pos > numLEDs) return;
    TColor tc;
    if(common){
    //先頭の色を共有する場合、コピーを１つオフセットする。
        for(int i = 1; i < led_pos; ++i){
            int mypos = led_pos + i - 1;
            if(mypos > numLEDs) return;
            getColor(i, &tc);
            setColor(mypos, tc);
            setBrightness(mypos, getBrightness(i));
        }    
    }else{
        for(int i = 0; i < led_pos; ++i){
            int mypos = led_pos + i;
            if(mypos > numLEDs) return;
            getColor(i, &tc);
            setColor(mypos, tc);
            setBrightness(mypos, getBrightness(i));
        }
    }
};
//led_pos の位置から先に反転コピーする。
void LedTape::reverseCopy(byte led_pos, bool common){
    if(led_pos == 0 || led_pos > numLEDs) return;
    TColor tc;
    if(common){
    //先頭の色を共有する場合、コピーを１つオフセットする。
        for(int i = (led_pos - 2), j = 0; i >= 0 ; --i, ++j){
            int mypos = led_pos + j;
            if(mypos > numLEDs) return;
            getColor(i, &tc);
            setColor(mypos, tc);
            setBrightness(mypos, getBrightness(i));
        }
    }else{
        for(int i = (led_pos - 1), j = 0; i >= 0 ; --i, ++j){
            int mypos = led_pos + j;
            if(mypos > numLEDs) return;
            getColor(i, &tc);
            setColor(mypos, tc);
            setBrightness(mypos, getBrightness(i));
        }
    }
};

void LedTape::segment(byte led_counts, ExecMode m){
    if(led_counts >= numLEDs) return;
   if(m == ClearAll || m == ClearAllAndAutoSend){
        // 何もしない。
    }
    int i = -1;
    while(true){
        ++i;
        int mypos = led_counts * i + 1;
        if(mypos > numLEDs) return;
        copy(mypos);
    }
    
    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

void LedTape::segmentReverse(byte led_counts, bool common, ExecMode m){
    if(led_counts >= numLEDs) return;
    if(m == ClearAll || m == ClearAllAndAutoSend){
        // 何もしない。
    }
    int i = -1;
    int mypos = 0;
    while(true){
        ++i;
        mypos += led_counts + 1;
        if(mypos > numLEDs) return;
        reverseCopy(mypos, common);
        if(common) --mypos;
    }
    
    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

void LedTape::mirror(ExecMode m){
    if(m == ClearAll || m == ClearAllAndAutoSend){
        // 何もしない。
    }
    int tmp = 0;
    byte tmp_pixels[COLOR_RGB * numLEDs];
    for(int i = 0, j = (numLEDs - 1); i < numLEDs; ++i, --j){
        for(int k = 0; k < 3; ++k){
            tmp_pixels[COLOR_RGB * j + k] = p_pixels[COLOR_RGB * i + k];    
        }    
    }
    for(int i = 0; i < numLEDs; ++i){
        for(int k = 0; k < 3; ++k){
            tmp = p_pixels[COLOR_RGB * i + k] + tmp_pixels[COLOR_RGB * i + k];
            tmp = tmp > 255 ? 255 : tmp;
            p_pixels[COLOR_RGB * i + k] = (byte)(tmp & 0xff);
        }
    }

    if(m == AutoSend || m == ClearAllAndAutoSend) send();
};

//輝度反転する。引数は反転対象の最後の位置
void LedTape::brightnessReverse(byte led_pos){
    //反転する。数が少ないので愚直に置き換える
    if(led_pos == 0 || led_pos > numLEDs) led_pos = numLEDs;
    byte tmp_br1, tmp_br2;
    int rem = led_pos % 2;
    int loopNums = led_pos / 2;    //ｉｎｔ変換で端数は切り捨てとなるので、商について入れ替えれば良い。
    for(int i = 0; i < loopNums; ++i){
        int mypos = (led_pos - 1) - i;
        tmp_br1 = getBrightness(i);
        tmp_br2 = getBrightness(mypos);
        setBrightness(i, tmp_br2);
        setBrightness(mypos, tmp_br1);
    }
};
//輝度コピー　引数は、コピーを始める先頭位置
void LedTape::brightnessCopy(byte led_pos, bool common){
    if(led_pos == 0 || led_pos > numLEDs) return;
    if(common){
    //先頭の色を共有する場合、コピーを１つオフセットする。
        for(int i = 1; i < led_pos; ++i){
            int mypos = led_pos + i - 1;
            if(mypos > numLEDs) return;
            setBrightness(mypos, getBrightness(i));
        }    
    }else{
        for(int i = 0; i < led_pos; ++i){
            int mypos = led_pos + i;
            if(mypos > numLEDs) return;
            setBrightness(mypos, getBrightness(i));
        }
    }
};
//輝度反転コピー　引数は、コピーを始める先頭位置
void LedTape::brightnessReverseCopy(byte led_pos, bool common){
    if(led_pos == 0 || led_pos > numLEDs) return;
    byte tmp_br;
    if(common){
    //先頭の色を共有する場合、コピーを１つオフセットする。
        for(int i = (led_pos - 2), j = 0; i >= 0 ; --i, ++j){
            int mypos = led_pos + j;
            if(mypos > numLEDs) return;
            tmp_br = getBrightness(i);
            setBrightness(mypos, tmp_br);
        }
    }else{
        for(int i = (led_pos - 1), j = 0; i >= 0 ; --i, ++j){
            int mypos = led_pos + j;
            if(mypos > numLEDs) return;
            tmp_br = getBrightness(i);
            setBrightness(mypos, tmp_br);
        }
    }
};
void LedTape::brightnessMirror(){
    int tmp = 0;
    byte tmp_brightness[numLEDs];
    for(int i = 0, j = (numLEDs - 1); i < numLEDs; ++i, --j){
        tmp_brightness[i] = getBrightness(j);        
    }
    for(int i = 0; i < numLEDs; ++i){
        tmp = p_brightness[i] + tmp_brightness[i];
        tmp = tmp > 255 ? 255 : tmp;
        p_brightness[i] = (byte)(tmp & 0xff);
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
void LedTape::clearAllBrightness(){
    if(p_brightness) memset(p_brightness, 0x00, numLEDs);
};
//輝度個別設定
void LedTape::setBrightness(int pos, byte br){
    if(pos >= 0 && pos < numLEDs){
        p_brightness[pos] = br;
    }
};
//輝度値個別取得
byte LedTape::getBrightness(int pos){
    if(pos >= 0 && pos < numLEDs){
        return p_brightness[pos];
    }
};

// 利用する全LEDの情報をシリアルで送信する。
volatile void LedTape::send(void){

    //輝度で送信する数値を計算し格納
    for(int i = 0 ; i < numLEDs ; ++i){
        int itmp0 = (int)(((unsigned int)(p_pixels[COLOR_RGB * i + 0]) * (unsigned int)(p_brightness[i])) / 255.0);
        int itmp1 = (int)(((unsigned int)(p_pixels[COLOR_RGB * i + 1]) * (unsigned int)(p_brightness[i])) / 255.0);
        int itmp2 = (int)(((unsigned int)(p_pixels[COLOR_RGB * i + 2]) * (unsigned int)(p_brightness[i])) / 255.0);
        p_epixels[COLOR_RGB * i + 0] = (byte) itmp0 & 0xff;
        p_epixels[COLOR_RGB * i + 1] = (byte) itmp1 & 0xff;
        p_epixels[COLOR_RGB * i + 2] = (byte) itmp2 & 0xff;
    }
    
    while((micros() - endTime) < 50L);
    // endTime is a private member (rather than global var) so that mutliple
    // instances on different pins can be quickly issued in succession (each
    // instance doesn't delay the next).
    noInterrupts();
    volatile byte *ptr = p_epixels;     // Pointer to next byte
	volatile byte *ptr_end = &p_epixels[(numBytes - 1)];     // Pointer to end
    volatile byte val;              // Current byte value
    for(int i = 0; i < numBytes ; ++i){
        val = *ptr;
		if(ptr != ptr_end) ptr++;
//<-- 2017/09/09
        if(val & 0x80){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x40){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x20){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x10){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x08){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x04){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x02){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }

        if(val & 0x01){
          OUTPUT_HIGH;
          T1H;
          OUTPUT_LOW;
          T1L;
        }else{
          OUTPUT_HIGH;
          T0H;
          OUTPUT_LOW;
          T0L;
        }
    }
// 2017/09/09 -->
/*<-- 2017/09/09        
        if(val & 0x80){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x40){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x20){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x10){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x08){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x04){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x02){
          code_1();
        }else{
          code_0();
        }

        if(val & 0x01){
          code_1();
        }else{
          code_0();
        }
    }
//2017/09/09 -->*/    
    interrupts();
    endTime = micros(); // Save EOD time for latch on next call
};

//*******************************************************************************************
//private
/*<-- 2017/09/09
inline void LedTape::code_1(){
	*m_Px |= m_On_u8Bit;		// 出力をHIGHに設定
	wait_T1H();
	*m_Px &= m_Off_u8Bit;		// 出力をLOWに設定
	wait_T1L();
}

inline void LedTape::code_0(){
	*m_Px |= m_On_u8Bit;		// 出力をHIGHに設定
	wait_T0H();
	*m_Px &= m_Off_u8Bit;		// 出力をLOWに設定
	wait_T0L();
}

inline void LedTape::wait_T0H(void){
    NOP();
};

inline void LedTape::wait_T0L(void){
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
};

inline void LedTape::wait_T1H(void){
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
    NOP();NOP();NOP();NOP();NOP();
};

inline void LedTape::wait_T1L(void){
    NOP();
};

void LedTape::test(void){
    while(1)
    {
        code_1();
    }
};
//2017/09/09 -->*/