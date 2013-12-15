/*GR-KURUMI Sketch Template Version: E0.94*/
//*************************************************************************************************
//* Author: hiro 2013/12/13
//*************************************************************************************************
#include <RLduino78.h>
#include "iodefine.h"
#include "LedTape.h"
#include "UserData.h"
#include "ColorLED.h"

//*********************************************************************
//　関数定義
//*********************************************************************
bool loadUserData(int point = -1);
void StartAutoPlay(byte strat_mem_pos, byte end_mem_pos);
void NextAutoPlay();
void StopAutoPlay();
bool funcSettings();
bool funcUserSettings();
bool funcRequest();
void funcExec();
void funcUserExec();
void funcInitializeUserData();
bool chkModeChange(char mc);
bool chkUserModeChange(char mc);
bool chkAutoPlay();
bool funcReadDATA(int ReadByte, bool save = true);

void rainbowCycle(byte wait, char mc);
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
void autoPlay();

//*********************************************************************
// インスタンス宣言
//*********************************************************************
//ROM用インスタンス
UserData userdata;
//LedTapeインスタンス
LedTape ledtape;
//ColorLEDインスタンス
ColorLED kurumiLED;

//*********************************************************************
// 定数宣言
//*********************************************************************
const byte iMod = 8;			//モードの分母（固定）
const int DATA_LENGTH = 30;		//設定データのByte数
const unsigned long KURUMI_LED_DELAY = 30;	//デバッグ用KURUMI LED Deley値
const int MIN_DELAY_TIME = 1;
enum ExecMode{ eExecLED, eUser};
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
//AutoPlay用変数
bool AutoPlay = false;
bool bolFuncCheckOut = false;
int autoplay_start_point = 0, autoplay_end_point = 0;
int autoplay_point = 0;
int autoplay_loops = 5;	//autoplay実行回数値
int autoplay_loop_count = 0;	//autoplayカウンタ


//*********************************************************************
//* SETUP
//*********************************************************************
void setup() {
	//*****************************************************************
	// UserData初期化
	userdata.setup();	//UserDataセットアップ　インスタンス化後に必ず呼ぶ必要がある。
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
	
	//**********************************************************************
	// 格納データの読み込み
	loadUserData();
	
	//**********************************************************************
	// 初期化 ledtapeセットアップ
	ledtape.setup(LED_length, USE_brightness);	//ledtapeセットアップ　インスタンス化後に必ず呼ぶ必要がある。
	funcUpDATAInitialize();
	Serial2.begin(9600);
	
	kurumiLED.White();
	delay(500);
	
	//AUTOPLAY
	if(userdata.getAutoPlay() == 1){
		StartAutoPlay(userdata.getAutoPlayFrom(), userdata.getAutoPlayTo());
	} 
	
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
			chkModeChange(LED_Mode);	//障害が起きても復旧可能とする。
			kurumiLED.Red();
			delay(1000);
			break;
	}
}

//*********************************************************************
//* 関数定義
//*********************************************************************
bool loadUserData(int point){
	// 格納データの読み込み
	if(point == -1){
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
		iCounter = (int)userdata.getMoveCount(b_point);
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
		delayTime = MIN_DELAY_TIME + userdata.getDelayTime(b_point);	
	}
	return true;
}
//　メモリの初期化
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
		//autoplay対応
		StopAutoPlay();
		
		byte Tmp_LED_Mode = Serial2.read();
		//読み込んだ値がセッティングモードではない場合　モードチェンジ
		if(Tmp_LED_Mode < 'A' || Tmp_LED_Mode > 'Z'){
			LED_Mode = Tmp_LED_Mode;
			//　カウントアップ
			++iCounter;
			//*********************************************************
			//* データ保存はパラメータ保存時に行うように変更
			//*　カウントデータ保存
			//if((int)'a' <= LED_Mode && LED_Mode <= (int)'d'){
				//データ格納
			//	userdata.setMoveCount((byte)(iCounter % 8));
			//}
			//LED動作モード格納
			//if(LED_Mode != Current_LED_Mode){
			//	if((int)'a' <= LED_Mode && LED_Mode <= (int)'i'){
			//		userdata.setLedMode(LED_Mode);
			//	}
			//}
			return LED_Mode == Current_LED_Mode ? false : true;
		}
		
		if(Tmp_LED_Mode == (byte)'X'){
			return funcSettings();
		}else if(Tmp_LED_Mode == (byte)'Y'){
			//ユーザーモードへ変更
			exec_mode = eUser;
			return true;
		}else if(Tmp_LED_Mode == (byte)'Z'){
			//データ読み込み、実行
			return funcRequest();
		}else if(Tmp_LED_Mode == (byte)'U'){	//'U'はユーザーモード時のデータなので入ってきても捨てる。
			//データ空読み込み
			funcReadDATA(21, false);
		}
		return false;
    }else{
		//読み込めない場合。
		return false;
	}
};
//*********************************************************************
//* ユーザーモードチェンジチェック関数
//* 戻り値：　変更があれば　true、無ければ　false
//* ユーザーモードに入ったら、Uxxxxx・・・のデータしか来ない！他に来るのは実行モードのデータのみ
//*********************************************************************
bool chkUserModeChange(char Current_LED_Mode){
	if (Serial2.available() > 0){
		//autoplay対応
		StopAutoPlay();
		
		funcUpDATAInitialize();
		//先頭データを覗く
		byte Tmp_LED_Mode = Serial2.peek();
		//読み込んだ値がユーザーモードのデータではない場合
		if(Tmp_LED_Mode != (byte)'U'){
			//ユーザーモードではない場合は読み出し
			byte Tmp_LED_Mode = Serial2.read();
			if(Tmp_LED_Mode == (byte)'X'){
				return funcSettings();
			}else if(Tmp_LED_Mode == (byte)'Y'){
				//ユーザーモードへ変更
				exec_mode = eUser;
				return true;
			}else if(Tmp_LED_Mode == (byte)'Z'){
				return funcRequest();
			}else{
				//実行モード用の値に変更する。
				exec_mode = eExecLED;
				LED_Mode = Tmp_LED_Mode;
				return true;
			}
			return false;
		}else{
			//ユーザモードでの設定をする
			return funcUserSettings();
		}
	}else{
		//読み込めない場合。
		return false;
	}
};
//*********************************************************************
//AutoPlay 指定のループ数を超えたら再読み込み。
//*********************************************************************
bool chkAutoPlay(){
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
void StartAutoPlay(byte strat_mem_pos, byte end_mem_pos){
	if(AutoPlay) return;
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
	//範囲に有効なループカウントが１つは存在する事を確認する。
	bool ExistsLoopNumber = false;
	for(int i = strat_mem_pos; i <= end_mem_pos; ++i){
		if(userdata.getLoops(i) > 0){
			ExistsLoopNumber = true;
			break;
		}
	}	
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
void NextAutoPlay(){
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
}
// AutoPlayを止める
void StopAutoPlay(){
	if(!AutoPlay) return;
	AutoPlay = false;
	autoplay_loop_count = 0;
	userdata.setAutoPlay(0x00);
	loadUserData();
}
//****************************************************************
//* セッティング関数
//　認識文字	'X'
// Setting String Format 合計30byte 
//	0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		
// "L00"		個数		最大輝度		使用輝度		DelayTime	Loop回数	AutoMode	0			0			"END"		***LED　Params　設定 保存
// "I00"		0			0			0			0			0			0			0			0			"END"		***Initialize　保存
// "I01"		MemOffset	0			0			0			0			0			0			0			"END"		***MemoryOffset　保存
// "I02"		AutoPlay	From		To			0			0			0			0			0			"END"		***AutoPlay用の値設定 保存
// "I03"		MemOffset	0			0			0			0			0			0			0			"END"		***メモリクリア
// "I10"		使用輝度		0			0			0			0			0			0			0			"END"		***使用輝度変更　非保存
// "I11"		DelayTime	0			0			0			0			0			0			0			"END"		***DelayTime 非保存		
//****************************************************************
bool funcSettings(){
	//初期化と読み込み
	funcUpDATAInitialize();
	if(!funcReadDATA(DATA_LENGTH)) return false;
	
	//読み込み判定、終わりが「END」であるか？
	if(UpDATA[27] == 'E' && UpDATA[28] == 'N' && UpDATA[29] == 'D'){
		int intLEDs, intMax, intUse, intDelayTime, intLoops, intAutoMode;
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
					//**********************************************
					//動作モードはここで保存するように変更　2013/12/15
					//LED動作モード格納
					if((int)'a' <= LED_Mode && LED_Mode <= (int)'i'){
						userdata.setLedMode(LED_Mode);
					}
					if((int)'a' <= LED_Mode && LED_Mode <= (int)'d'){
						//データ格納
						userdata.setMoveCount((byte)(iCounter % 8));
					}
					userdata.setExecMode((byte)eExecLED);
					
					//AutoPlayパラメータ保存
					intLoops = getNumberTo3CharArray(UpDATA, 15);
					intAutoMode = getNumberTo3CharArray(UpDATA, 18);
					userdata.setLoops(int2byte(intLoops));
					if(intAutoMode == 0){
						userdata.setLoopExecMode(0x00);
					}else{
						userdata.setLoopExecMode(0x01);
					}
				}
				return true;

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
					
					return true;
					
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
					
					return true;

				}else if(UpDATA[1] == '0' && UpDATA[2] == '2'){
					byte b_tmp = int2byte(getNumberTo3CharArray(UpDATA, 3));
					if(b_tmp == 1) userdata.setAutoPlay(0x01);
					byte b_from = int2byte(getNumberTo3CharArray(UpDATA, 6));
					byte b_to = int2byte(getNumberTo3CharArray(UpDATA, 9));
					userdata.setAutoPlayFrom(b_from);
					userdata.setAutoPlayTo(b_to);
					
					//Execute Auto Play
					StartAutoPlay(b_from, b_to);
					
					return true;

				}else if(UpDATA[1] == '0' && UpDATA[2] == '3'){
					byte b_tmp = int2byte(getNumberTo3CharArray(UpDATA, 3));
					if(b_tmp == userdata.getMemoryOffset()) return false;
					userdata.clear(b_tmp);
					
					return false;
					
				}else if(UpDATA[1] == '1' && UpDATA[2] == '0'){
					//　輝度の一時設定
					USE_brightness = (byte)getNumberTo3CharArray(UpDATA, 3);
					return true;
				}else if(UpDATA[1] == '1' && UpDATA[2] == '1'){
					// DelayTimeの一時設定
					delayTime = (byte)(MIN_DELAY_TIME + getNumberTo3CharArray(UpDATA, 3));
					return true;
				}
				break;
			
			default:
				break;
		}
	}
	return false;
}
//****************************************************************
//* 送信関数
//　認識文字	’Z'
// Request String Format 合計30byte 
//	0			1			2			3			4			5			6			7			8			9
// 3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		3byte		
// "R01"		0			0			0			0			0			0			0			0			"END"		***LED設定等送信要求
// "R02"		LED個数		最大輝度		使用輝度		ﾒﾓﾘｵﾌｾｯﾄ値	DelayTime	Loop回数	AutoMode	"END"		***返信	
// "R03"		0			0			0			0			0			0			0			0			"END"		***ユーザモード設定送信要求
// "R04"		動作Mode	輝度		R値			G値			B値			"END"		***返信	
// "R05"		0			0			0			0			0			0			0			0			"END"		***AUTOPLAY　FROM　TO 設定送信要求
// "R06"		FROM		TO			"END"		***返信	
//****************************************************************
bool funcRequest(){
	//初期化と読み込み
	funcUpDATAInitialize();
	if(!funcReadDATA(DATA_LENGTH)) return false;
	
	//読み込み判定、終わりが「END」であるか？
	if(UpDATA[27] == 'E' && UpDATA[28] == 'N' && UpDATA[29] == 'D'){
		if(UpDATA[0] == 'R'){
			//R01の場合の返信
			if(UpDATA[1] == '0' && UpDATA[2] == '1'){
				Serial2.print("R02");
				Serial2.print(funcNum2Str(LED_length,3));
				Serial2.print(funcNum2Str(MAX_brightness,3));
				Serial2.print(funcNum2Str(userdata.getUseBrightness(),3));
				Serial2.print(funcNum2Str(userdata.getMemoryOffset(),3));
				Serial2.print(funcNum2Str(userdata.getDelayTime(),3));
				Serial2.print(funcNum2Str(userdata.getLoops(),3));
				Serial2.print(funcNum2Str(userdata.getLoopExecMode(),3));
				Serial2.print("END");
				
				//使用輝度とdelayTimeは初期値を読み込む。（一時変更の対応のため）
				USE_brightness = userdata.getUseBrightness();	
				delayTime = MIN_DELAY_TIME + userdata.getDelayTime();
				exec_mode = eExecLED;
				return true;
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
				exec_mode = eUser;
				return true;
			}
			//R05の場合の返信
			if(UpDATA[1] == '0' && UpDATA[2] == '5'){
				//動作Mode	輝度		R値			G値			B値
				Serial2.print("R06");
				Serial2.print(funcNum2Str(userdata.getAutoPlayFrom(),3));
				Serial2.print(funcNum2Str(userdata.getAutoPlayTo(),3));
				Serial2.print("END");
				exec_mode = eExecLED;
				return true;
			}
		}
	}
	return false;
}
//****************************************************************
//* ユーザーモードセッティング関数
//　認識文字	’Y’
// UserSetting String Format 合計30byte 
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
	if(!funcReadDATA(21)) return false;
	//読み込み判定、終わりが「END」であるか？
	if(UpDATA[18] == 'E' && UpDATA[19] == 'N' && UpDATA[20] == 'D'){
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
					//ここで保存するように変更 2013/12/15
					userdata.setExecMode((byte)eUser);
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
//* シリアルポートからのデータ読み込み関数
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
//****************************************************************
void funcExec(){
	if(exec_mode != eExecLED) return;
	
	//userdata.setExecMode((byte)eExecLED);
	//USE_brightness = userdata.getUseBrightness();	
	//delayTime = MIN_DELAY_TIME + userdata.getDelayTime();
	
	while(true){
		if(exec_mode != eExecLED) return;
		if(bolFuncCheckOut) return;
		int i = 0;
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
				funcOff(LED_Mode, 50);
				break;
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
	//userdata.setExecMode((byte)eUser);
	USER_brightness = (byte)userdata.getUserBrightness();
	userdata.getUserColor(&TCUser);
	
	while(true){
		if(exec_mode != eUser) return;
		if(bolFuncCheckOut) return;
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
//* 動作関数にはモードチェック関数【chkModeChange】及び【chkAutoPlay】を含める事
//****************************************************************

//星（江口さんのやつのループ）
void funcStar(TColor tc, char mc, int mi){
	ledtape.setAllBrightness(USE_brightness);
	int star_count = 5;
	int current_pos = 0;
	while(true){
		if(current_pos == (LED_length + star_count)){
			current_pos = 0; 
			if(chkAutoPlay()) return;
		}
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
	int autoplay_count = 0;
	while(true){
		delay(delayTime);
		if(chkModeChange(mc)) return;
		if(iCounter != mi) return;
		++autoplay_count;
		if(autoplay_count == 10){
			autoplay_count = 0;
			if(chkAutoPlay()) return;
		}
	}
}

//Twincle
void funcStar1(TColor tc, char mc, int mi, bool b_randam){
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
		//delay(1);
		ledtape.clearAllColors();
		int randam_delay_time = random(5, 150);
		delay(randam_delay_time + my_delay_time);
		if(chkModeChange(mc)) return;
		if(iCounter != mi) return;
		++autoplay_count;
		if(autoplay_count == LED_length){
			autoplay_count = 0;
			if(chkAutoPlay()) return;
		}
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
		if(rCnt == LightCount){
			rCnt = 0;
			if(chkAutoPlay()) return;
		}
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
		if(chkAutoPlay()) return;
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
		if(chkAutoPlay()) return;
    }
}

// 虹色表示
void funcRainbow(char mc){
	ledtape.setAllBrightness(USE_brightness);
	int my_delay_time = 0;
    while(true)
    {
	    for(int i = 0 ; i < 5 ; i++)
		{
			my_delay_time = delayTime > MIN_DELAY_TIME ? delayTime - MIN_DELAY_TIME : 0;
			my_delay_time = my_delay_time < 26 ? 0 : my_delay_time - 26;	//デフォルトのスピード調整
			rainbowCycle(i + my_delay_time, mc);
			//注意：rainbowCycle内でchkModeChangeを呼んでいるのでここでは比較のみ。
			if(LED_Mode != mc) return;
			if(exec_mode != eExecLED) return;
			//注意：rainbowCycle内でchkAutoPlayを呼んでいる。
			if(bolFuncCheckOut) return;
		}
    }
}

void funcGradient(char mc){
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
		delay(delayTime);
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
					delay(delayTime);
					if(chkModeChange(mc)) return;
				}while(0 < x && x < 0xFF);		
			}
		}
		if(chkAutoPlay()) return;
	}
}

//消す
void funcOff(char mc, unsigned long ms){
	ledtape.setAllColors(g_t_off);
	int autoplay_count = 0;
	while(true){
		if(chkModeChange(mc)) return;
		delay(ms);
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
void rainbowCycle(byte wait, char mc) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< LED_length; i++) {
       Wheel(i, ((i * 256 / LED_length) + j) & 255);
    }
    ledtape.send();
	delay(wait);
	if(chkModeChange(mc)) return;
	if(j % 255 == 0) {
		if(chkAutoPlay()) return;
	}
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
//*********************************************************************
//* <-
//*********************************************************************

//*********************************************************************
//* ユーザーモード用関数
//*********************************************************************
void funcStar_User(char mc){
	ledtape.setAllBrightness(USER_brightness);
	int star_count = 5;
	int current_pos = 0;
	while(true){
		if(current_pos == (LED_length + star_count)){
			current_pos = 0;
			if(chkAutoPlay()) return;
		}
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
	int autoplay_count = 0;
	while(true){
		delay(delayTime);
		if(chkUserModeChange(mc)) return;
		if(chkAutoPlay()) return;
		++autoplay_count;
		if(autoplay_count == 10){
			autoplay_count = 0;
			if(chkAutoPlay()) return;
		}
	}
	
}

//Twincle
void funcStar1_User(char mc){
	ledtape.setAllBrightness(USER_brightness);
	int my_delay_time = 0;
	int autoplay_count = 0;
	while(true){
		my_delay_time = delayTime > MIN_DELAY_TIME ? delayTime - MIN_DELAY_TIME : 0;
		randomSeed(millis());
		int led = random(0, LED_length);
		ledtape.setColor(led, TCUser, LedTape::ClearAllAndAutoSend);
		//delay(1);
		ledtape.clearAllColors();
		int random_delay_time = random(5, 150);
		delay(random_delay_time + my_delay_time);
		if(chkUserModeChange(mc)) return;
		if(chkAutoPlay()) return;
		++autoplay_count;
		if(autoplay_count == LED_length){
			autoplay_count = 0;
			if(chkAutoPlay()) return;
		}
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
		if(rCnt == LightCount){
			rCnt = 0;
			if(chkAutoPlay()) return;
		}
		//送信
		ledtape.send();
		delay(delayTime);
		if(chkUserModeChange(mc)) return;
	}
}

//*********************************************************************
//* 輝くカチューシャ
//*********************************************************************
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
