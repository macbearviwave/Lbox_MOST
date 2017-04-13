// MOSTLink LoRa module by GlobalSat
//
// MOSTLink protocol
//

#ifndef __lora_packet_h
#define __lora_packet_h

#include <stdio.h>
#include <string.h>

#define CMD_REQ_SET_LORA_CONFIG 0x0001
#define CMD_REQ_DATA            0x0004
#define CMD_NOTIFY_LOCATION     0x0005
#define CMD_REQ_LOCATION        0x0006
#define CMD_SET_CONFIG_GEOF     0x0007
#define CMD_GET_CONFIG_GEOF     0x0008
#define CMD_REQ_AUTH_JOIN       0x0009
#define CMD_REQ_AUTH_CHALLENGE  0x000A

// Mediatek Cloud Sandbox: MCS
#define CMD_REQ_LOGIN_MCS       0x000C
#define CMD_REQ_LOGOUT          0x000D
#define CMD_REQ_STILL_ALIVE     0x000E
#define CMD_SEND_MCS_COMMAND    0x000F
#define CMD_NOTIFY_MCS_COMMAND  0x0010

// myDevices: Cayenne
#define CMD_REQ_LOGIN_MYDEVICES       0x0011
#define CMD_REQ_LOGOUT_MYDEVICES      0x0012
#define CMD_SEND_MYDEVICES_COMMAND    0x0013
#define CMD_NOTIFY_MYDEVICES_COMMAND  0x0014

#define CMD_ANS_SET_LORA_CONFIG 0x0200
#define CMD_ANS_DATA            0x0202
#define CMD_RET_CONFIG_GEOF     0x0203
#define CMD_ANS_AUTH_RESPONSE   0x0204
#define CMD_RET_AUTH_TOKEN      0x0205

// thingspeak, vinduino
#define CMD_NTF_UPLOAD_THINKSPEAK_FIELD 0x02A1
#define CMD_NTF_UPLOAD_VINDUINO_FIELD   0x1002
#define VINDUINO_API_KEY_LEN     16

typedef void (* CALLBACK_ReceData) (unsigned char *data, int szData);

extern CALLBACK_ReceData g_cbReceData;
extern CALLBACK_ReceData g_cbPacketReqData;
extern CALLBACK_ReceData g_cbPacketNotifyMcsCommand;


// config data for lora (30 bytes)
typedef struct SHeaderLink {
    unsigned char preamble[2];      // 0xFB FC
    unsigned char version;
    unsigned char length;
    // --------
    unsigned char flag;             // 0
    unsigned char receiver_id[8];
    unsigned char headerCrc;
} HeaderLink;

extern void parsePacket(uint8_t *packet, int szPacket);

extern void sendPacketThingSpeak(const char *apiKey, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7);
// MCS related:
extern void sendPacketReqLoginMCS(uint8_t *data, int szData);
extern void sendPacketSendMCSCommand(uint8_t *data, int szData);
extern boolean MCSparseDownlink(const char *strBuf, const char *strToken, int *pVal);

#endif // __lora_packet_h
