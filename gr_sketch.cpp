/*GR-KURUMI Sketch Template Version: E0.94*/
//*************************************************************************************************
//* Author: hiro 2014/02/10 V2.0
//*************************************************************************************************
#include <RLduino78.h>
#include "iodefine.h"
#include "LedTape.h"
#include "UserData.h"
#include "ColorLED.h"

//*********************************************************************
//　関数定義
//*********************************************************************
//Movement 列挙
enum Movement{ eNormal, eReverse, eDouble, eDoubleReverse, eMirror};
//ExecMode 列挙
enum ExecMode{ eExecLED, eUser};

volatile bool loadUserData(int point = -1);
volatile void StartAutoPlay(byte strat_mem_pos, byte end_mem_pos);
volatile void NextAutoPlay();
volatile void StopAutoPlay();
volatile bool funcParamsSave();
volatile bool funcSettings();
volatile bool funcUserSettings();
volatile bool funcRequest();
volatile void funcExec();
volatile void funcUserExec();
volatile void funcInitializeUserData();
volatile bool chkModeChange();
volatile bool my_delay_withChkModeChange(unsigned long ms);
volatile void my_delay(unsigned long ms);
volatile bool chkAutoPlay();
volatile bool readSerialData(int ReadByte);

volatile void rainbowCycle(byte wait, char mc);
volatile void Wheel(int index, byte WheelPos);

volatile void funcStar(TColor tc, char mc, int mi, bool b_random = false, Movement mm = eNormal);
volatile void funcTwincleStar(TColor tc, char mc, int mi, bool b_randam = false);
volatile void funcShootingStar(TColor tc, char mc, int mi, bool b_randam = false, Movement mm = eNormal);
volatile void funcLighting(TColor tc, char mc, int mi, bool b_randam = false);
volatile void funcColorflowChg(char mc, int mi, Movement mm = eNormal);
volatile void funcLightingAllColorChg(char mc);
volatile void funcRainbow(char mc);
volatile void funcGradient(char mc);
volatile void funcEguchiStar(TColor tc, char mc, int mi, unsigned long ms = 100);
volatile void funcOff(char mc, unsigned long ms);

volatile void funcStar_User(char mc, Movement mm = eNormal);
volatile void funcTwincleStar_User(char mc);
volatile void funcLighting_User(char mc);
volatile void funcShootingStar_User(char mc, Movement mm = eNormal);

String funcNum2Str(int num, int digit);
int funcStr2Num(String str);
int getNumberTo3CharArray(char *ch,int start = 0);
byte int2byte(int val);
volatile void funcUpDATAInitialize();
volatile void autoPlay();

//**********************************
// Earring用関数
volatile void earring_wheel(TColor *MyColor, byte WheelPos);
volatile void earring_movement();

//*********************************************************************
// インスタンス宣言
//*********************************************************************
//ROM用インスタンス
UserData userdata;
//LedTapeインスタンス
LedTape ledtape;
//ColorLEDインスタンス
ColorLED kurumiLED;
ColorLED earringLED;

//*********************************************************************
// 定数宣言
//*********************************************************************
static const byte iMod = 8;            //モードの分母（固定）
static const int DATA_LENGTH = 6;        		//設定データのByte数
static const int SETTING_DATA_LENGTH = 9;      
static const int USER_DATA_LENGTH = 6;        //ユーザデータのByte数
static const int REQUEST_DATA_LENGTH = 3;        //リクエストデータのByte数
static const unsigned long KURUMI_LED_DELAY = 30;    //デバッグ用KURUMI LED Deley値
static const int MIN_DELAY_TIME = 1;
static const char DATA_END_MARK = ',';
static const int ADD_CHKMODE_DATA_LENGTH = 2;	//先頭の文字とDATA_END_MARKの文字数を加算するための定数
static const int ADD_FUNC_DATA_LENGTH = 1;		//DATA_END_MARKの文字数を加算するための定数

//　変数宣言
volatile static ExecMode exec_mode;                //実行モード値（0=動作、1=設定、2=ユーザーモード）
volatile static char LED_Mode = 'a';            //LEDの動作モード 特に、設定モードは'X'
volatile static char User_LED_Mode ='a';        //ユーザーモード用の動作モード
volatile static int iCounter = 0;                //動作モードのカウンタ値
static char UpDATA[30];        //設定データ
volatile static byte MAX_brightness = 0x80;        //最大輝度
volatile static byte USE_brightness = 0x80;        //使用輝度
volatile static byte USER_brightness = 0x80;    //ユーザーモード用輝度
volatile static byte LED_length = 19;            //LED数
// TColor変数宣言
static TColor g_t_color[7];
static TColor g_t_off, g_t_on;
static TColor TCUser;                        //ユーザーモード用
volatile static int delayTime = 10;
static int intLoops, intLoopExecMode;
//AutoPlay用変数
volatile static bool AutoPlay = false;
volatile static bool bolFuncCheckOut = false;
volatile static int autoplay_start_point = 0, autoplay_end_point = 0;
volatile static int autoplay_point = 0;
volatile static int autoplay_loops = 0;    //autoplay実行回数値
volatile static int autoplay_loop_count = 0;    //autoplayカウンタ

//**************************
// Earring用変数
volatile static char earring_mode = 'a';
static TColor earring_color;
volatile static byte earring_brightness;
volatile static unsigned long earring_count = 0;
volatile static int earring_speed = 10;
volatile static const int earring_twincle_time = 30;
volatile static int earring_twincle_speed = 0;
volatile static int earring_wheel_count = 0;


//*********************************************************************
//* SETUP
//*********************************************************************
void setup() {
    //*****************************************************************
    // UserData初期化
    userdata.setup();    //UserDataセットアップ　インスタンス化後に必ず呼ぶ必要がある。
    //kurumi LED setup
    kurumiLED.setup();
    kurumiLED.setDelay(KURUMI_LED_DELAY);
    //TColor定義
    g_t_color[0].red = 0xFF;
    g_t_color[0].green = 0x00;
    g_t_color[0].blue = 0x00;
    g_t_color[1].red = 0x00;
    g_t_color[1].green = 0xFF;
    g_t_color[1].blue = 0x00;
    g_t_color[2].red = 0x00;
    g_t_color[2].green = 0x00;
    g_t_color[2].blue = 0xFF;
    g_t_color[3].red = 0xFF;
    g_t_color[3].green = 0xFF;
    g_t_color[3].blue = 0x00;
    g_t_color[4].red = 0xFF;
    g_t_color[4].green = 0x00;
    g_t_color[4].blue = 0xFF;
    g_t_color[5].red = 0x00;
    g_t_color[5].green = 0xFF;
    g_t_color[5].blue = 0xFF;
    g_t_color[6].red = 0xFF;
    g_t_color[6].green = 0xFF;
    g_t_color[6].blue = 0xFF;
    g_t_off.red = 0x00;
    g_t_off.green = 0x00;
    g_t_off.blue = 0x00;
    g_t_on.red = 0xFF;
    g_t_on.green = 0xFF;
    g_t_on.blue = 0xFF;

    kurumiLED.Red();
    delay(150);    
    //**********************************************************************
    // 格納データの読み込み
    loadUserData();
    
    kurumiLED.Green();
    delay(150);    
    //**********************************************************************
    // 初期化 ledtapeセットアップ
    ledtape.setup(LED_length, USE_brightness);    //ledtapeセットアップ　インスタンス化後に必ず呼ぶ必要がある。
    funcUpDATAInitialize();
    
    kurumiLED.Blue();
    delay(150);    
    
    Serial1.begin(9600);
    //Serial1.begin(115200);
    
    kurumiLED.White();
    delay(500);
    
    //AUTOPLAY
    if(userdata.getAutoPlay() == 1){
        StartAutoPlay(userdata.getAutoPlayFrom(), userdata.getAutoPlayTo());
    } 
    
    // Earring
    earringLED.setup(5, 9, 10, HIGH);
    earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
    
}

//*********************************************************************
//* LOOP eExecLED, eSettings, eUser
//*********************************************************************
void loop() {
    //AutoPlay
    NextAutoPlay();
    
    switch(exec_mode){
        //実行順には注意すること。
        case eExecLED:
            kurumiLED.Blue();
            funcExec();
            break;
        case eUser:
            kurumiLED.Yellow();
            funcUserExec();
            break;
        default:
            exec_mode = eExecLED;
            chkModeChange();    //障害が起きても復旧可能とする。
            //kurumiLED.Red();
            //delay(10);
            break;
    }
}

//*********************************************************************
//* 関数定義
//*********************************************************************
volatile bool loadUserData(int point){
    // 格納データの読み込み
    byte b_tmp;
    if(point == -1){    //default value
        LED_length = (byte)userdata.getLEDs();
        USE_brightness = (byte)userdata.getUseBrightness();
        LED_Mode = (char)userdata.getLedMode();
        iCounter = (int)((unsigned int)userdata.getMoveCount());
        MAX_brightness = (byte)userdata.getMaxBrightness();
        User_LED_Mode = (char)userdata.getUserLedMode();
        USER_brightness = (byte)userdata.getUserBrightness();
        userdata.getUserColor(&TCUser);
        if((int)(userdata.getExecMode()) == eExecLED){
            exec_mode = eExecLED;
        }else{
            exec_mode = eUser;
        }
        delayTime = MIN_DELAY_TIME + (unsigned int)userdata.getDelayTime();
        userdata.getEarringColor(&earring_color);
        earring_brightness = (byte)userdata.getEarringBrightness();
        earring_mode = (char)userdata.getEarringMode();
        earring_speed = (int)((unsigned int)userdata.getEarringSpeed());
    }else{
        //******************************************************
        //以下は基本的にAutoPlay用の読み込みに使用する。
        //よって、LEDの数は今のところ変更しないが、将来LEDの数も変更する場合はコメントを外す。
        //読み込み確認
        byte b_point = int2byte(point);
        if(!userdata.isEnableOffsetRange(b_point)) return false;
        if(!userdata.isInitData(b_point)) return false;
        //読み込み
        //LED_length = (byte)userdata.getLEDs(b_point);
        //ledtape.setLEDs(LED_length);
        USE_brightness = (byte)userdata.getUseBrightness(b_point);
        LED_Mode = (char)userdata.getLedMode(b_point);
        iCounter = (int)((unsigned int)userdata.getMoveCount(b_point));
        //ファンクションを作成していないので注意
        //MAX_brightness = (byte)userdata.getMaxBrightness(b_point);
        User_LED_Mode = (char)userdata.getUserLedMode(b_point);
        USER_brightness = (byte)userdata.getUserBrightness(b_point);
        userdata.getUserColor(b_point, &TCUser);
        if((int)(userdata.getExecMode(b_point)) == eExecLED){
            exec_mode = eExecLED;
        }else{
            exec_mode = eUser;
        }
        delayTime = MIN_DELAY_TIME + (unsigned int)userdata.getDelayTime(b_point);
        userdata.getEarringColor(b_point, &earring_color);
        earring_brightness = (byte)userdata.getEarringBrightness(b_point);
        earring_mode = (char)userdata.getEarringMode(b_point);
        earring_speed = (int)((unsigned int)userdata.getEarringSpeed(b_point));    

    }
    return true;
}
//　メモリの初期化
volatile void funcInitializeUserData(){
    //メモリ格納位置を変更できるようにする場合はここを注意すること。
    userdata.setMemoryOffset((byte)0);
    userdata.initialize();
}

//*********************************************************************
//* モードチェンジチェック関数
//* 戻り値：　変更があれば　true、無ければ　false
//*********************************************************************
volatile bool chkModeChange(){

	if (Serial1.available() == 0) return false;
	
	bool bResult = false;
	byte Tmp_LED_Mode;

	//autoplay対応
	StopAutoPlay();
	//先頭文字を覗く
	Tmp_LED_Mode = Serial1.peek();
	
	//有効なモードである事の検証
	while(true){
		//hand shake 方式にする。2014/02/16 文字'!'を取得したら”OK”を返し、500ms待つ。
		if(Tmp_LED_Mode == '!'){
			Serial1.write("OK");
			delay(50);
			unsigned long end_time = millis() + 500;
			//unsigned long msg_time = millis() + 10;
			Serial1.read();
			while(end_time > millis()){
				if(Serial1.available() != 0){
					Tmp_LED_Mode = Serial1.peek();
					if(Tmp_LED_Mode != '!'){
						break;
					}else{
						Serial1.read();	//もし"!"なら飲み込む
					}
					/*if(msg_time < millis()){
						Serial1.write("OK");
						delay(10);
						msg_time = millis() + 10;
					}*/
				}
			}
		}
		//noInterruptに対する読み取りの確実性から連続して文字'!'を指定することとしたので、この連続文字を除去する
		/*while(Tmp_LED_Mode == '!'){
			Serial1.read();
			if(Serial1.available() == 0) return	false;
			Tmp_LED_Mode = Serial1.peek();
			if(Tmp_LED_Mode != '!'){
				break;
			}
		}		
		*/
		
		//セッティング等モードであるか。。
		if('W' <= Tmp_LED_Mode && Tmp_LED_Mode <= 'Z'){
			break;
		}
		
		//実行モードの有効な文字であるか
		if(('a' <= Tmp_LED_Mode && Tmp_LED_Mode <= 'i') || ('o' <= Tmp_LED_Mode && Tmp_LED_Mode <= 'u')){
			break;
		}
		
		//文字が'0'の場合、その後ろの英小文字はユーザーモード用
		//但し、ここに来る場合はバグなので除去する。
		while(Tmp_LED_Mode == '0'){
			Serial1.read();
			if(Serial1.available() == 0) return	false;
			Tmp_LED_Mode = Serial1.peek();
			if('a' <= Tmp_LED_Mode && Tmp_LED_Mode <= 'z'){
				Serial1.read();
				break;
			}
		}
		
		//それ以外の文字の場合
		Serial1.read();	//関係ない文字なので読み飛ばす
		//再読み込み
		if(Serial1.available() == 0){
			return false;
		}else{
			Tmp_LED_Mode = Serial1.peek();
		}
	}
	
	//実行モードのLED MODE
	if(('a' <= Tmp_LED_Mode && Tmp_LED_Mode <= 'i') || ('o' <= Tmp_LED_Mode && Tmp_LED_Mode <= 'u')){
		LED_Mode = Tmp_LED_Mode;
		//　カウントアップ
		++iCounter;
		//空読みにより１文字消す。
		Serial1.read();
		if(exec_mode != eExecLED) exec_mode = eExecLED;
		return true;
	}
	
	//文字数が不足の場合等、次回のcall時に処理する。
	if(Tmp_LED_Mode == 'W'){
		if(Serial1.available() >= (SETTING_DATA_LENGTH + ADD_CHKMODE_DATA_LENGTH)){
			//先頭の設定文字を空読みにより１文字消す。
			Serial1.read();
			bResult = funcParamsSave();
			//ユーザモード実行中の場合はモード変更
			if(exec_mode == eUser && bResult){
				exec_mode = eExecLED;
			}
			return bResult;
		}else{
			return false;
		}
	}else if(Tmp_LED_Mode == 'X'){
		if(Serial1.available() >= (DATA_LENGTH + ADD_CHKMODE_DATA_LENGTH)){
			//先頭の設定文字を空読みにより１文字消す。
			Serial1.read();
			bResult = funcSettings();
			//ユーザモード実行中の場合はモード変更
			if(exec_mode == eUser && bResult){
				exec_mode = eExecLED;
			}
			return bResult;
		}else{
			return false;
		}
	}else if(Tmp_LED_Mode == 'Y'){
		if(Serial1.available() >= (USER_DATA_LENGTH + ADD_CHKMODE_DATA_LENGTH)){
			//先頭の設定文字を空読みにより１文字消す。
			Serial1.read();
			bResult = funcUserSettings();
			//コマンド実行中の場合はモード変更
			if(exec_mode == eExecLED && bResult){
				exec_mode = eUser;
			}
			return bResult;
		}else{
			return false;
		}
	}else if(Tmp_LED_Mode == 'Z'){
		if(Serial1.available() >= (REQUEST_DATA_LENGTH + ADD_CHKMODE_DATA_LENGTH)){
			//先頭の設定文字を空読みにより１文字消す。
			Serial1.read();
			//データ読み込み、実行。モード変更の必要がある場合は、funcRequest()の中で実施すること。
			return funcRequest();
		}else{
			return false;
		}
	}else{
		//通常、ここに来る事はない。
		kurumiLED.Red();
		delay(1000);
		Serial1.read();
		return false;
	}
};

//*********************************************************************
//AutoPlay 指定のループ数を超えたら再読み込み。
//*********************************************************************
volatile bool chkAutoPlay(){
    if(AutoPlay){
        if(autoplay_loops < 1){
            autoplay_loop_count = 0;
            bolFuncCheckOut = true;
            return true;
        }
        
        if(autoplay_loop_count < autoplay_loops - 1){
            ++autoplay_loop_count;
            return false; 
        }else{
            autoplay_loop_count = 0;
            bolFuncCheckOut = true;
            return true;
        }
    }
    return false;
}
// AutoPlayを実行する
volatile void StartAutoPlay(byte strat_mem_pos, byte end_mem_pos){
    if(AutoPlay) return;
	if(strat_mem_pos > UserData::MAX_OFFSET_VALUE) return;
	
    //範囲に有効なデータが１つは存在する事を確認する
    if(strat_mem_pos >= end_mem_pos) return;
    if(end_mem_pos > UserData::MAX_OFFSET_VALUE) end_mem_pos = UserData::MAX_OFFSET_VALUE;
    bool ExistsData = false;
    for(int i = strat_mem_pos; i <= end_mem_pos; ++i){
        if(userdata.isInitData(i)){
            ExistsData = true;
            break;
        }
    }
	if(!ExistsData) return;
	
    //範囲に有効なループカウントが１つは存在する事を確認する。
    bool ExistsLoopNumber = false;
    for(int i = strat_mem_pos; i <= end_mem_pos; ++i){
        if(userdata.getLoops(i) > 0){
            ExistsLoopNumber = true;
            break;
        }
    }   
	if(!ExistsLoopNumber) return;	
    
	if(ExistsData && ExistsLoopNumber){
        autoplay_start_point = strat_mem_pos;
        autoplay_end_point = end_mem_pos;
        AutoPlay = true;
        autoplay_loop_count = 0;
        autoplay_point = autoplay_start_point - 1;
        bolFuncCheckOut = false;
    }
}
//次に進める
volatile void NextAutoPlay(){
    //kurumiLED.Red();
    //delay(1000);
    if(!AutoPlay) return;
    bolFuncCheckOut = false;
    do{
        if(autoplay_point >= autoplay_end_point){
            autoplay_point = autoplay_start_point;
        }else{
            ++autoplay_point;
        }
    }while(!loadUserData(autoplay_point));
    autoplay_loops = userdata.getLoops(int2byte(autoplay_point));
    if(userdata.getLoopExecMode(int2byte(autoplay_point)) == 0x00){;
        exec_mode = eExecLED;
    }else{
        exec_mode = eUser;
    }
    //earring update
    earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
}
// AutoPlayを止める
volatile void StopAutoPlay(){
    if(!AutoPlay) return;
    AutoPlay = false;
    autoplay_loops = 0;
    autoplay_loop_count = 0;
    userdata.setAutoPlay(0x00);
    loadUserData();
    //earring update
    earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
}
//****************************************************************
//* セッティング関数(9byte)
//　認識文字  start:'W' end:DATA_END_MARK
// Setting String Format 合計30byte 
// 0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte        
// "L00"		LED個数		Loop回数				***LED 設定　&　保存
// "A00"		From		To						***AutoPlay 設定　&　保存
volatile bool funcParamsSave(){
    //初期化と読み込み
    if(!readSerialData(SETTING_DATA_LENGTH + ADD_FUNC_DATA_LENGTH)) return false;
   //読み込み判定、終わりが「END」であるか？
    if(UpDATA[SETTING_DATA_LENGTH] == DATA_END_MARK){
		// L00 LED　設定　&　保存
        if(UpDATA[0] == 'L' && UpDATA[1] == '0' && UpDATA[2] == '0'){
            int intLEDs = getNumberTo3CharArray(UpDATA, 3); 
			intLoops = getNumberTo3CharArray(UpDATA, 6); 
			//***********************************************************
			//LED Limitter
			if(intLEDs > MAX_OF_LED_NUMBERS) intLEDs = MAX_OF_LED_NUMBERS;
			if(intLEDs > 0x00 && intLEDs <= 0xFF && intLEDs != LED_length){
				//消灯
				ledtape.clearAllColors();
				ledtape.send();
				LED_length = int2byte(intLEDs);
				userdata.setLEDs(LED_length);
				ledtape.setLEDs(LED_length);
			}
			userdata.setUseBrightness(USE_brightness); 
			userdata.setMaxBrightness(MAX_brightness);
			int intDelayTime = delayTime - MIN_DELAY_TIME;
			if(0 <= intDelayTime && intDelayTime <= 100){
				userdata.setDelayTime(int2byte(intDelayTime));              
			}
			//**********************************************
			//動作モードはここで保存
			//LED動作モード格納
			if((int)'a' <= LED_Mode && LED_Mode <= (int)'i' || (int)'o' <= LED_Mode && LED_Mode <= (int)'u'){
				userdata.setLedMode(LED_Mode);
			}
			if((int)'a' <= LED_Mode && LED_Mode <= (int)'d'){
				//データ格納 カウント値は、全モードの最小公倍数で0～255の間とする。
				//現段階では、iMode（8種類）と5種類の動作切替　よって40の剰余を保存する。
				userdata.setMoveCount((byte)(iCounter % (iMod * 5)));
			}
			userdata.setExecMode((byte)eExecLED);                 
			//AutoPlayパラメータ保存
			userdata.setLoops(int2byte(intLoops));
			userdata.setLoopExecMode(int2byte(intLoopExecMode));
			return true;
		}else if(UpDATA[0] == 'A' && UpDATA[1] == '0' && UpDATA[2] == '0'){
		    byte b_from = int2byte(getNumberTo3CharArray(UpDATA, 3));
            byte b_to = int2byte(getNumberTo3CharArray(UpDATA, 6));
            userdata.setAutoPlay(0x01);
			userdata.setAutoPlayFrom(b_from);
			userdata.setAutoPlayTo(b_to);
			//Execute Auto Play
			StartAutoPlay(b_from, b_to);
			return true;
		}
	}
}
//****************************************************************
//* セッティング関数(6byte)
//　認識文字    start:'X'  end:DATA_END_MARK
// Setting String Format 合計30byte 
// 0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte        
// "L01"		LED個数		
// "L02"		最大輝度		
// "L03"		使用輝度		
// "L04"		DelayTime	
// "L05"		Loop回数	
// "L06"		AutoMode	        ***実行モードかユーザモードか
// "I00"		0			        ***Initialize
// "I01"		MemOffset	        ***MemoryOffset　保存
// "I02"		MemOffset	        ***メモリクリア
// "I20"		0			        ***イヤリング保存
// "I21"		動作Mode	        ***イヤリング動作モード
// "I22"		red			        ***イヤリング　赤
// "I23"		green		        ***イヤリング　緑
// "I24"		blue		        ***イヤリング　青
// "I25"		輝度		        ***イヤリング　輝度
// "I26"		Speed		        ***イヤリング　スピード　　　!!!AutoPlayは別にする。
//****************************************************************
volatile bool funcSettings(){
    //初期化と読み込み
    if(!readSerialData(DATA_LENGTH + ADD_FUNC_DATA_LENGTH)) return false;
	
   //読み込み判定、終わりが「END」であるか？
    if(UpDATA[DATA_LENGTH] == DATA_END_MARK){
        int intLEDs, intDelayTime;
        switch(UpDATA[0]){
            //*********************************************************
            //* LED Params 設定
            //*********************************************************
            case 'L':
				// L01 LED個数設定
                if(UpDATA[1] == '0' && UpDATA[2] == '1'){
                    intLEDs = getNumberTo3CharArray(UpDATA, 3); 
					if(intLEDs == (int)ledtape.getLEDs()) return false;
                    //***********************************************************
                    //LED Limitter
                    if(intLEDs > MAX_OF_LED_NUMBERS) intLEDs = MAX_OF_LED_NUMBERS;
                    if(intLEDs > 0x00 && intLEDs <= 0xFF && intLEDs != LED_length){
						//消灯
						ledtape.clearAllColors();
						ledtape.send();
						LED_length = int2byte(intLEDs);
                        userdata.setLEDs(LED_length);
                    }
					return true;
				}else if(UpDATA[1] == '0' && UpDATA[2] == '2'){
					byte tmp_Max_brightness = int2byte(getNumberTo3CharArray(UpDATA, 3));
					if(tmp_Max_brightness != MAX_brightness){
						MAX_brightness = tmp_Max_brightness;
						if(MAX_brightness < USE_brightness){
							USE_brightness = MAX_brightness;
							return true;
						}
					}
					return false;
				}else if(UpDATA[1] == '0' && UpDATA[2] == '3'){
					byte tmp_Use_brightness = int2byte(getNumberTo3CharArray(UpDATA, 3)); 
					if(tmp_Use_brightness > MAX_brightness){
						tmp_Use_brightness = MAX_brightness;
					}
					if(tmp_Use_brightness != USE_brightness){
						USE_brightness = tmp_Use_brightness;
						return true;
					}
					return false;
				}else if(UpDATA[1] == '0' && UpDATA[2] == '4'){
					int tmp_delay_time = getNumberTo3CharArray(UpDATA, 3);
					if(tmp_delay_time != (delayTime - MIN_DELAY_TIME)){
						delayTime = MIN_DELAY_TIME + getNumberTo3CharArray(UpDATA, 3);
					}
					return false;
				}else if(UpDATA[1] == '0' && UpDATA[2] == '5'){
					intLoops = getNumberTo3CharArray(UpDATA, 3);
					return false;
				}else if(UpDATA[1] == '0' && UpDATA[2] == '6'){
					intLoopExecMode = getNumberTo3CharArray(UpDATA, 3);
					return false;
				}
			//*********************************************************
            //* Initialize
            //*********************************************************
            case 'I':
                //初期化
                if(UpDATA[1] == '0' && UpDATA[2] == '0'){
                    funcInitializeUserData();
                    //**********************************************************************
                    // 格納データの読み込み
                    loadUserData();
                    ledtape.setLEDs(LED_length);
                    //earring update
                    earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
                    return true;   
					
                //メモリ位置変更
                }else if(UpDATA[1] == '0' && UpDATA[2] == '1'){
                    byte b_tmp = int2byte(getNumberTo3CharArray(UpDATA, 3));
					//if(userdata.getMemoryOffset() == b_tmp) return false;
                    byte first_LEDs = 19;
                    bool first_setting = !userdata.isInitData(b_tmp);
                    //メモリが設定が初めての場合
                    if(first_setting && b_tmp != 0){    
                        first_LEDs = userdata.getLEDs(0);
                    }
                    userdata.setMemoryOffset(b_tmp);
                    //メモリが設定が初めての場合
                    if(first_setting && b_tmp != 0){
                        userdata.setLEDs(first_LEDs);
                    }
                    //**********************************************************************
                    // 格納データの読み込み
                    loadUserData();
                    ledtape.setLEDs(LED_length);
                    //earring update
                    earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
                    return true;
					
				//メモリクリア
				}else if(UpDATA[1] == '0' && UpDATA[2] == '2'){
                    byte b_tmp = int2byte(getNumberTo3CharArray(UpDATA, 3));
                    if(b_tmp == userdata.getMemoryOffset()) return false;
                    userdata.clear(b_tmp);
                    return false;

				// earring設定 保存
				}else if(UpDATA[1] == '2' && UpDATA[2] == '0'){	
                    userdata.setEarringMode(earring_mode);
                    userdata.setEarringColor(&earring_color);
                    userdata.setEarringBrightness(earring_brightness);
                    userdata.setEarringSpeed(earring_speed);
				// earring設定	
                }else if(UpDATA[1] == '2' && UpDATA[2] == '1'){	
					earring_mode = (char)UpDATA[5];
				}else if(UpDATA[1] == '2' && UpDATA[2] == '2'){	
					earring_color.red = (byte)(getNumberTo3CharArray(UpDATA, 3));
				}else if(UpDATA[1] == '2' && UpDATA[2] == '3'){
                    earring_color.green = (byte)(getNumberTo3CharArray(UpDATA, 3));
				}else if(UpDATA[1] == '2' && UpDATA[2] == '4'){
                    earring_color.blue = (byte)(getNumberTo3CharArray(UpDATA, 3));
				}else if(UpDATA[1] == '2' && UpDATA[2] == '5'){
                    earring_brightness = (byte)(getNumberTo3CharArray(UpDATA, 3));
				}else if(UpDATA[1] == '2' && UpDATA[2] == '6'){
                    earring_speed = (byte)(getNumberTo3CharArray(UpDATA, 3));
				}
				earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
				return false;
				
            default:
                break;
        }
    }
    return false;
}
//****************************************************************
//* 送信関数
//　認識文字    start:’Z'  end:DATA_END_MARK
// Request String Format 端末側 3byte 
// 0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte        
// "R01"																												***LED設定等送信要求
// "R02"		LED個数		最大輝度		使用輝度		ﾒﾓﾘｵﾌｾｯﾄ値	DelayTime	Loop回数	AutoMode						***返信    
// "R03"																												***ユーザモード設定送信要求
// "R04"		動作Mode	輝度		R値			G値			B値			DelayTime									***返信    
// "R05"																												***AUTOPLAY　FROM　TO 設定送信要求
// "R06"		FROM		TO																							***返信    
// "R21"																												***イヤリング色・輝度送信要求
// "R22"		動作Mode	R値			G値			B値			輝度		speed										***返信    
// "R23"																												***イヤリング色・輝度リセット要求
// "R24"																												***イヤリングOFF要求
//****************************************************************
volatile bool funcRequest(){
    //初期化と読み込み
    if(!readSerialData(REQUEST_DATA_LENGTH + ADD_FUNC_DATA_LENGTH)) return false;
    //読み込み判定、終わりが「END」であるか？
    if(UpDATA[REQUEST_DATA_LENGTH] == DATA_END_MARK){
        if(UpDATA[0] == 'R'){
            //R01の場合の返信
            if(UpDATA[1] == '0' && UpDATA[2] == '1'){
				intLoops = userdata.getLoops();
				intLoopExecMode = userdata.getLoopExecMode();
				//MAX_brightness = userdata.getMaxBrightness();
				//USE_brightness = userdata.getUseBrightness();
				//delayTime = MIN_DELAY_TIME + (unsigned int)userdata.getDelayTime();
                Serial1.print("R02");
                Serial1.print(funcNum2Str(LED_length,3));
                Serial1.print(funcNum2Str(MAX_brightness,3));
                Serial1.print(funcNum2Str(USE_brightness,3));
                Serial1.print(funcNum2Str(userdata.getMemoryOffset(),3));
                Serial1.print(funcNum2Str(delayTime - MIN_DELAY_TIME,3));
                Serial1.print(funcNum2Str(intLoops,3));
                Serial1.print(funcNum2Str(intLoopExecMode,3));
                Serial1.print(DATA_END_MARK);
                if(exec_mode != eExecLED) exec_mode = eExecLED;
				return true;
            }
            //R03の場合の返信
            if(UpDATA[1] == '0' && UpDATA[2] == '3'){
                //動作Mode    輝度        R値            G値            B値
                userdata.getUserColor(&TCUser);
                Serial1.print("R04");
                Serial1.print("00");
                Serial1.print((char)userdata.getUserLedMode());
                Serial1.print(funcNum2Str(userdata.getUserBrightness(),3));
                Serial1.print(funcNum2Str(TCUser.red,3));
                Serial1.print(funcNum2Str(TCUser.green,3));
                Serial1.print(funcNum2Str(TCUser.blue,3));
                Serial1.print(funcNum2Str(userdata.getDelayTime(),3));
                Serial1.print(DATA_END_MARK);
                if(exec_mode != eUser) exec_mode = eUser;
                return true;
            }
            //R05の場合の返信
            if(UpDATA[1] == '0' && UpDATA[2] == '5'){
                //動作Mode    輝度        R値            G値            B値
                Serial1.print("R06");
                Serial1.print(funcNum2Str(userdata.getAutoPlayFrom(),3));
                Serial1.print(funcNum2Str(userdata.getAutoPlayTo(),3));
                Serial1.print(DATA_END_MARK);
                if(exec_mode != eExecLED){
                    exec_mode = eExecLED;
                    return true;
                }else{
                    return false;
                }
            }
            //R21 or R23 or R24 の場合の返信
            if(UpDATA[1] == '2' && ( UpDATA[2] == '1' || UpDATA[2] == '3' || UpDATA[2] == '4')){
                if(UpDATA[2] == '3'){    //リセット
                    earring_mode = (char)userdata.getEarringMode();
                    userdata.getEarringColor(&earring_color);
                    earring_brightness = (byte)userdata.getEarringBrightness();
                    earring_speed = (int)((unsigned int)userdata.getEarringSpeed());
                }
				if(UpDATA[2] == '4'){    //OFF
					earring_color.red = 0x00;
					earring_color.green = 0x00;
					earring_color.blue = 0x00;
					earring_brightness = 0x00;
                }
                //R値            G値            B値            輝度
                Serial1.print("R22");
                Serial1.print("00");
                Serial1.print(earring_mode);
                Serial1.print(funcNum2Str(earring_color.red,3));
                Serial1.print(funcNum2Str(earring_color.green,3));
                Serial1.print(funcNum2Str(earring_color.blue,3));
                Serial1.print(funcNum2Str(earring_brightness,3));
                Serial1.print(funcNum2Str(earring_speed,3));
                Serial1.print(DATA_END_MARK);
                if(exec_mode != eExecLED) exec_mode = eExecLED;
                return true;
            }
        }
    }
    return false;
}
//****************************************************************
//* ユーザーモードセッティング関数 (6byte)
//　認識文字    start:’Y’    end:DATA_END_MARK
// UserSetting String Format 合計30byte 
//　0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte        
// "U01"		動作Mode	
// "U02"		輝度		
// "U03"		R値			
// "U04"		G値			
// "U05"		B値			
// "U06"		DelayTime	
// "U10"		0					//設定値保存
//****************************************************************
volatile bool funcUserSettings(){
    if(exec_mode != eUser) exec_mode = eUser; 
    //**************************************
    // 読み込み 6 byte 【フォーマットを変更した場合はここも変更すること。】
    if(!readSerialData(USER_DATA_LENGTH + ADD_FUNC_DATA_LENGTH)) return false;
    //読み込み判定、終わりが「END」であるか？
	if(UpDATA[USER_DATA_LENGTH] == DATA_END_MARK){
		if(UpDATA[1] == '0' && UpDATA[2] == '1'){
			byte bMM = (byte)UpDATA[5];
			if(User_LED_Mode == (char)bMM) return false;
			if('a' <= bMM && bMM <= 'l'){
				User_LED_Mode = (char)bMM;
				return true;
			}
			return false;
		}else if(UpDATA[1] == '0' && UpDATA[2] == '2'){
			int tmp_bright = int2byte(getNumberTo3CharArray(UpDATA, 3));
			if(tmp_bright == USER_brightness) return false;
			USER_brightness = tmp_bright;
			return true;
		}else if(UpDATA[1] == '0' && UpDATA[2] == '3'){
			byte tmp_red = int2byte(getNumberTo3CharArray(UpDATA, 3));
			if(tmp_red == TCUser.red) return false;
			TCUser.red = tmp_red;
			return true;
		}else if(UpDATA[1] == '0' && UpDATA[2] == '4'){
			byte tmp_green = int2byte(getNumberTo3CharArray(UpDATA, 3));
			if(tmp_green == TCUser.green) return false;
			TCUser.green = tmp_green;
			return true;
		}else if(UpDATA[1] == '0' && UpDATA[2] == '5'){
			byte tmp_blue = int2byte(getNumberTo3CharArray(UpDATA, 3));
			if(tmp_blue == TCUser.blue) return false;
			TCUser.blue = tmp_blue;
			return true;
		}else if(UpDATA[1] == '0' && UpDATA[2] == '6'){
			delayTime = MIN_DELAY_TIME + getNumberTo3CharArray(UpDATA, 3); 
			return false;
		}else if(UpDATA[1] == '1' && UpDATA[2] == '0'){
			//動作モード保存
            userdata.setUserLedMode((byte)User_LED_Mode);                
            userdata.setUserBrightness(USER_brightness);
            userdata.setUserColor(&TCUser);
            userdata.setExecMode((byte)eUser);
			int intDelayTime = delayTime - MIN_DELAY_TIME;
            if(0 <= intDelayTime && intDelayTime <= 100){
                userdata.setDelayTime(int2byte(intDelayTime));                
            } 
			return false;
		}
	}else{
        return false;
    }
}

//****************************************************************
//* シリアルポートからのデータ読み込み関数
//* UpDATAに引数「ReadByte」読み込む　但し、MAXはDATA_LENGTH
//* 引数には、先頭１文字を除いた数を指定すること。
//****************************************************************
volatile bool readSerialData(int ReadByte){
	//kurumiLED.Red();
	//delay(1000);
	if(!(Serial1.available() >= (ReadByte))) return false;	
    //初期化
	funcUpDATAInitialize();
    for(int j = 0; j < ReadByte; ++j){
        UpDATA[j] = Serial1.read();
    }
	//kurumiLED.White();
	//delay(1000);
    return true;
}

//****************************************************************
//* LED 発光動作実行関数
//****************************************************************
volatile void funcExec(){
    if(exec_mode != eExecLED) return;
        
    while(true){
        if(exec_mode != eExecLED) return;
        if(bolFuncCheckOut) return;
        int i = 0, j = 0;
        if((iCounter % (iMod * 5)) == 0) iCounter = (iMod * 5);
        //***********************************
        //-->輝くカチューシャ
        // 互換を維持したかったが、数字のため誤動作の原因になっている。よって英小文字のみの指定に変更する。2013/12/11
        switch(LED_Mode){
            case 'o':
                funcEguchiStar(g_t_color[0], LED_Mode, iCounter);
                break;
            case 'p':
                funcEguchiStar(g_t_color[1], LED_Mode, iCounter);
                break;
            case 'q':
                funcEguchiStar(g_t_color[2], LED_Mode, iCounter);    
                break;
            case 'r':
                funcEguchiStar(g_t_color[3], LED_Mode, iCounter);
                break;
            case 's':
                funcEguchiStar(g_t_color[4], LED_Mode, iCounter); 
                break;
            case 't':
                funcEguchiStar(g_t_color[5], LED_Mode, iCounter);  
                break;
            case 'u':
                funcEguchiStar(g_t_color[6], LED_Mode, iCounter);
                break;
            //<-- 輝くカチューシャ
            //*****************************
            case 'a':
                i = iCounter % iMod;
                j = (iCounter / iMod) % 5;
                if(j == 0){
                    if( i < (iMod - 1)){
                        funcStar(g_t_color[i], LED_Mode, iCounter, false, eNormal);
                    }else{
                        funcStar(g_t_color[0], LED_Mode, iCounter, true, eNormal);
                    }
                }else if(j == 1){
                    if( i < (iMod - 1)){
                        funcStar(g_t_color[i], LED_Mode, iCounter, false, eReverse);
                    }else{
                        funcStar(g_t_color[0], LED_Mode, iCounter, true, eReverse);
                    }                
                }else if(j == 2){
                    if( i < (iMod - 1)){
                        funcStar(g_t_color[i], LED_Mode, iCounter, false, eDouble);
                    }else{
                        funcStar(g_t_color[0], LED_Mode, iCounter, true, eDouble);
                    }                
                }else if(j == 3){
                    if( i < (iMod - 1)){
                        funcStar(g_t_color[i], LED_Mode, iCounter, false, eDoubleReverse);
                    }else{
                        funcStar(g_t_color[0], LED_Mode, iCounter, true, eDoubleReverse);
                    }                
                }else if(j == 4){
                    if( i < (iMod - 1)){
                        funcStar(g_t_color[i], LED_Mode, iCounter, false, eMirror);
                    }else{
                        funcStar(g_t_color[0], LED_Mode, iCounter, true, eMirror);
                    }                
                }            
                break;
            case 'b':
                i = iCounter % iMod;
                if( i < (iMod - 1)){
                    funcLighting(g_t_color[i], LED_Mode, iCounter, false);
                }else{
                    funcLighting(g_t_color[0], LED_Mode, iCounter, true); //randomはminからmax-1の間の整数
                }
                break;
            case 'c':
                i = iCounter % iMod;
                if( i < (iMod -1)){
                    funcTwincleStar(g_t_color[i], LED_Mode, iCounter);
                }else{
                    funcTwincleStar(g_t_color[0], LED_Mode, iCounter, true); //先頭カラーは何でもよい。
                }
                break;
            case 'd':
                i = iCounter % iMod;
                j = (iCounter / iMod) % 5;                
                if(j == 0){
                    if( i < (iMod - 1)){
                        funcShootingStar(g_t_color[i], LED_Mode, iCounter, false, eNormal);
                    }else{
                        funcShootingStar(g_t_color[0], LED_Mode, iCounter, true, eNormal); 
                    }
                }else if(j == 1){
                    if( i < (iMod - 1)){
                        funcShootingStar(g_t_color[i], LED_Mode, iCounter, false, eReverse);
                    }else{
                        funcShootingStar(g_t_color[0], LED_Mode, iCounter, true, eReverse);
                    }                
                }else if(j == 2){
                    if( i < (iMod - 1)){
                        funcShootingStar(g_t_color[i], LED_Mode, iCounter, false, eDouble);
                    }else{
                        funcShootingStar(g_t_color[0], LED_Mode, iCounter, true, eDouble);
                    }                
                }else if(j == 3){
                    if( i < (iMod - 1)){
                        funcShootingStar(g_t_color[i], LED_Mode, iCounter, false, eDoubleReverse);
                    }else{
                        funcShootingStar(g_t_color[0], LED_Mode, iCounter, true, eDoubleReverse);
                    }                
                }else if(j == 4){
                    if( i < (iMod - 1)){
                        funcShootingStar(g_t_color[i], LED_Mode, iCounter, false, eMirror);
                    }else{
                        funcShootingStar(g_t_color[0], LED_Mode, iCounter, true, eMirror);
                    }                
                }            
                break;
            case 'e':
                funcLightingAllColorChg(LED_Mode);
                break;
            case 'f':
                j = iCounter % 4;
                if(j == 0){
                    funcColorflowChg(LED_Mode, iCounter, eNormal);
                }else if(j == 1){
                    funcColorflowChg(LED_Mode, iCounter, eReverse);            
                }else if(j == 2){
                    funcColorflowChg(LED_Mode, iCounter, eDouble);            
                }else if(j == 3){
                    funcColorflowChg(LED_Mode, iCounter, eDoubleReverse);        
                }                    
                break;
            case 'g':
                funcGradient(LED_Mode); 
                break;
            case 'h':
                funcRainbow(LED_Mode);
                break;
            case 'i':
                funcOff(LED_Mode, 50);
                break;
            default:
				chkModeChange();
                break;
        }
    }
}
//****************************************************************
//* LED 発光動作ユーザーモード実行関数
//****************************************************************
volatile void funcUserExec(){
    if(exec_mode != eUser) return;
      
    while(true){
        if(exec_mode != eUser) return;
        if(bolFuncCheckOut) return;
        switch(User_LED_Mode){
            case 'a':
                funcStar_User(User_LED_Mode, eNormal);
                break;
            case 'b':
                funcLighting_User(User_LED_Mode);
                break;
            case 'c':
                funcTwincleStar_User(User_LED_Mode);
                break;
            case 'd':
                funcShootingStar_User(User_LED_Mode, eNormal);
                break;
            case 'e':
                funcStar_User(User_LED_Mode, eReverse);
                break;
            case 'f':
                funcStar_User(User_LED_Mode, eDouble);
                break;
            case 'g':
                funcStar_User(User_LED_Mode, eDoubleReverse);
                break;
            case 'h':
                funcStar_User(User_LED_Mode, eMirror);
                break;
            case 'i':
                funcShootingStar_User(User_LED_Mode, eReverse);
                break;
            case 'j':
                funcShootingStar_User(User_LED_Mode, eDouble);
                break;
            case 'k':
                funcShootingStar_User(User_LED_Mode, eDoubleReverse);
                break;
            case 'l':
                funcShootingStar_User(User_LED_Mode, eMirror);
                break;
            default:
				chkModeChange();
                break;
        }
    }
}

//****************************************************************
//* LED 発光動作関数
//* 動作関数にはモードチェック関数【chkModeChange】及び【chkAutoPlay】を含める事
//****************************************************************

//星（江口さんのやつのループ）
volatile void funcStar(TColor tc, char mc, int mi, bool b_random, Movement mm){
    ledtape.setAllBrightness(USE_brightness);
    int rem = 0, my_LEDs = LED_length;
    int star_count = 5;
    int current_pos = 0;
    
    if(mm == eDouble || mm == eDoubleReverse){
        rem = LED_length % 2;
        my_LEDs = (LED_length / 2) + rem;
    }
    
    while(true){
        if(current_pos == (my_LEDs + star_count)){
            if(b_random){
                randomSeed(millis());
                tc = g_t_color[random(0, (iMod -1))];
            }
            current_pos = 0; 
            if(chkAutoPlay()) return;
        }
        ledtape.clearAllColors();
        for(int i = 0; i < (my_LEDs + star_count); ++i){
            if(i > (current_pos - star_count) && i <= current_pos){
                if(i < my_LEDs) ledtape.setColor(i, tc, LedTape::NoClearAndNoSend);
            }else{
                if(i < my_LEDs) ledtape.setColor(i, g_t_off, LedTape::NoClearAndNoSend);
            }
        }        
        ++current_pos;
        //reverse?
        if(mm == eReverse) ledtape.reverse(LED_length, LedTape::NoClearAndNoSend);
        //double?
        if(mm == eDouble){
            ledtape.reverseCopy(my_LEDs, (rem == 1));
        }
        //double reverse?
        if(mm == eDoubleReverse){
            ledtape.reverse(my_LEDs, LedTape::NoClearAndNoSend);
            ledtape.reverseCopy(my_LEDs, (rem == 1));
        }
        //mirror?
        if(mm == eMirror){
            ledtape.mirror(LedTape::NoClearAndNoSend);
        }
        ledtape.send();
        if(my_delay_withChkModeChange(delayTime)) return;
        if(iCounter != mi) return;    
    }
}

//点灯
volatile void funcLighting(TColor tc, char mc, int mi, bool b_random){
    ledtape.setAllBrightness(USE_brightness);
    ledtape.setAllColors(tc);
    int autoplay_count = 0;
    while(true){
        if(my_delay_withChkModeChange(delayTime)) return;
        if(iCounter != mi) return;
        ++autoplay_count;
        if(autoplay_count == 10){
            autoplay_count = 0;
            if(chkAutoPlay()) return;
        }
    }
}

//Twincle
volatile void funcTwincleStar(TColor tc, char mc, int mi, bool b_randam){
    ledtape.setAllBrightness(USE_brightness);
    int my_delay_time = 0;
    int autoplay_count = 0;
    while(true){
        my_delay_time = delayTime > MIN_DELAY_TIME ? delayTime - MIN_DELAY_TIME : 0;
        randomSeed(millis());
        int led = random(0, LED_length);
        if(b_randam){
            int j = random(0, 7);
            ledtape.setColor(led, g_t_color[j], LedTape::ClearAllAndAutoSend);
        }else{
            ledtape.setColor(led, tc, LedTape::ClearAllAndAutoSend);
        }
        //my_delay(1);
        ledtape.clearAllColors();
        int randam_delay_time = random(5, 150);
		if(my_delay_withChkModeChange(randam_delay_time + my_delay_time)) return;
        if(iCounter != mi) return;
        ++autoplay_count;
        if(autoplay_count == LED_length){
            autoplay_count = 0;
            if(chkAutoPlay()) return;
        }
    }
    
}

//流星
volatile void funcShootingStar(TColor tc, char mc, int mi, bool b_randam, Movement mm){
    //色設定
    ledtape.setAllColors(tc, LedTape::ClearAll);
    ledtape.setAllBrightness(0);
    
    int rem = 0, my_LEDs = LED_length;
    if(mm == eDouble || mm == eDoubleReverse){
        rem = LED_length % 2;
        my_LEDs = (LED_length / 2) + rem;
    }
    
    //輝度の配列を作成し計算する。 y = 255 / X^2 で輝度を計算する。よって、最大でも16個以上は輝度が0になる。
    int LightCount = 0;
    for(int i = 0; i < 20; ++i){
        if((USE_brightness / pow((i + 1), 2)) > 0) ++LightCount;
    }
    int arrCnt = my_LEDs + LightCount;
    byte br[arrCnt];
    for(int i = 0; i < arrCnt; ++i){
        br[i] = (byte)(USE_brightness / pow((i + 1), 2));
    }
    
    int rCnt = 0;
    while(true){
        //ランダムの場合は、１送信毎に流れる星の色を変える。※１回流れる毎に変える場合はコメントのゼロリセットの場所で色を指定すること。
        if(b_randam){
            randomSeed(millis());
            int k = random(0, 7);
            ledtape.setAllColors(g_t_color[k], LedTape::ClearAll);
        }
        //先頭位置
        int j = rCnt;
        ledtape.clearAllBrightness();
        for(int i = 0; i < arrCnt; ++i){    //外側のループは星の長さ＋LEDの個数の配列の添え字
            if(j >= 0 && j < my_LEDs){
                // j 位置のLEDの輝度を br[i] に設定
                ledtape.setBrightness(j, br[i]);
            }
            --j;
        }
        ++rCnt;
        //ゼロリセット　配列長でリセットする。
        if(rCnt == arrCnt){
            rCnt = 0;
            if(chkAutoPlay()) return;
        }
        //reverse?
        if(mm == eReverse) ledtape.brightnessReverse(LED_length);
        //double?
        if(mm == eDouble){
            ledtape.brightnessReverseCopy(my_LEDs, (rem == 1));
        }
        //double reverse?
        if(mm == eDoubleReverse){
            ledtape.brightnessReverse(my_LEDs);
            ledtape.brightnessReverseCopy(my_LEDs, (rem == 1));
        }
        //mirror?
        if(mm == eMirror){
            ledtape.brightnessMirror();
        }
        //送信
        ledtape.send();
		if(my_delay_withChkModeChange(delayTime)) return;
        if(iCounter != mi) return;
    }    
}

//点灯 色が流れるように順々に変わる
volatile void funcColorflowChg(char mc, int mi, Movement mm){
    ledtape.setAllBrightness(USE_brightness);
    
    int rem = 0, my_LEDs = LED_length;
    if(mm == eDouble || mm == eDoubleReverse){
        rem = LED_length % 2;
        my_LEDs = (LED_length / 2) + rem;
    }
    
    while(true){
        int prev_c = 0;
        for(int j = 0; j < 7; ++j){        
            for(int i = 0; i < (my_LEDs + rem) ; ++i){
                ledtape.setAllColors(g_t_color[( j == 0 ? 6: (j - 1))], LedTape::NoClearAndNoSend);
                for(int k = 0; k <= i; ++k){
                    ledtape.setColor(k, g_t_color[j], LedTape::NoClearAndNoSend);
                }
                //reverse?
                if(mm == eReverse) ledtape.reverse(LED_length, LedTape::NoClearAndNoSend);
                //double?
                if(mm == eDouble){
                    ledtape.reverseCopy((my_LEDs), (rem == 1));
                }
                //double reverse?
                if(mm == eDoubleReverse){
                    ledtape.reverse(my_LEDs, LedTape::NoClearAndNoSend);
                    ledtape.reverseCopy(my_LEDs, (rem == 1));
                }
                //送信
                ledtape.send();                    
				if(my_delay_withChkModeChange(delayTime)) return;
                if(iCounter != mi) return;
            }
        }
        if(chkAutoPlay()) return;
    }
}

// 色
volatile void funcLightingAllColorChg(char mc){
    ledtape.setAllBrightness(USE_brightness);
    while(true){
        for(int i = 0; i < 7; ++i){
            ledtape.setAllColors(g_t_color[i]);
			if(my_delay_withChkModeChange(delayTime * 40)) return;
        }
        if(chkAutoPlay()) return;
    }
}

// 虹色表示
volatile void funcRainbow(char mc){
    ledtape.setAllBrightness(USE_brightness);
    int my_delay_time = 0;
    while(true)
    {
        for(int i = 0 ; i < 5 ; i++)
        {
            my_delay_time = delayTime > MIN_DELAY_TIME ? delayTime - MIN_DELAY_TIME : 0;
            my_delay_time = my_delay_time < 26 ? 0 : my_delay_time - 26;    //デフォルトのスピード調整
            rainbowCycle(i + my_delay_time, mc);
            //注意：rainbowCycle内でchkModeChangeを呼んでいるのでここでは比較のみ。
            if(LED_Mode != mc) return;
            if(exec_mode != eExecLED) return;
            //注意：rainbowCycle内でchkAutoPlayを呼んでいる。
            if(bolFuncCheckOut) return;
        }
    }
}

volatile void funcGradient(char mc){
    ledtape.setAllBrightness(USE_brightness);
    TColor tmp_c[LED_length];
    TColor tc;
    byte x;
    int value_of_span = 10;
    for(int i = 0; i < LED_length; ++i){
        tmp_c[i].red = 0xFF;
        tmp_c[i].green = 0xFF;
        tmp_c[i].blue = 0xFF;
    }
    while(true)
    {
        //全点灯からスタート
        tc.red = 0xFF;
        tc.green = 0xFF;
        tc.blue = 0xFF;
        ledtape.setAllColors(tc);
		if(my_delay_withChkModeChange(delayTime)) return;
        //外ループ
        for(int i = 0; i < 3; ++i){
            for(int j = 0; j < 4; ++j){
                x = j < 2 ? 0xFF : 0;
                do{
                    if(j < 2){
                        //--x;
                        x = (x - value_of_span) < 0 ? 0 : (x - value_of_span);
                    }else{
                        //++x;
                        x = (x + value_of_span) > 0xFF ? 0xFF : (x + value_of_span);
                    }
                    if(j == 0 || j == 2){
                        switch(i){
                            case 0:
                                tc.red = x;
                                break;
                            case 1:
                                tc.green = x;
                                break;
                            case 2:
                                tc.blue = x;
                                break;
                        }
                    }else{
                        switch(i){
                            case 0:
                                tc.green = x;
                                break;
                            case 1:
                                tc.blue = x;
                                break;
                            case 2:
                                tc.red = x;
                                break;
                        }
                    }
                    //カラーを一つずらす
                    for(int k = LED_length - 1; k > 0; --k){
                        tmp_c[k] = tmp_c[k - 1];
                    }
                    tmp_c[0].red = tc.red;
                    tmp_c[0].green = tc.green;
                    tmp_c[0].blue = tc.blue;
                    for(int k = 0; k < LED_length; ++k){
                        ledtape.setColor(k, tmp_c[k], LedTape::NoClearAndNoSend);
                    }
                    ledtape.send();
					if(my_delay_withChkModeChange(delayTime)) return;
                }while(0 < x && x < 0xFF);        
            }
        }
        if(chkAutoPlay()) return;
    }
}

//消す
volatile void funcOff(char mc, unsigned long ms){
    ledtape.setAllColors(g_t_off);
    int autoplay_count = 0;
    while(true){
		if(my_delay_withChkModeChange(ms)) return;
        ++autoplay_count;
        if(autoplay_count == 10){
            autoplay_count = 0;
            if(chkAutoPlay()) return;
        }
    }
}

//*********************************************************************
//* Rainbowで使用する関数
//*->
//*********************************************************************
// Slightly different, this makes the rainbow equally distributed throughout
volatile void rainbowCycle(byte wait, char mc) {
	uint16_t i, j;
	if(ledtape.getBrightness(0) != USE_brightness){
		ledtape.setAllBrightness(USE_brightness);
	}
	for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
		for(i=0; i< LED_length; i++) {
			Wheel(i, ((i * 256 / LED_length) + j) & 255);
		}
		ledtape.send();
		if(my_delay_withChkModeChange(wait)) return;
		if(j % 255 == 0) {
			if(chkAutoPlay()) return;
		}
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
volatile void Wheel(int index, byte WheelPos) {
	if(WheelPos < 85) {
		ledtape.setColor(index, (255 - WheelPos * 3), (WheelPos * 3), 0, LedTape::NoClearAndNoSend);
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		ledtape.setColor(index, 0, (255 - WheelPos * 3), (WheelPos * 3), LedTape::NoClearAndNoSend);
	} else {
		WheelPos -= 170;
		ledtape.setColor(index, (WheelPos * 3), 0, (255 - WheelPos * 3), LedTape::NoClearAndNoSend);
	}
}
//*********************************************************************
//* <-
//*********************************************************************

//*********************************************************************
//* ユーザーモード用関数
//*********************************************************************
volatile void funcStar_User(char mc, Movement mm){
    ledtape.setAllBrightness(USER_brightness);
    int rem = 0, my_LEDs = LED_length;
    int star_count = 5;
    int current_pos = 0;
    
    if(mm == eDouble || mm == eDoubleReverse){
        rem = LED_length % 2;
        my_LEDs = (LED_length / 2) + rem;
    }
    
    while(true){
        if(current_pos == (my_LEDs + star_count)){
            current_pos = 0; 
            if(chkAutoPlay()) return;
        }
        ledtape.clearAllColors();
        for(int i = 0; i < (my_LEDs + star_count); ++i){
            if(i > (current_pos - star_count) && i <= current_pos){
                if(i < my_LEDs) ledtape.setColor(i, TCUser, LedTape::NoClearAndNoSend);
            }else{
                if(i < my_LEDs) ledtape.setColor(i, g_t_off, LedTape::NoClearAndNoSend);
            }
        }        
        ++current_pos;
        //reverse?
        if(mm == eReverse) ledtape.reverse(LED_length, LedTape::NoClearAndNoSend);
        //double?
        if(mm == eDouble){
            ledtape.reverseCopy(my_LEDs, (rem == 1));
        }
        //double reverse?
        if(mm == eDoubleReverse){
            ledtape.reverse(my_LEDs, LedTape::NoClearAndNoSend);
            ledtape.reverseCopy(my_LEDs, (rem == 1));
        }
        //mirror?
        if(mm == eMirror){
            ledtape.mirror(LedTape::NoClearAndNoSend);
        }
        
        ledtape.send();
		if(my_delay_withChkModeChange(delayTime)) return;
    }
}

//点灯
volatile void funcLighting_User(char mc){
    ledtape.setAllBrightness(USER_brightness);
    ledtape.setAllColors(TCUser);
    int autoplay_count = 0;
    while(true){
		if(my_delay_withChkModeChange(delayTime)) return;
        ++autoplay_count;
        if(autoplay_count == 10){
            autoplay_count = 0;
            if(chkAutoPlay()) return;
        }
    }
    
}

//Twincle
volatile void funcTwincleStar_User(char mc){
    ledtape.setAllBrightness(USER_brightness);
    int my_delay_time = 0;
    int autoplay_count = 0;
    while(true){
        my_delay_time = delayTime > MIN_DELAY_TIME ? delayTime - MIN_DELAY_TIME : 0;
        randomSeed(millis());
        int led = random(0, LED_length);
        ledtape.setColor(led, TCUser, LedTape::ClearAllAndAutoSend);
        //my_delay(1);
        ledtape.clearAllColors();
        int random_delay_time = random(5, 150);
		if(my_delay_withChkModeChange(random_delay_time + my_delay_time)) return;
        ++autoplay_count;
        if(autoplay_count == LED_length){
            autoplay_count = 0;
            if(chkAutoPlay()) return;
        }
    }
}

//流星
volatile void funcShootingStar_User(char mc, Movement mm){
    //色設定
    ledtape.setAllColors(TCUser, LedTape::ClearAll);
    ledtape.setAllBrightness(0);
    
    int rem = 0, my_LEDs = LED_length;
    if(mm == eDouble || mm == eDoubleReverse){
        rem = LED_length % 2;
        my_LEDs = (LED_length / 2) + rem;
    }
    chkModeChange();
    //輝度の配列を作成し計算する。 y = 255 / X^2 で輝度を計算する。よって、最大でも16個以上は輝度が0になる。
    int LightCount = 0;
    for(int i = 0; i < 20; ++i){
        if((USER_brightness / pow((i + 1), 2)) > 0) ++LightCount;
    }
    int arrCnt = my_LEDs + LightCount;
    byte br[arrCnt];
    for(int i = 0; i < arrCnt; ++i){
        br[i] = (byte)(USER_brightness / pow((i + 1), 2));
    }
    chkModeChange();
    int rCnt = 0;
    while(true){
        //先頭位置
        int j = rCnt;
        ledtape.clearAllBrightness();
        for(int i = 0; i < arrCnt; ++i){    //外側のループは星の長さ＋LEDの個数の配列の添え字
            if(j >= 0 && j < my_LEDs){
                // j 位置のLEDの輝度を br[i] に設定
                ledtape.setBrightness(j, br[i]);
            }
            --j;
        }
        ++rCnt;
        //ゼロリセット　配列長でリセットする。
        if(rCnt == arrCnt){
            rCnt = 0;
            if(chkAutoPlay()) return;
        }
        //reverse?
        if(mm == eReverse) ledtape.brightnessReverse(LED_length);
        //double?
        if(mm == eDouble){
            ledtape.brightnessReverseCopy(my_LEDs, (rem == 1));
        }
        //double reverse?
        if(mm == eDoubleReverse){
            ledtape.brightnessReverse(my_LEDs);
			chkModeChange();
            ledtape.brightnessReverseCopy(my_LEDs, (rem == 1));
        }
        //mirror?
        if(mm == eMirror){
            ledtape.brightnessMirror();
        }    

        //送信
        ledtape.send();
		if(my_delay_withChkModeChange(delayTime)) return;
    }
}

//*********************************************************************
//* 輝くカチューシャ
//*********************************************************************
volatile void funcEguchiStar(TColor tc, char mc, int mi, unsigned long ms){
    int MyCounter = mi;
    ledtape.setAllBrightness(USER_brightness);
    int star_count = 5;
    int current_pos = 0;
    for(int j = 0; j < (LED_length + star_count); ++j){
        if(current_pos == (LED_length + star_count)) current_pos = 0; 
        ledtape.clearAllColors();
        for(int i = 0; i < (LED_length + star_count); ++i){
            if(i > (current_pos - star_count) && i <= current_pos){
                if(i < LED_length) ledtape.setColor(i, tc, LedTape::NoClearAndNoSend);
            }else{
                if(i < LED_length) ledtape.setColor(i, g_t_off, LedTape::NoClearAndNoSend);
            }
        }        
        ++current_pos;        
        ledtape.send();
		if(my_delay_withChkModeChange(delayTime)) return;
    }
    
    while(true){
		if(my_delay_withChkModeChange(ms)) return;
        if(iCounter != MyCounter){
            MyCounter = iCounter;
            break;
        }
        //***************************************************
        //AutoPlayが設定された場合にこのファンクションから抜けるために必要。
        if(AutoPlay){
            bolFuncCheckOut = true;
            return;
        }
    }
}

//*********************************************************************
// 文字通信処理用関数
//*********************************************************************
//*********************************************************************
//* 数値文字の左側のゼロを除去する
//* 引数　【数値文字列】
//*********************************************************************
String removeLeftZeroFromStr(String s){
    String str;
    for(int i = 0; i < s.length(); ++i){
        if(s.charAt(i) != '0'){
            str = s.substring(i, s.length());
            break;
        }
    }
    if(str.length()==0) str = "0";
    return str;
}
//*********************************************************************
//* 数値文字の左側のゼロを追加する
//* 引数　【数値文字列】、【桁数】
//*********************************************************************
String addLeftZeroFromStr(String s, int digit){
    if(s.length()>=digit){
        return s;
    }else{
        int tmp = digit - s.length();
        String s_tmp;
        for(int i = 0; i < tmp; ++i){
            s_tmp += "0";
        }
        return s_tmp + s;
    }
}
//*********************************************************************
//* 数字→文字変換関数
//* 引数　【数値（int）】、【桁数（int）】
//*********************************************************************
String funcNum2Str(int num, int digit){
        String result;        
        String str(num);
        int len = str.length();
        int add_cnt = digit - len;
        for(int i = 0; i < add_cnt; ++i){
          result += "0";
        }
        result += str;
        return result;
}
//*********************************************************************
//* 文字→数字変換関数
//* 引数　【文字（string）】、【桁数（int）】
//*********************************************************************
int funcStr2Num(String str){
    int result = 0;
    int len = (int)str.length();
    char ch[len];
    str.toCharArray(ch, len);
    for(int i = 0; i < len; ++i){
        result += (int)((ch[i] - '0') * pow(10, (len - (1 + i))) + 0.5);
    } 
    return result;
}
//*********************************************************************
//* 文字配列→数値変換関数
//*********************************************************************
int getNumberTo3CharArray(char *ch,int start){
    int result = 0;
    for(int i = 0; i < 3; ++i){
		if('0' <= ch[start + i] && ch[start + i] <= '9'){
			result += (int)((ch[start + i] - '0') * pow(10, (3 - (1 + i))) + 0.5);

		}
    }
    return result;
}
//*********************************************************************
//* byte→int
//*********************************************************************
byte int2byte(int val){
    return (byte)(val & 0xFF);
}

//*********************************************************************
//* UpDATA Initialize
//*********************************************************************
volatile void funcUpDATAInitialize(){
    for(int i = 0; i < DATA_LENGTH; ++i){
        UpDATA[i] = 0x00;
    }
}
//*********************************************************************
//* delay モード変更のチェック値を返す。
//*********************************************************************
volatile bool my_delay_withChkModeChange(unsigned long ms){
	unsigned long start = millis();
	unsigned long e_start = start;
	do{
		if(chkModeChange()) return true;
		if(millis() - e_start >= 1){
			earring_movement();    //イヤリング関数の実行
			e_start = millis();
		}
	}while((millis() - start) < ms);
	return false;
}
//*********************************************************************
//* delay 分割実行。
//*********************************************************************
volatile void my_delay(unsigned long ms){
	for(int i = 0; i < ms; ++i){
		earring_movement();    //イヤリング関数の実行
		delay(1);
	}
}
//*********************************************************************
//* Earring用関数
//*********************************************************************
volatile void earring_movement(){
    ++earring_count;
    switch(earring_mode){
        case 'a':    //Lighting
            //特段の処理無し
            break;
        case 'b':    //Twincle
            if(earring_count == 1){
                // LED ON
                earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
                earring_twincle_speed = random(1, 5) * earring_speed;
            }else if(earring_count == earring_twincle_time){
                // LED OFF
                earringLED.LightingLED(0,0,0, earring_brightness);
            }else if(earring_count >= (earring_twincle_time + earring_twincle_speed)){
                earring_count = 0;
                randomSeed(millis());
                earring_twincle_speed = random(1, 5) * earring_speed;
            }
            break;
        case 'c':    //Rainbow
            if(earring_count >= (10 + earring_speed)){
                earring_count = 0;
                earring_wheel(&earring_color, (byte)(earring_wheel_count & 0xff));
                earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);    
                if(earring_wheel_count < 255){
                    ++earring_wheel_count;
                }else{
                    earring_wheel_count = 0;
                }
            }
            break;
        case 'd':    //Twincle Rainbow
            if(earring_count == 1){
                // GET LED COLOR
                earring_wheel(&earring_color, (byte)(earring_wheel_count & 0xff));
                // LED ON
                earringLED.LightingLED(earring_color.red, earring_color.green, earring_color.blue, earring_brightness);
                earring_twincle_speed = random(1, 5) * earring_speed;
                if(earring_wheel_count < 255){
                    earring_wheel_count += 5;
                }else{
                    earring_wheel_count = 0;
                }
            }else if(earring_count == earring_twincle_time){
                // LED OFF
                earringLED.LightingLED(0,0,0, earring_brightness);
            }else if(earring_count >= (earring_twincle_time + earring_twincle_speed)){
                earring_count = 0;
                randomSeed(millis());
                earring_twincle_speed = random(1, 5) * earring_speed;
            }
            break;
        default:
            break;
    }
}
volatile void earring_wheel(TColor *MyColor, byte WheelPos) {
    if(WheelPos < 85) {
        MyColor->red = (255 - WheelPos * 3);
        MyColor->green = (WheelPos * 3);
        MyColor->blue = 0;
    } else if(WheelPos < 170) {
        WheelPos -= 85;
        MyColor->red = 0;
        MyColor->green = (255 - WheelPos * 3);
        MyColor->blue = (WheelPos * 3);
    } else {
        WheelPos -= 170;
        MyColor->red = (WheelPos * 3);
        MyColor->green = 0;
        MyColor->blue = (255 - WheelPos * 3);
    }
}
