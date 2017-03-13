/* Main Program for Lbox001
----  Test/Demo Code
*/
#include <stdio.h>
#include <string.h>

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

extern void sendPacketThingSpeak(const char *apiKey, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7);
