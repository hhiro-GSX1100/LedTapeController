#ifndef _USERDATA_H_
#define _USERDATA_H_

#include <eeprom.h>
#include "LedTape.h"

//***************************************************************************************
//* 【注意】メモリ読み込み、書き込み位置は、getMemoryPointを咬ませる事。
//* 読み込み位置を更新した場合は、private変数MemoryPointを更新すること。
//* メモリ読み込み位置は、(MEMORY_OFFSET_VALUE_ADDRESSに格納されている値×DATA_CAPACITY byte)+ DATA_OFFSETとする。
//* 【重要】スケッチの　setup　実行前のにインスタンス化しコンストラクタが呼ばれてにEEPROMが呼応しない事に対応し、　
//*   インスタンス化後には必ず UserData::setup() を実行すること。
//***************************************************************************************
class UserData{
	public:
		//コンストラクタ
		UserData();
		~UserData();
		void initialize(bool initMemoryOffset = true);								//初期化関数
		void setup();									//セットアップ関数
		byte getLEDs();									//LEDの使用個数
		byte getMaxBrightness();						//最大輝度
		byte getUseBrightness();						//使用輝度
		byte getLedMode();								//LED動作モード
		byte getMoveCount();							//カウント値
		void getUserColor(TColor *tc);					//ユーザー設定カラー値
		byte getUserLedMode();							//ユーザー設定動作モード
		byte getUserBrightness();						//ユーザー設定輝度
		byte getExecMode();								//実行モード
		byte getMemoryOffset();							//ユーザー設定のメモリを格納する場所をオフセットする場合のオフセット値
		byte getDelayTime();
		void setLEDs(byte LEDs);						//LEDの個数をセットする
		void setMaxBrightness(byte MaxBrightness);		//最大輝度
		void setUseBrightness(byte UseBrightness);		//使用輝度
		void setLedMode(byte MoveMode);					//LED動作モード
		void setMoveCount(byte MoveCount);				//カウント値
		void setUserColor(TColor *tc);					//ユーザー設定カラー値
		void setUserLedMode(byte MoveMode);				//ユーザー設定動作モード
		void setUserBrightness(byte UserBrightness);	//ユーザー設定輝度
		void setExecMode(byte ExecMode);				//実行モード
		void setMemoryOffset(byte offset);				//ユーザー設定のメモリを格納する場所をオフセットする場合のオフセット値
		void setDelayTime(byte DelayTime);
	private:
		//メモリーを格納する位置を保持する場所（先頭アドレス）
		const unsigned long MEMORY_OFFSET_VALUE_ADDRESS = 0;
		//チェックキー　この値が保持されていない場合は最初の使用とみなして使用する全メモリ帯域を初期化する。
		const int chkMemoryInitializeKey[4] = {1,1,9,2};
		const unsigned long MemoryInitializeKeyPoint = 5;
		//Offset可能な最大値、これを超えると位置は「0」とされるので注意すること。
		const byte MAX_OFFSET_VALUE = 0x64;			//100
		//メモリ保持位置を決める列挙値先頭から16byteは予備のために空けておく。
		const unsigned long DATA_OFFSET = 16;		//16
		const unsigned long DATA_CAPACITY = 32;		//32
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
			eDelayTime
		};
		unsigned long getMemoryPoint(DataPoint dp);
};

#endif // _USERDATA_H_