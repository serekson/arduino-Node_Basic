#include <Arduino.h>
#include <SPI.h>
#include <EEPROM.h>
#include <mrf24j40_driver.h>
#include <mrf24j40_client.h>
#include <avr/wdt.h>

#define PIN_CLK 13    //SPI Clock            DI
#define PIN_SDI 12    //SPI MISO             DI
#define PIN_SDO 11    //SPI MOSI             DO
#define PIN_CS 10     //SPI SS                DI
#define PIN_INT 3     //MRF24J40 Interrupt    DO

MRFClient mrf(PIN_CS, PIN_INT);  //Create wireless object

bool WD_en;

void setup() {
  Serial.begin(19200);
  
  initialize_mrf();
  
  WD_en=1;
  wdt_enable(WDTO_4S);
}

void initialize_mrf() {
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.begin();

  mrf.reset();
  mrf.init_client();

  attachInterrupt(1, mrf_isr, FALLING);   
  interrupts();
}

void mrf_isr() {
  mrf.int_mrf=1;
}

void loop() {
  mrf.client_loop();

  if(mrf.cmd_count>0) {
    //proc_cmd();
    Serial.println("Proc Command");
  }

  if(WD_en) {
    wdt_reset();
  }
}
