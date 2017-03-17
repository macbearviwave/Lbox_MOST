// Lbox LoRaLAN by MOSTLink protocol
//
// 2017 viWave, All rights reserved
///////////////////////////////////////////

#include "lora_util.h"
#include "lbox.h"

unsigned char LM130_RCmd[] = {0xFF, 0x4C, 0xCF, 0x52, 0xA1, 0x52, 0xF0};
unsigned char LM130_WCmd[] = {0xFF, 0x4C, 0xCF, 0x52, 0xA1, 0x57, 0xF1};
unsigned long LM130_Wait = 600;
unsigned char g_buf[99];

DataLora g_dataLora;

///////////////////////////////////////////

void printBinary(const unsigned char *data, const int szData)
{
#ifdef USE_DEBUG_OUTPUT
    char strOut[256] = {0};
    char strHex[8] = {0};
    int i;
    for (i = 0; i < szData; i++) {
        sprintf(strHex, "%02X", data[i]);
        strcat(strOut, strHex);
    }
    printf("%s (%d)\n", strOut, szData);
#endif // USE_DEBUG_OUTPUT
}

///////////////////////////////////////////

uint8_t getCrc(const uint8_t *dataBuffer, const uint8_t length) {
    uint8_t crc = 0;
    for (uint8_t i=0; i<length; i++) {
        crc^=dataBuffer[i];
    }
    //printf("%x\n", crc);
    return crc;
}

/////////////////////////////////////////
// send data via LoRa
int sendData(uint8_t *data, int szData)
{
    if (szData > 99) {
        printDebug("Error: Send too many chars(%d)", szData);
        return -1;
    }
    LoRa_Tx(data, szData);
    Delay_Tick(100);

    printDebug("Send > ");
    printBinary(data, szData);
    return szData;
}
///////////////////////////////////////////

int receData(unsigned char *buf, const int interval)
{	
    int cnt = 0;
    int nRet = 0;
    buf[0] = 0xFF;
    int duration = 0, deltaTick = 100;
	while (duration < interval)	// try 3 sec
	{
		nRet = LoRa_Rx(buf);
		if (nRet != 0) {
            buf[nRet] = 0;
            printDebug("Rece < ");
            printBinary(buf, nRet);          
            break;
        }
		cnt++;
        duration += deltaTick;
		Delay_Tick(deltaTick);
	}
  
    return nRet;
}

int isValidConfig(const DataLora *data)
{      // begin(0x24), end(0x21)
  return (data->tagBegin == 0x24 && data->tagEnd == 0x21) ? 1 : 0;
}

void printConfig(DataLora *data)
{
    if (isValidConfig(data) <= 0) {
        printDebug("***LoRa config invalid\n");
        return;
    }
    char strModule[8] = {0}, strVer[8] = {0};
    memcpy(strModule, data->module_no, 4);
    memcpy(strVer, data->ver_no, 7);
    printDebug("LoRa:%s ver:%7s, MAC:", strModule, strVer);
    printBinary(data->mac_addr, 8);
    
    long nFrequency;
    nFrequency = ((long)data->freq[0] << 16) + ((long)data->freq[1] << 8) + data->freq[2];
    printDebug("    Config freq(%d), g_id(%d)\n", nFrequency, data->group_id);
    printDebug("    datarate(%d), power(%d), wakeup(%d)\n------\n", data->data_rate, data->power, data->wakeup_time);
}

int receConfig(DataLora *data)
{
    // read LoRa response
    unsigned char buf[99] = {0};
    int szBuf = receData(buf, 5000);
    int szConfig = sizeof(DataLora);
    if (szBuf >= szConfig) {
        memcpy(data, buf, szConfig);
    
        // config result
        printConfig(data);
    }
    else {
        printDebug("config n/a (%d/%d)!\n", szBuf, szConfig);
    }
    Delay_Tick(2000);
    return szBuf;
}

int readConfig(DataLora *data)
{
  	Set_LM130_Mode(4);		// Mode 4: setup mode
	Delay_Tick(LM130_Wait);
	LoRa_Tx(LM130_RCmd, 7);
	Delay_Tick(LM130_Wait);
    
    return receConfig(data);
}

int writeConfig(long freq, unsigned char group_id, char data_rate, char power, char wakeup_time)
{
    unsigned char cmdWrite[16] = {0xFF,0x4C,0xCF,0x52,0xA1,0x57,0xF1};
    cmdWrite[7] = (freq >> 16) & 0xff;
    cmdWrite[8] = (freq >> 8) & 0xff;
    cmdWrite[9] = freq & 0xff;
    cmdWrite[10] = group_id;
    cmdWrite[11] = data_rate;
    cmdWrite[12] = power;
    cmdWrite[13] = 3;               // uart baud
    cmdWrite[14] = 0;               // uart check
    cmdWrite[15] = wakeup_time;     // wakeup time

	Delay_Tick(LM130_Wait);
    Set_LM130_Mode(4);		// Mode 4: setup mode
	Delay_Tick(LM130_Wait);
    LoRa_Tx(cmdWrite, 16);
	Delay_Tick(LM130_Wait);
    
    DataLora data;
    return receConfig(&data);
}

///////////////////////////////////////////
void initLora()
{
 	ConfigLoRa(9600);
	LM130_Start();
 
	while (ReadBusy()==0)
	{
		Delay_Tick(100);
	}
    readConfig(&g_dataLora);
}

///////////////////////////////////////////
void showSensorOnboard()
{
    int temp = THS_Read_Value(0);
    int humidity = THS_Read_Value(1);
    int light = LS_Read_Value();
    printDebug("Temp(%d), Humidity(%d), light(%d)\n", temp, humidity, light);  
}

///////////////////////////////////////////
void blinkLed(int nCount, int msOn, int msOff, int idPin)
{
    SetGPIO(idPin, 1);  // Set to GPO
	WriteGPO(idPin, 0);
    int i;
    for (i = 0; i < nCount; i++) {
        WriteGPO(idPin, 0);     // turn-on
        Delay_Tick(msOn);
        WriteGPO(idPin, 1);     // turn-off
        Delay_Tick(msOff);
    }
}

void blinkSOS(int msDot)
{
    blinkLed(3, msDot, msDot, 1);
    Delay_Tick(msDot * 2);
    blinkLed(3, msDot * 3, msDot, 1);
    Delay_Tick(msDot * 2);
    blinkLed(3, msDot, msDot, 1);
}
#include <stdarg.h>

void printDebug(const char* format, ...)
{
#ifdef USE_DEBUG_OUTPUT
    char buffer[256];

    va_list argptr;
    va_start(argptr, format);
    vsnprintf(buffer, 255, format, argptr);
    va_end(argptr);
    
    printf(buffer);
#endif // USE_DEBUG_OUTPUT
}