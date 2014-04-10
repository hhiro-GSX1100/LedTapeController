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
	EEPROM.write(getMemoryPoint(eLEDs), 19);				//初期値：19
	EEPROM.write(getMemoryPoint(eMaxBrightness), 0x80);		//初期値：128
	EEPROM.write(getMemoryPoint(eUseBrightness), 0x80);		//初期値：128
	EEPROM.write(getMemoryPoint(eLedMode), (byte)'a');		//初期値：'a'
	EEPROM.write(getMemoryPoint(eMoveCount), 0x00);			//初期値：0
	EEPROM.write(getMemoryPoint(eRedValue), 0x80);			//初期値：128
	EEPROM.write(getMemoryPoint(eGreenValue), 0x80);		//初期値：128
	EEPROM.write(getMemoryPoint(eBlueValue), 0x80);			//初期値：128
	EEPROM.write(getMemoryPoint(eUserLedMode), (byte)'a');	//初期値：'a'
	EEPROM.write(getMemoryPoint(eUserBrightness), 0x80);	//初期値：128
	EEPROM.write(getMemoryPoint(eExecMode), 0x00);			//初期値：0
	EEPROM.write(getMemoryPoint(eDelayTime), 0x14);			//初期値：20
	EEPROM.write(getMemoryPoint(eLoops), 0x05);				//初期値：5	
	EEPROM.write(getMemoryPoint(eLoopExecMode), 0x00);		//初期値：0
	EEPROM.write(getMemoryPoint(eEarringRed), 0x00);		//初期値：0
	EEPROM.write(getMemoryPoint(eEarringGreen), 0x00);		//初期値：0
	EEPROM.write(getMemoryPoint(eEarringBlue), 0x00);		//初期値：0
	EEPROM.write(getMemoryPoint(eEarringBrightness), 0x00);	//初期値：0
	EEPROM.write(getMemoryPoint(eEarringSpeed), 0x0a);		//初期値：10
	EEPROM.write(getMemoryPoint(eEarringMode), (byte)'a');	//初期値：'a'
	
	EEPROM.write(getMemoryPoint(eInit), INIT_VALUE);		//設定完了値：0x80
};
void UserData::setup(){
	//SET m_MemoryPoint
	byte b = (byte)EEPROM.read(MEMORY_OFFSET_VALUE_ADDRESS);
	if(!isManagedOffsetRange(b)){
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

//******************************************************
//* getter
//******************************************************
byte UserData::getAutoPlay(){
	return (byte)EEPROM.read(AUTOPLAY_ADDRESS);
}
byte UserData::getAutoPlayFrom(){
	return (byte)EEPROM.read(AUTOPLAY_FROM);
}
byte UserData::getAutoPlayTo(){
	return (byte)EEPROM.read(AUTOPLAY_TO);
}
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
byte UserData::getLoops(){
	return (byte)EEPROM.read(getMemoryPoint(eLoops));
};	
byte UserData::getLoopExecMode(){
	return (byte)EEPROM.read(getMemoryPoint(eLoopExecMode));
};	
void UserData::getEarringColor(TColor *tc){
	tc->red = (byte)EEPROM.read(getMemoryPoint(eEarringRed));
	tc->green = (byte)EEPROM.read(getMemoryPoint(eEarringGreen));
	tc->blue = (byte)EEPROM.read(getMemoryPoint(eEarringBlue));
};
byte UserData::getEarringBrightness(){
	return (byte)EEPROM.read(getMemoryPoint(eEarringBrightness));
};
byte UserData::getEarringSpeed(){
	return (byte)EEPROM.read(getMemoryPoint(eEarringSpeed));
};
byte UserData::getEarringMode(){
	return (byte)EEPROM.read(getMemoryPoint(eEarringMode));
};
//メモリ格納値ループに使用するgetter等関数
bool UserData::isEnableOffsetRange(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return ((byte)EEPROM.read(getMemoryPoint(offset_pos, eInit))) == INIT_VALUE;
	}
	return false;
};	
byte UserData::getLEDs(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eLEDs));
	}
	return 19;
};
byte UserData::getUseBrightness(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eUseBrightness));
	}
	return 0x00;
};
byte UserData::getLedMode(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eLedMode));
	}
	return 0x00;
};
byte UserData::getMoveCount(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eMoveCount));
	}
	return 0x00;
};
void UserData::getUserColor(byte offset_pos, TColor *tc){
	if(isManagedOffsetRange(offset_pos)){
		tc->red = (byte)EEPROM.read(getMemoryPoint(offset_pos, eRedValue));
		tc->green = (byte)EEPROM.read(getMemoryPoint(offset_pos, eGreenValue));
		tc->blue = (byte)EEPROM.read(getMemoryPoint(offset_pos, eBlueValue));
	}
};
byte UserData::getUserLedMode(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eUserLedMode));
	}
	return 0x00;
};
byte UserData::getUserBrightness(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eUserBrightness));
	}
	return 0x00;
};
byte UserData::getExecMode(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eExecMode));
	}
	return 0x00;
};
byte UserData::getDelayTime(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eDelayTime));
	}
	return 0x00;
};
byte UserData::getLoops(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eLoops));
	}
	return 0x00;
};
byte UserData::getLoopExecMode(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eLoopExecMode));
	}
	return 0x00;
};
void UserData::getEarringColor(byte offset_pos, TColor *tc){
	if(isManagedOffsetRange(offset_pos)){
		tc->red = (byte)EEPROM.read(getMemoryPoint(offset_pos, eEarringRed));
		tc->green = (byte)EEPROM.read(getMemoryPoint(offset_pos, eEarringGreen));
		tc->blue = (byte)EEPROM.read(getMemoryPoint(offset_pos, eEarringBlue));
	}
};
byte UserData::getEarringBrightness(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eEarringBrightness));
	}
	return 0x00;
};
byte UserData::getEarringSpeed(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eEarringSpeed));
	}
	return 0x00;
};
byte UserData::getEarringMode(byte offset_pos){
	if(isManagedOffsetRange(offset_pos)){
		return (byte)EEPROM.read(getMemoryPoint(offset_pos, eEarringMode));
	}
	return 0x00;
};
//******************************************************
//* setter
//******************************************************
void UserData::setAutoPlay(byte AP){
	if(EEPROM.read(AUTOPLAY_ADDRESS) != AP) EEPROM.write(AUTOPLAY_ADDRESS, AP);
};
void UserData::setAutoPlayFrom(byte AF){
	if(EEPROM.read(AUTOPLAY_FROM) != AF) EEPROM.write(AUTOPLAY_FROM, AF);
};
void UserData::setAutoPlayTo(byte AT){
	if(EEPROM.read(AUTOPLAY_TO) != AT) EEPROM.write(AUTOPLAY_TO, AT);
};
void UserData::setLEDs(byte LC){
	//LED NUMBER Limitter 
	if(LC > MAX_OF_LED_NUMBERS) LC = MAX_OF_LED_NUMBERS;
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
void UserData::setLoops(byte Loops){
	if(EEPROM.read(getMemoryPoint(eLoops)) != Loops) EEPROM.write(getMemoryPoint(eLoops), Loops);
};
void UserData::setLoopExecMode(byte LoopExecMode){
	if(EEPROM.read(getMemoryPoint(eLoopExecMode)) != LoopExecMode) EEPROM.write(getMemoryPoint(eLoopExecMode), LoopExecMode);
};
void UserData::setEarringColor(TColor *tc){
	if(EEPROM.read(getMemoryPoint(eEarringRed)) != tc->red) EEPROM.write(getMemoryPoint(eEarringRed), (byte)tc->red);
	if(EEPROM.read(getMemoryPoint(eEarringGreen)) != tc->green) EEPROM.write(getMemoryPoint(eEarringGreen), (byte)tc->green);
	if(EEPROM.read(getMemoryPoint(eEarringBlue)) != tc->blue) EEPROM.write(getMemoryPoint(eEarringBlue), (byte)tc->blue);
};
void UserData::setEarringBrightness(byte EB){
	if(EEPROM.read(getMemoryPoint(eEarringBrightness)) != EB) EEPROM.write(getMemoryPoint(eEarringBrightness), EB);
};
void UserData::setEarringSpeed(byte ES){
	if(EEPROM.read(getMemoryPoint(eEarringSpeed)) != ES) EEPROM.write(getMemoryPoint(eEarringSpeed), ES);
};
void UserData::setEarringMode(byte EM){
	if(EEPROM.read(getMemoryPoint(eEarringMode)) != EM) EEPROM.write(getMemoryPoint(eEarringMode), EM);
};
//******************************************************
//* public function
//******************************************************
//メモリ読み込み位置をセットする。
void UserData::setMemoryOffset(byte offset){
	if(!isManagedOffsetRange(offset)){
		offset = 0x00;
	}
	EEPROM.write(MEMORY_OFFSET_VALUE_ADDRESS, offset);
	//情報更新
	m_MemoryPoint = (offset * DATA_CAPACITY) + DATA_OFFSET;
	//*************************************************************************************
	//eInitの値が初期値ではない場合にだけ初期化する。
	if((byte)EEPROM.read(getMemoryPoint(eInit)) != INIT_VALUE) initialize(false);
};	
//メモリをクリアする。(設定値に0を書き込むだけ。)但し、現在使用中のものはクリアできない。
bool UserData::clear(byte memory_point){
	if(!isManagedOffsetRange(memory_point)) return false;
	//現在使用中の位置ではない事の確認
	if(memory_point == getMemoryOffset()) return false;
	//読み込み位置の初期化値の確認
	unsigned long mp = (unsigned long)((memory_point * DATA_CAPACITY + DATA_OFFSET) + eInit);
	if(EEPROM.read(mp) == INIT_VALUE){
		EEPROM.write(mp, 0x00);
		return true;
	}else{
		return false;
	}
};

//******************************************************
//* private function
//******************************************************
byte UserData::getMemoryOffset(){
	return (byte)EEPROM.read(MEMORY_OFFSET_VALUE_ADDRESS);
};
//メモリーのオフセット値から読み込むべきメモリ位置を取得する。
unsigned long UserData::getMemoryPoint(DataPoint dp){
	return (unsigned long) (m_MemoryPoint + (unsigned long)dp);
};
//メモリーのオフセット値から読み込むべきメモリ位置を取得する。
unsigned long UserData::getMemoryPoint(byte memory_point, DataPoint dp){
	return (unsigned long) ((memory_point * DATA_CAPACITY + DATA_OFFSET) + (unsigned long)dp);
};

//管理対象範囲のOffset値かを確認する。
bool UserData::isManagedOffsetRange(byte memory_point){
	if(memory_point > MAX_OFFSET_VALUE) return false;
	return true;
};
//初期化済みのデータであるかを確認する。
bool UserData::isInitData(byte offset_pos){
	if(EEPROM.read(getMemoryPoint(offset_pos, eInit)) == INIT_VALUE) return true;
	return false;
};