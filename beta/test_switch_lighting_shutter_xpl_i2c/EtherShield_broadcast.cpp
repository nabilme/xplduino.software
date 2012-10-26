/*
 * Arduino ENC28J60 Ethernet shield UDP broadcast client
 */
#include "EtherShield_broadcast.h"
#include "config.h"
#include "parserXpl.h"

#include "Arduino.h"

#include "MemoryFree.h"
// Note: This software implements a web server and a web browser.
// The web server is at "myip" 
// 
// Please modify the following lines. mac and ip have to be unique
// in your local area network. You can not have the same numbers in
// two devices:
// how did I get the mac addr? Translate the first 3 numbers into ascii is: TUX
static uint8_t mymac[6] = {
  0x54,0x55,0x58,0x10,0x00,0x26};

static uint8_t myip[4] = {
  192,168,1,26};

static uint8_t broadcastip[4] = {
  // 255,255,255,255};
  192,168,1,255};

#define BROADCAST 0xFF
#define LF 10

//#define MESSAGE_BUFFER_MAX 140
#define BUFFER_MAX 300  // max lenght of a line

// Port 52240
// #define DEST_PORT_L  0x10
// #define DEST_PORT_H  0xCC

const char iphdr[] PROGMEM ={
  0x45,0,0,0x82,0,0,0x40,0,0x20}; // 0x82 is the total len on ip, 0x20 is ttl (time to live), the second 0,0 is IP-identification and may be changed.
//~ 
//~ struct UDPPayload {
  //~ uint32_t time;            // Time
  //~ uint16_t temperature;     // Temp in 1/10 degree
  //~ uint16_t data[10];        //watts data
  //~ uint16_t errorCount;      // count of errors in the XML.
  //~ uint16_t timeout_count;   // count of all protocol lockups
//~ };

    //~ char udpPayload[40]="\0"; // 213

//~ UDPPayload udpPayload;
// uint8_t  srcport = 3865;

// Port 3865 // TODO: un seul endroit pour modifier le port UDP
#define DEST_PORT_L  0x19
#define DEST_PORT_H  0xF
#define DEBUG_ETHERNET
//~ #define XPL_PORT 0xF19
#define XPL_PORT 3865
// verifier si on filtre bien les autres ports que 3865
// Packet udpPayload, must be big enough to packet and payload
#define udpPayload_SIZE 300
//~ char buf[udpPayload_SIZE+1];
uint8_t buf[udpPayload_SIZE+1];
//~ static char buf[udpPayload_SIZE+1];

EtherShield es=EtherShield();
long lastUpdate = 0;

void setup_udp(){

  /*initialize enc28j60*/
  es.ES_enc28j60Init(mymac);

  //init the ethernet/ip layer:
  es.ES_init_ip_arp_udp_tcp(mymac,myip, XPL_PORT);


#ifdef DEBUG_ETHERNET
    Serial.print("filtre ERXFCON=");
    Serial.println(es.ES_enc28j60Revision(ERXFCON),BIN);

    Serial.print("==================== free RAM memory= ");
    Serial.print(freeMemory());
    Serial.println(" ====================");
#endif  
    Serial.println(" ---- setup udp ok ----");

}


void SendBroadcastData(uint8_t *buf, uint8_t lenght, uint16_t dest_port) {

    uint8_t i=0;
    uint16_t ck;

/*
    Serial.println("------------udpPayload buffer: ");

    Serial.print(udpPayload);

    Serial.print("length of --> "); 
    Serial.println(sizeof(udpPayload));
*/

    /// Setup the MAC addresses for ethernet header
    while(i<6){
        buf[ETH_DST_MAC +i]=BROADCAST; // destination mac (Broadcast address)
        buf[ETH_SRC_MAC +i]=mymac[i]; // source mac
        i++;
    }
    buf[ETH_TYPE_H_P] = ETHTYPE_IP_H_V; // Ethernet type field (2bytes)
    buf[ETH_TYPE_L_P] = ETHTYPE_IP_L_V;
   
    //~ es.ES_fill_buf_p(&buf[IP_P],9,iphdr); // iphdr 0x0E TODO: c quoi ? -> joue sur l'identification
    es.ES_fill_buf_p(&buf[IP_P],1,iphdr); // iphdr 0x0E TODO: c quoi ? -> joue sur l'identification

    /// IP Header
    //~ buf[IP_TOTLEN_L_P]=28+sizeof( buf-UDP_DATA_P );
    //~ buf[IP_TOTLEN_L_P]=28+udpPayload_SIZE;
    buf[IP_TOTLEN_L_P]=28+lenght; // Longueur totale (header+data)
    buf[IP_PROTO_P]=IP_PROTO_UDP_V; // protocole: udp
    i=0;
    while(i<4){
        buf[IP_DST_P+i]=broadcastip[i]; // IP destination
        buf[IP_SRC_P+i]=myip[i];        // IP source
        i++;
    }
    es.ES_fill_ip_hdr_checksum(buf);    //calcul checksum du header ?

    /// UDP Header
    buf[UDP_DST_PORT_H_P]= dest_port >> 8; // port destination (higher)
    buf[UDP_DST_PORT_L_P]= dest_port & 0x00FF; // port destination (lower)
    buf[UDP_SRC_PORT_H_P]=DEST_PORT_H; // port source (higher)
    buf[UDP_SRC_PORT_L_P]=DEST_PORT_L; // port source (lower)
    
    buf[UDP_LEN_H_P]=0; // longueur data + header UDP (higher)
    //~ buf[UDP_LEN_L_P]=8+sizeof( buf-UDP_DATA_P ); // fixed len
    //~ buf[UDP_LEN_L_P]=8+udpPayload_SIZE; // fixed len
    buf[UDP_LEN_L_P]=8+lenght; // fixed len
    // zero the checksum
    buf[UDP_CHECKSUM_H_P]=0;
    buf[UDP_CHECKSUM_L_P]=0;
    // copy the data:
    i=0;

  // Create correct checksum
  //~ ck=es.ES_checksum(&buf[IP_SRC_P], 16 + sizeof( buf-UDP_DATA_P ),1);
  //~ ck=es.ES_checksum(&buf[IP_SRC_P], 16 + udpPayload_SIZE,1);
  ck=es.ES_checksum(&buf[IP_SRC_P], 16 + lenght,1);
  buf[UDP_CHECKSUM_H_P]=ck>>8;
  buf[UDP_CHECKSUM_L_P]=ck& 0xff;
  //~ es.ES_enc28j60PacketSend(42 + sizeof( buf-UDP_DATA_P ), buf);
  //~ es.ES_enc28j60PacketSend(42 + udpPayload_SIZE, buf);
  es.ES_enc28j60PacketSend(42 + lenght, buf);

#ifdef DEBUG_ETHERNET 
     // affiche le contenu complet du buffer (header IP, header UDP, data)
    Serial.print("Header :");
    while(i< 42){
        Serial.print(buf[i], HEX);
        i++;
    }
    Serial.println();
    Serial.println("Data :");
    while(i< (42 + lenght)){
        Serial.write(buf[i]);
        i++;
    }
#endif


}


void loop_udp(){

    uint16_t dat_p;
    // handle ping and wait for a tcp or udp packet
    dat_p=es.ES_packetloop_icmp_tcp(buf,es.ES_enc28j60PacketReceive(udpPayload_SIZE, buf));

    // if packet available
    if(dat_p){

        Serial.print("E#Rx-");
        Serial.println(dat_p-42);


    #ifdef DEBUG_INCOMING
        for(int i=0; i<dat_p; i++){
            if(buf[UDP_DATA_P+i]==0){break;}
            Serial.write(buf[UDP_DATA_P+i]);
        }
    #endif

        parse(buf, dat_p-42);

    } 

}



