#include <stdio.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"

#define LGFX_USE_V1         
#include <LovyanGFX.hpp>    

#include "FeatherS3_ILI9341_conf.h"  // Custom TFT configuration

#include "display_helper.hpp"
#include "storage_helper.hpp"

char txt[100];

extern "C" {void app_main(); }

void app_main(void)
{
    lcd.init();			// Initialize LovyanGFX
	lv_init();  		// Initialize lvgl
    lv_display_init();	// Configure LVGL
	
	init_sdspi();		// SD SPI

    // CONTENT PANEL W/H = 90%, Color = GREY, Border = RED
    lv_obj_t * contentPanel = lv_obj_create(lv_scr_act());
    lv_obj_set_size(contentPanel, LV_PCT(90), LV_PCT(90));
	lv_obj_set_style_bg_color(contentPanel, lv_palette_darken(LV_PALETTE_GREY, 4),0);
    lv_obj_set_style_border_width(contentPanel, 5, 5);
	lv_obj_set_style_border_color(contentPanel, lv_palette_main(LV_PALETTE_RED),0);
    lv_obj_align(contentPanel, LV_ALIGN_CENTER, 0, 0);
	
	// SHOW LVGL VERSION IN THE CENTER
    sprintf(txt,"LVGL v%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
    lv_obj_t *label = lv_label_create(contentPanel); 
	lv_obj_set_style_text_color(label,lv_palette_main(LV_PALETTE_YELLOW),0);
    lv_label_set_text(label, txt);                   
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);    
		
    while (1)
    {
        lv_timer_handler(); // let the GUI do its work 
        vTaskDelay(1);
    }	
}
