
#ifndef __TDA7491_H__
#define __TDA7491_H__


   typedef enum
   {
      GPIO_81,
      GPIO_82,
      GPIO_83,
      GPIO_86,
      GPIO_91,
      #if(BOARD_USED == BOARD_93xx_CJ2)
      GPIO_95,
      #endif
   } gmt_GPIO_ID;

   typedef struct
   {
      gmt_GPIO_ID PIN_MUTE;
      gmt_GPIO_ID PIN_STBY;
      gmt_GPIO_ID PIN_GAIN_0;
      gmt_GPIO_ID PIN_GAIN_1;
   } gmt_TDA7491_PIN_INFO;

                  void TDA7491_GPIO_SET(gmt_GPIO_ID index, BOOL set);
gmt_TDA7491_PIN_INFO * TDA7491_GetPinInfo(void);
                  void TDA7491_Pin_Config(void);
                  void TDA7491_STBY(BOOL stby);
                  void TDA7491_MUTE(BOOL mute);
                  void TDA7491_GAIN(BYTE level);
                  void TDA7491_ENABLE(void);
                  void TDA7491_DISABLE(void);
                  void TDA7491_Initialize(void);

#endif
