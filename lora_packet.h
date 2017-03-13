/* Main Program for Lbox001
----  Test/Demo Code
*/
#include <stdio.h>
#include <string.h>

extern void printBinary(const unsigned char *data, const int szData);

// config data for lora (30 bytes)
typedef struct SDataLora {
  char tagBegin;      // 0x24
  // --------
  char module_no[4];  // 101H
  char ver_no[7];     // version
  unsigned char mac_addr[8];  // 8 bytes of device mac address
  unsigned char group_id;     // 0~255
  unsigned char freq[3];      // 915000KHz, 868000KHz
  char data_rate;     // (_0_)0.81K, (1)1.46K, (2)2.6K, (3)4.56K, (4) 9.11K, (5)18.23Kbps
  char power;         // 0 ~ 7
  char uart_baud;     // uart baud: (0)1200, (1)2400, (2)4800, (_3_)9600, (4)19200, (5)38400, (6)57600
  char uart_check;    // (_0_)none, (1)odd, (2)even
  char wakeup_time;   // (0)50ms, 100ms, 200ms, 400ms, 600ms, (_5_)1s, 1.5s, 2s, 2.5s, 3s, 4s, (0xb)5s
  // --------
  char tagEnd;        // 0x21

} DataLora;

// config LoRa
extern int readConfig(DataLora *data);
extern int writeConfig(long freq, unsigned char group_id, char data_rate, char power, char wakeup_time);

// send/rece
extern int receData(unsigned char *buf, const int interval);

extern void initLora();
