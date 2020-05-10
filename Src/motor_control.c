#include "ch.h"
#include <Inc/motor_control.h>
#include <Inc/audio_processing.h>
#include <motors.h>


static THD_WORKING_AREA(motor_control_thd_wa, 128);
static THD_FUNCTION(motor_control_thd, arg)
{
	(void) arg;
    chRegSetThreadName(__FUNCTION__);
    systime_t time;
	static uint8_t rob_pos=0; //position note du robot en debut de boucle
	static uint8_t note=6; //position codee de la frequence ecoutee
	static uint8_t diff = 0; // Variable de condition
	static uint8_t rotation = 0;//1 si rotation en cours, 0 sinon
	static float freq_temp = 0;
	static float frequency = 0;


	while(1)
	{
		frequency = get_frequency();
    	time = chVTGetSystemTime();
	//Codage de la frequence ecoutee en fonction du schema
	//condition rotation==0 necessaire pour eviter tout changements de comportements avant la fin d'une rotation,
	//on rend le robot sourd jusqu'a la fin de la rotation
		if (rotation==0 && (freq_temp != frequency))
		{
			freq_temp = frequency;	//set current frequency
			left_motor_set_pos(0); 	//reset motor encoder counts
			right_motor_set_pos(0);
			if ((frequency>=DO3_L) && (frequency<DO3_H)) note = DO; 		//Do3:261
			else if ((frequency>=RE3_L) && (frequency<RE3_H)) note = RE;	//Re3:293
			else if ((frequency>=MI3_L) && (frequency<MI3_H)) note = MI;	//Mi3:329
			else if ((frequency>=FA3_L) && (frequency<FA3_H)) note = FA;	//Fa3:349
			else if ((frequency>=SOL3_L) && (frequency<SOL3_H)) note = SOL; //Sol3:392
			else if ((frequency>=LA3_L) && (frequency<LA3_H)) note = LA;	//La3:440
			else if ((frequency>=DO4_L) && (frequency<DO4_H)) note = DO; 	//Do4: 523
			else if ((frequency>=RE4_L) && (frequency<RE4_H)) note = RE; 	//Re4: 587
			else if ((frequency>=MI4_L) && (frequency<MI4_H)) note = MI;	//Mi4: 659
			else if ((frequency>=FA4_L) && (frequency<FA4_H)) note = FA; 	//Fa4: 698
			else if ((frequency>=SOL4_L) && (frequency<SOL4_H)) note = SOL; //Sol4:783
			else if ((frequency>=LA4_L) && (frequency<LA4_H)) note = LA; 	//La4: 880
			else note=STOP; //condition stop
		}


		// condition de deplacement
		if(note == rob_pos) // Si position_robot==position_son_ecoutee alors va tout droit
		{
			go_straight();
		}
		else if((note > rob_pos) && (note != STOP))// Cas 1 de rotation,
		{
			diff = note - rob_pos;
			if (diff >= HALFWAY)
			{
				//turn left 6-diff steps
				turn_left();
				rotation=1;
				if ((left_motor_get_pos()<(-1)*(((float)53)/((float)41))*(((float)1000)/((float)6))*(6-diff)) &&
					(right_motor_get_pos()>(((float)53)/((float)41))*(((float)1000)/((float)6))*(6-diff)))
					//(53/41)=rapport entre le diametre du robot et les roues, 1000 nombre de microstep par rotation de roues,
					//6-diff nombre de 6e de cercle a effectuer
				{
					stop();
					rotation=0;
					left_motor_set_pos(0);
					right_motor_set_pos(0);
					rob_pos = note; //new position
				}
			}
			else
			{
				//turn right diff steps
				turn_right();
				rotation=1;
				if ((right_motor_get_pos()<(-1)*((float)53/(float)41)*((float)1000/(float)6)*diff) &&
					(left_motor_get_pos()>((float)53/(float)41)*((float)1000/(float)6)*diff))
					//diff 6e de cercle a effectuer
				{
					stop();
					rotation=0;
					left_motor_set_pos(0);
					right_motor_set_pos(0);
					rob_pos = note; //new position
				}
			}
		}
		else if (note < rob_pos) // Cas de 2 de rotation
		{
			diff = rob_pos - note;
			if (diff>=3)
			{
				//turn right 6-diff steps
				turn_right();
				rotation=1;
				if ((right_motor_get_pos()<((-1)*((float)53/(float)41)*((float)1000/(float)6)*(6-diff))) &&
					(left_motor_get_pos()>(((float)53/(float)41)*((float)1000/(float)6)*(6-diff))))
				{
					stop();
					rotation=0;
					left_motor_set_pos(0);
					right_motor_set_pos(0);
					rob_pos = note; //new position
				}
			}
			else // turn left diff steps
			{
				turn_left();
				rotation=1;
				if ((left_motor_get_pos()<(-1)*((float)53/(float)41)*((float)1000/(float)6)*diff) &&
					(right_motor_get_pos()>((float)1000/(float)6)*((float)53/(float)41)*diff))
				{
					stop();
					rotation=0;
					left_motor_set_pos(0);
					right_motor_set_pos(0);
					rob_pos = note; //new position
				}
			}
		}
		else stop();
		chThdSleepUntilWindowed(time, time + MS2ST(1)); //reduced the refresh rate to 1kHz
	}
}

void go_straight(void)
{
	left_motor_set_speed(600);
	right_motor_set_speed(600);
}
void turn_right(void)
{
	left_motor_set_speed(1000);
	right_motor_set_speed(-1000);
}
void turn_left(void)
{
	left_motor_set_speed(-1000);
	right_motor_set_speed(1000);
}
void stop(void)
{
	left_motor_set_speed(0);
	right_motor_set_speed(0);
}

void motor_control_start(void)
{
	chThdCreateStatic(motor_control_thd_wa, sizeof(motor_control_thd_wa), NORMALPRIO, motor_control_thd, NULL);
}
