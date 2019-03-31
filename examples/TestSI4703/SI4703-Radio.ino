///
/// \file  SI4703-Radio.ino
/// \brief An Arduino sketch to operate a SI4703 chip based radio + STM32Duino, using the Radio library.
/// \author Felice Murolo
/// \copyright Copyright (c) 2019 by Felice Murolo, Salerno, Italia.\n
/// \email linuxboy@giove.tk
/// This work is licensed under a LGPL license.\n
///
/// Thanks to Matthias Hertel, http://www.mathertel.de for his RADIO library
/// 
/// Open the Serial console with 57600 baud to interact and see the current radio information.
///
/// Wiring
/// ------ 
/// The SI4703 board has to be connected by using the following connections:
/// | STM32Duino pin     | Radio chip signal  | 
/// | -------------------| -------------------| 
/// | 3.3V               | VCC                | 
/// | GND                | GND                | 
/// | PB6                | SCLK               | 
/// | PB7                | SDIO               | 
/// | PB4                | RST                |
///
/// ChangeLog:
/// ----------
/// * 24.03.2019 created.

#include <Arduino.h>
#include <Wire.h>
#include <radio.h>
#include <SI4703.h>
#include <RDSParser.h>

// ----- Fixed settings here. -----

#define FIX_BAND     RADIO_BAND_FM   ///< The band that will be tuned by this sketch is FM.
#define FIX_STATION  9820            ///< The station that will be initially tuned by this sketch (98.2 Mhz).
#define FIX_VOLUME   5               ///< The volume that will be set by this sketch

struct preset_t {
  char name[12];
  uint16_t freq;
};

#define PRESET_NUM 4
struct preset_t preset[PRESET_NUM] = { "Deejay", 9590 , "RDS", 9820, "Virgin", 9000, "Radio24", 10350 };

SI4703 radio;    // Create an instance of Class for Si4703 Chip
RDSParser rds;   // Create an instance of Class for RDS parsing

// some string container for RDS
char radioName[80]; 
char radioTime[20];
char radioText[80];

// useful trim routine
char * trim(char* str)
{
    if(!str) return("");
    char* ptr = str;
    int len = strlen(ptr);
    while(len-1 > 0 && isspace(ptr[len-1])) ptr[--len] = 0;
    while(*ptr && isspace(*ptr)) ++ptr, --len;
    memmove(str, ptr, len + 1);
    return(str);
}

// RDS callback
void manageRDS(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4) {
  rds.processData(block1, block2, block3, block4);
}

// Update the ServiceName text
void getServiceName(char *name)
{
  //Serial.print("RDS:"); Serial.println(name);
  sprintf(radioName,"%s",trim(name));

} // getServiceName()

// Update the RDS Time text
void getTime(uint8_t hour, uint8_t minute) {
  sprintf(radioTime, "RDS-Time: %02d:%02d",hour,minute);
} // getTime()

// Update the RDS Text
void getText(char *text) {
  sprintf(radioText,"RDS-Text: %s",text);
} // getText


void setup() {
  // open the Serial port
  Serial.begin(57600);
  delay(200);

  // Enable information to the Serial port
  radio.debugEnable();

  // Initialize the Radio 
  radio.init();

  // Set all radio setting to the fixed values.
  radio.setBandFrequency(FIX_BAND, FIX_STATION);
  radio.setVolume(FIX_VOLUME);
  radio.setMono(false);
  radio.setMute(false);

  // add a callback for RDS management
  radio.attachReceiveRDS(manageRDS);

  // add some callback to get RDS ServiceName, Time and Text, when they are ready
  rds.attachServicenNameCallback(getServiceName);
  rds.attachTimeCallback(getTime);
  rds.attachTextCallback(getText);
 
} // setup


/// show the current chip data every about 3 seconds.
void loop() {
  char s[20];
  char option;
  int i,j;
  bool flag;
  String q;
  RADIO_INFO rinfo;
  AUDIO_INFO ainfo;
  
  //radio.formatFrequency(s, sizeof(s));
  float rf = (float)radio.getFrequency()/(float)100;
  sprintf(s,"%.2f Mhz",rf);
  Serial.println();
  Serial.println("================================================================================");
  Serial.print("Station: ");
  Serial.print(radioName);Serial.print("@");Serial.println(s);
  
  radio.getAudioInfo(&ainfo);
  radio.getRadioInfo(&rinfo);
  int vol = ainfo.volume;

  Serial.print(rinfo.rds    ? "HasRDS,"    : "NoRDS,");
  Serial.print(rinfo.stereo ? " STEREO," : "  MONO,");
  Serial.print(" RSSI: "); Serial.print(rinfo.rssi); Serial.print(",");
  Serial.print(" Volume: "); Serial.print(vol); Serial.print(",");
  Serial.println(ainfo.mute ? " MUTED"  : " NOT MUTED");
  Serial.println(radioText);
  Serial.println(radioTime);
  Serial.println("================================================================================");
  Serial.println();
  Serial.println("'+' Volume Up, '-' Volume Down, 'm' Mute Toggle");
  Serial.println("'u' Seek Up, 'd' Seek Down, 'fnnnnn' Set frequency to nnnnn");

  for (i=0; i<PRESET_NUM; i++) {
    sprintf(s,"'%d' %s",i,preset[i].name);
    Serial.println(s);
  }
  Serial.println();
  
  int loop = 0;
  while(true) {
    // check for RDS data
    radio.checkRDS();
    if (Serial.available()) {
      option = Serial.read();
      if (option == '+') {
        if (vol < 15) {
          vol += 1;
          radio.setVolume(vol);
        }
      }
      else if (option == '-') {
        if (vol > 0) {
          vol -= 1;
          radio.setVolume(vol);
        }
      }
      else if (option == 'u') {
        rds.init();
        radio.seekUp(true);
      }
      else if (option == 'd') {
        rds.init();
        radio.seekDown(true);
      }
      else if (isdigit(option)) {
        i = option - '0';
        if (i<PRESET_NUM) {
          rds.init();
          radio.setFrequency(preset[i].freq);
        }
      }
      else if (option == 'm') {
        radio.setMute(!radio.getMute());
      }
      else if (option == 'f') {
        i = Serial.available();
        Serial.readBytes(s,i);
        Serial.println(s);
        s[i-1]=0;
        i = strlen(s);
        if (i == 4 || i == 5) {
          flag = true;
          for (j=0; j<i; j++) {
            if (!isdigit(s[j])) {
              flag = false;
            }
          }
          q = String(s);
          if (flag) {
            i = q.toInt();
            if (i >= 8750 && i <= 10800) {
              rds.init();
              radio.setFrequency(i);
            }
          }
        }
      }
    }
    delay(10);
    loop += 10;
    if (loop > 1000) break;
  }
} // loop






// End.
