/**
 * @file lv_port_disp_templ.c
 *
 */

 /*Copy this file as "lv_port_disp.c" and set this value to "1" to enable content*/
#if 1

/*********************
 *      INCLUDES
 *********************/
#include "lv_port_disp.h"
#include "../../lvgl.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void disp_init(void);

static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p);
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//        const lv_area_t * fill_area, lv_color_t color);

/**********************
 *  STATIC VARIABLES
 **********************/
lv_color_t buf_1[0x10000] ; //
//lv_color_t buf_2[MY_DISP_HOR_RES*MY_DISP_VER_RES] ;//__attribute__((section(".sdram"))); //
/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_disp_init(void)
{
    /*-------------------------
     * Initialize your display
     * -----------------------*/
    disp_init();

    /*-----------------------------
     * Create a buffer for drawing
     *----------------------------*/
    static lv_disp_draw_buf_t draw_buf_dsc_1;

    lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, 0x10000);   /*In
     /*Initialize the display buffer*/

    /*-----------------------------------
     * Register the display in LVGL
     *----------------------------------*/

    static lv_disp_drv_t disp_drv;                         /*Descriptor of a display driver*/
    lv_disp_drv_init(&disp_drv);                    /*Basic initialization*/

    /*Set up the functions to access to your display*/

    /*Set the resolution of the display*/
    disp_drv.hor_res = MY_DISP_HOR_RES;
    disp_drv.ver_res = MY_DISP_VER_RES;

    /*Used to copy the buffer's content to the display*/
    disp_drv.flush_cb = disp_flush;

    /*Set a display buffer*/
    disp_drv.draw_buf = &draw_buf_dsc_1;

    /*Required for double buffer)*/
    //disp_drv.full_refresh = 1;

    /* Fill a memory array with a color if you have GPU.
     * Note that, in lv_conf.h you can enable GPUs that has built-in support in LVGL.
     * But if you have a different GPU you can use with this callback.*/
    //disp_drv.gpu_fill_cb = gpu_fill;

    /*Finally register the driver*/
    lv_disp_drv_register(&disp_drv);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/*Initialize your display and the required peripherals.*/
static void disp_init(void)
{
#if(MCU_LCD)
  LCD_Init();
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port,LCD_BL_Pin,GPIO_PIN_SET);	 //点亮背光
  LCD_Display_Dir(1);
#elif (RGB_LCD)
  LTDC_Init();
  LTDC_Clear(RED);
  HAL_Delay(1000);
#endif
}

/*Flush the content of the internal buffer the specific area on the display
 *You can use DMA or any hardware acceleration to do this operation in the background but
 *'lv_disp_flush_ready()' has to be called when finished.*/
static void disp_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    /*The most simple case (but also the slowest) to put all pixels to the screen one-by-one*/

    uint16_t height,width;
    width=area->x2-area->x1+1; 			//得到填充的宽度
    height=area->y2-area->y1+1;			//高度
    uint16_t size = width * height;
#if(MCU_LCD)
//    uint16_t i,j;
//    for(i=0;i<height;i++)
//    {
//      LCD_SetCursor(area->x1,area->y1+i);   	//设置光标位置
//      LCD_WriteRAM_Prepare();     //开始写入GRAM
//      for(j=0;j<width;j++)
//      {
//        LCD->LCD_RAM=color_p->full;//写入数据
//        color_p++;
//      }
//    }
    LCD_Set_Window(area->x1, area->y1, width, height);
    LCD_WriteRAM_Prepare();
    for(uint16_t i = 0; i < size; i++)
    {
      LCD->LCD_RAM=*((uint16_t *)color_p+i);
    }
#elif (RGB_LCD)
    LTDC_Color_Fill(area->x1, area->y1, area->x2, area->y2,(uint16_t *)color_p);
#endif




    /*IMPORTANT!!!
     *Inform the graphics library that you are ready with the flushing*/
    lv_disp_flush_ready(disp_drv);
}

/*OPTIONAL: GPU INTERFACE*/

/*If your MCU has hardware accelerator (GPU) then you can use it to fill a memory with a color*/
//static void gpu_fill(lv_disp_drv_t * disp_drv, lv_color_t * dest_buf, lv_coord_t dest_width,
//                    const lv_area_t * fill_area, lv_color_t color)
//{
//    /*It's an example code which should be done by your GPU*/
//    int32_t x, y;
//    dest_buf += dest_width * fill_area->y1; /*Go to the first line*/
//
//    for(y = fill_area->y1; y <= fill_area->y2; y++) {
//        for(x = fill_area->x1; x <= fill_area->x2; x++) {
//            dest_buf[x] = color;
//        }
//        dest_buf+=dest_width;    /*Go to the next line*/
//    }
//}


#else /*Enable this file at the top*/

/*This dummy typedef exists purely to silence -Wpedantic.*/
typedef int keep_pedantic_happy;
#endif
