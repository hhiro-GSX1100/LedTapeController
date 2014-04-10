//*************************************************************************************************
//* Author: hiro 2013/12/13 Earring対応 2014/1/2
//* 【注意】メモリ読み込み、書き込み位置は、getMemoryPointを咬ませる事。
//* 読み込み位置を更新した場合は、private変数MemoryPointを更新すること。
//* メモリ読み込み位置は、(MEMORY_OFFSET_VALUE_ADDRESSに格納されている値×DATA_CAPACITY byte)+ DATA_OFFSETとする。
//* 【重要】スケッチの　setup　実行前のにインスタンス化しコンストラクタが呼ばれてにEEPROMが呼応しない事に対応し、　
//*   インスタンス化後には必ず UserData::setup() を実行すること。
//*************************************************************************************************
#ifndef _USERDATA_H_
#define _USERDATA_H_

#include <EEPROM.h>
#include "LedTape.h"

class UserData{
	public:
		//Offset可能な最大値、これを超えると位置は「0」とされるので注意すること。
		static const byte MAX_OFFSET_VALUE = 0x64;			//100
		
		//コンストラクタ
		UserData();
		~UserData();
		void initialize(bool initMemoryOffset = true);	//初期化関数
		void setup();									//セットアップ関数
		
		byte getAutoPlay();								//AutoPlayの値
		byte getAutoPlayFrom();							//AutoPlayのFROM値
		byte getAutoPlayTo();							//AutoPlayのTO値
		byte getLEDs();									//LEDの使用個数
		byte getMaxBrightness();						//最大輝度
		byte getUseBrightness();						//使用輝度
		byte getLedMode();								//LED動作モード
		byte getMoveCount();							//カウント値
		void getUserColor(TColor *tc);					//ユーザー設定カラー値
		byte getUserLedMode();							//ユーザー設定動作モード
		byte getUserBrightness();						//ユーザー設定輝度
		byte getExecMode();								//実行モード（実行モードで動作か、ユーザモードで動作かを保存する）
		byte getMemoryOffset();							//設定値のメモリを格納する場所をオフセットする場合のオフセット値
		byte getDelayTime();							//スピード値
		byte getLoops();								//Autoplayでのループ回数
		byte getLoopExecMode();							//AutoPlayでの実行モード
		void getEarringColor(TColor *tc);				//イヤリングのカラー
		byte getEarringBrightness();					//イヤリングの輝度
		byte getEarringSpeed();							//イヤリングの速度
		byte getEarringMode();							//イヤリングの動作モード
		
		//******************************************************
		//メモリ格納値ループに使用するgetter等関数
		bool isEnableOffsetRange(byte offset_pos);		//使用可能であるかをブール値で返す。
		bool isInitData(byte offset_pos);				//初期化済みであるかをブール値で返す。
		byte getLEDs(byte offset_pos);					//LEDの使用個数
		byte getUseBrightness(byte offset_pos);			//使用輝度（メモリ位置指定）
		byte getLedMode(byte offset_pos);				//LED動作モード（メモリ位置指定）
		byte getMoveCount(byte offset_pos);				//カウント値（メモリ位置指定）
		void getUserColor(byte offset_pos, TColor *tc);	//ユーザー設定カラー値（メモリ位置指定）
		byte getUserLedMode(byte offset_pos);			//ユーザー設定動作モード（メモリ位置指定）
		byte getUserBrightness(byte offset_pos);		//ユーザー設定輝度（メモリ位置指定）
		byte getExecMode(byte offset_pos);				//実行モード（実行モードで動作か、ユーザモードで動作かを保存する）
		byte getDelayTime(byte offset_pos);				//実行スピード
		byte getLoops(byte offset_pos);					//AutoPlayでのループ回数
		byte getLoopExecMode(byte offset_pos);			//AutoPlayでの実行モード
		void getEarringColor(byte offset_pos, TColor *tc);	//イヤリングのカラー
		byte getEarringBrightness(byte offset_pos);		//イヤリングの輝度
		byte getEarringSpeed(byte offset_pos);			//イヤリングの速度
		byte getEarringMode(byte offset_pos);			//イヤリングの動作モード
		
		void setAutoPlay(byte AutoPlay);				//AutoPlayの値
		void setAutoPlayFrom(byte AutoPlayFrom);		//AutoPlayのFROM値
		void setAutoPlayTo(byte AutoPlayTo);			//AutoPlayのTo値
		void setLEDs(byte LEDs);						//LEDの個数をセットする
		void setMaxBrightness(byte MaxBrightness);		//最大輝度
		void setUseBrightness(byte UseBrightness);		//使用輝度
		void setLedMode(byte MoveMode);					//LED動作モード
		void setMoveCount(byte MoveCount);				//カウント値
		void setUserColor(TColor *tc);					//ユーザー設定カラー値
		void setUserLedMode(byte MoveMode);				//ユーザー設定動作モード
		void setUserBrightness(byte UserBrightness);	//ユーザー設定輝度
		void setExecMode(byte ExecMode);				//実行モード
		void setDelayTime(byte DelayTime);				//実行スピード
		void setLoops(byte Loops);						//AutoPlayでのループ回数
		void setLoopExecMode(byte LoopExecMode);		//AutoPlayでの実行モード
		void setEarringColor(TColor *tc);				//イヤリングのカラー
		void setEarringBrightness(byte EarringBrightness);		//イヤリングの輝度
		void setEarringSpeed(byte EarringSpeed);		//イヤリングの速度
		void setEarringMode(byte EarringMode);			//イヤリングの動作モード
		
		void setMemoryOffset(byte offset);				//設定値のメモリを格納する場所をオフセットする場合のオフセット値
		bool clear(byte memory_point);					//メモリをクリアする。但し、現在使用中のメモリはクリアできない。
	private:
		//メモリーを格納する位置を保持する場所（先頭アドレス）
		static const unsigned long MEMORY_OFFSET_VALUE_ADDRESS = 0;
		static const unsigned long AUTOPLAY_ADDRESS = 1;
		static const unsigned long AUTOPLAY_FROM = 2;
		static const unsigned long AUTOPLAY_TO = 3;
		//チェックキー　この値が保持されていない場合は最初の使用とみなして使用する全メモリ帯域を初期化する。
		const int chkMemoryInitializeKey[4] = {1,1,9,2};
		static const unsigned long MemoryInitializeKeyPoint = 5;
		//設定済みの値
		static const byte INIT_VALUE = 0x80;
		//メモリ保持位置を決める列挙値先頭から16byteは予備のために空けておく。
		static const unsigned long DATA_OFFSET = 16;		//16
		static const unsigned long DATA_CAPACITY = 32;		//32
		//メモリ読み込み位置は、(MEMORY_OFFSET_VALUE_ADDRESSに格納されている値×DATA_CAPACITY byte)+ DATA_OFFSETとする。
		unsigned long m_MemoryPoint;
		
		enum DataPoint {
			eInit,
			eLEDs,
			eMaxBrightness,
			eUseBrightness,
			eLedMode,
			eMoveCount,
			eRedValue,
			eGreenValue,
			eBlueValue,
			eUserLedMode,
			eUserBrightness,
			eExecMode,
			eDelayTime,
			eLoops,
			eLoopExecMode,
			eEarringRed,
			eEarringGreen,
			eEarringBlue,
			eEarringBrightness,
			eEarringSpeed,
			eEarringMode
		};
		
		unsigned long getMemoryPoint(DataPoint dp);
		unsigned long getMemoryPoint(byte memory_point, DataPoint dp);
		bool isManagedOffsetRange(byte memory_point);
};

#endif // _USERDATA_H_