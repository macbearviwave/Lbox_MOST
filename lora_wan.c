// Lbox LoRaWAN
// AT command
//
// 2017 viWave, All rights reserved
///////////////////////////////////////////

#include "lora_wan.h"
#include "lora_util.h"
#include "lbox.h"
#include "stdlib.h"

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
boolean WANsetTx(int port, char *confirm, char *payload)
{
    sprintf(_strBuf, "AAT2 Tx=%d,%s,%s", port, confirm, payload);
    const char *strRet = command(_strBuf);
    boolean bTxCmd = isOK(strRet);
    if (bTxCmd) {
      const int nLen = strlen(strRet);
      if (nLen > 4) {
        const char *strRet2 = strRet + 4;
        if (strstr(strRet2, "Tx_no_free_ch") == strRet2) {
            bTxCmd = false;
        }
      }
    }
    return bTxCmd;
}

/////////////////////////////////////////
// TestMode: 0 - disable, 1 - enable
boolean WANsetTestMode(int mode)
{
    sprintf(_strBuf, "AAT1 TestMode=%d", mode);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}

int WANgetTestMode()
{
    const char *strRet = command("AAT1 TestMode=?");
    return atoi(strRet);
}
/////////////////////////////////////////
// DevAddr 02410113
boolean WANsetDevAddr(const char *strAddr)
{
    sprintf(_strBuf, "AAT2 DevAddr=%s", strAddr);
    const char *strRet = command(_strBuf);

    return isOK(strRet);
}
const char *WANgetDevAddr()
{
    return command("AAT2 DevAddr=?");
}

/////////////////////////////////////////
// DevEui
boolean WANsetDevEui(const char *strEui)
{
    sprintf(_strBuf, "AAT2 DevEui=%s", strEui);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}

const char *WANgetDevEui()
{
    return command("AAT2 DevEui=?");
}

/////////////////////////////////////////
// AppEui
boolean WANsetAppEui(const char *strEui)
{
    sprintf(_strBuf, "AAT2 AppEui=%s", strEui);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
const char *WANgetAppEui()
{
    return command("AAT2 AppEui=?");
}

/////////////////////////////////////////
// NwkSKey
boolean WANsetNwkSKey(const char *strKey)
{
    sprintf(_strBuf, "AAT2 NwkSKey=%s", strKey);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
const char *WANgetNwkSKey()
{
    return command("AAT2 NwkSKey=?");
}

/////////////////////////////////////////
// AppSKey
boolean WANsetAppSKey(const char *strKey)
{
    sprintf(_strBuf, "AAT2 AppSKey=%s", strKey);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
const char *WANgetAppSKey()
{
    return command("AAT2 AppSKey=?");
}

/////////////////////////////////////////
// AppKey
boolean WANsetAppKey(const char *strKey)
{
    sprintf(_strBuf, "AAT2 AppKey=%s", strKey);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
const char *WANgetAppKey()
{
    return command("AAT2 AppKey=?");
}

/////////////////////////////////////////
// ADR: 0 - disable, 1 - enable
boolean WANsetADR(int state)
{
    sprintf(_strBuf, "AAT2 ADR=%d", state);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetADR()
{
    const char *strRet = command("AAT2 ADR=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// EVK_TxCycle: 1~254 sec
boolean WANsetEVK_TxCycle(int interval)
{
    sprintf(_strBuf, "AAT1 EVK_TxCycle=%d", interval);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetEVK_TxCycle()
{
    const char *strRet = command("AAT1 EVK_TxCycle=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// JoinMode: 0 - disable, 1 - enable
boolean WANsetJoinMode(int mode)
{
    sprintf(_strBuf, "AAT2 JoinMode=%d", mode);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetJoinMode()
{
    const char *strRet = command("AAT2 JoinMode=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// reTx: 0~8
boolean WANsetreTx(int retry)
{
    sprintf(_strBuf, "AAT2 reTx=%d", retry);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetreTx()
{
    const char *strRet = command("AAT2 reTx=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// RxDelay1: 100000~10000000
boolean WANsetRxDelay1(long delayMicroSec)
{
    sprintf(_strBuf, "AAT2 RxDelay1=%ld", delayMicroSec);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
long WANgetRxDelay1()
{
    const char *strRet = command("AAT2 RxDelay1=?");
    return atol(strRet);
}

/////////////////////////////////////////
// DutyCycle: 0 - disable, 1 - enable
boolean WANsetDutyCycle(int state)
{
    sprintf(_strBuf, "AAT2 DutyCycle=%d", state);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetDutyCycle()
{
    const char *strRet = command("AAT2 DutyCycle=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// PLCheck: 0 - disable, 1 - enable
boolean WANsetPLCheck(int state)
{
    sprintf(_strBuf, "AAT2 PLCheck=%d", state);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetPLCheck()
{
    const char *strRet = command("AAT2 PLCheck=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// Rx2_Freq_DR: frequency 000000001~999999999 in Hz, data-rate 0~15
boolean WANsetRx2_Freq_DR(long freq, int dataRate)
{
    sprintf(_strBuf, "AAT2 Rx2_Freq_DR=%ld,%d", freq, dataRate);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
void WANgetRx2_Freq_DR(long *pFreq, int *pDataRate)
{
    const char *strRet = command("AAT2 Rx2_Freq_DR=?");
    char *token;
    token = strstr(strRet, "Freq.");
    if (token != NULL) {
        *pFreq = atol(token + 5);
    }
    token = strstr(strRet, "DR");
    if (token != NULL) {
        *pDataRate = atoi(token + 2);
    }
#ifdef USE_DEBUG_OUTPUT
    printDebug("%dHz, dataRate:%d\n", *pFreq, *pDataRate);
#endif // USE_DEBUG_OUTPUT
}

/////////////////////////////////////////
// ClassMode: 0 - Class A, 2 - Class C
boolean WANsetClassMode(int mode)
{
    sprintf(_strBuf, "AAT2 ClassMode=%d", mode);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetClassMode()
{
    const char *strRet = command("AAT2 ClassMode=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// Rx1DrOffset: index of Rx1 offset 0~3
boolean WANsetRx1DrOffset(int offset)
{
    sprintf(_strBuf, "AAT2 Rx1DrOffset=%d", offset);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetRx1DrOffset()
{
    const char *strRet = command("AAT2 Rx1DrOffset=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// Tx_Channel[x]:
//      channel: US 0~71, EU 0~15
//      frequency 000000001~999999999 in Hz
//      data-rate 0~15
//      channel state: 0 - close, 1 - open
//      band grouping: US 0, EU 0~3
boolean WANsetTx_Channel(int channel, long freq, int dataRate, int channelOpen, int bandGroup)
{
    sprintf(_strBuf, "AAT2 Tx_Channel=%d,%ld,%d,%d,%d", channel, freq, dataRate, channelOpen, bandGroup);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
void WANgetTx_Channel(int channel, long *pFreq, int *pDataRate, int *pChannelOpen, int *pBandGroup)
{
    sprintf(_strBuf, "AAT2 Tx_Channel%d=?", channel);
    const char *strRet = command(_strBuf);
    char *token;
    token = strstr(strRet, "Freq.");
    if (token != NULL) {
        *pFreq = atol(token + 5);
    }
    token = strstr(strRet, "DrRange.");
    if (token != NULL) {
        *pDataRate = atoi(token + 8);
    }
    token = strstr(strRet, "Status");
    if (token != NULL) {
        *pChannelOpen = atoi(token + 6);
    }
    token = strstr(strRet, "Band");
    if (token != NULL) {
        *pBandGroup = atoi(token + 4);
    }
#ifdef USE_DEBUG_OUTPUT
    printDebug("Ch_%d, Freq:%dHz, dataRate:%d, status:%d, band:%d\n", channel, *pFreq, *pDataRate, *pChannelOpen, *pBandGroup);
#endif // USE_DEBUG_OUTPUT
}

/////////////////////////////////////////
// Tx_Band[x]: band grouping: US 0, EU 0~3, dutyCycle 1~9999, ID-power 0~15
boolean WANsetTx_Band(int bandGroup, int dutyCycle, int idPower)
{
    sprintf(_strBuf, "AAT2 Tx_Band=%d,%d,%d", bandGroup, dutyCycle, idPower);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
void WANgetTx_Band(int bandGroup, int *pDutyCycle, int *pIdPower)
{
    if (bandGroup >= 0) {
        sprintf(_strBuf, "AAT2 Tx_Band%d=?", bandGroup);
    }
    else {
        strcpy(_strBuf, "AAT2 Tx_Band=?");
    }
    const char *strRet = command(_strBuf);
    char *token;
    token = strstr(strRet, "DutyCycle.");
    if (token != NULL) {
        *pDutyCycle = atoi(token + 10);
    }
    token = strstr(strRet, "TxPower.");
    if (token != NULL) {
        *pIdPower = atoi(token + 8);
    }
#ifdef USE_DEBUG_OUTPUT
    printDebug("band:%d, dutyCycle:%d, TxPower ID:%d\n", bandGroup, *pDutyCycle, *pIdPower);

#endif // USE_DEBUG_OUTPUT
}

/////////////////////////////////////////
// Uplink_Count
int WANgetUplink_Count()
{
    const char *strRet = command("AAT2 Uplink_Count=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// Downlink_Count
int WANgetDownlink_Count()
{
    const char *strRet = command("AAT2 Downlink_Count=?");
    return atoi(strRet);
}

/////////////////////////////////////////
// Tx_Power[x]: ID-power 0~15, dbm: US 0~30, EU 0~20
boolean WANsetTx_Power(int idPower, int dbmPower)
{
    sprintf(_strBuf, "AAT2 Tx_Power=%d,%d", idPower, dbmPower);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetTx_Power(int idPower)
{
    if (idPower >= 0) {
        sprintf(_strBuf, "AAT2 Tx_Power%d=?", idPower);
    }
    else {
        strcpy(_strBuf, "AAT2 Tx_Power=?");
    }
    
    const char *strRet = command(_strBuf);
    const char *token = strstr(strRet, ", ");
    int nRet = 0;
    if (token != NULL) {
        nRet = atoi(token + 2);
    }
    return nRet;
}

/////////////////////////////////////////
// Pl_Max_Length[x]: dataRate 0~15, payload length: 0~255
boolean WANsetPl_Max_Length(int dataRate, int lenPayload)
{
    sprintf(_strBuf, "AAT2 Pl_Max_Length=%d,%d", dataRate, lenPayload);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetPl_Max_Length(int dataRate)
{
    if (dataRate >= 0) {
        sprintf(_strBuf, "AAT2 Pl_Max_Length%d=?", dataRate);
    }
    else {
        strcpy(_strBuf, "AAT2 Pl_Max_Length=?");
    }
    
    const char *strRet = command(_strBuf);
    const char *token = strstr(strRet, "MaxLength.");
    int nRet = 0;
    if (token != NULL) {
        nRet = atoi(token + 10);
    }
    return nRet;
}

/////////////////////////////////////////
// Plre_Max_Length[x]: dataRate 0~15, payload length: 0~255
boolean WANsetPlre_Max_Length(int dataRate, int lenPayload)
{
    sprintf(_strBuf, "AAT2 Plre_Max_Length=%d,%d", dataRate, lenPayload);
    const char *strRet = command(_strBuf);
    
    return isOK(strRet);
}
int WANgetPlre_Max_Length(int dataRate)
{
    if (dataRate >= 0) {
        sprintf(_strBuf, "AAT2 Plre_Max_Length%d=?", dataRate);
    }
    else {
        strcpy(_strBuf, "AAT2 Plre_Max_Length=?");
    }
    
    const char *strRet = command(_strBuf);
    const char *token = strstr(strRet, "MaxLength.");
    int nRet = 0;
    if (token != NULL) {
        nRet = atoi(token + 10);
    }
    return nRet;
}
