/* Main Program for Lbox001
----  Test/Demo Code
*/

#include "lora_util.h"
#include "lora_packet.h"

// callback ptr
CALLBACK_ReceData g_cbReceData = NULL;
CALLBACK_ReceData g_cbPacketReqData = NULL;
CALLBACK_ReceData g_cbPacketNotifyMcsCommand = NULL;


void prepareHeader(HeaderLink *pHead, char cDirection, int szPacket)
{
    pHead->preamble[0] = 0xFB;
    pHead->preamble[1] = 0xFC;
    pHead->version = 0x0B;
    pHead->length = szPacket;
    pHead->flag = (cDirection ? 0x08 : 0);
    memcpy(pHead->receiver_id, g_dataLora.mac_addr, 8);
    pHead->headerCrc = getCrc((uint8_t*)pHead, 13);
}

// uplink
void sendPacketThingSpeak(const char *apiKey, float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7)
{
    const int szPacket = 67;
    uint8_t packet[67] = {0};
    
    // for header
    const int szHeader = sizeof(HeaderLink);
    HeaderLink header;
    prepareHeader(&header, 0, szPacket);
    
    memcpy(packet, &header, szHeader);
    
    uint8_t pos = 0;
    // for payload
    uint8_t *pPayload = packet + szHeader;
    pPayload[0] = 0x0B;     // version
    pPayload[1] = 0xA1;     // cmdID = 0xA102
    pPayload[2] = 0x02;
    
    // apiKey
    int szKey = 16;
    pos = 3;
    memcpy(pPayload + pos, apiKey, szKey);
    
    pos += szKey;
    // assign 8 data to payload
    float arrDat[8] = {f0, f1, f2, f3, f4, f5, f6, f7};
    int i = 0;
    for (i = 0; i < 8; i++) {
        memcpy(pPayload + pos, arrDat + i, 4);
        pos +=  4;
    }
    
    // option flag
    pPayload[pos++] = 0;

    // packet CRC
    packet[pos] = getCrc(packet, pos);
           
    // send data by LoRa
    LoRa_Tx(packet, szPacket);
}

void parsePacket(uint8_t *packet, int szPacket)
{   const int szHeader = sizeof(HeaderLink);
    if (packet[0] == 0xFB && packet[1] == 0xFC && szPacket > szHeader)
    {    // for MOST Link protocol
        HeaderLink header;
        memcpy(&header, packet, szHeader);
        
        // header CRC
        int pos = szHeader - 1; 
        int crcHeader = (packet[pos] == getCrc(packet, szHeader - 1));
        int crcPacket = (packet[pos] == getCrc(packet, szPacket - 1));
        
        pos = szHeader + 1;
        int cmdID = packet[pos] + (packet[pos + 1] << 8);
        printDebug("payload cmdID = %d, receID = ", cmdID);
        printBinary(header.receiver_id, 8);
        if (0 == memcmp(g_dataLora.mac_addr, header.receiver_id, 8))
        {                  
            switch (cmdID) {
            case CMD_REQ_DATA:
                if (g_cbPacketReqData) {
                    // payload for REQ_DATA
                    int pos = 14 + 1 + 2;
                    short *interval = (short*)(packet + pos);
                    pos += 2;
                    uint8_t szData = packet[pos];
                    pos++;
                    uint8_t *pData = packet + pos;
                    g_cbPacketReqData(pData, szData);
                }
                break;
            case CMD_NOTIFY_MCS_COMMAND:
                if (g_cbPacketNotifyMcsCommand) {
                    int pos = 14 + 1 + 2;
                    uint8_t szData = packet[pos];
                    pos++;
                    uint8_t *pData = packet + pos;
                    g_cbPacketNotifyMcsCommand(pData, szData);
                }
                break;
            default:
                break;
            }
        }
    }
}
