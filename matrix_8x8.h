
#pragma once

#include <Adafruit_NeoPixel.h>

#include "display_task_handler.h"

class Matrix8x8TaskHandler : public DisplayTaskHandler
{
private:
    static const uint8_t TASK_PRIORITY = 5;
    static const int REFRESH_TIME = 15;
    static const int BASE_DELAY = 20;
    static const int WIDTH = 8;
    static const int HEIGHT = 8;
    static const int LED_COUNT = WIDTH * HEIGHT;
    static const int LED_PIN = 6;
    static const int BRIGHTNESS = 1;
    const uint32_t Yellow = Adafruit_NeoPixel::Color(6 * BRIGHTNESS, 6 * BRIGHTNESS, 0);
    const uint32_t Amber = Adafruit_NeoPixel::Color(6 * BRIGHTNESS, 2 * BRIGHTNESS, 0);
    const uint32_t Orange = Adafruit_NeoPixel::Color(6 * BRIGHTNESS, 1 * BRIGHTNESS, 0);
    const uint32_t OrangeDark = Adafruit_NeoPixel::Color(5 * BRIGHTNESS, 1 * BRIGHTNESS, 0);
    const uint32_t OrangeDarker = Adafruit_NeoPixel::Color(4 * BRIGHTNESS, 1 * BRIGHTNESS, 0);
    const uint32_t OrangeDarkest = Adafruit_NeoPixel::Color(3 * BRIGHTNESS, 1 * BRIGHTNESS, 0);
    const uint32_t Black = Adafruit_NeoPixel::Color(0, 0, 0);
    const uint32_t Colors[6] = {Yellow, Amber, Orange, OrangeDark, OrangeDarker, OrangeDarkest};
    const uint32_t NumColors = 6;

    struct Pixel
    {
        uint32_t color;
        int delay;
    };

    Adafruit_NeoPixel _pixels = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
    Pixel _pixelData[LED_COUNT];

public:
    Matrix8x8TaskHandler() {}
    bool createTask() override;

private:
    void task(void *parameters) override;
};

bool Matrix8x8TaskHandler::createTask()
{
    if (_taskHandle != NULL)
    {
        log_w("Task already started");
        return false;
    }

    log_d("Initializing matrix");
    _pixels.begin();

    for (int i = 0; i < LED_COUNT; i++)
    {
        _pixelData[i].color = Colors[random(0, NumColors)];
        _pixelData[i].delay = random(BASE_DELAY * 5, BASE_DELAY * 10);

        _pixels.setPixelColor(i, _pixelData[i].color);
    }

    xTaskCreate(taskWrapper, "Matrix8x8Task", 4096, this, TASK_PRIORITY, &_taskHandle);

    log_d("Matrix setup complete");

    return true;
}

void Matrix8x8TaskHandler::task(void *parameters)
{
    log_d("Starting Matrix8x8Task");

    while (1)
    {
        if (!_display)
        {
            _pixels.clear();
            _pixels.show();
            delay(REFRESH_TIME);
            continue;
        }

        for (int i = 0; i < LED_COUNT; i++)
        {
            if (--_pixelData[i].delay > 0)
                continue;

            _pixelData[i].color = _pixelData[i].color == Black
                                      ? Colors[random(0, 6)]
                                      : Black;
            _pixelData[i].delay = random(BASE_DELAY, BASE_DELAY * 2);

            _pixels.setPixelColor(i, _pixelData[i].color);
        }

        _pixels.show();
        delay(REFRESH_TIME);
    }
}
