// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: H743DS18A

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * ui_WaveChart;
lv_obj_t * ui_name11;
lv_obj_t * ui_name12;
lv_obj_t * ui_Panel1;
lv_obj_t * ui_name1;
lv_obj_t * ui_amp1;
lv_obj_t * ui_Panel2;
lv_obj_t * ui_name2;
lv_obj_t * ui_amp2;
lv_obj_t * ui_Panel3;
lv_obj_t * ui_name3;
lv_obj_t * ui_amp3;
lv_obj_t * ui_Panel4;
lv_obj_t * ui_name4;
lv_obj_t * ui_amp4;
lv_obj_t * ui_Panel5;
lv_obj_t * ui_name5;
lv_obj_t * ui_amp5;
lv_obj_t * ui_Panel6;
lv_obj_t * ui_name6;
lv_obj_t * ui_freq1;
lv_obj_t * ui_Panel7;
lv_obj_t * ui_name7;
lv_obj_t * ui_freq2;
lv_obj_t * ui_Panel8;
lv_obj_t * ui_name8;
lv_obj_t * ui_freq3;
lv_obj_t * ui_Panel9;
lv_obj_t * ui_name9;
lv_obj_t * ui_freq4;
lv_obj_t * ui_Panel10;
lv_obj_t * ui_name10;
lv_obj_t * ui_freq5;
lv_obj_t * ui_name13;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{

    // ui_Screen1

    ui_Screen1 = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_WaveChart

    ui_WaveChart = lv_chart_create(ui_Screen1);

    lv_obj_set_width(ui_WaveChart, 440);
    lv_obj_set_height(ui_WaveChart, 294);

    lv_obj_set_x(ui_WaveChart, -98);
    lv_obj_set_y(ui_WaveChart, -70);

    lv_obj_set_align(ui_WaveChart, LV_ALIGN_CENTER);

    // ui_name11

    ui_name11 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_name11, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name11, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name11, 546);
    lv_obj_set_y(ui_name11, -210);

    lv_obj_set_align(ui_name11, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name11, "单位:mAPP");

    lv_obj_set_style_text_font(ui_name11, &ui_font_HMOS18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_name12

    ui_name12 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_name12, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name12, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name12, 5);
    lv_obj_set_y(ui_name12, -226);

    lv_obj_set_align(ui_name12, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name12, "-10^3/mA");

    lv_obj_set_style_text_font(ui_name12, &ui_font_HMOS18, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel1

    ui_Panel1 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel1, 253);
    lv_obj_set_height(ui_Panel1, 50);

    lv_obj_set_x(ui_Panel1, 540);
    lv_obj_set_y(ui_Panel1, 50);

    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_name1

    ui_name1 = lv_label_create(ui_Panel1);

    lv_obj_set_width(ui_name1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name1, -10);
    lv_obj_set_y(ui_name1, 0);

    lv_obj_set_align(ui_name1, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name1, "一次基波");

    lv_obj_set_style_text_font(ui_name1, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_amp1

    ui_amp1 = lv_label_create(ui_Panel1);

    lv_obj_set_width(ui_amp1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_amp1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_amp1, 10);
    lv_obj_set_y(ui_amp1, 0);

    lv_obj_set_align(ui_amp1, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_amp1, "0.000mA");

    lv_obj_set_style_text_font(ui_amp1, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel2

    ui_Panel2 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel2, 253);
    lv_obj_set_height(ui_Panel2, 50);

    lv_obj_set_x(ui_Panel2, 540);
    lv_obj_set_y(ui_Panel2, 111);

    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);

    // ui_name2

    ui_name2 = lv_label_create(ui_Panel2);

    lv_obj_set_width(ui_name2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name2, -10);
    lv_obj_set_y(ui_name2, 0);

    lv_obj_set_align(ui_name2, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name2, "二次谐波");

    lv_obj_set_style_text_font(ui_name2, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_amp2

    ui_amp2 = lv_label_create(ui_Panel2);

    lv_obj_set_width(ui_amp2, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_amp2, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_amp2, 10);
    lv_obj_set_y(ui_amp2, 0);

    lv_obj_set_align(ui_amp2, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_amp2, "0.000mA");

    lv_obj_set_style_text_font(ui_amp2, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel3

    ui_Panel3 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel3, 253);
    lv_obj_set_height(ui_Panel3, 50);

    lv_obj_set_x(ui_Panel3, 540);
    lv_obj_set_y(ui_Panel3, 172);

    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);

    // ui_name3

    ui_name3 = lv_label_create(ui_Panel3);

    lv_obj_set_width(ui_name3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name3, -10);
    lv_obj_set_y(ui_name3, 0);

    lv_obj_set_align(ui_name3, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name3, "三次谐波");

    lv_obj_set_style_text_font(ui_name3, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_amp3

    ui_amp3 = lv_label_create(ui_Panel3);

    lv_obj_set_width(ui_amp3, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_amp3, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_amp3, 10);
    lv_obj_set_y(ui_amp3, 0);

    lv_obj_set_align(ui_amp3, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_amp3, "0.000mA");

    lv_obj_set_style_text_font(ui_amp3, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel4

    ui_Panel4 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel4, 253);
    lv_obj_set_height(ui_Panel4, 50);

    lv_obj_set_x(ui_Panel4, 540);
    lv_obj_set_y(ui_Panel4, 233);

    lv_obj_clear_flag(ui_Panel4, LV_OBJ_FLAG_SCROLLABLE);

    // ui_name4

    ui_name4 = lv_label_create(ui_Panel4);

    lv_obj_set_width(ui_name4, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name4, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name4, -10);
    lv_obj_set_y(ui_name4, 0);

    lv_obj_set_align(ui_name4, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name4, "四次谐波");

    lv_obj_set_style_text_font(ui_name4, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_amp4

    ui_amp4 = lv_label_create(ui_Panel4);

    lv_obj_set_width(ui_amp4, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_amp4, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_amp4, 10);
    lv_obj_set_y(ui_amp4, 0);

    lv_obj_set_align(ui_amp4, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_amp4, "0.000mA");

    lv_obj_set_style_text_font(ui_amp4, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel5

    ui_Panel5 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel5, 253);
    lv_obj_set_height(ui_Panel5, 50);

    lv_obj_set_x(ui_Panel5, 540);
    lv_obj_set_y(ui_Panel5, 290);

    lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE);

    // ui_name5

    ui_name5 = lv_label_create(ui_Panel5);

    lv_obj_set_width(ui_name5, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name5, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name5, -10);
    lv_obj_set_y(ui_name5, 0);

    lv_obj_set_align(ui_name5, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name5, "五次谐波");

    lv_obj_set_style_text_font(ui_name5, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_amp5

    ui_amp5 = lv_label_create(ui_Panel5);

    lv_obj_set_width(ui_amp5, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_amp5, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_amp5, 10);
    lv_obj_set_y(ui_amp5, 0);

    lv_obj_set_align(ui_amp5, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_amp5, "0.000mA");

    lv_obj_set_style_text_font(ui_amp5, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_Panel6

    ui_Panel6 = lv_obj_create(ui_Screen1);

    lv_obj_set_width(ui_Panel6, 259);
    lv_obj_set_height(ui_Panel6, 50);

    lv_obj_set_x(ui_Panel6, 8);
    lv_obj_set_y(ui_Panel6, 348);

    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);

    // ui_name6

    ui_name6 = lv_label_create(ui_Panel6);

    lv_obj_set_width(ui_name6, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name6, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name6, -10);
    lv_obj_set_y(ui_name6, 0);

    lv_obj_set_align(ui_name6, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name6, "基波频率");

    lv_obj_set_style_text_font(ui_name6, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_freq1

    ui_freq1 = lv_label_create(ui_Panel6);

    lv_obj_set_width(ui_freq1, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_freq1, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_freq1, 10);
    lv_obj_set_y(ui_freq1, 0);

    lv_obj_set_align(ui_freq1, LV_ALIGN_RIGHT_MID);

    lv_label_set_text(ui_freq1, "1000.000Hz");

    lv_obj_set_style_text_font(ui_freq1, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

//    // ui_Panel7
//
//    ui_Panel7 = lv_obj_create(ui_Screen1);
//
//    lv_obj_set_width(ui_Panel7, 259);
//    lv_obj_set_height(ui_Panel7, 50);
//
//    lv_obj_set_x(ui_Panel7, 273);
//    lv_obj_set_y(ui_Panel7, 348);
//
//    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_SCROLLABLE);
//
//    // ui_name7
//
//    ui_name7 = lv_label_create(ui_Panel7);
//
//    lv_obj_set_width(ui_name7, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_name7, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_name7, -10);
//    lv_obj_set_y(ui_name7, 0);
//
//    lv_obj_set_align(ui_name7, LV_ALIGN_LEFT_MID);
//
//    lv_label_set_text(ui_name7, "二次谐频");
//
//    lv_obj_set_style_text_font(ui_name7, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_freq2
//
//    ui_freq2 = lv_label_create(ui_Panel7);
//
//    lv_obj_set_width(ui_freq2, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_freq2, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_freq2, 10);
//    lv_obj_set_y(ui_freq2, 0);
//
//    lv_obj_set_align(ui_freq2, LV_ALIGN_RIGHT_MID);
//
//    lv_label_set_text(ui_freq2, "1000.000Hz");
//
//    lv_obj_set_style_text_font(ui_freq2, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_Panel8
//
//    ui_Panel8 = lv_obj_create(ui_Screen1);
//
//    lv_obj_set_width(ui_Panel8, 259);
//    lv_obj_set_height(ui_Panel8, 50);
//
//    lv_obj_set_x(ui_Panel8, 536);
//    lv_obj_set_y(ui_Panel8, 348);
//
//    lv_obj_clear_flag(ui_Panel8, LV_OBJ_FLAG_SCROLLABLE);
//
//    // ui_name8
//
//    ui_name8 = lv_label_create(ui_Panel8);
//
//    lv_obj_set_width(ui_name8, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_name8, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_name8, -10);
//    lv_obj_set_y(ui_name8, 0);
//
//    lv_obj_set_align(ui_name8, LV_ALIGN_LEFT_MID);
//
//    lv_label_set_text(ui_name8, "三次谐频");
//
//    lv_obj_set_style_text_font(ui_name8, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_freq3
//
//    ui_freq3 = lv_label_create(ui_Panel8);
//
//    lv_obj_set_width(ui_freq3, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_freq3, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_freq3, 10);
//    lv_obj_set_y(ui_freq3, 0);
//
//    lv_obj_set_align(ui_freq3, LV_ALIGN_RIGHT_MID);
//
//    lv_label_set_text(ui_freq3, "1000.000Hz");
//
//    lv_obj_set_style_text_font(ui_freq3, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_Panel9
//
//    ui_Panel9 = lv_obj_create(ui_Screen1);
//
//    lv_obj_set_width(ui_Panel9, 259);
//    lv_obj_set_height(ui_Panel9, 50);
//
//    lv_obj_set_x(ui_Panel9, 8);
//    lv_obj_set_y(ui_Panel9, 410);
//
//    lv_obj_clear_flag(ui_Panel9, LV_OBJ_FLAG_SCROLLABLE);
//
//    // ui_name9
//
//    ui_name9 = lv_label_create(ui_Panel9);
//
//    lv_obj_set_width(ui_name9, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_name9, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_name9, -10);
//    lv_obj_set_y(ui_name9, 0);
//
//    lv_obj_set_align(ui_name9, LV_ALIGN_LEFT_MID);
//
//    lv_label_set_text(ui_name9, "四次谐频");
//
//    lv_obj_set_style_text_font(ui_name9, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_freq4
//
//    ui_freq4 = lv_label_create(ui_Panel9);
//
//    lv_obj_set_width(ui_freq4, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_freq4, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_freq4, 10);
//    lv_obj_set_y(ui_freq4, 0);
//
//    lv_obj_set_align(ui_freq4, LV_ALIGN_RIGHT_MID);
//
//    lv_label_set_text(ui_freq4, "1000.000Hz");
//
//    lv_obj_set_style_text_font(ui_freq4, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_Panel10
//
//    ui_Panel10 = lv_obj_create(ui_Screen1);
//
//    lv_obj_set_width(ui_Panel10, 259);
//    lv_obj_set_height(ui_Panel10, 50);
//
//    lv_obj_set_x(ui_Panel10, 273);
//    lv_obj_set_y(ui_Panel10, 410);
//
//    lv_obj_clear_flag(ui_Panel10, LV_OBJ_FLAG_SCROLLABLE);
//
//    // ui_name10
//
//    ui_name10 = lv_label_create(ui_Panel10);
//
//    lv_obj_set_width(ui_name10, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_name10, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_name10, -10);
//    lv_obj_set_y(ui_name10, 0);
//
//    lv_obj_set_align(ui_name10, LV_ALIGN_LEFT_MID);
//
//    lv_label_set_text(ui_name10, "五次谐频");
//
//    lv_obj_set_style_text_font(ui_name10, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);
//
//    // ui_freq5
//
//    ui_freq5 = lv_label_create(ui_Panel10);
//
//    lv_obj_set_width(ui_freq5, LV_SIZE_CONTENT);
//    lv_obj_set_height(ui_freq5, LV_SIZE_CONTENT);
//
//    lv_obj_set_x(ui_freq5, 10);
//    lv_obj_set_y(ui_freq5, 0);
//
//    lv_obj_set_align(ui_freq5, LV_ALIGN_RIGHT_MID);
//
//    lv_label_set_text(ui_freq5, "1000.000Hz");
//
//    lv_obj_set_style_text_font(ui_freq5, &ui_font_HMOS24, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_name13

    ui_name13 = lv_label_create(ui_Screen1);

    lv_obj_set_width(ui_name13, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_name13, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_name13, 6);
    lv_obj_set_y(ui_name13, 93);

    lv_obj_set_align(ui_name13, LV_ALIGN_LEFT_MID);

    lv_label_set_text(ui_name13, "-10^3/Hz");

    lv_obj_set_style_text_font(ui_name13, &ui_font_HMOS18, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    lv_disp_load_scr(ui_Screen1);
}

