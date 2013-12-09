#include "UserData.h"
//コンストラクタ setup　以前にインスタンス化されるとEEPROMが使用できずバグになる。
//よってコンストラクタでは処理しない。
UserData::UserData(){
	m_MemoryPoint = DATA_OFFSET;
};
UserData::~UserData(){};
void UserData::initialize(bool initMemoryOffset){
	//*************************************************************************************
	//->Kurumiに最初にインストールした際の初期化
	bool initChk = true;
	for(int i = 0; i < 4; ++i){
		if((int)EEPROM.read(MemoryInitializeKeyPoint + i) != chkMemoryInitializeKey[i]){
			initChk = false;
			break;
		}
	}
	//チェックキーと一致しなかった場合は使用する全メモリを初期化してチェックキーを書き込む
	if(!initChk){
		int memoryLastPoint = DATA_OFFSET + (DATA_CAPACITY * MAX_OFFSET_VALUE);
		for(int i = DATA_OFFSET;i <= memoryLastPoint; ++i){
			EEPROM.write((unsigned long)i, 0x00); 
		}
		//チェックキーの書き込み
		for(int i = 0; i < 4; ++i){
			EEPROM.write(MemoryInitializeKeyPoint + i,(byte)chkMemoryInitializeKey[i]);
		}		
	}
	//<-Kurumiに最初にインストールした際の初期化
	//*************************************************************************************

	if(initMemoryOffset){
		EEPROM.write(MEMORY_OFFSET_VALUE_ADDRESS, 0x00);	//メモリ保存オフセット値
		m_MemoryPoint = DATA_OFFSET;						//(0x00 * DATA_CAPACITY) + DATA_OFFSET;
	}
	EEPROM.write(getMemoryPoint(eLEDs), 0x13);				//初期値：19
	EEPROM.write(getMemoryPoint(eMaxBrightness), 0x80);		//初期値：128
	EEPROM.write(getMemoryPoint(eUseBrightness), 0x80);		//初期値：128
	EEPROM.write(getMemoryPoint(eLedMode), (byte)'1');		//初期値：'1'
	EEPROM.write(getMemoryPoint(eMoveCount), 0x00);			//初期値：0
	EEPROM.write(getMemoryPoint(eRedValue), 0x80);			//初期値：128
	EEPROM.write(getMemoryPoint(eGreenValue), 0x80);		//初期値：128
	EEPROM.write(getMemoryPoint(eBlueValue), 0x80);			//初期値：128
	EEPROM.write(getMemoryPoint(eUserLedMode), (byte)'a');	//初期値：'a'
	EEPROM.write(getMemoryPoint(eUserBrightness), 0x80);	//初期値：128
	EEPROM.write(getMemoryPoint(eExecMode), 0x00);			//初期値：0
	EEPROM.write(getMemoryPoint(eDelayTime), 0x14);			//初期値：20
	
	EEPROM.write(getMemoryPoint(eInit), 0x80);				//設定完了値：0x80
};
void UserData::setup(){
	//SET m_MemoryPoint
	byte b = (byte)EEPROM.read(MEMORY_OFFSET_VALUE_ADDRESS);
	if(b > MAX_OFFSET_VALUE){
		b = 0x00;
		EEPROM.write(MEMORY_OFFSET_VALUE_ADDRESS, b);
	}
	m_MemoryPoint = (b * DATA_CAPACITY) + DATA_OFFSET;
	//*************************************************************************************
	//kurumiを最初に使用した際に、キーを確認してメモリ領域を初期化する。
	for(int i = 0; i < 4; ++i){
		if((int)EEPROM.read(MemoryInitializeKeyPoint + i) != chkMemoryInitializeKey[i]){
			initialize(true);
			break;
		}
	}
};
byte UserData::getLEDs(){
	return (byte)EEPROM.read(getMemoryPoint(eLEDs));
};
byte UserData::getMaxBrightness(){
	return (byte)EEPROM.read(getMemoryPoint(eMaxBrightness));
};
byte UserData::getUseBrightness(){
	return (byte)EEPROM.read(getMemoryPoint(eUseBrightness));
};
byte UserData::getLedMode(){
	return (byte)EEPROM.read(getMemoryPoint(eLedMode));
};
byte UserData::getMoveCount(){
	return (byte)EEPROM.read(getMemoryPoint(eMoveCount));
};
void UserData::getUserColor(TColor *tc){
	tc->red = (byte)EEPROM.read(getMemoryPoint(eRedValue));
	tc->green = (byte)EEPROM.read(getMemoryPoint(eGreenValue));
	tc->blue = (byte)EEPROM.read(getMemoryPoint(eBlueValue));
};
byte UserData::getUserLedMode(){
	return (byte)EEPROM.read(getMemoryPoint(eUserLedMode));
};
byte UserData::getUserBrightness(){
	return (byte)EEPROM.read(getMemoryPoint(eUserBrightness));
};
byte UserData::getExecMode(){
	return (byte)EEPROM.read(getMemoryPoint(eExecMode));
};	
byte UserData::getDelayTime(){
	return (byte)EEPROM.read(getMemoryPoint(eDelayTime));
};	

void UserData::setLEDs(byte LC){
	if(EEPROM.read(getMemoryPoint(eLEDs)) != LC) EEPROM.write(getMemoryPoint(eLEDs), LC);
};
void UserData::setMaxBrightness(byte MB){
	if(EEPROM.read(getMemoryPoint(eMaxBrightness)) != MB) EEPROM.write(getMemoryPoint(eMaxBrightness), MB);
};
void UserData::setUseBrightness(byte UB){
	if(EEPROM.read(getMemoryPoint(eUseBrightness)) != UB) EEPROM.write(getMemoryPoint(eUseBrightness), UB);
};
void UserData::setLedMode(byte MM){
	if(EEPROM.read(getMemoryPoint(eLedMode)) != MM) EEPROM.write(getMemoryPoint(eLedMode), MM);
};
void UserData::setMoveCount(byte MC){
	if(EEPROM.read(getMemoryPoint(eMoveCount)) != MC) EEPROM.write(getMemoryPoint(eMoveCount), MC);
};
void UserData::setUserColor(TColor *tc){
	if(EEPROM.read(getMemoryPoint(eRedValue)) != tc->red) EEPROM.write(getMemoryPoint(eRedValue), (byte)tc->red);
	if(EEPROM.read(getMemoryPoint(eGreenValue)) != tc->green) EEPROM.write(getMemoryPoint(eGreenValue), (byte)tc->green);
	if(EEPROM.read(getMemoryPoint(eBlueValue)) != tc->blue) EEPROM.write(getMemoryPoint(eBlueValue), (byte)tc->blue);
};
void UserData::setUserLedMode(byte MM){
	if(EEPROM.read(getMemoryPoint(eUserLedMode)) != MM) EEPROM.write(getMemoryPoint(eUserLedMode), MM);
};
void UserData::setUserBrightness(byte UB){
	if(EEPROM.read(getMemoryPoint(eUserBrightness)) != UB) EEPROM.write(getMemoryPoint(eUserBrightness), UB);
};
void UserData::setExecMode(byte EM){
	if(EEPROM.read(getMemoryPoint(eExecMode)) != EM) EEPROM.write(getMemoryPoint(eExecMode), EM);
};
void UserData::setDelayTime(byte DT){
	if(EEPROM.read(getMemoryPoint(eDelayTime)) != DT) EEPROM.write(getMemoryPoint(eDelayTime), DT);
};

//メモリ読み込み位置をセットする。
void UserData::setMemoryOffset(byte offset){
	if(offset > MAX_OFFSET_VALUE){
		offset = 0x00;
	}
	EEPROM.write(MEMORY_OFFSET_VALUE_ADDRESS, offset);
	//情報更新
	m_MemoryPoint = (offset * DATA_CAPACITY) + DATA_OFFSET;
	//*************************************************************************************
	//eInitの値が初期値ではない場合にだけ初期化する。
	if((byte)EEPROM.read(getMemoryPoint(eInit)) != 0x80) initialize(false);
};	
byte UserData::getMemoryOffset(){
	return (byte)EEPROM.read(MEMORY_OFFSET_VALUE_ADDRESS);
};
//メモリーのオフセット値から読み込むべきメモリ位置を取得する。
unsigned long UserData::getMemoryPoint(DataPoint dp){
	return (unsigned long) (m_MemoryPoint + (unsigned long)dp);
};
