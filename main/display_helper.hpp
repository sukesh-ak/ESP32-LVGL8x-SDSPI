#include <lvgl.h>

#define LV_TICK_PERIOD_MS 1
//#define MODE_DARK 1

/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = TFT_WIDTH;
static const uint16_t screenHeight = TFT_HEIGHT ;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];
static lv_color_t buf2[screenWidth * 10];
static lv_disp_t *disp;
static lv_theme_t *theme_current;
static lv_color_t bg_theme_color;
static LGFX lcd;            // declare display variable

/*** Function declaration ***/
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#ifdef TOUCH_ENABLED
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
#endif

static void lv_tick_task(void *arg);

void lv_display_init()
{
    // Setting display to landscape
    // if (lcd.width() < lcd.height()) 
    //lcd.setRotation(lcd.getRotation() ^ 2);
        
    lcd.setBrightness(128);
    lcd.setColorDepth(24); 
  
    /* LVGL : Setting up buffer to use for display */
    lv_disp_draw_buf_init(&draw_buf, buf, buf2, screenWidth * 10);

    /*** LVGL : Setup & Initialize the display device driver ***/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
	disp = lv_disp_drv_register(&disp_drv);
    
#ifdef TOUCH_ENABLED    
    //*** LVGL : Setup & Initialize the input device driver ***
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = touchpad_read;
    lv_indev_drv_register(&indev_drv);
#endif

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
        };
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    // Setup theme
    theme_current = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE),
                                           lv_palette_main(LV_PALETTE_RED),
                                           LV_USE_THEME_DEFAULT, /*Light or dark mode*/
                                           &lv_font_montserrat_14);

    //lv_disp_set_theme(disp, th); /*Assign the theme to the display*/    
    bg_theme_color = theme_current->flags & LV_USE_THEME_DEFAULT ? lv_palette_darken(LV_PALETTE_GREY, 4) : lv_palette_lighten(LV_PALETTE_GREY, 1);
}

// Display callback to flush the buffer to screen 
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    lcd.startWrite();
    lcd.setAddrWindow(area->x1, area->y1, w, h);
    lcd.pushPixels((uint16_t *)&color_p->full, w * h, true);
    lcd.endWrite();

    lv_disp_flush_ready(disp);
}

/* Setting up tick task for lvgl */
static void lv_tick_task(void *arg)
{
    (void)arg;
    lv_tick_inc(LV_TICK_PERIOD_MS);
}

#ifdef TOUCH_ENABLED
// Touchpad callback to read the touchpad
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;
    bool touched = lcd.getTouch(&touchX, &touchY);

    if (!touched)
    {
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;

        // Set the coordinates
        data->point.x = touchX;
        data->point.y = touchY;
    }
}
#endif