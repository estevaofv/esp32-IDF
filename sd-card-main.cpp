#ifndef ARDUINO_ARCH_ESP32
#define ARDUINO_ARCH_ESP32
#endif

#include <Arduino.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"

#define LED_PIN 13
#define TAG "ESP32_ARDUINO"



/*#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif
*/



extern "C"
{
   #include "sdkconfig.h"
   #include "esp_log.h"
   #include "esp32-hal-log.h"
   #include "freertos/FreeRTOS.h"
   #include "freertos/task.h"
   #include "stdio.h"
   void hello_task(void *pvParameter);
   void app_main(void);
}

void hello_task(void *pvParameter)
{
    printf("Hello world!\n");
    while(1){
    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
    printf("Boom ..\n");
    fflush(stdout);
   }
    //esp_restart();
}


void app_main(void) {
   initArduino();
    xTaskCreate(&hello_task, "hello_task", 2048, NULL, 5, NULL);

   ESP_LOGI(TAG,"Starting...");
Serial.begin(115200);

 /*  pinMode(LED_PIN,OUTPUT);
   digitalWrite(LED_PIN,HIGH);
   delay(1000);
   digitalWrite(LED_PIN,LOW);
   delay(1000);
   ESP_LOGI(TAG,"Done...");*/
    if(!SD.begin()){
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if(cardType == CARD_NONE){
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

