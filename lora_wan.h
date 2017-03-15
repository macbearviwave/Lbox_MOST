/* Main Program for Lbox001
----  Test/Demo Code
*/

// LoRa WAN
extern void initLoraWAN();

extern bool isOK(const char *strResponse);


extern void WANupdateFW();
extern void WANsaveSetting();
extern const char *WANgetFwVersion();
extern void WANreset();
extern void WANsetSLEEP();
extern void WANrestore();

// send payload by Tx: port: 1~223, cnf/uncnf, Hex-payload(11 bytes)
extern boolean WANsetTx(int port, char *confirm, char *payload);

// TestMode: 0 - disable, 1 - enable
extern boolean WANsetTestMode(int mode);
extern int WANgetTestMode();

extern boolean WANsetDevAddr(const char *strAddr);
extern const char *WANgetDevAddr();

extern boolean WANsetDevEui(const char *strEui);
extern const char *WANgetDevEui();

extern boolean setAppEui(const char *strAddr);
extern const char *WANgetAppEui();

extern boolean setNwkSKey(const char *strKey);
extern const char *WANgetNwkSKey();

extern boolean setAppSKey(const char *strKey);
extern const char *WANgetAppSKey();

extern boolean setAppKey(const char *strKey);
extern const char *WANgetAppKey();

// ADR: 0 - disable, 1 - enable
extern boolean WANsetADR(int state);
extern int WANgetADR();

// EVK_TxCycle: 1~254 sec
extern boolean WANsetEVK_TxCycle(int interval);
extern int WANgetEVK_TxCycle();

// JoinMode: 0 - disable, 1 - enable
extern boolean WANsetJoinMode(int mode);
extern int WANgetJoinMode();

// reTx: 0~8
extern boolean WANsetreTx(int retry);
extern int WANgetreTx();

// RxDelay1: 100000~10000000
extern boolean WANsetRxDelay1(long delayMicroSec);
extern long WANgetRxDelay1();

// DutyCycle: 0 - disable, 1 - enable
extern boolean WANsetDutyCycle(int state);
extern int WANgetDutyCycle();

// PLCheck: 0 - disable, 1 - enable
extern boolean WANsetPLCheck(int state);
extern int WANgetPLCheck();

// Rx2_Freq_DR: frequency 000000001~999999999 in Hz, data-rate 0~15
extern boolean WANsetRx2_Freq_DR(long freq, int dataRate);
extern void WANgetRx2_Freq_DR(long &freq, int &dataRate);

// ClassMode: 0 - Class A, 2 - Class C
extern boolean WANsetClassMode(int mode);
extern int WANgetClassMode();

// Rx1DrOffset: index of Rx1 offset 0~3
extern boolean WANsetRx1DrOffset(int offset);
extern int WANgetRx1DrOffset();

// Tx_Channel[x]:
//      channel: US 0~71, EU 0~15
//      frequency 000000001~999999999 in Hz
//      data-rate 0~15
//      channel state: 0 - close, 1 - open
//      band grouping: US 0, EU 0~3
extern boolean WANsetTx_Channel(int channel, long freq, int dataRate, int channelOpen, int bandGroup);
extern void WANgetTx_Channel(int channel, long &freq, int &dataRate, int &channelOpen, int &bandGroup);

// Tx_Band[x]: band grouping: US 0, EU 0~3, dutyCycle 1~9999, ID-power 0~15
extern boolean WANsetTx_Band(int bandGroup, int dutyCycle, int idPower);
extern void WANgetTx_Band(int bandGroup, int &dutyCycle, int &idPower);

// Uplink_Count
extern int WANgetUplink_Count();

// Downlink_Count
extern int WANgetDownlink_Count();

// Tx_Power[x]: ID-power 0~15, dbm: US 0~30, EU 0~20
extern boolean WANsetTx_Power(int idPower, int dbmPower);
extern int WANgetTx_Power(int idPower);

// Pl_Max_Length[x]: dataRate 0~15, payload length: 0~255
extern boolean WANsetPl_Max_Length(int dataRate, int lenPayload);
extern int WANgetPl_Max_Length(int dataRate);

// Plre_Max_Length[x]: dataRate 0~15, payload length: 0~255
extern boolean WANsetPlre_Max_Length(int dataRate, int lenPayload);
extern int WANgetPlre_Max_Length(int dataRate);
