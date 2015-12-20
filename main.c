/*
    PLAY Embedded demos - Copyright (C) 2014-2015 Rocco Marco Guglielmi

    This file is part of PLAY Embedded demos.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
    Tested under ChibiOS/RT 3.0.1, Project version 1.0
 */
#include "ch.h"
#include "hal.h"
#include "chprintf.h"

BaseSequentialStream * chp = (BaseSequentialStream *) &SD2;
static int32_t mean;
static bool flag = FALSE;
static float lastvalue;
static uint32_t percentage=0;
static uint32_t percentage2=0;
static uint32_t percentage3=0;
static uint32_t percentage4=0;
static uint32_t percentage5=0;
static uint32_t increment=2;

static uint8_t c;
static uint8_t orangeControl;

static bool newChar = FALSE;
static bool newPercentage = FALSE;
/*===========================================================================*/
/* ADC related code                                                          */
/*===========================================================================*/

/*
 * In this demo we want to use a single channel to sample voltage across
 * the potentiometer.
 */
#define MY_NUM_CH                                              1
#define MY_SAMPLING_NUMBER                                    10

static adcsample_t sample_buff[MY_NUM_CH * MY_SAMPLING_NUMBER];

/*
 * ADC conversion group.
 * Mode:        Linear buffer, 10 samples of 1 channel, SW triggered.
 * Channels:    IN0.
 */
static const ADCConversionGroup my_conversion_group = {
  FALSE,                            /*NOT CIRCULAR*/
  MY_NUM_CH,                        /*NUMB OF CH*/
  NULL,                             /*NO ADC CALLBACK*/
  NULL,                             /*NO ADC ERROR CALLBACK*/
  0,                                /* CR1 */
  ADC_CR2_SWSTART,                  /* CR2 */
  0,                                /* SMPR1 */
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_3),  /* SMPR2 */
  ADC_SQR1_NUM_CH(MY_NUM_CH),       /* SQR1 */
  0,                                /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN1)   /* SQR3 */
};

/*===========================================================================*/
/* Serial related code                                                          */
/*===========================================================================*/

static THD_WORKING_AREA(waRead, 128);
static THD_FUNCTION(Thd6,arg) {
  (void)arg;
    chRegSetThreadName("thread_callback");

    while (TRUE){
        /* This will wait for a character to be received */
        //uint8_t c = sdGet(&SD2); //get the character
        c = sdGet(&SD2); //get the character
        sdPut(&SD2, c);  //  put the character on sd2
        newChar = TRUE;
    }
}
/*===========================================================================*/
/* PWM related code                                                          */
/*===========================================================================*/

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
   if(percentage)
   //palSetPad(GPIOD, GPIOD_PIN7);
   palSetPad(GPIOD, GPIOD_LED3);

}

static void pwmc1cb(PWMDriver *pwmp) {

  (void)pwmp;
  //palClearPad(GPIOD, GPIOD_PIN7);
  palClearPad(GPIOD, GPIOD_LED3);
  }

static PWMConfig pwmcfg = {
  1000000,                                    /* 1MHz PWM clock frequency.   */
  1000,                                       /* Initial PWM period 1ms.       */
  pwmpcb,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static void pwmpcb2(PWMDriver *pwmp) {

  (void)pwmp;
   if(percentage2)
   //palSetPad(GPIOD, GPIOD_PIN7);
   palSetPad(GPIOD, GPIOD_LED4);

}

static void pwmc1cb2(PWMDriver *pwmp) {

  (void)pwmp;
  //palClearPad(GPIOD, GPIOD_PIN7);
  palClearPad(GPIOD, GPIOD_LED4);
  }

static PWMConfig pwmcfg2 = {
  1000000,                                    /* 1MHz PWM clock frequency.   */
  1000,                                       /* Initial PWM period 1ms.       */
  pwmpcb2,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb2},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static void pwmpcb3(PWMDriver *pwmp) {

  (void)pwmp;
   if(percentage3)
   //palSetPad(GPIOD, GPIOD_PIN7);
   palSetPad(GPIOD, GPIOD_LED5);

}

static void pwmc1cb3(PWMDriver *pwmp) {

  (void)pwmp;
  //palClearPad(GPIOD, GPIOD_PIN7);
  palClearPad(GPIOD, GPIOD_LED5);
  }

static PWMConfig pwmcfg3 = {
  1000000,                                    /* 1MHz PWM clock frequency.   */
  1000,                                       /* Initial PWM period 1ms.       */
  pwmpcb3,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb3},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static void pwmpcb4(PWMDriver *pwmp) {

  (void)pwmp;
   if(percentage4)
   //palSetPad(GPIOD, GPIOD_PIN7);
   palSetPad(GPIOD, GPIOD_LED6);

}

static void pwmc1cb4(PWMDriver *pwmp) {

  (void)pwmp;
  //palClearPad(GPIOD, GPIOD_PIN7);
  palClearPad(GPIOD, GPIOD_LED6);
  }

static PWMConfig pwmcfg4 = {
  1000000,                                    /* 1MHz PWM clock frequency.   */
  1000,                                       /* Initial PWM period 1ms.       */
  pwmpcb4,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb4},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

static void pwmpcb5(PWMDriver *pwmp) {

  (void)pwmp;
   if(percentage5)
   palSetPad(GPIOD, GPIOD_PIN7);

}

static void pwmc1cb5(PWMDriver *pwmp) {

  (void)pwmp;
  palClearPad(GPIOD, GPIOD_PIN7);
  }

static PWMConfig pwmcfg5 = {
  10000000,                                    /* 1MHz PWM clock frequency.   */
  1000,                                       /* Initial PWM period 1ms.       */
  pwmpcb5,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, pwmc1cb5},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

/*===========================================================================*/
/* Common functions                                                          */
/*===========================================================================*/

/*
 * Retrieve the integer part of value
 */
static int32_t ftomod(float value){
  if (value >= 0)
    return (int32_t) value;
  else
    return (int32_t) -1 * value;
}

/*
 * Retrieve the decimal part of value
 */
static uint32_t ftodp(float value) {

  if (value >= 0)
    return (uint32_t) ((value - ftomod (value)) * 1000);
  else
    return (uint32_t) ((-value - ftomod (value)) * 1000);
}

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*static THD_WORKING_AREA(waThd1, 256);
static THD_FUNCTION(Thd1, arg) {


  (void) arg;
  chRegSetThreadName("Led handler");
  pwmStart(&PWMD1, &pwmcfg);

  while(TRUE) {
    percentage = lastvalue * 10000 / 3;
    if(percentage < 100){
      percentage = 0;
      pwmDisableChannel(&PWMD1, 0);
      palClearPad(GPIOD, GPIOD_LED4);
    }

    else if(percentage > 9900){
      percentage = 10000;
      pwmDisableChannel(&PWMD1, 0);
      palSetPad(GPIOD, GPIOD_LED4);

    }
    else{
      pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, percentage));
      pwmEnableChannelNotification(&PWMD1, 0);
      pwmEnablePeriodicNotification(&PWMD1);
      //palSetPad(GPIOD, GPIOD_LED6);
    }
    chThdSleepMilliseconds(1);
  }
}*/

static THD_WORKING_AREA(waThd1, 256);
static THD_FUNCTION(Thd1, arg) {


  (void) arg;
  chRegSetThreadName("Led handler3");
  pwmStart(&PWMD1, &pwmcfg);

  while(TRUE) {
    //percentage = lastvalue * 10000 / 3;
    //if(percentage < 100){
    if(orangeControl == 'A'){
      percentage = 0;
      pwmDisableChannel(&PWMD1, 0);
      palClearPad(GPIOD, GPIOD_LED3);
    }

    else if(orangeControl =='q'){
      percentage = percentage + 500;
      orangeControl='p';

    /*else if(percentage > 9900){
      percentage = 10000;
      pwmDisableChannel(&PWMD1, 0);
      palSetPad(GPIOD, GPIOD_LED3);

*/
    }
    else if(orangeControl =='z'){
      if (percentage > 0) percentage = percentage - 500;
      orangeControl='p';
    }

    else if(orangeControl == 'h'){
      percentage = 0;
      pwmDisableChannel(&PWMD1, 0);
  	  palTogglePad(GPIOD, GPIOD_LED3);
  	  chThdSleepMilliseconds(500);
    }

    else if(orangeControl == 'y'){
      pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, percentage));
      pwmEnableChannelNotification(&PWMD1, 0);
      pwmEnablePeriodicNotification(&PWMD1);
      if(percentage >= 0 && percentage <= 9900){
    	  percentage = percentage + increment;
          }
      else if (percentage < 0 || percentage > 9900){
	  increment = -increment;
      }
  	  chThdSleepMilliseconds(1);
    }

    else if(orangeControl!='h'||'A'){

      pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, percentage));
      pwmEnableChannelNotification(&PWMD1, 0);
      pwmEnablePeriodicNotification(&PWMD1);
    }
    chThdSleepMilliseconds(1);
  }

  //chThdSleepMilliseconds(1);
}

static THD_WORKING_AREA(waThd2, 256);
static THD_FUNCTION(Thd2, arg) {


  (void) arg;
  chRegSetThreadName("Led handler4");
  pwmStart(&PWMD5, &pwmcfg2);

  while(TRUE) {
    //percentage = lastvalue * 10000 / 3;


	  if(percentage2 < 100){
      percentage2 = 0;
      pwmDisableChannel(&PWMD5, 0);
      palClearPad(GPIOD, GPIOD_LED4);
    }

    else if(percentage2 > 9900){
      percentage2 = 10000;
      pwmDisableChannel(&PWMD5, 0);
      palSetPad(GPIOD, GPIOD_LED4);

    }
    else{
      pwmEnableChannel(&PWMD5, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, percentage2));
      pwmEnableChannelNotification(&PWMD5, 0);
      pwmEnablePeriodicNotification(&PWMD5);
    }
    chThdSleepMilliseconds(1);
  }


  //chThdSleepMilliseconds(1);
}

/*static THD_WORKING_AREA(waThd2, 512);
static THD_FUNCTION(Thd2, arg) {

  unsigned ii;
  (void) arg;
  chRegSetThreadName("Led handler");*/
  /*
   * Activates the ADC1 driver.
   */
  /*adcStart(&ADCD1, NULL);
  while(TRUE) {
    adcConvert(&ADCD1, &my_conversion_group, sample_buff, MY_SAMPLING_NUMBER);
*/
    /* Making mean of sampled values.*/
    /*mean = 0;
    for (ii = 0; ii < MY_NUM_CH * MY_SAMPLING_NUMBER; ii++) {
      mean += sample_buff[ii];
    }
    mean /= MY_NUM_CH * MY_SAMPLING_NUMBER;
    lastvalue = (float)mean * 3 / 4096;
    flag = TRUE;
  }
}*/

static THD_WORKING_AREA(waThd3, 256);
static THD_FUNCTION(Thd3, arg) {

  (void) arg;
  chRegSetThreadName("Led handler5");
  pwmStart(&PWMD4, &pwmcfg3);

    while(TRUE) {
      //percentage = lastvalue * 10000 / 3;
      if(percentage3 < 100){
        percentage3 = 0;
        pwmDisableChannel(&PWMD4, 0);
        palClearPad(GPIOD, GPIOD_LED5);
      }

      else if(percentage3 == 555){
            pwmDisableChannel(&PWMD4, 0);
        	  //chThdSleepMilliseconds(500);
            while(percentage3==555){
            //pwmDisableChannel(&PWMD1, 0);
            //palTogglePad(GPIOD, GPIOD_LED3);
            //palClearPad(GPIOD, GPIOD_LED3);
        	  //chThdSleepMilliseconds(500);
        	  palTogglePad(GPIOD, GPIOD_LED5);
            //palSetPad(GPIOD, GPIOD_LED3);
        	  chThdSleepMilliseconds(500);
            }
      }
      else if(percentage3 > 9900){
        percentage3= 10000;
        pwmDisableChannel(&PWMD4, 0);
        palSetPad(GPIOD, GPIOD_LED5);

      }
      else{
        pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, percentage3));
        pwmEnableChannelNotification(&PWMD4, 0);
        pwmEnablePeriodicNotification(&PWMD4);
      }
      chThdSleepMilliseconds(1);
    }
}

static THD_WORKING_AREA(waThd4, 256);
static THD_FUNCTION(Thd4, arg) {

  (void) arg;
  chRegSetThreadName("Led handler6");
  pwmStart(&PWMD3, &pwmcfg4);

    while(TRUE) {
      //percentage = lastvalue * 10000 / 3;
      if(percentage4 < 100){
        percentage4 = 0;
        pwmDisableChannel(&PWMD3, 0);
        palClearPad(GPIOD, GPIOD_LED6);
      }

      else if(percentage4 > 9900){
        percentage4= 10000;
        pwmDisableChannel(&PWMD3, 0);
        palSetPad(GPIOD, GPIOD_LED6);

      }
      else{
        pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, percentage4));
        pwmEnableChannelNotification(&PWMD3, 0);
        pwmEnablePeriodicNotification(&PWMD3);
      }
      chThdSleepMilliseconds(1);
    }
}

static THD_WORKING_AREA(waThd5, 256);
static THD_FUNCTION(Thd5, arg) {

  (void) arg;
  chRegSetThreadName("Led handler6");
  pwmStart(&PWMD9, &pwmcfg5);

    while(TRUE) {
      //percentage = lastvalue * 10000 / 3;
      if(percentage5 < 100){
        percentage5 = 0;
        pwmDisableChannel(&PWMD9, 0);
        palClearPad(GPIOD, GPIOD_PIN7);
      }

      else if(percentage5 > 9900){
        percentage5= 10000;
        pwmDisableChannel(&PWMD9, 0);
        palSetPad(GPIOD, GPIOD_PIN7);

      }
      else{
        pwmEnableChannel(&PWMD9, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD9, percentage5));
        pwmEnableChannelNotification(&PWMD9, 0);
        pwmEnablePeriodicNotification(&PWMD9);
      }
      chThdSleepMilliseconds(1);
    }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Setting up analog inputs used by the demo.
   */
  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_ANALOG);

  /*
   * Activates the serial driver 2 using the driver default configuration.
   */
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(7));
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(7));
  sdStart(&SD2, NULL);


  /* Setting up GPIO output to drive external LED*/
  palSetPadMode(GPIOD, 7, PAL_MODE_OUTPUT_PUSHPULL);


  chThdCreateStatic(waThd1, sizeof(waThd1), NORMALPRIO + 1, Thd1, NULL);
  chThdCreateStatic(waThd2, sizeof(waThd2), NORMALPRIO + 1, Thd2, NULL);
  chThdCreateStatic(waThd3, sizeof(waThd3), NORMALPRIO + 1, Thd3, NULL);
  chThdCreateStatic(waThd4, sizeof(waThd4), NORMALPRIO + 1, Thd4, NULL);
  chThdCreateStatic(waThd5, sizeof(waThd5), NORMALPRIO + 1, Thd5, NULL);
  chThdCreateStatic(waRead, sizeof(waRead), NORMALPRIO + 3, Thd6, NULL);

  /*
   * Normal main() thread activity, in this demo it checks flag status. If flag
   * is true, last value is printed and then flag is lowered. If error is true
   * an error message is printed.
   */
  while (TRUE) {

   /* if (flag) {
      chprintf(chp, "Last value: %d.%03.d V \r\n", ftomod(lastvalue),
               ftodp(lastvalue));
      flag = FALSE;
      chThdSleepMilliseconds(150);
      chprintf(chp, "\033[2J\033[1;1H");
    }*/


	  if (newChar) {
	      switch(c){
                  case 'a':
        	              //palSetPad(GPIOD, GPIOD_LED3);
                	      orangeControl = 'a';
                	      percentage = 5000;
                          break;
                  case 'A':
                          //palClearPad(GPIOD, GPIOD_LED3);
             	          orangeControl = 'A';
                          percentage = 0;
                	      break;
	              case 'q':
	            	      //palSetPad(GPIOD, GPIOD_LED3);
            	          orangeControl = 'q';
	                      //percentage = percentage + 500;
	            	      break;
	              case 'z':
	                      //palClearPad(GPIOD, GPIOD_LED3);
            	          orangeControl = 'z';
	                      //if (percentage > 0) percentage = percentage - 500;
	            	      break;


	              case 's':
	            	      //palSetPad(GPIOD, GPIOD_LED4);
	            	      percentage2 = 5000;
	                      break;
	              case 'S':
	                      //palClearPad(GPIOD, GPIOD_LED4);
	            	      percentage2 = 0;
	                      break;
	              case 'w':
	            	      //palSetPad(GPIOD, GPIOD_LED4);
	                      percentage2 = percentage2 + 500;
	            	      break;
	              case 'x':
	                      //palClearPad(GPIOD, GPIOD_LED4);
	                      if (percentage2 > 0) percentage2 = percentage2 - 500;
	            	      break;

	              case 'd':
	                      //palSetPad(GPIOD, GPIOD_LED5);
	                      percentage3 = 5000;
	                      break;
	              case 'D':
	                      //palClearPad(GPIOD, GPIOD_LED5);
	                      percentage3 = 0;
	                      break;
	              case 'e':
	            	      //palSetPad(GPIOD, GPIOD_LED4);
	                      percentage3 = percentage3 + 500;
	            	      break;
	              case 'c':
	                      //palClearPad(GPIOD, GPIOD_LED4);
	                      if (percentage3 > 0) percentage3 = percentage3 - 500;
	            	      break;


	              case 'f':
	                      //palSetPad(GPIOD, GPIOD_LED5);
	                      percentage4 = 5000;
	                      break;
	              case 'F':
	                      //palClearPad(GPIOD, GPIOD_LED5);
	                      percentage4 = 0;
	                      break;
	              case 'r':
	            	      //palSetPad(GPIOD, GPIOD_LED4);
	                      percentage4 = percentage4 + 500;
	            	      break;
	              case 'v':
	                      //palClearPad(GPIOD, GPIOD_LED4);
	                      if (percentage4 > 0) percentage4 = percentage4 - 500;
	            	      break;

	              case 'g':
	                      //palSetPad(GPIOD, GPIOD_PIN7);
	                      percentage5 = 5000;
	                      break;
	              case 'G':
	                      //palClearPad(GPIOD, GPIOD_PIN7);
	                      percentage5 = 0;
	                      break;
	              case 't':
	            	      //palSetPad(GPIOD, GPIOD_PIN7);
	                      percentage5 = percentage5 + 500;
	            	      break;
	              case 'b':
	                      //palClearPad(GPIOD, GPIOD_PIN7);
	                      if (percentage5 > 0) percentage5 = percentage5 - 500;
	            	      break;

	              case 'h':
	            	  	  //percentage = 555;
	            	  	  orangeControl = 'h';
	            	  	  //newPercentage = TRUE;
	            	      break;

	              case 'y':
	            	  	  //percentage = 555;
	            	  	  orangeControl = 'y';
	            	  	  //newPercentage = TRUE;
	            	      break;


	              case 'k':
	            	  	  percentage3 = 555;
	            	      break;


	              default:
	                      chprintf(chp, "invalidChar\n");
	      }
	      newChar = FALSE;
	  }

    chThdSleepMilliseconds(1);
  }
}
