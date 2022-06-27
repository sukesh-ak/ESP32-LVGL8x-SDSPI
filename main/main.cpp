#include <stdio.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

//#define ENABLE_TEST_TIMER   // Enable/Disable TIMER used for testing

#define HEADER_HEIGHT 30 
#define FOOTER_HEIGHT 30 

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

// Enable one of the device/display from below
//#include "FeatherS3_ILI9341_conf.h"   // Custom TFT configuration
#include "WT32SCO1_conf.h"              // WT32-SC01 auto config

#include "display_helper.hpp"
#include "storage_helper.hpp"

#include <gui.hpp>

#ifdef ENABLE_TEST_TIMER
static void once_timer_callback(void* arg);
static void periodic_timer_callback(void* arg);
#endif

extern "C" { void app_main(); }

void app_main(void)
{
    lcd.init();        // Initialize LovyanGFX
    lv_init();         // Initialize lvgl
    lv_display_init(); // Configure LVGL

#ifdef ENABLE_TEST_TIMER
/*********************** [START] TIMERS FOR TESTING *********************/

    // Timer which trigger only once
    const esp_timer_create_args_t once_timer_args = {
            .callback = &once_timer_callback,
            .name = "once"
    };
    esp_timer_handle_t once_timer;
    ESP_ERROR_CHECK(esp_timer_create(&once_timer_args, &once_timer));
    ESP_ERROR_CHECK(esp_timer_start_once(once_timer, 1000000)); 


    // Timer which trigger periodically
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            .name = "periodic"
    };

    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 5000000));
  
  /*********************** [END] TIMERS FOR TESTING *********************/
#endif
    lv_setup_styles();

    // STATUS / TITLE BAR
    create_header(lv_scr_act());
    create_content(lv_scr_act());
    create_footer(lv_scr_act());

#ifdef SD_ENABLED
    // Initializing SDSPI 
    if (init_sdspi() != ESP_OK) // SD SPI
        lv_style_set_text_color(&style_storage, lv_palette_main(LV_PALETTE_RED));
    else 
        lv_style_set_text_color(&style_storage, lv_palette_main(LV_PALETTE_GREEN));
    
    lv_obj_refresh_style(icon_storage, LV_PART_ANY, LV_STYLE_PROP_ANY);
#endif
    while (1)
    {
        lv_timer_handler(); // let the GUI do its work
        //lv_timer_handler_run_in_period(5); /* run lv_timer_handler() every 5ms */
        vTaskDelay(1);
    }

/*
    // Won't reach here but just for sanity :)
    // stop and delete the timers
    ESP_ERROR_CHECK(esp_timer_stop(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_delete(periodic_timer));
    ESP_ERROR_CHECK(esp_timer_stop(once_timer));
    ESP_ERROR_CHECK(esp_timer_delete(once_timer));
*/    
}

#ifdef ENABLE_TEST_TIMER
static void once_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Once timer, time since boot: %lld us", time_since_boot);
}

static void periodic_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Periodic timer, time since boot: %lld us", time_since_boot);
}
#endif