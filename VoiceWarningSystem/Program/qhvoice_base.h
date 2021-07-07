
#include "Arduino.h"
#include "SoftwareSerial.h"

uint8_t send_buf[10] = {
	0x7E, 0xFF, 06, 00, 00, 00, 00, 00, 00, 0xEF};
uint8_t recv_buf[10];


void qhvoice_set_reply (boolean state); 

void qhvoice_fill_cmd (uint8_t cmd, uint16_t arg);
void qhvoice_fill_cmd (uint8_t cmd);

void qhvoice_set_serial (HardwareSerial &theSerial); 

void qhvoice_set_serial (SoftwareSerial &theSerial); 


uint16_t qhvoice_get_checksum (uint8_t *thebuf); 

void qhvoice_fill_checksum (); 

void qhvoice_play_physical (uint16_t num); 
void qhvoice_play_physical (); 

//
void qhvoice_next (); 

//
void qhvoice_prev (); 

//0x06 set volume 0-30
void qhvoice_set_volume (uint16_t volume); 

//0x07 set EQ0/1/2/3/4/5    Normal/Pop/Rock/Jazz/Classic/Bass
void qhvoice_set_EQ (uint16_t eq); 

//0x09 set device 1/2/3/4/5 U/SD/AUX/SLEEP/FLASH
void qhvoice_set_device (uint16_t device); 

//
void qhvoice_sleep (); 

//
void qhvoice_reset (); 

//
void qhvoice_pause (); 

//
void qhvoice_stop (); 

//
void qhvoice_play (); 

//specify a qhvoice file in qhvoice folder in your tf card, "qhvoice_play (1);" mean play "qhvoice/0001.qhvoice"
void qhvoice_play (uint16_t num); 

//
void qhvoice_get_state (); 

//
void qhvoice_get_volume (); 

//
void qhvoice_get_u_sum (); 

//
void qhvoice_get_tf_sum (); 

//
void qhvoice_get_flash_sum (); 

//
void qhvoice_get_tf_current (); 

//
void qhvoice_get_u_current (); 

//
void qhvoice_get_flash_current (); 

//set single loop 
void qhvoice_single_loop (boolean state); 

void qhvoice_single_play (uint16_t num); 

//
void qhvoice_DAC (boolean state); 

//
void qhvoice_random_play (); 

