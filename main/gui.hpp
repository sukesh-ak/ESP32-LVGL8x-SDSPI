/*********************
 *      DEFINES
 *********************/
#define HEADER_HEIGHT 30 
#define FOOTER_HEIGHT 30 

char txt[200];

/******************
 *  LV DEFINES
 ******************/
static const lv_font_t *font_large;
static const lv_font_t *font_normal;
static const lv_font_t *font_symbol;

static lv_obj_t *panel_header;
static lv_obj_t *panel_status;
static lv_obj_t *panel_container;

static lv_obj_t *label_title;
static lv_obj_t *label_message;

static lv_obj_t *icon_storage;
static lv_obj_t *icon_wifi;

/******************
 *  LVL STYLES
 ******************/
static lv_style_t style_message;
static lv_style_t style_title;
static lv_style_t style_storage;
static lv_style_t style_wifi;

/******************
 *  LVL FUNCS & EVENTS
 ******************/
static void create_header(lv_obj_t *parent);
static void create_content(lv_obj_t *parent);
static void create_footer(lv_obj_t *parent);
static void display_message(const char * fmt, ...);
static void panel_status_eventhandler(lv_event_t* e);

void lv_setup_styles()
{
    font_symbol = &lv_font_montserrat_14;
    font_normal = &lv_font_montserrat_14;
    font_large = &lv_font_montserrat_20;
    
    // DASHBOARD TITLE
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);
    lv_style_set_align(&style_title, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_left(&style_title, 15);

    // SD CARD
    lv_style_init(&style_storage);
    lv_style_set_text_color(&style_storage, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_text_font(&style_storage, font_symbol);
    lv_style_set_align(&style_storage, LV_ALIGN_RIGHT_MID);
    lv_style_set_pad_right(&style_storage, 15);

    // WIFI
    lv_style_init(&style_wifi);
    lv_style_set_text_color(&style_wifi, lv_palette_main(LV_PALETTE_GREY));
    lv_style_set_text_font(&style_wifi, font_symbol);
    lv_style_set_align(&style_wifi, LV_ALIGN_RIGHT_MID);
    lv_style_set_pad_right(&style_wifi, 45);

    // FOOTER MESSAGE
    lv_style_init(&style_message);
    lv_style_set_text_color(&style_message, lv_palette_main(LV_PALETTE_YELLOW));
    lv_style_set_opa(&style_message, LV_OPA_COVER);
    lv_style_set_text_font(&style_message, font_normal);
    lv_style_set_align(&style_message, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_left(&style_message, 15);    
}

static void create_header(lv_obj_t *parent)
{
    // HEADER PANEL
    panel_header = lv_obj_create(parent);
    lv_obj_set_size(panel_header,LV_PCT(100),HEADER_HEIGHT);
    lv_obj_set_style_bg_color(panel_header, lv_palette_darken(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_pad_all(panel_header, 0, 0);
    lv_obj_set_style_border_width(panel_header, 0, 0);
    lv_obj_set_style_radius(panel_header, 0, 0);
    lv_obj_set_align(panel_header, LV_ALIGN_TOP_MID);

    label_title = lv_label_create(panel_header);
    lv_label_set_text(label_title, "DASHBOARD");
    lv_obj_add_style(label_title, &style_title, 0);

    // HEADER STATUS ICON PANEL
    panel_status = lv_obj_create(panel_header);
    lv_obj_set_style_bg_opa(panel_status,LV_OPA_TRANSP,0);
    lv_obj_set_size(panel_status,120,LV_PCT(100));
    lv_obj_set_style_pad_all(panel_status, 0, 0);
    lv_obj_set_style_border_width(panel_status, 0, 0);
    lv_obj_set_style_radius(panel_status, 0, 0);
    lv_obj_set_style_align(panel_status,LV_ALIGN_RIGHT_MID,0);

    // WIFI
    icon_wifi = lv_label_create(panel_status);
    lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);
    lv_obj_add_style(icon_wifi, &style_wifi, 0);

    // SD CARD
    icon_storage = lv_label_create(panel_status);
    lv_label_set_text(icon_storage, LV_SYMBOL_SD_CARD);
    lv_obj_add_style(icon_storage, &style_storage, 0);  

    lv_obj_add_event_cb(panel_status, panel_status_eventhandler, LV_EVENT_CLICKED, NULL);
}

static void create_content(lv_obj_t *parent)
{
    // CONTENT PANEL CONTAINER
    panel_container = lv_obj_create(parent);
    lv_obj_set_size(panel_container, LV_HOR_RES -20, LV_VER_RES - (HEADER_HEIGHT + FOOTER_HEIGHT + 20));
    lv_obj_set_style_bg_opa(panel_container,LV_OPA_TRANSP,0);
    lv_obj_set_style_border_width(panel_container, 0, 0);
    lv_obj_align(panel_container, LV_ALIGN_TOP_MID, 0, HEADER_HEIGHT+10);
    lv_obj_set_style_pad_all(panel_container,0,0);

    //lv_obj_set_scrollbar_mode(panel_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(panel_container, LV_FLEX_FLOW_ROW_WRAP);

    // Islands #1
    lv_obj_t *contentPanel = lv_obj_create(panel_container);
    lv_obj_set_size(contentPanel, LV_PCT(100), 200);
    lv_obj_set_style_bg_color(contentPanel, lv_palette_darken(LV_PALETTE_GREY, 4), 0);
    lv_obj_set_style_border_width(contentPanel, 0, 0);


    // Islands #2
    lv_obj_t *contentPanel1 = lv_obj_create(panel_container);
    lv_obj_set_size(contentPanel1, LV_PCT(100), 100);
    lv_obj_set_style_bg_color(contentPanel1, lv_palette_darken(LV_PALETTE_GREY, 4), 0);
    lv_obj_set_style_border_width(contentPanel1, 0, 0);

    // SHOW LVGL VERSION IN THE CENTER
    sprintf(txt, "LVGL v%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
    lv_obj_t *label = lv_label_create(contentPanel1);
    lv_obj_set_style_text_color(label, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_label_set_text(label, txt);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

}

static void create_footer(lv_obj_t *parent)
{
    lv_obj_t *panel_footer = lv_obj_create(parent);
    lv_obj_set_size(panel_footer,LV_PCT(100),FOOTER_HEIGHT);
    lv_obj_set_align(panel_footer, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_bg_color(panel_footer, lv_palette_darken(LV_PALETTE_GREY, 4), 0);
    lv_obj_set_style_pad_all(panel_footer, 0, 0);
    lv_obj_set_style_border_width(panel_footer, 0, 0);
    lv_obj_set_style_radius(panel_footer, 0, 0);

    // Create simple label and show LVGL message
    label_message = lv_label_create(panel_footer);      // full screen as the parent
    lv_obj_add_style(label_message, &style_message, 0);

    // Show LVGL version
    display_message("LVGL v%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
}

static void display_message(const char * fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    vsprintf(txt,fmt,args);
    lv_label_set_text(label_message,txt);
    lv_label_set_long_mode(label_message, LV_LABEL_LONG_SCROLL_CIRCULAR);
    va_end(args);
}

static void panel_status_eventhandler(lv_event_t* e)
{
    display_message("Status icons touched! Status icons touched! Status icons touched! ");
}
