/*
  * ESPRESSIF MIT License
  *
  * Copyright (c) 2017 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
  *
  * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
  * it is free of charge, to any person obtaining a copy of this software and associated
  * documentation files (the "Software"), to deal in the Software without restriction, including
  * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
  * to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in all copies or
  * substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  *
  */

#include <stdio.h>
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "iot_light.h"
#include "unity.h"

#define CHANNEL_ID_R    0
#define CHANNEL_ID_G    1
#define CHANNEL_ID_B    2
#define CHANNEL_R_IO    15
#define CHANNEL_G_IO    18
#define CHANNEL_B_IO    19

#define LIGHT_FULL_DUTY ((1 << LEDC_TIMER_13_BIT) - 1)

void light_test()
{
    printf("before light create, heap: %d\n", esp_get_free_heap_size());
    light_handle_t light = iot_light_create(LEDC_TIMER_0, LEDC_HIGH_SPEED_MODE, 1000, 3, LEDC_TIMER_13_BIT);
    iot_light_channel_regist(light, CHANNEL_ID_R, CHANNEL_R_IO, LEDC_CHANNEL_0);
    iot_light_channel_regist(light, CHANNEL_ID_G, CHANNEL_G_IO, LEDC_CHANNEL_1);
    iot_light_channel_regist(light, CHANNEL_ID_B, CHANNEL_B_IO, LEDC_CHANNEL_2);
    iot_light_duty_write(light, CHANNEL_ID_R, LIGHT_FULL_DUTY, LIGHT_DUTY_FADE_2S);
    iot_light_breath_write(light, CHANNEL_ID_R, 4000);
    iot_light_breath_write(light, CHANNEL_ID_B, 8000);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    iot_light_blink_starte(light, (1<<CHANNEL_ID_R)|(1<<CHANNEL_ID_G)|(1<<CHANNEL_ID_B), 100);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    iot_light_blink_stop(light);
    iot_light_duty_write(light, CHANNEL_ID_R, LIGHT_FULL_DUTY, LIGHT_DUTY_FADE_2S);
    iot_light_duty_write(light, CHANNEL_ID_G, LIGHT_FULL_DUTY / 8, LIGHT_DUTY_FADE_2S);
    iot_light_duty_write(light, CHANNEL_ID_B, LIGHT_FULL_DUTY / 4, LIGHT_DUTY_FADE_2S);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    printf("after light create, heap: %d\n", esp_get_free_heap_size());
    iot_light_delete(light);
    printf("after light delete, heap: %d\n", esp_get_free_heap_size());
}

TEST_CASE("Light test", "[light][iot]")
{
    light_test();
}
