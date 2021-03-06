/* RMT example -- RGB LED Strip

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "py/obj.h"
#include "py/runtime.h"
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include<stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"
int bytes[4];
static const char *TAG = "example";
#define CONFIG_EXAMPLE_RMT_TX_GPIO 18
#define CONFIG_EXAMPLE_STRIP_LED_NUMBER 100

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)
static int start_rgb;
void draw_pixel(int j,int  red,int green, int blue);
void write(int time);
/*
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
/*
void clear()
{
    ESP_ERROR_CHECK(strip->clear(strip, 100));
}
*/
//void led_strip_hsv2rgb(uint32_t h, uint32_t s, uint32_t v, uint32_t *r, uint32_t *g, uint32_t *b)
int  led_strip_hsv2rgb(uint32_t hue, uint32_t saturation, uint32_t value)
{
  /*  h %= 360; // h -> [0,360]
    uint32_t rgb_max = v * 2.55f;
    uint32_t rgb_min = rgb_max * (100 - s) / 100.0f;

    uint32_t i = h / 60;
    uint32_t diff = h % 60;

    // RGB adjustment amount by hue
    uint32_t rgb_adj = (rgb_max - rgb_min) * diff / 60;*/
   
    uint32_t r = 0;
    uint32_t g = 0;
    uint32_t b = 0;
   uint32_t i = floor(hue * 6);
   uint32_t f = hue * 6 - i;
    uint32_t p = value * (1 - saturation);
    uint32_t q = value * (1 - f * saturation);
    uint32_t t = value * (1 - (1 - f) * saturation);    
    uint32_t index = i % 6;
    switch (index) {
    case 0:
        r = value;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = value;
        b = p;
        break;
    case 2:
        r =p;
        g = value;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = value;
        break;
    case 4:
        r = t;
        g = p;
        b = value;
        break;
    default:
        r = value;
        g = p;
        b = q;
        break;
    }
    bytes[0] = r*255;
    bytes[1] = g*255;
    bytes[2] = b*255;
    bytes[3]= index;
   for(int i=0;i<4;i++)
    {
      printf("%d \n",bytes[i]);
    }
  return bytes;
}
static led_strip_t *strip;

STATIC void initialize()
{   

    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;
     printf("hey i am in initialize function\n");
    int hue;
    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));
 
    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(200, (led_strip_dev_t)config.channel);
    
    strip= led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
         //printf("hey i am in initialize function 4\n");
    }
    // Clear LED strip (turn off all LEDs)
    //ESP_ERROR_CHECK(strip->clear(strip, 100));

    

}


void write(int time)
{
  //  uint32_t time = mp_obj_get_int(a_obj);
  ESP_ERROR_CHECK(strip->refresh(strip, time));
}

void draw_pixel( int j,int  red,int green, int blue)
{
  ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
}

void write_val(mp_obj_t a_obj)
{
  int time = mp_obj_get_int(a_obj);
  //  uint32_t time = mp_obj_get_int(a_obj);
  ESP_ERROR_CHECK(strip->refresh(strip, time));
}

void clear_buf(mp_obj_t a_obj)
{
  int timee = mp_obj_get_int(a_obj);
  strip->clear(strip, timee);
}

STATIC void get_color_HSV(mp_obj_t a_obj,mp_obj_t b_obj,mp_obj_t c_obj)
{
    int hue = mp_obj_get_int(a_obj);
    int saturation = mp_obj_get_int(b_obj);
    int  value= mp_obj_get_int(c_obj);


    uint16_t start_rgb = 0;
    printf("hey i am in  get color!\n");
  //  while (true) {
        //for (int i = 0; i < 3; i++) {
           // for (int j = i; i < 10000; j += 3) {
                // Build RGB values
                //hue = j * 360 / 200 + start_rgb;
               
               int *ptr =  led_strip_hsv2rgb(hue, saturation, value);
                uint32_t red = ptr[0];
                uint32_t green = ptr[1] ;
                uint32_t blue = ptr[2] ;
               int  j = ptr[3];
                for(int i=0;i<4;i++)
                {
                  printf("%d \n",ptr[i]);
                }
                //draw_pixel( ind, red, green, blue);
                draw_pixel( 5, red, green, blue);
               // int time =90;
               // write_val(time);
               //ESP_ERROR_CHECK(strip->set_pixel(strip, j, red, green, blue));
            //}
            // Flush RGB values to LEDs
           
           //ESP_ERROR_CHECK(strip->refresh(strip, 100));
           // vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
           // strip->clear(strip, 50);
           // vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
       // }
       // start_rgb += 60;
  //  }
}





MP_DEFINE_CONST_FUN_OBJ_0(initialize_obj,  initialize);
MP_DEFINE_CONST_FUN_OBJ_3(get_color_HSV_obj,  get_color_HSV);
MP_DEFINE_CONST_FUN_OBJ_1(write_val_obj,  write_val);
MP_DEFINE_CONST_FUN_OBJ_1(clear_buf_obj,  clear_buf);

STATIC const mp_rom_map_elem_t led_strip_main_module_globals_table [] =
{

 { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_led_strip_main) },
 { MP_ROM_QSTR(MP_QSTR_initialize),  MP_ROM_PTR(& initialize_obj) },
 { MP_ROM_QSTR(MP_QSTR_get_color_HSV),  MP_ROM_PTR(& get_color_HSV_obj) },
 { MP_ROM_QSTR(MP_QSTR_write_val),  MP_ROM_PTR(& write_val_obj) },
 { MP_ROM_QSTR(MP_QSTR_clear_buf),  MP_ROM_PTR(& clear_buf_obj) },
};

STATIC MP_DEFINE_CONST_DICT(led_strip_main_module_globals,led_strip_main_module_globals_table);

const mp_obj_module_t led_strip_main_user_cmodule = 
{
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&led_strip_main_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_led_strip_main, led_strip_main_user_cmodule, 1);
