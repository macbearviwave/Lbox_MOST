/* Main Program for Lbox001
----  Thingspeak, myDevices(Cayenne)
    sensor: 
        temperature
        humidity
        light
        reed
*/
#include <stdio.h>
#include <string.h>
#include "lbox.h"
#include "lora_util.h"
#include "lora_wan.h"
#include "lora_packet.h"

void Board_Init();
void I2C_Test();
void ADC_Test();
void Flash_LED();
void LM130_Test();
void LoRaWAN_Test();

// Macbear
void sendUplinkSensor();

void main(void)
{
    printDebug("init thingspeak/myDevices\n");
  
	unsigned char BX_ID;
	
	Board_Init();  

    blinkLed(10, 60, 60, 1);   // debug by blinkLed
    
    AIO_Start();
	int Va0 = Get_AIval(0);
	int Va1 = Get_AIval(1);
 
  	BX_ID = GetID();
	switch (BX_ID & 0x0F)
	{
		case 0x01:
		case 0x02:
		case 0x03:
			LM130_Test();
			break;
		default:
            break;
	}
    blinkSOS(300);              // debug by blinkSOS
}


void Board_Init()
{
// MCU Peripheral Initialization
	LboxStartUp();
	ConfigGPIO();
	
	SetGPIO(1, 1);  // Set GPIO1 to GPO
	SetGPIO(2, 0);  // Set GPIO2 to GPI
	WriteGPO(1, 0);

	ConfigI2C();
	ConfigRS485(9600);
	ConfigRS232(57600);
	ConfigAI();
	
// on board LightSensor Device initialization
	LS_Init();
}

char strDst[256] = {0};

////////////////////////////
unsigned char reedCurr = 1, reedPrev = 1;
unsigned long tickNow;

////////////////////////////
// return true: reed status change
bool OnReedChange()
{
    bool bRet = false;
    ReadGPI(2, &reedCurr);
        
    if (reedPrev != reedCurr) {     // reed status change
        reedPrev = reedCurr;
        tickNow = Get_Time_Tick();
        printDebug("onReed = %d (%ld)\n", reedCurr, tickNow);
     	WriteGPO(1, reedCurr);      // GPO#1 LED on when reed opened
        bRet = true;
    }
    return bRet;
}

// ****** NOTICE ******
// TODO: Before you run your application, you need a thingspeak account.
const char *apikeyThingspeak = "TCB071BJBRT3W7VZ6";
// TODO: Before you run your application, you need a myDevices Cayenne account.
const char *MQTTUsername = "b1ccd4a0-d30f-11e6-97cc-8758d0339dd8";
const char *MQTTPassword = "dd183e02322d645286cb9076bf76cfb2f2a094c4";
const char *ClientID =     "e45fb9c0-3ba9-11e7-93dc-b360a7de190c";


void funcPacketReqData(unsigned char *data, int szData)
{
    blinkSOS(50);
    // 
    showSensorOnboard();
    printDebug("REQ_DATA\n");

    // temperature, humidity, light, reed
    float fTemp = THS_Read_Value(0) / 100.0f;
    float fHumidity = THS_Read_Value(1) / 100.0f;
    int light = LS_Read_Value();
	ReadGPI(2, &reedCurr);
   	WriteGPO(1, reedCurr);      // GPO#1 LED on when reed opened

    int nBoxID = GetID();  
    if (nBoxID & 0x01) {
        // send packet to thingspeak
        sendPacketThingSpeak(apikeyThingspeak,fTemp,fHumidity,light,reedCurr,55,66,7,8);
        Delay_Tick(500);
    }
    if (nBoxID & 0x02) {
        // send packet to myDevices(Cayenne)
        sprintf(strDst, "2,temp,f,%d", (int)fTemp);
        sendPacketSendMydevicesCommand((uint8_t*)strDst, strlen(strDst));
        Delay_Tick(500);
      
        sprintf(strDst, "3,rel_hum,p,%d", (int)fHumidity);
        sendPacketSendMydevicesCommand((uint8_t*)strDst, strlen(strDst));
        Delay_Tick(500);
        
        sprintf(strDst, "4,lum,lux,%d", (int)light);
        sendPacketSendMydevicesCommand((uint8_t*)strDst, strlen(strDst));
        Delay_Tick(500);
        
        sprintf(strDst, "1,relay,null,%d", (int)reedCurr);
        sendPacketSendMydevicesCommand((uint8_t*)strDst, strlen(strDst));
        Delay_Tick(500);
    }  
}

// Mediatek Cloud Sandbox (MCS)
/*
const char *strDevice = "DAqjhn8J,oYWff0vnH0O8x4xr";  // starter
const char *strDispTemperature = "DISP_T";
const char *strDispHumidity = "DISP_H";
*/
const char *strDevice = "Df1yWOef,7VzSiBty1sgYTGMt";  // usgmostlink
const char *strDispTemperature = "DISP_TEMPERATURE";
const char *strDispHumidity = "DISP_HUMIDITY";
const char *strCtrlFan = "CTRL_FAN";
const char *strCtrlLed = "CTRL_LED";
const char *strCtrlUpdate = "CTRL_UPDATE";        // refresh sensor
 
void funcPacketNotifyMcsCommand(unsigned char *data, int szData)
{
    int nVal;
    const char *strBuf = (const char *)data;
    if (MCSparseDownlink(strBuf, strCtrlFan, &nVal))
    {
        printDebug("MCS: %s = %d\n", strCtrlFan, nVal);
    }
    else if (MCSparseDownlink(strBuf, strCtrlLed, &nVal))
    {
        printDebug("MCS: %s = %d\n", strCtrlLed, nVal);
    }
    else if (MCSparseDownlink(strBuf, strCtrlUpdate, &nVal))
    {
        printDebug("MCS: %s = %d\n", strCtrlUpdate, nVal);
        sendUplinkSensor();
    }
}

void refreshControlState()
{
    // recover update-state: standby
    sprintf(strDst, "%s,%s,,0", strDevice, strCtrlUpdate);
    sendPacketSendMCSCommand((uint8_t*)strDst, strlen(strDst));
    Delay_Tick(700);
}

void sendUplinkSensor()
{
    char cBuzy0 = ReadBusy();
    float fTemp = THS_Read_Value(0) / 100.0f;
    float fHumidity = THS_Read_Value(1) / 100.0f;
    sprintf(strDst, "%s,%s,,%.1f\n%s,,%.1f", strDevice, strDispTemperature, fTemp, strDispHumidity, fHumidity);
    sendPacketSendMCSCommand((uint8_t*)strDst, strlen(strDst));

    char cBuzy1 = ReadBusy();
    Delay_Tick(700);
    blinkSOS(80); 
    char cBuzy2 = ReadBusy();
 //   printDebug("BZ: %d, %d, %d---\n", cBuzy0, cBuzy1, cBuzy2); 
}
  
void setLoraFrequency(int idCh)
{
    int nChannel = idCh * 1111 + 910000;
    writeConfig(nChannel, 0, 0, 7, 3);
    Set_LM130_Mode(2);		// Mode 2: wakeup mode

    
    printDebug("LoRa Freq: %d kHz\n", nChannel);

    Delay_Tick(500);
    blinkSOS(80); 
    
    char strTmp[256] = {0};
    sprintf(strTmp, "LBOX LoRa Freq: %d kHz", nChannel);

    printDebug("(S): %s\n", strTmp);
    sendData((unsigned char*)strTmp, strlen(strTmp));
    Delay_Tick(500);
}

void LM130_Test()
{
   	int nBoxID = GetID();  
  	int idChannel = GetID() >> 5;
    nBoxID = nBoxID & 0x0F;
    
    initLora();
     
    setLoraFrequency(idChannel);

    ///////////////////
    // custom callback
    g_cbPacketReqData = funcPacketReqData;
    g_cbPacketNotifyMcsCommand = funcPacketNotifyMcsCommand;
    
    blinkLed(nBoxID, 200, 100, 1);   // debug by blinkLed
   
    // login myDevices
    sendPacketReqLoginMydevices(MQTTUsername, MQTTPassword, ClientID);
   
    // login MCS
//    sendPacketReqLoginMCS((uint8_t*)strDevice, strlen(strDevice));
//    Delay_Tick(1000);
 //   refreshControlState();      // update to ready
 //   sendUplinkSensor();         // temp,humidity
  
    unsigned char buf[99];
    int szBuf;

	unsigned long Gtick, Prd = 1000;

	Gtick = Get_Time_Tick();
    long txTick = Gtick;
    while (1) {
        // check new channel
        int idCh_New = GetID() >> 5;
        if (idCh_New != idChannel) {
          setLoraFrequency(idCh_New);
          idChannel = idCh_New;
        }
      
        szBuf = receData(buf, 10);
        if (szBuf > 0)
        {
            parsePacket(buf, szBuf);
        }
        
        Delay_Tick(10);
        OnReedChange();      
        
        if (tickNow - txTick > 10000) {
            txTick = tickNow;
            printDebug("TX at ts(%d)\n", Gtick);
        }
    }
}

void LoRaWAN_Test()
{
    printDebug("LoRaWAN test\n");

    initLoraWAN();
    unsigned char buf[99] = {0};
    int szBuf = 0;
    
//    LoRaHwReset();  // Lora hardware reset

    // firmware version    
    const char *strResult;
//    strResult = WANgetFwVersion();      printDebug(strResult);
    
    // deviceEUI
    strResult = WANgetDevEui();         printDebug(strResult);
//    strResult = WANgetDevAddr();        printDebug(strResult);
//    strResult = WANgetAppEui();         printDebug(strResult);

    bool bRet = WANsetTx(3, "cnf", "112233AABBCC");
    printDebug("result: TX %s\n", bRet ? "ok" : "fail");
    
	unsigned long Gtick, Prd = 2000;
	unsigned char IP_Status, OP_Status;
	Gtick = Get_Time_Tick();
    long txTick = Gtick;
    while (1) {
        szBuf = receData(buf, 10);
        if (szBuf > 0)
        {
            printDebug("(R): %s", buf);
            printBinary(buf, szBuf);
        }
        
        Delay_Tick(10);
        long tickNow = Get_Time_Tick();
 		if(tickNow - Gtick > Prd)  // blink LED per second
		{
			ReadGPI(2, &IP_Status);
			if (!IP_Status)
			{
				ReadGPI(1, &OP_Status);
				WriteGPO(1, OP_Status^0x01);
			}
			Gtick = tickNow;
            
//            int Va0 = Get_AIval(0);
//            int Va1 = Get_AIval(1);
//            printf("ts(%d), analog(%d,%d))\n", Gtick, Va0, Va1);
        }
    }
}  

/*******************************************************************************
      END FILE
*******************************************************************************/
