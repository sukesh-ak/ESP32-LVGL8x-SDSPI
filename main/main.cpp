#include <stdio.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "FeatherS3_ILI9341_conf.h" // Custom TFT configuration

#include "display_helper.hpp"
#include "storage_helper.hpp"

char txt[100];
static void once_timer_callback(void* arg);
static void periodic_timer_callback(void* arg);

extern "C"
{
    void app_main();
}

void app_main(void)
{
    lcd.init();        // Initialize LovyanGFX
    lv_init();         // Initialize lvgl
    lv_display_init(); // Configure LVGL

/*********************** [START] TIMERS FOR TESTING *********************/
    // Timer with callback every 5second
    // Just show card details
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &periodic_timer_callback,
            .name = "periodic"
    };

    // Timer which trigger only once
    // Initialize SDSPI from this timer after 2seconds
    const esp_timer_create_args_t once_timer_args = {
            .callback = &once_timer_callback,
            .name = "once"
    };

    esp_timer_handle_t once_timer;
    esp_timer_handle_t periodic_timer;

    // Create the timer
    ESP_ERROR_CHECK(esp_timer_create(&once_timer_args, &once_timer));
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));

    // Start the timers 
    ESP_ERROR_CHECK(esp_timer_start_once(once_timer, 1000000)); 
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, 5000000));
  /*********************** [END] TIMERS FOR TESTING *********************/

    // CONTENT PANEL W/H = 90%, Color = GREY, Border = RED
    lv_obj_t *contentPanel = lv_obj_create(lv_scr_act());
    lv_obj_set_size(contentPanel, LV_PCT(90), LV_PCT(90));
    lv_obj_set_style_bg_color(contentPanel, lv_palette_darken(LV_PALETTE_GREY, 4), 0);
    lv_obj_set_style_border_width(contentPanel, 5, 0);
    lv_obj_set_style_border_color(contentPanel, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_align(contentPanel, LV_ALIGN_CENTER, 0, 0);

    // SHOW LVGL VERSION IN THE CENTER
    sprintf(txt, "LVGL v%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
    lv_obj_t *label = lv_label_create(contentPanel);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_label_set_text(label, txt);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    while (1)
    {
        lv_timer_handler(); // let the GUI do its work
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

static void once_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Initialize SDSPI, time since boot: %lld us", time_since_boot);
    init_sdspi(); // SD SPI
}

static void periodic_timer_callback(void* arg)
{
    int64_t time_since_boot = esp_timer_get_time();
    ESP_LOGI(TAG, "Card info every 2s, time since boot: %lld us", time_since_boot);
    sdmmc_card_print_info(stdout, sdcard);
}
