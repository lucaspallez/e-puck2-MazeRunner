
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
    static uint8_t average_count = 0;
    static uint16_t prox_left_avg = 0, prox_right_avg = 0, prox_left_temp = 0, prox_right_temp = 0;


    while(1)
    {
    	time = chVTGetSystemTime();

		if((get_calibrated_prox(IR_FRONT_LEFT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_FRONT_RIGHT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_49D_LEFT) >= PROX_THRESHOLD) ||
		   (get_calibrated_prox(IR_49D_RIGHT) >= PROX_THRESHOLD)) //If everything is alright
		{
			palClearPad(GPIOD, GPIOD_LED_FRONT);
			palClearPad(GPIOB, GPIOB_LED_BODY);
		}
		else //If something is wrong (wall very close)
		{
			palSetPad(GPIOD, GPIOD_LED_FRONT);
			palSetPad(GPIOB, GPIOB_LED_BODY);
		}


		prox_left_temp += get_calibrated_prox(IR_LEFT);
		prox_right_temp += get_calibrated_prox(IR_RIGHT);

		if(average_count == 10) //taking 10 values and averaging them
		{
			prox_left_avg = prox_left_temp/average_count;
			prox_right_avg = prox_right_temp/average_count;
			prox_side_diff =  prox_left_avg - prox_right_avg;	//Getting sensor data and taking the difference
			prox_side_average = (prox_left_avg + prox_right_avg)/NB_SENSOR;
			//chBSemSignal(&sendToComputer_sem);
			//chprintf((BaseSequentialStream *)&SDU1, "D_diff_t:%d\nD_avg_t:%d\n", prox_side_diff, prox_side_average); //works
			average_count = 0;
			prox_left_temp = 0;
			prox_right_temp = 0;
		}
		average_count++;

		chThdSleepUntilWindowed(time, time + MS2ST(10)); //reduced the sample rate to 100Hz
    }
}

static THD_WORKING_AREA(score_thd_wa, 256);
static THD_FUNCTION(score_thd, arg) //Works but not reliable at all. Need to find a way to get more stable value out of the sensors
{
	(void) arg;
    chRegSetThreadName(__FUNCTION__);
    systime_t time;
	static float score = 0;
	static uint64_t counter = 0;
	static uint8_t must_send = 0;

    while(1)
    {
    	time = chVTGetSystemTime();

		if(prox_side_diff <= NOISE_THRESHOLD)
		{
			score++; //+1 for perfect score
			counter++; 	//+1 to counter to make 100%
		}
		else if(prox_side_diff < WALL_WIDTH_THRESHOLD)
		{
			if((((float) prox_side_diff)/((float) prox_side_average)) < MAX_SCORE_PER_COUNT)
				score += (1 - ((float) prox_side_diff)/((float) prox_side_average));
			counter++;
		}
		score /= counter; //final score calculation [%]
		if(must_send == 10) //reduce debug message frequency to 1Hz
		{
			chBSemSignal(&sendToComputer_sem);
			chprintf((BaseSequentialStream *)&SDU1, "Score:%f\n Diff:%d\n Avg:%d\n", score, prox_side_diff, prox_side_average);
			must_send = 0;
		}
		must_send++;
		chThdSleepUntilWindowed(time, time + MS2ST(100)); //reduced the sample rate to 10Hz
    }

}
void wall_detection_start(void)
{
	chThdCreateStatic(wall_detection_thd_wa, sizeof(wall_detection_thd_wa), NORMALPRIO, wall_detection_thd, NULL);
}

void score_calculation_start(void)
{
	chThdCreateStatic(score_thd_wa, sizeof(score_thd_wa), NORMALPRIO, score_thd, NULL);
}
