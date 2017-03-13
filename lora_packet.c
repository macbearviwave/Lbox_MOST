/* Main Program for Lbox001
----  Test/Demo Code
*/

#include "lora_util.h"
#include "lora_packet.h"

extern DataLora g_dataLora;

void prepareHeader(HeaderLink &head, bool bDirection)
{
    head.preamble[0] = 0xFB;
    head.preamble[1] = 0xFC;
    head.version = 0x0B;
    head.length = 12;
    head.flag = (bDirection ? 0x08 : 0);
    memcpy(head.receiver_id, g_dataLora.mac_addr, 8)
    head.headerCrc = getCrc(&head, 13);
}

// uplink
void sendPacketThingSpeak(const char *apiKey, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7)
{
    HeaderLink head;
    
    
}
