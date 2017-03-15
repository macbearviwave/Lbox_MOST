/* Main Program for Lbox001
----  Test/Demo Code
*/

#include "lora_wan.h"
#include "lora_util.h"
#include "lbox.h"

#define MAX_SIZE_CMD     60
#define MAX_SIZE_BUF     99
char _strBuf[MAX_SIZE_BUF + 1];

///////////////////////////////////////////
void initLoraWAN()
{
 	ConfigLoRa(57600);
 	LM130_Start();
    Delay_Tick(1000);
    int szRece = receData((uint8_t*)_strBuf, 6000);
}
///////////////////////////////////////////

const char *command(const char *strCmd)
{
    if (NULL == strCmd || strlen(strCmd) > MAX_SIZE_CMD - 3)
        return NULL;
    
    char strFull[MAX_SIZE_CMD] = {0};
    sprintf(strFull, "%s\r\n", strCmd);     // add CR,LR
    sendData((uint8_t*)strFull, strlen(strFull));
    
    int szRece = receData((uint8_t*)_strBuf, 6000);
    _strBuf[szRece] = 0;

    if (szRece > 0) {
        printDebug("+++ AT Response is OK.\n");
    }
    else {
        printDebug("!!! AT Response nothing.\n");
    }

    return _strBuf;
}

bool isOK(const char *strResponse)
{
    boolean bRet = false;
    if (NULL != strResponse && strlen(strResponse) >= 2) {
        bRet = ('o' == strResponse[0] && 'k' == strResponse[1]);
    }
    return bRet;
}

void WANupdateFW()
{
    command("AAT1 UpdateFW");
}
void WANsaveSetting()
{
    command("AAT1 Save");
}
const char *WANgetFwVersion()
{
    return command("AAT1 FwVersion");
}
void WANreset()
{
    command("AAT1 Reset");
}

void WANsetSLEEP()
{
    command("AAT1 SLEEP");
}

void WANrestore()
{
    command("AAT1 Restore");
}

/////////////////////////////////////////
// send payload by Tx: port: 1~223, cnf/uncnf, Hex-payload(11 bytes)
boolean MOSTLoraWAN::setTx(int port, char *confirm, char *payload)
{
    sprintf(_strBuf, "AAT2 Tx=%d,%s,%s", port, confirm, payload);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}

/////////////////////////////////////////
// TestMode: 0 - disable, 1 - enable
boolean MOSTLoraWAN::setTestMode(int mode)
{
    sprintf(_strBuf, "AAT1 TestMode=%d", mode);
    command(_strBuf);
}
int MOSTLoraWAN::getTestMode()
{
    char *strRet = command(F("AAT1 TestMode=?"));
    return atoi(strRet);
}
/////////////////////////////////////////
// DevAddr 02410113
boolean MOSTLoraWAN::setDevAddr(const char *strAddr)
{
    sprintf(_strBuf, "AAT2 DevAddr=%s", strAddr);
    char *strRet = command(_strBuf);

    return isOK(strRet);
}
char *MOSTLoraWAN::getDevAddr()
{
    return command(F("AAT2 DevAddr=?"));
}

/////////////////////////////////////////
// DevEui
boolean MOSTLoraWAN::setDevEui(const char *strEui)
{
    sprintf(_strBuf, "AAT2 DevEui=%s", strEui);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}

const char *WANgetDevEui()
{
    return command("AAT2 DevEui=?");
}
/*
/////////////////////////////////////////
// AppEui
boolean MOSTLoraWAN::setAppEui(const char *strEui)
{
    sprintf(_strBuf, "AAT2 AppEui=%s", strEui);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
char *MOSTLoraWAN::getAppEui()
{
    return command(F("AAT2 AppEui=?"));
}

/////////////////////////////////////////
// NwkSKey
boolean MOSTLoraWAN::setNwkSKey(const char *strKey)
{
    sprintf(_strBuf, "AAT2 NwkSKey=%s", strKey);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
char *MOSTLoraWAN::getNwkSKey()
{
    return command(F("AAT2 NwkSKey=?"));
}

/////////////////////////////////////////
// AppSKey
boolean MOSTLoraWAN::setAppSKey(const char *strKey)
{
    sprintf(_strBuf, "AAT2 AppSKey=%s", strKey);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
char *MOSTLoraWAN::getAppSKey()
{
    return command(F("AAT2 AppSKey=?"));
}

/////////////////////////////////////////
// AppKey
boolean MOSTLoraWAN::setAppKey(const char *strKey)
{
    sprintf(_strBuf, "AAT2 AppKey=%s", strKey);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
char *MOSTLoraWAN::getAppKey()
{
    return command(F("AAT2 AppKey=?"));
}

/////////////////////////////////////////
// ADR: 0 - disable, 1 - enable
boolean MOSTLoraWAN::setADR(int state)
{
    sprintf(_strBuf, "AAT2 ADR=%d", state);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getADR()
{
    char *strRet = command(F("AAT2 ADR=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// EVK_TxCycle: 1~254 sec
boolean MOSTLoraWAN::setEVK_TxCycle(int interval)
{
    sprintf(_strBuf, "AAT1 EVK_TxCycle=%d", interval);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getEVK_TxCycle()
{
    char *strRet = command(F("AAT1 EVK_TxCycle=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// JoinMode: 0 - disable, 1 - enable
boolean MOSTLoraWAN::setJoinMode(int mode)
{
    sprintf(_strBuf, "AAT2 JoinMode=%d", mode);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getJoinMode()
{
    char *strRet = command(F("AAT2 JoinMode=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// reTx: 0~8
boolean MOSTLoraWAN::setreTx(int retry)
{
    sprintf(_strBuf, "AAT2 reTx=%d", retry);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getreTx()
{
    char *strRet = command(F("AAT2 reTx=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// RxDelay1: 100000~10000000
boolean MOSTLoraWAN::setRxDelay1(long delayMicroSec)
{
    sprintf(_strBuf, "AAT2 RxDelay1=%ld", delayMicroSec);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
long MOSTLoraWAN::getRxDelay1()
{
    char *strRet = command(F("AAT2 RxDelay1=?"));
    return atol(strRet);
}

/////////////////////////////////////////
// DutyCycle: 0 - disable, 1 - enable
boolean MOSTLoraWAN::setDutyCycle(int state)
{
    sprintf(_strBuf, "AAT2 DutyCycle=%d", state);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getDutyCycle()
{
    char *strRet = command(F("AAT2 DutyCycle=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// PLCheck: 0 - disable, 1 - enable
boolean MOSTLoraWAN::setPLCheck(int state)
{
    sprintf(_strBuf, "AAT2 PLCheck=%d", state);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getPLCheck()
{
    char *strRet = command(F("AAT2 PLCheck=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// Rx2_Freq_DR: frequency 000000001~999999999 in Hz, data-rate 0~15
boolean MOSTLoraWAN::setRx2_Freq_DR(long freq, int dataRate)
{
    sprintf(_strBuf, "AAT2 Rx2_Freq_DR=%ld,%d", freq, dataRate);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
void MOSTLoraWAN::getRx2_Freq_DR(long &freq, int &dataRate)
{
    char *strRet = command(F("AAT2 Rx2_Freq_DR=?"));
    char *token;
    token = strstr(strRet, "Freq.");
    if (token != NULL) {
        freq = atol(token + 5);
    }
    token = strstr(strRet, "DR");
    if (token != NULL) {
        dataRate = atoi(token + 2);
    }
#ifdef DEBUG_LORA
    debugSerial.print(freq);
    debugSerial.print(F("Hz, dataRate:"));
    debugSerial.println(dataRate);
#endif // DEBUG_LORA
}

/////////////////////////////////////////
// ClassMode: 0 - Class A, 2 - Class C
boolean MOSTLoraWAN::setClassMode(int mode)
{
    sprintf(_strBuf, "AAT2 ClassMode=%d", mode);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getClassMode()
{
    char *strRet = command(F("AAT2 ClassMode=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// Rx1DrOffset: index of Rx1 offset 0~3
boolean MOSTLoraWAN::setRx1DrOffset(int offset)
{
    sprintf(_strBuf, "AAT2 Rx1DrOffset=%d", offset);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getRx1DrOffset()
{
    char *strRet = command(F("AAT2 Rx1DrOffset=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// Tx_Channel[x]:
//      channel: US 0~71, EU 0~15
//      frequency 000000001~999999999 in Hz
//      data-rate 0~15
//      channel state: 0 - close, 1 - open
//      band grouping: US 0, EU 0~3
boolean MOSTLoraWAN::setTx_Channel(int channel, long freq, int dataRate, int channelOpen, int bandGroup)
{
    sprintf(_strBuf, "AAT2 Tx_Channel=%d,%ld,%d,%d,%d", channel, freq, dataRate, channelOpen, bandGroup);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
void MOSTLoraWAN::getTx_Channel(int channel, long &freq, int &dataRate, int &channelOpen, int &bandGroup)
{
    sprintf(_strBuf, "AAT2 Tx_Channel%d=?", channel);
    char *strRet = command(_strBuf);
    char *token;
    token = strstr(strRet, "Freq.");
    if (token != NULL) {
        freq = atol(token + 5);
    }
    token = strstr(strRet, "DrRange.");
    if (token != NULL) {
        dataRate = atoi(token + 8);
    }
    token = strstr(strRet, "Status");
    if (token != NULL) {
        channelOpen = atoi(token + 6);
    }
    token = strstr(strRet, "Band");
    if (token != NULL) {
        bandGroup = atoi(token + 4);
    }
#ifdef DEBUG_LORA
    debugSerial.print(F("Ch_"));
    debugSerial.print(channel);
    debugSerial.print(F(", Freq:"));
    debugSerial.print(freq);
    debugSerial.print(F("Hz, dataRate:"));
    debugSerial.print(dataRate);
    debugSerial.print(F(", status:"));
    debugSerial.print(channelOpen);
    debugSerial.print(F(", band:"));
    debugSerial.println(bandGroup);
#endif // DEBUG_LORA
}

/////////////////////////////////////////
// Tx_Band[x]: band grouping: US 0, EU 0~3, dutyCycle 1~9999, ID-power 0~15
boolean MOSTLoraWAN::setTx_Band(int bandGroup, int dutyCycle, int idPower)
{
    sprintf(_strBuf, "AAT2 Tx_Band=%d,%d,%d", bandGroup, dutyCycle, idPower);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
void MOSTLoraWAN::getTx_Band(int bandGroup, int &dutyCycle, int &idPower)
{
    if (bandGroup >= 0) {
        sprintf(_strBuf, "AAT2 Tx_Band%d=?", bandGroup);
    }
    else {
        strcpy(_strBuf, "AAT2 Tx_Band=?");
    }
    char *strRet = command(_strBuf);
    char *token;
    token = strstr(strRet, "DutyCycle.");
    if (token != NULL) {
        dutyCycle = atoi(token + 10);
    }
    token = strstr(strRet, "TxPower.");
    if (token != NULL) {
        idPower = atoi(token + 8);
    }
#ifdef DEBUG_LORA
    debugSerial.print(F("band:"));
    debugSerial.print(bandGroup);
    debugSerial.print(F(", dutyCycle:"));
    debugSerial.print(dutyCycle);
    debugSerial.print(F(", TxPower ID:"));
    debugSerial.println(idPower);

#endif // DEBUG_LORA */
}

/////////////////////////////////////////
// Uplink_Count
int MOSTLoraWAN::getUplink_Count()
{
    char *strRet = command(F("AAT2 Uplink_Count=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// Downlink_Count
int MOSTLoraWAN::getDownlink_Count()
{
    char *strRet = command(F("AAT2 Downlink_Count=?"));
    return atoi(strRet);
}

/////////////////////////////////////////
// Tx_Power[x]: ID-power 0~15, dbm: US 0~30, EU 0~20
boolean MOSTLoraWAN::setTx_Power(int idPower, int dbmPower)
{
    sprintf(_strBuf, "AAT2 Tx_Power=%d,%d", idPower, dbmPower);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getTx_Power(int idPower)
{
    if (idPower >= 0) {
        sprintf(_strBuf, "AAT2 Tx_Power%d=?", idPower);
    }
    else {
        strcpy(_strBuf, "AAT2 Tx_Power=?");
    }
    
    char *strRet = command(_strBuf);
    char *token = strstr(strRet, ", ");
    int nRet = 0;
    if (token != NULL) {
        nRet = atoi(token + 2);
    }
    return nRet;
}

/////////////////////////////////////////
// Pl_Max_Length[x]: dataRate 0~15, payload length: 0~255
boolean MOSTLoraWAN::setPl_Max_Length(int dataRate, int lenPayload)
{
    sprintf(_strBuf, "AAT2 Pl_Max_Length=%d,%d", dataRate, lenPayload);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getPl_Max_Length(int dataRate)
{
    if (dataRate >= 0) {
        sprintf(_strBuf, "AAT2 Pl_Max_Length%d=?", dataRate);
    }
    else {
        strcpy(_strBuf, "AAT2 Pl_Max_Length=?");
    }
    
    char *strRet = command(_strBuf);
    char *token = strstr(strRet, "MaxLength.");
    int nRet = 0;
    if (token != NULL) {
        nRet = atoi(token + 10);
    }
    return nRet;
}

/////////////////////////////////////////
// Plre_Max_Length[x]: dataRate 0~15, payload length: 0~255
boolean MOSTLoraWAN::setPlre_Max_Length(int dataRate, int lenPayload)
{
    sprintf(_strBuf, "AAT2 Plre_Max_Length=%d,%d", dataRate, lenPayload);
    char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int MOSTLoraWAN::getPlre_Max_Length(int dataRate)
{
    if (dataRate >= 0) {
        sprintf(_strBuf, "AAT2 Plre_Max_Length%d=?", dataRate);
    }
    else {
        strcpy(_strBuf, "AAT2 Plre_Max_Length=?");
    }
    
    char *strRet = command(_strBuf);
    char *token = strstr(strRet, "MaxLength.");
    int nRet = 0;
    if (token != NULL) {
        nRet = atoi(token + 10);
    }
    return nRet;
}
