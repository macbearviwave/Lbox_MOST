#ifndef __LBOX_H
#define __LBOX_H
/* External variables --------------------------------------------------------*/
/* External functions --------------------------------------------------------*/

extern void LboxStartUp(void);

/* RS232 API -----------------------------------------------------------------*/
extern void ConfigRS232(unsigned long);
extern void RS232_Tx(unsigned char*, unsigned char);
extern unsigned char RS232_Rx(unsigned char*);

/* RS485 API -----------------------------------------------------------------*/
extern void ConfigRS485(unsigned long);
extern void RS485_Tx(unsigned char*, unsigned char);
extern unsigned char RS485_Rx(unsigned char*);

/* GPIO API ------------------------------------------------------------------*/
extern void ConfigGPIO(void);
extern unsigned char GetID(void);
extern unsigned char SetGPIO(unsigned char, unsigned char);
extern unsigned char ReadGPI(unsigned char, unsigned char*);
extern unsigned char WriteGPO(unsigned char, unsigned char);
extern void Get_LM130_Mode(unsigned char*);
extern void Set_LM130_Mode(unsigned char);
extern void LM130_Start(void);
extern unsigned char ReadBusy(void);

/* LoRa API ------------------------------------------------------------------*/
extern void ConfigLoRa(unsigned long);
extern void LoRa_Tx(unsigned char*, unsigned char);
extern unsigned short LoRa_Rx(unsigned char*);
extern unsigned long Get_Time_Tick(void);
extern void Delay_Tick(unsigned long);
extern void LS_Init(void);
extern long THS_Read_Value(unsigned char);
extern unsigned short LS_Read_Value(void);

/* I2C API ------------------------------------------------------------------*/
extern void ConfigI2C(void);

/* ADC API ------------------------------------------------------------------*/
extern void ConfigAI(void);
extern void AIO_Start(void);
extern unsigned long Get_VDDA(void);
extern unsigned long Get_IntTemp(void);
extern unsigned long Get_AIval(unsigned char);

#endif /* __LBOX_H */
