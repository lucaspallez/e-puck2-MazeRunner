
#include <Inc/proximity_control.h>
#include <sensors/proximity.h>
#include <ch.h>
#include <hal.h>
#include <usbcfg.h>
#include <chprintf.h>


static BSEMAPHORE_DECL(sendToComputer_sem, TRUE);
static uint16_t prox_side_average = 0;
static int16_t prox_side_diff = 0;

static THD_WORKING_AREA(wall_detection_thd_wa, 256);
static THD_FUNCTION(wall_detection_thd, arg)
{
	(void) arg;
    chRegSetThreadName(__FUNCTION__);
    systime_t time;
    static uint8_t score_calculation = 0;
	static float score = 0;
	static uint32_t score_temp = 0;
	static uint32_t counter = 0;

    while(1)
    {
    	time = chVTGetSystemTime();

		if((get_calibrated_prox(IR_FRONT_LEFT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_FRONT_RIGHT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_49D_LEFT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_49D_RIGHT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_LEFT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_RIGHT) >= PROX_THRESHOLD)) //If everything is alright
		{
			palClearPad(GPIOD, GPIOD_LED_FRONT);
			palClearPad(GPIOB, GPIOB_LED_BODY);
			counter++;
		}
		else //If something is wrong (wall very close)
		{
			palSetPad(GPIOD, GPIOD_LED_FRONT);
			palSetPad(GPIOB, GPIOB_LED_BODY);
			score_temp++;
			counter++;
		}

		if(score_calculation == 20) //reduced calculation rate and print to 5Hz
		{
			score = ((float) score_temp/ (float) counter)*100; //final score calculation [%]
			chBSemSignal(&sendToComputer_sem);
			chprintf((BaseSequentialStream *)&SDU1, "Score:%.2f\n", score);
			score_calculation = 0;
		}
		score_calculation++;

		chThdSleepUntilWindowed(time, time + MS2ST(10)); //reduced the sample rate to 100Hz
    }
}

void wall_detection_start(void)
{
	chThdCreateStatic(wall_detection_thd_wa, sizeof(wall_detection_thd_wa), NORMALPRIO, wall_detection_thd, NULL);
}
