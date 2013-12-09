/*GR-KURUMI Sketch Template Version: E0.94*/
#include <RLduino78.h>
#include "iodefine.h"
#include "LedTape.h"
#include "UserData.h"

//*********************************************************************
// Pin 22,23,24 are assigned to RGB LEDs.
int led_red   = 22; // LOW active
int led_green = 23; // LOW active
int led_blue  = 24; // LOW active

//*********************************************************************
//　関数定義
void funcSettings();
void funcExec();
void funcUserExec();
void funcRequest();
bool funcUserSettings();
void funcInitializeUserData();
bool chkModeChange(char mc);
bool chkUserModeChange(char mc);
bool funcReadDATA(int ReadByte, bool save = true);
void rainbowCycle(byte wait);
void Wheel(int index, byte WheelPos);
void funcStar(TColor tc, char mc, int mi);
void funcStar1(TColor tc, char mc, int mi, bool b_randam = false);
volatile void funcStar2(TColor tc, char mc, int mi, bool b_randam = false);
void funcLighting(TColor tc, char mc, int mi);
void funcColorflowChg(char mc);
void funcLightingAllColorChg(char mc);
void funcRainbow(char mc);
void funcGradient(char mc);
void funcEguchiStar(TColor tc, char mc, int mi, unsigned long ms = 100);
void funcOff(char mc, unsigned long ms);

void funcStar_User(char mc);
void funcStar1_User(char mc);
void funcLighting_User(char mc);
volatile void funcStar2_User(char mc);

String funcNum2Str(int num, int digit);
int funcStr2Num(String str);
void splitByteCharArray(char ch[], char *p, int start = 0, int len = 3);
int funcCharArray2Num(char ch[], int len = 3);
int getNumberTo3CharArray(char ch[],int start = 0);
byte int2byte(int val);
void funcUpDATAInitialize();
void funcKurumiLEDRed();
void funcKurumiLEDGreen();
void funcKurumiLEDBlue();
void funcKurumiLEDYellow();
void funcKurumiLEDAqua();
void funcKurumiLEDFechsia();
void funcKurumiLEDWhite();
//*********************************************************************
// インスタンス宣言
//ROM用インスタンス
UserData userdata;
//LedTapeインスタンス
LedTape ledtape;

//*********************************************************************
// 定数宣言
const byte iMod = 8;			//モードの分母（固定）
const int DATA_LENGTH = 30;		//設定データのByte数
const unsigned long KURUKI_LED_DELAY = 30;	//デバッグ用KURUMI LED Deley値
const int MIN_DELAY_TIME = 5;
enum ExecMode{ eExecLED, eUser, eSettings, eRequest };
//　変数宣言
enum ExecMode exec_mode;				//実行モード値（0=動作、1=設定、2=ユーザーモード）
char LED_Mode = 'a';			//LEDの動作モード 特に、設定モードは'X'
char User_LED_Mode ='a';		//ユーザーモード用の動作モード
int iCounter = 0;				//動作モードのカウンタ値
char UpDATA[DATA_LENGTH];		//設定データ
byte MAX_brightness = 0x80;		//最大輝度
byte USE_brightness = 0x80;		//使用輝度
byte USER_brightness = 0x80;	//ユーザーモード用輝度
byte LED_length = 19;			//LED数
// TColor変数宣言
TColor g_t_color[7];
TColor g_t_off, g_t_on;
TColor TCUser;						//ユーザーモード用
int delayTime = 10;

//*********************************************************************
//* SETUP
//*********************************************************************
void setup() {
	//*****************************************************************
	// UserData初期化
	//userdata.setMemoryOffset(0);
	userdata.setup();	//UserDataセットアップ　インスタンス化後に必ず呼ぶ必要がある。
	
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
	
	//**********************************************************************
	// 格納データの読み込み
	LED_length = (byte)userdata.getLEDs();
	USE_brightness = (byte)userdata.getUseBrightness();
	LED_Mode = (char)userdata.getLedMode();
	iCounter = (int)userdata.getMoveCount();
	MAX_brightness = (byte)userdata.getMaxBrightness();
	User_LED_Mode = (char)userdata.getUserLedMode();
	USER_brightness = (byte)userdata.getUserBrightness();
	userdata.getUserColor(&TCUser);
	if((int)(userdata.getExecMode()) == eExecLED){
		exec_mode = eExecLED;
	}else{
		exec_mode = eUser;
	}
	delayTime = MIN_DELAY_TIME + userdata.getDelayTime();
	
	//**********************************************************************
	// 初期化 ledtapeセットアップ
	ledtape.setup(LED_length, USE_brightness);	//ledtapeセットアップ　インスタンス化後に必ず呼ぶ必要がある。
	funcUpDATAInitialize();
	Serial2.begin(9600);
	
	//**********************************************************************
	// KURUMI LED 初期設定
	pinMode(led_red, OUTPUT);
	pinMode(led_green, OUTPUT);
	pinMode(led_blue, OUTPUT);
	digitalWrite(led_red, HIGH);
	digitalWrite(led_green, HIGH);
	digitalWrite(led_blue, HIGH);
	funcKurumiLEDWhite();
	delay(500);
}

//*********************************************************************
//* LOOP eExecLED, eSettings, eUser
//*********************************************************************
void loop() {
	switch(exec_mode){
		//実行順には注意すること。
		case eExecLED:
			funcKurumiLEDBlue();
			funcExec();
			break;
		case eUser:
			funcKurumiLEDYellow();
			funcUserExec();
			break;
		case eRequest:
			funcKurumiLEDAqua();
			funcRequest();
			break;
		case eSettings:
			funcKurumiLEDGreen();
			funcSettings();
			break;
		default:
			chkModeChange(LED_Mode);	//障害が起きても復旧可能とする。
			funcKurumiLEDRed();
			delay(1000);
			break;
	}
}

//*********************************************************************
//* 関数定義
//*********************************************************************
//*********************************************************************
//* ledtape初期値設定
//*********************************************************************
void funcInitializeUserData(){
	//メモリ格納位置を変更できるようにする場合はここを注意すること。
	userdata.setMemoryOffset((byte)0);
	userdata.initialize();
}
//*********************************************************************
//* モードチェンジチェック関数
//* 戻り値：　変更があれば　true、無ければ　false
//*********************************************************************
bool chkModeChange(char Current_LED_Mode){
	if (Serial2.available() > 0){
		byte Tmp_LED_Mode = Serial2.read();
		//読み込んだ値がセッティングモードではない場合　値をメモリに格納してモードチェンジ
		if(Tmp_LED_Mode < 'A' || Tmp_LED_Mode > 'Z'){
			LED_Mode = Tmp_LED_Mode;
			//　カウントアップ
			if(LED_Mode >= '1' && LED_Mode <= '7'){
				++iCounter;
			}
			//　カウントデータ保存
			if((int)'a' <= LED_Mode && LED_Mode <= (int)'d'){
				++iCounter;
				//データ格納
				userdata.setMoveCount((byte)(iCounter % 8));
			}
			//LED動作モード格納
			if(LED_Mode != Current_LED_Mode){
				if((int)'a' <= LED_Mode && LED_Mode <= (int)'h'){
					userdata.setLedMode(LED_Mode);
				}
			}
			return LED_Mode == Current_LED_Mode ? false : true;
		}
		
		if(Tmp_LED_Mode == (byte)'X'){
			exec_mode = eSettings;
			//データ読み込み
			return funcReadDATA(DATA_LENGTH);
		}else if(Tmp_LED_Mode == (byte)'Y'){
			//ユーザーモードへ変更
			//ユーザーモードに入る入り方は、実行モード時に'Y'を送信する。つまりここと、リクエストで'R03'を送信する場合があるので注意。
			exec_mode = eUser;
			return true;
		}else if(Tmp_LED_Mode == (byte)'Z'){
			exec_mode = eRequest;
			//データ読み込み
			return funcReadDATA(DATA_LENGTH);
		}else if(Tmp_LED_Mode == (byte)'U'){	//'U'はユーザーモード時のデータなので入ってきても捨てる。
			//データ空読み込み
			funcReadDATA(21, false);
		}
		//その他の場合は変更しない。
		return false;
    }
	//読み込めない場合は変更しない。
	return false;
};
//*********************************************************************
//* ユーザーモードチェンジチェック関数
//* ユーザーモードに入ったら、Uxxxxx・・・のデータしか来ない！他に来るのは実行モードのデータのみ
//* 戻り値：　変更があれば　true、無ければ　false
//*********************************************************************
bool chkUserModeChange(char Current_LED_Mode){
	if (Serial2.available() > 0){
		funcUpDATAInitialize();
		//先頭データを除く
		byte Tmp_LED_Mode = Serial2.peek();
		//読み込んだ値がユーザーモードのデータではない場合
		if(Tmp_LED_Mode != (byte)'U'){
			//ユーザーモードではない場合は読み出し
			byte Tmp_LED_Mode = Serial2.read();
			if(Tmp_LED_Mode == (byte)'X'){
				exec_mode = eSettings;
				//データ読み込み
				return funcReadDATA(DATA_LENGTH);
			}else if(Tmp_LED_Mode == (byte)'Y'){
				return false;
			}else if(Tmp_LED_Mode == (byte)'Z'){
				exec_mode = eRequest;
				//データ読み込み
				return funcReadDATA(DATA_LENGTH);
			}else{
				//実行モード用の値に変更する。
				exec_mode = eExecLED;
				LED_Mode = Tmp_LED_Mode;
				//userdata.setExecMode((byte)0x01);		funcExecの先頭で保存するように変更
				//USE_brightness = userdata.getUseBrightness();		
				return true;
			}
		}else{
			//ユーザモードでの設定をする
			return funcUserSettings();
		}
	}else{
		return false;
	}
	//読み込めない場合
	return false;
};

//****************************************************************
//* セッティング関数
//　'X'
// Setting String Format 合計30byte 
//	0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		
// "L00"		個数		最大輝度		使用輝度		DelayTime	0			0			0			0			"END"		***LED　Params　設定 保存
// "I00"		0			0			0			0			0			0			0			0			"END"		***Initialize　保存
// "I01"		MemOffset	0			0			0			0			0			0			0			"END"		***MemoryOffset　保存
// "I10"		使用輝度		0			0			0			0			0			0			0			"END"		***使用輝度変更　非保存
// "I11"		DelayTime	0			0			0			0			0			0			0			"END"		***DelayTime 非保存		
//****************************************************************
void funcSettings(){
	if(exec_mode != eSettings) return; 
	//読み込み判定、終わりが「END」であるか？
	if(UpDATA[27] == 'E' && UpDATA[28] == 'N' && UpDATA[29] == 'D'){
		int intLEDs, intMax, intUse, intDelayTime;
		byte bMax, bUse, bDelayTime;
		switch(UpDATA[0]){
			//*********************************************************
			//* LED Params 設定
			//*********************************************************
			case 'L':
				if(UpDATA[1] == '0' && UpDATA[2] == '0'){
					//消灯
					ledtape.clearAllColors();
					ledtape.send();
					//funcKurumiLEDBlue();		//*********************CHK
					//delay(1000);
					intLEDs = getNumberTo3CharArray(UpDATA, 3); 
					//LED個数保存　最大255個
					if(intLEDs > 0x00 && intLEDs <= 0xFF){
						//メモリ格納
						userdata.setLEDs(int2byte(intLEDs));
						//現在の実行値修正
						LED_length = int2byte(intLEDs);
						ledtape.setLEDs(LED_length);
					}
					//輝度保存
					intMax = getNumberTo3CharArray(UpDATA, 6);
					intUse = getNumberTo3CharArray(UpDATA, 9);
					intDelayTime = getNumberTo3CharArray(UpDATA, 12);
					bMax = int2byte(intMax);
					bUse = int2byte(intUse);
					if(intMax > 0 && intMax <= 0xFF){
						if(intMax < intUse){
							bUse = bMax;
						}
						userdata.setMaxBrightness(bMax);
						userdata.setUseBrightness(bUse);	
					}
					USE_brightness = bUse;
					MAX_brightness = bMax;
					if(0 <= intDelayTime && intDelayTime <= 100){
						bDelayTime = int2byte(intDelayTime);
						userdata.setDelayTime(bDelayTime);
						delayTime = MIN_DELAY_TIME + intDelayTime;				
					}
				}
				break;

			//*********************************************************
			//* Initialize
			//*********************************************************
			case 'I':

				//初期化
				if(UpDATA[1] == '0' && UpDATA[2] == '0'){
					funcInitializeUserData();

					//**********************************************************************
					// 格納データの読み込み
					LED_length = userdata.getLEDs();
					ledtape.setLEDs(LED_length);
					USE_brightness = userdata.getUseBrightness();
					LED_Mode = (char)userdata.getLedMode();
					iCounter = (int)userdata.getMoveCount();
					MAX_brightness = userdata.getMaxBrightness();
					
				//メモリ位置変更
				}else if(UpDATA[1] == '0' && UpDATA[2] == '1'){
					byte b_tmp = int2byte(getNumberTo3CharArray(UpDATA, 3));
					userdata.setMemoryOffset(b_tmp);
					
					//**********************************************************************
					// 格納データの読み込み
					LED_length = userdata.getLEDs();
					ledtape.setLEDs(LED_length);
					USE_brightness = userdata.getUseBrightness();
					LED_Mode = (char)userdata.getLedMode();
					iCounter = (int)userdata.getMoveCount();
					MAX_brightness = userdata.getMaxBrightness();

				}else if(UpDATA[1] == '1' && UpDATA[2] == '0'){
					//　輝度の一時設定
					USE_brightness = (byte)getNumberTo3CharArray(UpDATA, 3);	
				}else if(UpDATA[1] == '1' && UpDATA[2] == '1'){
					// DelayTimeの一時設定
					delayTime = (byte)(MIN_DELAY_TIME + getNumberTo3CharArray(UpDATA, 3));
				}
				break;
			
			default:
				break;
		}
		
		//*********************************************************
		//* リブートメッセージ
		//*********************************************************
		//funcKurumiLEDRed();	//*********************CHK
		//リブートメッセージ
		//ledtape.setAllColors(g_t_on);
		//delay(100);
		//ledtape.setAllColors(g_t_off);
		//delay(100);
	}
	//実行モードにする。
	LED_Mode = (char)userdata.getLedMode();
	exec_mode = eExecLED;
	funcUpDATAInitialize();
}
//****************************************************************
//* 送信関数
//　’Z'
// Setting String Format 合計30byte 
//	0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		
// "R01"		0			0			0			0			0			0			0			0			"END"		***LED設定等送信要求
// "R02"		LED個数		最大輝度		使用輝度		ﾒﾓﾘｵﾌｾｯﾄ値	DelayTime	"END"		***返信	
// "R03"		0			0			0			0			0			0			0			0			"END"		***ユーザモード設定送信要求
// "R04"		動作Mode	輝度		R値			G値			B値			"END"		***返信	
//****************************************************************
void funcRequest(){
	if(exec_mode != eRequest) return; 
	//読み込み判定、終わりが「END」であるか？
	if(UpDATA[27] == 'E' && UpDATA[28] == 'N' && UpDATA[29] == 'D'){
		if(UpDATA[0] == 'R'){
			//funcKurumiLEDYellow();
			//delay(1000);
			//R01の場合の返信
			if(UpDATA[1] == '0' && UpDATA[2] == '1'){
				Serial2.print("R02");
				Serial2.print(funcNum2Str(LED_length,3));
				Serial2.print(funcNum2Str(MAX_brightness,3));
				Serial2.print(funcNum2Str(userdata.getUseBrightness(),3));
				Serial2.print(funcNum2Str(userdata.getMemoryOffset(),3));
				Serial2.print(funcNum2Str(userdata.getDelayTime(),3));
				Serial2.print("END");
				
				//使用輝度とdelayTimeは初期値を読み込む。（一時変更の対応のため）
				USE_brightness = userdata.getUseBrightness();	
				delayTime = MIN_DELAY_TIME + userdata.getDelayTime();
				//funcKurumiLEDFechsia();
				//delay(1000);
			}
			//R03の場合の返信
			if(UpDATA[1] == '0' && UpDATA[2] == '3'){
				//動作Mode	輝度		R値			G値			B値
				userdata.getUserColor(&TCUser);
				Serial2.print("R04");
				Serial2.print("00");
				Serial2.print((char)userdata.getUserLedMode());
				Serial2.print(funcNum2Str(userdata.getUserBrightness(),3));
				Serial2.print(funcNum2Str(TCUser.red,3));
				Serial2.print(funcNum2Str(TCUser.green,3));
				Serial2.print(funcNum2Str(TCUser.blue,3));
				Serial2.print("END");
				//ユーザーモードにリターンする。
				//ユーザーモードに入る入り方は、実行モード時に'Y'を送信する。及びリクエストで'R03'を送信する場合、つまりここの場合があるので注意。
				exec_mode = eUser;
				return;
			}
		}
	}
	//実行モードにする。
	if(exec_mode != eExecLED){
		LED_Mode = (char)userdata.getLedMode();
		exec_mode = eExecLED;
		funcUpDATAInitialize();
	}
}
//****************************************************************
//* ユーザーモードセッティング関数
//　’Y’
// Setting String Format 合計30byte 
//	0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		
// "U01"		動作Mode	輝度		R値			G値			B値			"END"	//保存
// "U02"		動作Mode	輝度		R値			G値			B値			"END"	//
//****************************************************************
bool funcUserSettings(){
	if(exec_mode != eUser) return false; 
	byte bMM;
	byte bUB;
	int iUB;
	if(Serial2.available() == 0) return false;
	//読み込み 21 byte
	bool chk;
	chk = funcReadDATA(21);
	//読み込み判定、終わりが「END」であるか？
	if(chk && UpDATA[18] == 'E' && UpDATA[19] == 'N' && UpDATA[20] == 'D'){
		if(UpDATA[0] == 'U'){
			if((UpDATA[1] == '0' && UpDATA[2] == '1') || (UpDATA[1] == '0' && UpDATA[2] == '2')){
				bMM = (byte)UpDATA[5];
				iUB = getNumberTo3CharArray(UpDATA, 6);
				bUB = int2byte(iUB);
				TCUser.red = int2byte(getNumberTo3CharArray(UpDATA, 9));
				TCUser.green = int2byte(getNumberTo3CharArray(UpDATA, 12));
				TCUser.blue = int2byte(getNumberTo3CharArray(UpDATA, 15));
				if(UpDATA[2] == '1'){					
					//動作モード保存
					userdata.setUserLedMode(bMM);				
					userdata.setUserBrightness(bUB);
					userdata.setUserColor(&TCUser);
				}
				//動作設定値変更
				User_LED_Mode = (char)bMM;
				USER_brightness = bUB;
				return true;
			}else{
				return false;
			}
		}else{
			return false;
		}
	}else{
		return false;
	}
}
//****************************************************************
//* データ読み込み関数
//* UpDATAに引数「ReadByte」読み込む　但し、MAXはDATA_LENGTH
//****************************************************************
bool funcReadDATA(int ReadByte, bool save){
	//'A'-'Z'はExecModeの変更と規定したので以下でチェンジする。
	if(ReadByte > DATA_LENGTH) ReadByte = DATA_LENGTH;
	//初期化
	funcUpDATAInitialize();
	//要求Byte数読み込めるようになるまで待つ とりあえず30×5ms
	bool chk = false;
	for(int i = 0; i < DATA_LENGTH; ++i){
		if(Serial2.available() >= ReadByte){
			for(int j = 0; j < ReadByte; ++j){
				if(save){
					UpDATA[j] = Serial2.read();
				}else{
					Serial2.read();
				}
			}
			chk = true;
			break;
		}
		delay(5);
	}
	return chk;
}
//****************************************************************
//* LED 発光動作実行関数
//* 動作関数にはモードチェック関数【chkModeChange】を含める事
//****************************************************************
void funcExec(){
	if(exec_mode != eExecLED) return;
	
	userdata.setExecMode((byte)eExecLED);
	//USE_brightness = userdata.getUseBrightness();	
	//delayTime = MIN_DELAY_TIME + userdata.getDelayTime();
	
	while(true){
		if(exec_mode != eExecLED) return;
		int i = 0;
		//***********************************
		//-->輝くカチューシャ
		switch(LED_Mode){
			case '1':
				funcEguchiStar(g_t_color[0], LED_Mode, iCounter);
				break;
			case '2':
				funcEguchiStar(g_t_color[1], LED_Mode, iCounter);
				break;
			case '3':
				funcEguchiStar(g_t_color[2], LED_Mode, iCounter);	
				break;
			case '4':
				funcEguchiStar(g_t_color[3], LED_Mode, iCounter);
				break;
			case '5':
				funcEguchiStar(g_t_color[4], LED_Mode, iCounter); 
				break;
			case '6':
				funcEguchiStar(g_t_color[5], LED_Mode, iCounter);  
				break;
			case '7':
				funcEguchiStar(g_t_color[6], LED_Mode, iCounter);
				break;
			case '8':
				funcEguchiStar(TCUser, LED_Mode, iCounter);
				break;
			//<-- 輝くカチューシャ
			//*****************************
			case 'a':
				i = iCounter % iMod;
				if( i < (iMod - 1)){
					funcStar(g_t_color[i], LED_Mode, iCounter);
				}else{
					randomSeed(millis());
					funcStar(g_t_color[random(0, (iMod -1))], LED_Mode, iCounter);
				}
				break;
			case 'b':
				i = iCounter % iMod;
				if( i < (iMod - 1)){
					funcLighting(g_t_color[i], LED_Mode, iCounter);
				}else{
					randomSeed(millis());
					funcLighting(g_t_color[random(0, (iMod - 1))], LED_Mode, iCounter); //randomはminからmax-1の間の整数
				}
				break;
			case 'c':
				i = iCounter % iMod;
				if( i < (iMod -1)){
					funcStar1(g_t_color[i], LED_Mode, iCounter);
				}else{
					funcStar1(g_t_color[0], LED_Mode, iCounter, true); //先頭カラーは何でもよい。
				}
				break;
			case 'd':
				i = iCounter % iMod;
				if( i < (iMod -1)){
					funcStar2(g_t_color[i], LED_Mode, iCounter);
				}else{
					funcStar2(g_t_color[0], LED_Mode, iCounter, true); //先頭カラーは何でもよい。
				}
				break;
			case 'e':
				funcLightingAllColorChg(LED_Mode);
				break;
			case 'f':
				funcColorflowChg(LED_Mode); 
				break;
			case 'g':
				funcGradient(LED_Mode); 
				break;
			case 'h':
				funcRainbow(LED_Mode);
				break;
			case 'i':
			default:
				funcOff(LED_Mode, 50);
				break;
		}
	}
}
//****************************************************************
//* LED 発光動作ユーザーモード実行関数
//****************************************************************
void funcUserExec(){
	if(exec_mode != eUser) return;

	User_LED_Mode = (char)userdata.getUserLedMode();
	userdata.setExecMode((byte)eUser);
	USER_brightness = (byte)userdata.getUserBrightness();
	userdata.getUserColor(&TCUser);
	
	while(true){
		if(exec_mode != eUser) return;
		switch(User_LED_Mode){
			case 'a':
				funcStar_User(User_LED_Mode);
				break;
			case 'b':
				funcLighting_User(User_LED_Mode);
				break;
			case 'c':
				funcStar1_User(User_LED_Mode);
				break;
			case 'd':
				funcStar2_User(User_LED_Mode);
				break;
			default:
				exec_mode = eExecLED;
				break;
		}
	}
}

//****************************************************************
//* LED 発光動作関数
//****************************************************************
// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(byte wait, char mc) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< LED_length; i++) {
       Wheel(i, ((i * 256 / LED_length) + j) & 255);
    }
    ledtape.send();
	delay(wait);
    //delay(wait + (delayTime - MIN_DELAY_TIME));
	if(chkModeChange(mc)) return;
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
void Wheel(int index, byte WheelPos) {
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

//実行するファンクションの定義
//星（江口さんのやつのループ）
void funcStar(TColor tc, char mc, int mi){
	ledtape.setAllBrightness(USE_brightness);
	int star_count = 5;
	int current_pos = 0;
	while(true){
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
		delay(delayTime);
		if(chkModeChange(mc)) return;
		if(iCounter != mi) return;
	}
}

//点灯
void funcLighting(TColor tc, char mc, int mi){
	ledtape.setAllBrightness(USE_brightness);
	ledtape.setAllColors(tc);
	while(true){
		delay(delayTime);
		if(chkModeChange(mc)) return;
		if(iCounter != mi) return;
	}
}
//Twincle
void funcStar1(TColor tc, char mc, int mi, bool b_randam){
	ledtape.setAllBrightness(USE_brightness);
	while(true){
		randomSeed(millis());
		int led = random(0, LED_length);
		if(b_randam){
			int j = random(0, 7);
			ledtape.setColor(led, g_t_color[j], LedTape::ClearAllAndAutoSend);
		}else{
			ledtape.setColor(led, tc, LedTape::ClearAllAndAutoSend);
		}
		//delay(1);
		ledtape.clearAllColors();
		int randam_delay_time = random(5, 150);
		delay(randam_delay_time);
		if(chkModeChange(mc)) return;
		if(iCounter != mi) return;
	}
	
}
//流星
volatile void funcStar2(TColor tc, char mc, int mi, bool b_randam){
	//色設定
	ledtape.setAllColors(tc, LedTape::ClearAll);
	ledtape.setAllBrightness(0);
	//輝度の配列を作成し計算する。 y = 255 / X^2 で輝度を計算する。よって、最大でも16個以上は輝度が0になる。
	byte br[LED_length];
	int LightCount = LED_length;
	for(int i = 0; i < LED_length; ++i){
		br[i] = (byte)(USE_brightness / pow((i + 1), 2));
		if(br[i] > 0) ++LightCount;
	}
	int rCnt = 0;
	while(true){
		//ランダムの場合は、毎回流れる星の色を変える。
		if(b_randam){
			randomSeed(millis());
			int k = random(0, 7);
			ledtape.setAllColors(g_t_color[k], LedTape::ClearAll);
		}
		//先頭位置計算
		int j = rCnt % LightCount;
		for(int i = 0; i < LED_length; ++i){
			if(j >= 0 && j < LED_length){
				// j 位置のLEDの輝度を br[i] に設定
				ledtape.setBrightness(j, br[i]);
			}
			--j;
		}
		++rCnt;
		//ゼロリセット
		if(rCnt == LightCount) rCnt = 0;
		//送信
		ledtape.send();
		delay(delayTime);
		if(chkModeChange(mc)) return;
		if(iCounter != mi) return;
	}	
}
//点灯 色が流れるように順々に変わる
void funcColorflowChg(char mc){
	ledtape.setAllBrightness(USE_brightness);
	while(true){
		for(int j = 0; j < 7; ++j){
			for(int i = 0 ; i < LED_length ; ++i){
				ledtape.setColor(i, g_t_color[j]);
				delay(delayTime);
				if(chkModeChange(mc)) return;
			}
        }
    }
}

// 色
void funcLightingAllColorChg(char mc){
	ledtape.setAllBrightness(USE_brightness);
	while(true){
		for(int i = 0; i < 7; ++i){
			ledtape.setAllColors(g_t_color[i]);
			delay(delayTime * 40);
			if(chkModeChange(mc)) return;
		}
    }
}

// 虹色表示
void funcRainbow(char mc){
	ledtape.setAllBrightness(USE_brightness);
    while(true)
    {
	    for(int i = 0 ; i < 5 ; i++)
		{
			rainbowCycle(i, mc);
			//注意：rainbowCycle内でchkModeChangeを呼んでいるのでここでは比較のみ。
			if(LED_Mode != mc) return;
			if(exec_mode != eExecLED) return;
		}
    }
}

void funcGradient(char mc){
	ledtape.setAllBrightness(USE_brightness);
	TColor tmp_c[LED_length];
	TColor tc;
	byte x;
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
		delay(delayTime);
		//外ループ
		for(int i = 0; i < 3; ++i){
			for(int j = 0; j < 4; ++j){
				x = j < 2 ? 0xFF : 0;
				do{
					if(j < 2){
						//--x;
						x = (x - 4) < 0 ? 0 : (x - 4);
					}else{
						//++x;
						x = (x + 4) > 0xFF ? 0xFF : (x + 4);
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
					delay(delayTime);
					if(chkModeChange(mc)) return;
				}while(0 < x && x < 0xFF);		
			}
		}
	}
}
//消す
void funcOff(char mc, unsigned long ms){
	ledtape.setAllColors(g_t_off);
	while(true){
		if(chkModeChange(mc)) return;
		delay(ms);
	}
}

void funcStar_User(char mc){
	ledtape.setAllBrightness(USER_brightness);
	int star_count = 5;
	int current_pos = 0;
	while(true){
		if(current_pos == (LED_length + star_count)) current_pos = 0; 
		ledtape.clearAllColors();
		for(int i = 0; i < (LED_length + star_count); ++i){
			if(i > (current_pos - star_count) && i <= current_pos){
				if(i < LED_length) ledtape.setColor(i, TCUser, LedTape::NoClearAndNoSend);
			}else{
				if(i < LED_length) ledtape.setColor(i, g_t_off, LedTape::NoClearAndNoSend);
			}
		}		
		++current_pos;		
		ledtape.send();
		delay(delayTime);
		if(chkUserModeChange(mc)) return;
	}
}

//点灯
void funcLighting_User(char mc){
	ledtape.setAllBrightness(USER_brightness);
	ledtape.setAllColors(TCUser);
	while(true){
		delay(delayTime);
		if(chkUserModeChange(mc)) return;
	}
}
//Twincle
void funcStar1_User(char mc){
	ledtape.setAllBrightness(USER_brightness);
	while(true){
		randomSeed(millis());
		int led = random(0, LED_length);
		ledtape.setColor(led, TCUser, LedTape::ClearAllAndAutoSend);
		//delay(1);
		ledtape.clearAllColors();
		int random_delay_time = random(5, 150);
		delay(random_delay_time + (delayTime - MIN_DELAY_TIME));
		if(chkUserModeChange(mc)) return;
	}
}

//流星
volatile void funcStar2_User(char mc){
	//色設定
	ledtape.setAllColors(TCUser, LedTape::ClearAll);
	ledtape.setAllBrightness(0);
	//輝度の配列を作成し計算する。 y = 255 / X^2 で輝度を計算する。よって、16個以上は輝度が0になる。
	byte br[LED_length];
	int LightCount = LED_length;
	for(int i = 0; i < LED_length; ++i){
		br[i] = (byte)(USER_brightness / pow((i + 1), 2));
		if(br[i] > 0) ++LightCount;
	}
	
	int rCnt = 0;
	while(true){
		//先頭位置計算
		int j = rCnt % LightCount;
		for(int i = 0; i < LED_length; ++i){
			if(j >= 0 && j < LED_length){
				// j 位置のLEDの輝度を br[i] に設定
				ledtape.setBrightness(j, br[i]);
			}
			--j;
		}
		++rCnt;
		//ゼロリセット
		if(rCnt == LightCount) rCnt = 0;
		//送信
		ledtape.send();
		delay(delayTime);
		if(chkUserModeChange(mc)) return;
	}
}

//輝くカチューシャ
void funcEguchiStar(TColor tc, char mc, int mi, unsigned long ms){
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
		delay(delayTime);
	}
	
	while(true){
		delay(ms);
		if(chkModeChange(mc)) return;
		if(iCounter != MyCounter){
			MyCounter = iCounter;
			break;
		}
	}
}
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
	String s;
	int tmp;
	for(int i = 0; i < digit; ++i){
		tmp = (int)(num % (int)pow(10, (digit-i)) / pow(10, (digit -(i+1))));
		s +=(char)('0' + tmp);
	} 
	return s;
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
		result += (ch[i] - '0') * pow(10, (len - (1 + i)));
	} 
	return result;
}
//*********************************************************************
//* 文字→数字変換関数
//* 引数　【文字配列（char[]）】、【桁数（int）】
//*********************************************************************
//*********************************************************************
//* 文字→数字変換関数
//* 引数　【文字配列（char[]）】、【桁数（int）】
//*********************************************************************
int funcCharArray2Num(char ch[], int len){
	int result = 0;
	for(int i = 0; i < len; ++i){
		result += (int)(ch[i] - '0') * pow(10, (len - (1 + i)));
	} 
	return result;
}
//*********************************************************************
//* 文字切り出し用関数
//* 文字配列→文字配列
//*********************************************************************
void splitByteCharArray(char ch[], char *p, int start, int len){
	for(int i = 0; i < len; ++i){
		p[i] = ch[start + i];
	}
}
//*********************************************************************
//* 文字配列→数値変換関数
//*********************************************************************
int getNumberTo3CharArray(char ch[],int start){
	char tCh[3];
	splitByteCharArray(ch, tCh, start, 3);
	return funcCharArray2Num(tCh, 3);
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
void funcUpDATAInitialize(){
	for(int i = 0; i < DATA_LENGTH; ++i){
		UpDATA[i] = 0x00;
	}
}
//*********************************************************************
//* デバック用KURUMI LED 関数
//*********************************************************************
void funcKurumiLEDRed(){
	digitalWrite(led_red, LOW);
	digitalWrite(led_green, HIGH);
	digitalWrite(led_blue, HIGH);
	delay(KURUKI_LED_DELAY);
}
void funcKurumiLEDGreen(){
	digitalWrite(led_red, HIGH);
	digitalWrite(led_green, LOW);
	digitalWrite(led_blue, HIGH);
	delay(KURUKI_LED_DELAY);
}
void funcKurumiLEDBlue(){
	digitalWrite(led_red, HIGH);
	digitalWrite(led_green, HIGH);
	digitalWrite(led_blue, LOW);
	delay(KURUKI_LED_DELAY);
}
void funcKurumiLEDYellow(){
	digitalWrite(led_red, LOW);
	digitalWrite(led_green, LOW);
	digitalWrite(led_blue, HIGH);
	delay(KURUKI_LED_DELAY);
}
void funcKurumiLEDAqua(){
	digitalWrite(led_red, HIGH);
	digitalWrite(led_green, LOW);
	digitalWrite(led_blue, LOW);
	delay(KURUKI_LED_DELAY);
}
void funcKurumiLEDFechsia(){
	digitalWrite(led_red, LOW);
	digitalWrite(led_green, HIGH);
	digitalWrite(led_blue, LOW);
	delay(KURUKI_LED_DELAY);
}
void funcKurumiLEDWhite(){
	digitalWrite(led_red, LOW);
	digitalWrite(led_green, LOW);
	digitalWrite(led_blue, LOW);
	delay(KURUKI_LED_DELAY);
}