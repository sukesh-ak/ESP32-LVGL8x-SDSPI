#include <lvgl.h>

#define LV_TICK_PERIOD_MS 1

/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = TFT_WIDTH;
static const uint16_t screenHeight = TFT_HEIGHT ;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

static LGFX lcd;            // declare display variable

/*** Function declaration ***/
void display_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#ifdef TOUCH_ENABLED
void touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data);
#endif

static void lv_tick_task(void *arg);

void lv_display_init()
{
    lcd.setBrightness(255);
    lcd.setColorDepth(24); 
  
    /* LVGL : Setting up buffer to use for display */
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

    /*** LVGL : Setup & Initialize the display device driver ***/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = display_flush;
    disp_drv.draw_buf = &draw_buf;
	lv_disp_drv_register(&disp_drv);
    
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
