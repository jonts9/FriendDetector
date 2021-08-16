#include "./esppl_functions.h"
#define LIST_SIZE 1

uint8_t friendmac[LIST_SIZE][ESPPL_MAC_LEN] = {
  //D4:63:C6:82:95:C8 - motorola da veia
  //A8:9C:ED:E8:F0:D4 - mi9
  {0xD4, 0x63, 0xC6, 0x82, 0x95, 0xC8}
  //{0xA8, 0x9C, 0xED, 0xE8, 0xF0, 0xD4}
};

String friendname[LIST_SIZE] = {
  "MotorolaVeia-d4:63:c6:82:95:c8"
};

void setup() {
  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  esppl_init(cb);
}

int cooldown = 0;
void ledInterno() {
  digitalWrite(LED_BUILTIN, LOW);
}
void turnoff() {
  digitalWrite(LED_BUILTIN, HIGH);
}

bool maccmp(uint8_t *mac1, uint8_t *mac2) {
  for (int i = 0; i < ESPPL_MAC_LEN; i++) {
    if (mac1[i] != mac2[i]) {
      return false;
    }
  }
  return true;
}

void cb(esppl_frame_info *info) {
  for (int i = 0; i < LIST_SIZE; i++) {
    if (maccmp(info->sourceaddr, friendmac[i]) || maccmp(info->receiveraddr, friendmac[i])) {
      Serial.printf("\n%s is here! :)", friendname[i].c_str());
      cooldown = 20000;
      ledInterno();
    }

    else {
      Serial.printf("\n cooldown: %d", cooldown);
      cooldown--;
      if(cooldown == 0) {
        turnoff();
      }
      if(cooldown == -25) {
        ledInterno();
      }
      if(cooldown == -27) {
        cooldown = 0;
        turnoff();
      }
      /*
      if (cooldown > 0) {
        cooldown--;
      }
      else {

        turnoff();
      }
      */
    }
  }
}


void loop() {
  ledInterno();
  delay(500);
  turnoff();
  delay(500);
  ledInterno();
  delay(500);
  turnoff();
  delay(500);
  ledInterno();
  delay(500);
  turnoff();
  delay(500);
  esppl_sniffing_start();
  while (true) {
    for (int i = ESPPL_CHANNEL_MIN; i <= ESPPL_CHANNEL_MAX; i++ ) {
      esppl_set_channel(i);
      while (esppl_process_frames()) {
      }
    }
  }
}
