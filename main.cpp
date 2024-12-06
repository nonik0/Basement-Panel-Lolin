#include "matrix_8x8.h"
#include "wifi_services.h"

#define SCB_PIN 10 // on "Lolin" I2C connector


Matrix8x8TaskHandler matrix8x8;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  delay(5000);
  log_d("Starting setup...");

  pinMode(SCB_PIN, OUTPUT);
  digitalWrite(SCB_PIN, true);

  wifiServices.setup(DEVICE_NAME);
  
  matrix8x8.createTask();
  wifiServices.createTask();

  wifiServices.registerSetDisplayCallback([&](bool state)
                                          { matrix8x8.setDisplay(state); });
  wifiServices.registerSetDisplayCallback([&](bool state)
                                          { digitalWrite(SCB_PIN, state); });

  log_d("Setup complete");
}

void loop()
{
}