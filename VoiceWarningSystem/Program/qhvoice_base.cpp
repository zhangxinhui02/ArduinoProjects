

#include <Arduino.h>
#include <SoftwareSerial.h>
//#include  "DFPlayer_Mini_qhvoice.h"

extern uint8_t send_buf[10];
extern uint8_t recv_buf[10];

static void(*send_func)() = NULL;
static HardwareSerial * _hardware_serial = NULL;
static SoftwareSerial * _software_serial = NULL;
static boolean is_reply = false;

//
void qhvoice_set_reply (boolean state) {
	is_reply = state;
	send_buf[4] = is_reply;
}

//
static void fill_uint16_bigend (uint8_t *thebuf, uint16_t data) {
	*thebuf =	(uint8_t)(data>>8);
	*(thebuf+1) =	(uint8_t)data;
}


//calc checksum (1~6 byte)
uint16_t qhvoice_get_checksum (uint8_t *thebuf) {
	uint16_t sum = 0;
	for (int i=1; i<7; i++) {
		sum += thebuf[i];
	}
	return -sum;
}

//fill checksum to send_buf (7~8 byte)
void qhvoice_fill_checksum () {
	uint16_t checksum = qhvoice_get_checksum (send_buf);
	fill_uint16_bigend (send_buf+7, checksum);
}

//
void h_send_func () {
	for (int i=0; i<10; i++) {
		_hardware_serial->write (send_buf[i]);
	}
}

//
void s_send_func () {
	for (int i=0; i<10; i++) {
		_software_serial->write (send_buf[i]);
	}
}

//
//void qhvoice_set_serial (HardwareSerial *theSerial) {
void qhvoice_set_serial (HardwareSerial &theSerial) {
	_hardware_serial = &theSerial;
	send_func = h_send_func;
}

//
void qhvoice_set_serial (SoftwareSerial &theSerial) {
	_software_serial = &theSerial;
	send_func = s_send_func;
}

//
void qhvoice_send_cmd (uint8_t cmd, uint16_t arg) {
	send_buf[3] = cmd;
	fill_uint16_bigend ((send_buf+5), arg);
	qhvoice_fill_checksum ();
	send_func ();
}

//
void qhvoice_send_cmd (uint8_t cmd) {
	send_buf[3] = cmd;
	fill_uint16_bigend ((send_buf+5), 0);
	qhvoice_fill_checksum ();
	send_func ();
}


//
void qhvoice_play_physical (uint16_t num) {
	qhvoice_send_cmd (0x03, num);
}

//
void qhvoice_play_physical () {
	qhvoice_send_cmd (0x03);
}

//
void qhvoice_next () {
	qhvoice_send_cmd (0x01);
}

//
void qhvoice_prev () {
	qhvoice_send_cmd (0x02);
}

//0x06 set volume 0-30
void qhvoice_set_volume (uint16_t volume) {
	qhvoice_send_cmd (0x06, volume);
}

//0x07 set EQ0/1/2/3/4/5    Normal/Pop/Rock/Jazz/Classic/Bass
void qhvoice_set_EQ (uint16_t eq) {
	qhvoice_send_cmd (0x07, eq);
}

//0x09 set device 1/2/3/4/5 U/SD/AUX/SLEEP/FLASH
void qhvoice_set_device (uint16_t device) {
	qhvoice_send_cmd (0x09, device);
}

//
void qhvoice_sleep () {
	qhvoice_send_cmd (0x0a);
}

//
void qhvoice_reset () {
	qhvoice_send_cmd (0x0c);
}

//
void qhvoice_play () {
	qhvoice_send_cmd (0x0d);
}

//
void qhvoice_pause () {
	qhvoice_send_cmd (0x0e);
}

//
void qhvoice_stop () {
	qhvoice_send_cmd (0x16);
}

//play qhvoice file in qhvoice folder in your tf card
void qhvoice_play (uint16_t num) {
	qhvoice_send_cmd (0x12, num);
}

//
void qhvoice_get_state () {
	qhvoice_send_cmd (0x42);
}

//
void qhvoice_get_volume () {
	qhvoice_send_cmd (0x43);
}


//
void qhvoice_get_u_sum () {
	qhvoice_send_cmd (0x47);
}

//
void qhvoice_get_tf_sum () {
	qhvoice_send_cmd (0x48);
}

//
void qhvoice_get_flash_sum () {
	qhvoice_send_cmd (0x49);
}


//
void qhvoice_get_tf_current () {
	qhvoice_send_cmd (0x4c);
}

//
void qhvoice_get_u_current () {
	qhvoice_send_cmd (0x4b);
}


//
void qhvoice_get_flash_current () {
	qhvoice_send_cmd (0x4d);
}

//
void qhvoice_single_loop (boolean state) {
	qhvoice_send_cmd (0x19, !state);
}

//add 
void qhvoice_single_play (uint16_t num) {
	qhvoice_play (num);
	delay (10);
	qhvoice_single_loop (true); 
	//qhvoice_send_cmd (0x19, !state);
}

//
void qhvoice_DAC (boolean state) {
	qhvoice_send_cmd (0x1a, !state);
}

//
void qhvoice_random_play () {
	qhvoice_send_cmd (0x18);
}


