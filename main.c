/*
     SerialLedControl- Copyright (C) 2015-2016 Chris Gaddis

    This file is part of SerialLedControl.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless requigreen by applicable law or agreed to in writing, software
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
#include <math.h>


BaseSequentialStream * chp = (BaseSequentialStream *) &SD2;
static int32_t mean;
static bool flag = FALSE;
static float lastvalue;

static uint32_t orangePercentage=0;
static uint32_t greenPercentage=0;
static uint32_t redPercentage=0;
static uint32_t bluePercentage=0;
static uint32_t extPercentage=0;

static uint32_t time;
static uint8_t c;

static double orangeIntensity;
static double greenIntensity;
static double redIntensity;
static double blueIntensity;
static double extIntensity;


static uint32_t orangePeriod;
static uint32_t greenPeriod;
static uint32_t redPeriod;
static uint32_t bluePeriod;
static uint32_t extPeriod;


static uint8_t orangeControl;
static uint8_t greenControl;
static uint8_t redControl;
static uint8_t blueControl;
static uint8_t extControl;




static bool newChar = FALSE;
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
 * Mode:        Linear buffer, 10 samples of 1 channel, SW triggegreen.
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
   if(orangePercentage)
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
   if(greenPercentage)
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
   if(redPercentage)
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
   if(bluePercentage)
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
   if(extPercentage)
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
/* Thread code.                                                             */
/*===========================================================================*/

static THD_WORKING_AREA(waThd1, 256);
static THD_FUNCTION(Thd1, arg) {


  (void) arg;
  chRegSetThreadName("Led handler3");
  pwmStart(&PWMD1, &pwmcfg);

  while(TRUE) {
    //percentage = lastvalue * 10000 / 3;
    //if(percentage < 100){

	if(orangeControl == 'a'){
    orangePercentage = 5000;
	}

    else if(orangeControl == 'A'){
      orangePercentage = 0;
      pwmDisableChannel(&PWMD1, 0);
      palClearPad(GPIOD, GPIOD_LED3);
    }

    else if(orangeControl =='q'){
      orangePercentage = orangePercentage + 500;
      orangeControl='p';

    }
    else if(orangeControl =='z'){
      if (orangePercentage > 0) orangePercentage = orangePercentage - 500;
      orangeControl='p';
    }

    else if(orangeControl == 'h'){
      orangePercentage = 0;
      pwmDisableChannel(&PWMD1, 0);
  	  palTogglePad(GPIOD, GPIOD_LED3);
  	  chThdSleepMilliseconds(500);
    }

    else if(orangeControl == 'H'){
      pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, orangePercentage));
      pwmEnableChannelNotification(&PWMD1, 0);
      pwmEnablePeriodicNotification(&PWMD1);

      orangePeriod = 20000;
      orangeIntensity = cos(2*M_PI/orangePeriod*time);
      orangePercentage=orangeIntensity*10000;
  	  chThdSleepMilliseconds(1);
    }

    else if(orangeControl!='h'||'H' || 'A'){

      pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, orangePercentage));
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
  pwmStart(&PWMD3, &pwmcfg2);

  while(TRUE) {
    //percentage = lastvalue * 10000 / 3;
    //if(percentage < 100){
    if(greenControl == 's'){
	  greenPercentage = 5000;
      pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, greenPercentage));
      pwmEnableChannelNotification(&PWMD3, 0);
      pwmEnablePeriodicNotification(&PWMD3);
    }

	else if (greenControl == 'S'){
      greenPercentage = 0;
      pwmDisableChannel(&PWMD3, 0);
      palClearPad(GPIOD, GPIOD_LED4);
    }

    else if(greenControl =='w'){
      greenPercentage = greenPercentage + 500;
      greenControl='p';

    }
    else if(greenControl =='x'){
      if (greenPercentage > 0) greenPercentage = greenPercentage - 500;
      greenControl='p';
    }

    else if(greenControl == 'j'){
      greenPercentage = 0;
      pwmDisableChannel(&PWMD3, 0);
  	  palTogglePad(GPIOD, GPIOD_LED4);
  	  chThdSleepMilliseconds(500);
    }

    else if(greenControl == 'J'){
      pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, greenPercentage));
      pwmEnableChannelNotification(&PWMD3, 0);
      pwmEnablePeriodicNotification(&PWMD3);

      greenPeriod = 20000;
      greenIntensity = cos(2*M_PI/greenPeriod*time);
      greenPercentage=greenIntensity*10000;
  	  chThdSleepMilliseconds(1);
    }

    else if(greenControl!='j'||'S' || 'J'){

      pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, greenPercentage));
      pwmEnableChannelNotification(&PWMD3, 0);
      pwmEnablePeriodicNotification(&PWMD3);
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
	  chRegSetThreadName("Led handler4");
	  pwmStart(&PWMD4, &pwmcfg3);

	  while(TRUE) {
	    //percentage = lastvalue * 10000 / 3;
	    //if(percentage < 100){
	     if(redControl == 'd'){
		   redPercentage = 5000;
		 }
	     else if(redControl == 'D'){
	      redPercentage = 0;
	      pwmDisableChannel(&PWMD4, 0);
	      palClearPad(GPIOD, GPIOD_LED5);
	    }

	    else if(redControl =='e'){
	      redPercentage = redPercentage + 500;
	      redControl='p';

	    }
	    else if(redControl =='c'){
	      if (redPercentage > 0) redPercentage = redPercentage - 500;
	      redControl='p';
	    }

	    else if(redControl == 'k'){
	      redPercentage = 0;
	      pwmDisableChannel(&PWMD4, 0);
	  	  palTogglePad(GPIOD, GPIOD_LED5);
	  	  chThdSleepMilliseconds(500);
	    }

	    else if(redControl == 'K'){
	      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, redPercentage));
	      pwmEnableChannelNotification(&PWMD4, 0);
	      pwmEnablePeriodicNotification(&PWMD4);

	      redPeriod = 20000;
	      redIntensity = cos(2*M_PI/redPeriod*time);
	      redPercentage=redIntensity*10000;
	  	  chThdSleepMilliseconds(1);
	    }

	    else if(redControl!='k'||'D' || 'K'){

	      pwmEnableChannel(&PWMD4, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, redPercentage));
	      pwmEnableChannelNotification(&PWMD4, 0);
	      pwmEnablePeriodicNotification(&PWMD4);
	    }


      chThdSleepMilliseconds(1);
    }
}

static THD_WORKING_AREA(waThd4, 256);
static THD_FUNCTION(Thd4, arg) {

	  (void) arg;
	  chRegSetThreadName("Led handler4");
	  pwmStart(&PWMD5, &pwmcfg4);

	  while(TRUE) {
	    //percentage = lastvalue * 10000 / 3;
	    //if(percentage < 100){
		if(blueControl == 'f'){
		  bluePercentage = 5000;
		}

	    else if(blueControl == 'F'){
	      bluePercentage = 0;
	      pwmDisableChannel(&PWMD5, 0);
	      palClearPad(GPIOD, GPIOD_LED6);
	    }

	    else if(blueControl =='r'){
	      bluePercentage = bluePercentage + 500;
	      blueControl='p';

	    }
	    else if(blueControl =='v'){
	      if (bluePercentage > 0) bluePercentage = bluePercentage - 500;
	      blueControl='p';
	    }

	    else if(blueControl == 'l'){
	      bluePercentage = 0;
	      pwmDisableChannel(&PWMD5, 0);
	  	  palTogglePad(GPIOD, GPIOD_LED6);
	  	  chThdSleepMilliseconds(500);
	    }

	    else if(blueControl == 'L'){
	      pwmEnableChannel(&PWMD5, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, bluePercentage));
	      pwmEnableChannelNotification(&PWMD5, 0);
	      pwmEnablePeriodicNotification(&PWMD5);

	      bluePeriod = 20000;
	      blueIntensity = cos(2*M_PI/bluePeriod*time);
	      bluePercentage=blueIntensity*10000;
	  	  chThdSleepMilliseconds(1);
	    }

	    else if(blueControl!='l'||'F' || 'L'){

	      pwmEnableChannel(&PWMD5, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD5, bluePercentage));
	      pwmEnableChannelNotification(&PWMD5, 0);
	      pwmEnablePeriodicNotification(&PWMD5);
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
    //if(percentage < 100){

	if(extControl == 'g'){
	  extPercentage = 5000;
	}

    else if(extControl == 'G'){
      extPercentage = 0;
      pwmDisableChannel(&PWMD9, 0);
      palClearPad(GPIOD, GPIOD_PIN7);
    }

    else if(extControl =='t'){
      extPercentage = extPercentage + 500;
      extControl='p';

    }
    else if(extControl =='b'){
      if (extPercentage > 0) extPercentage = extPercentage - 500;
      extControl='p';
    }

    else if(extControl == ';'){
      extPercentage = 0;
      pwmDisableChannel(&PWMD9, 0);
  	  palTogglePad(GPIOD, GPIOD_PIN7);
  	  chThdSleepMilliseconds(500);
    }

    else if(extControl == ':'){
      pwmEnableChannel(&PWMD9, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD9, extPercentage));
      pwmEnableChannelNotification(&PWMD9, 0);
      pwmEnablePeriodicNotification(&PWMD9);

      extPeriod = 20000;
      extIntensity = cos(2*M_PI/extPeriod*time);
      extPercentage=extIntensity*10000;
  	  chThdSleepMilliseconds(1);
    }

    else if(extControl!=';'||'G' || ':'){

      pwmEnableChannel(&PWMD9, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD9, extPercentage));
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
   * - HAL initialization, this also initializes the configugreen device drivers
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
   * is true, last value is printed and then flag is lowegreen. If error is true
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
                	      orangeControl = 'a';
                          break;
                  case 'A':
             	          orangeControl = 'A';
                	      break;
	              case 'q':
            	          orangeControl = 'q';
	            	      break;
	              case 'z':
            	          orangeControl = 'z';
	            	      break;
	              case 'h':
	            	  	  orangeControl = 'h';
	            	      break;

	              case 'H':
	            	  	  orangeControl = 'H';
	            	      break;

                  case 's':
                	      greenControl = 's';
                          break;
                  case 'S':
             	          greenControl = 'S';
                	      break;
	              case 'w':
            	          greenControl = 'w';
	            	      break;
	              case 'x':
            	          greenControl = 'x';
	            	      break;
	              case 'j':
	            	  	  greenControl = 'j';
	            	      break;
	              case 'J':
	            	  	  greenControl = 'J';
	            	      break;

                  case 'd':
                	      redControl = 'd';
                          break;
                  case 'D':
             	          redControl = 'D';
                	      break;
	              case 'e':
            	          redControl = 'e';
	            	      break;
	              case 'c':
            	          redControl = 'c';
	            	      break;
	              case 'k':
	            	  	  redControl = 'k';
	            	      break;

	              case 'K':
	            	  	  redControl = 'K';
	            	      break;


                  case 'f':
                	      blueControl = 'f';
                          break;
                  case 'F':
             	          blueControl = 'F';
                	      break;
	              case 'r':
            	          blueControl = 'r';
	            	      break;
	              case 'v':
            	          blueControl = 'v';
	            	      break;
	              case 'l':
	            	  	  blueControl = 'l';
	            	      break;

	              case 'L':
	            	  	  blueControl = 'L';
	            	      break;

                  case 'g':
                	      extControl = 'g';
                          break;
                  case 'G':
             	          extControl = 'G';
                	      break;
	              case 't':
            	          extControl = 't';
	            	      break;
	              case 'b':
            	          extControl = 'b';
	            	      break;
	              case ';':
	            	  	  extControl = ';';
	            	      break;

	              case ':':
	            	  	  extControl = ':';
	            	      break;


	              default:
	                      chprintf(chp, "invalidChar\n");
	      }
	      newChar = FALSE;
	  }

    time = chVTGetSystemTimeX();
    chThdSleepMilliseconds(1);

  }
}
