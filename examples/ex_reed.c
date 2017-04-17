/* Main Program for Lbox001
----  Test/Demo Code
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
void LoRaWAN_Reed();
void Reed_Test();

// Macbear
void sendUplinkSensor();

void main(void)
{
    printDebug("=== init reed for LoRaWAN ===\n");
  
	unsigned char BX_ID;
	
	Board_Init();  

    blinkLed(10, 60, 60, 1);   // debug by blinkLed
    
    AIO_Start();
	int Va0 = Get_AIval(0);
	int Va1 = Get_AIval(1);
 
  	BX_ID = GetID();
	switch (BX_ID)
	{
		case 0x01:
            LoRaWAN_Reed();
            break;
		case 0x02:
			break;
		case 0x03:
            Flash_LED();
			break;
		default:
            Reed_Test();
            break;
	}
    blinkSOS(300);              // debug by blinkSOS
}

void Flash_LED()
{
	unsigned long Gtick, Prd=1500;
	unsigned char IP_Status, OP_Status;
	unsigned char Tx_char[30];
	unsigned short Rx_len;
	unsigned char p_cnt=0;
	char ca1, ca2;
	
	Gtick = Get_Time_Tick();
	while (1)
	{
		if((Get_Time_Tick()-Gtick) > Prd)
		{
			ReadGPI(2, &IP_Status);
			if (!IP_Status)
			{
				ReadGPI(1, &OP_Status);
				WriteGPO(1, OP_Status^0x01);
			}
			Gtick = Get_Time_Tick();
			p_cnt++;
		}
		
	}
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

void Reed_Test()
{
    blinkLed(3, 200, 50, 1);   // debug by blinkLed

	tickNow = Get_Time_Tick();
    printDebug("Testing Reed: %ld\n", tickNow);
    while (1) {
        OnReedChange();      
        Delay_Tick(10);
    }
}

//LoRaWAN Reed sensor to GtIot
void LoRaWAN_Reed()
{
    blinkSOS(100);              // debug by blinkSOS

	tickNow = Get_Time_Tick();
    printDebug("LoRaWAN Reed: %ld\n", tickNow);

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


    bool bReedChange = false;
    int nCountChange = 0;
    const unsigned long periodTx = 6000;
    unsigned long tickTx = 0;
    while (1) {
        szBuf = receData(buf, 10);
        if (szBuf > 0)
        {
            printDebug("(R): %s", buf);
            printBinary(buf, szBuf);
        }
        bReedChange = OnReedChange();      
        if (bReedChange) {
            nCountChange++;
        }
        // reed changed, TX to GtIot by LoRaWAN
        if (nCountChange > 0) {
            tickNow = Get_Time_Tick();
            const int elpaseTx = tickNow - tickTx;
            if (0 == tickTx || (elpaseTx >= periodTx)) {
                char strTx[20];
                sprintf(strTx, "%02X%02X%08X", reedCurr, nCountChange, tickNow);
                printDebug("*** TX: %s, reed = %d, count(%d) in %d\n", strTx, reedCurr, nCountChange, elpaseTx);
                
                bool bResultTx = WANsetTx(3, "cnf", strTx);
                printDebug("result: TX %s\n", bResultTx ? "ok" : "fail");
                
                tickTx = Get_Time_Tick();
                if (bResultTx) {
                    nCountChange = 0;
                    blinkLed(3, 100, 100, 1);   // debug by blinkLed
                }
                else
                    blinkLed(1, 100, 100, 1);   // debug by blinkLed
                // restore debug LED
                WriteGPO(1, reedCurr);      // GPO#1 LED on when reed opened
            }
        }  
        
        Delay_Tick(10);
    }
}  

/*******************************************************************************
      END FILE
*******************************************************************************/
