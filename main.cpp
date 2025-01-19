#include "matrix_8x8.h"
#include "wifi_services.h"

// on "Lolin" I2C connector
#define EXT_DISPLAY_PIN1 8
#define EXT_DISPLAY_PIN2 10

Matrix8x8TaskHandler matrix8x8;
WifiServices wifiServices;

void setup()
{
  Serial.begin(115200);
  delay(5000);
  log_d("Starting setup...");

  pinMode(EXT_DISPLAY_PIN1, OUTPUT);
  pinMode(EXT_DISPLAY_PIN2, OUTPUT);
  digitalWrite(EXT_DISPLAY_PIN1, true);
  digitalWrite(EXT_DISPLAY_PIN2, true);

  wifiServices.setup(DEVICE_NAME);

  matrix8x8.createTask();
  wifiServices.createTask();

  wifiServices.registerSetDisplayCallback([&](bool state)
                                          { matrix8x8.setDisplay(state);
                                            digitalWrite(EXT_DISPLAY_PIN1, state);
                                            digitalWrite(EXT_DISPLAY_PIN2, state); });

  log_d("Setup complete");
}

void loop()
{
}