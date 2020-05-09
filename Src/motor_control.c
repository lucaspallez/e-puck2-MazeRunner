
#include <Inc/motor_control.h>
#include <motors.h>

void motor_control(float frequency) // dit au robot de tourner a droite, gauche ou de continuer tout
// droit en fonction du son ecoute.
{
	static uint8_t x=0; //position note du robot en début de boucle
	static uint8_t f=6; //position codée de la frequence ecoutee
	static uint8_t diff = 0; // Variable de condition
	static uint8_t rotation = 0;//1 si rotation en cours, 0 sinon
	static uint16_t freq_temp = 0;

	//Codage de la frequence ecoutee en fonction du schema
	//condition rotation==0 necessaire pour eviter tout changements de comportements avant la fin d'une rotation,
	//on rend le robot sourd jusqu'a la fin de la rotation
	if (rotation==0 && (freq_temp != frequency))
	{
		freq_temp = frequency;
		left_motor_set_pos(0);
		right_motor_set_pos(0);
		if ((frequency>=250) && (frequency<270)) f=0; //Do3:261
		else if ((frequency>=270) && (frequency<310)) f=1;//Re3:293
		else if ((frequency>=310) && (frequency<340)) f=2;//Mi3:329
		else if ((frequency>=340) && (frequency<365)) f=3;//Fa3:349
		else if ((frequency>=365) && (frequency<410)) f=4;//Sol3:392
		else if ((frequency>=410) && (frequency<480)) f=5;//La3:440
		else if ((frequency>=480) && (frequency<540)) f=0; //Do4: 523
		else if ((frequency>=540) && (frequency<610)) f=1; //Re4: 587
		else if ((frequency>=610) && (frequency<675)) f=2; //Mi4: 659
    	else if ((frequency>=675) && (frequency<740)) f=3; //Fa4: 698
		else if ((frequency>=740) && (frequency<830)) f=4; //Sol4:783
		else if ((frequency>=830) && (frequency<900)) f=5; //La4: 880
		else f=6; //condition stop
	}


	// condition de deplacement
	if(f==x) // Si position_robot==position_son_ecoutee alors va tout droit
	{
		go_straight();
	}
	else if((f>x) && (f!=6))// Cas 1 de rotation,
	{
		diff=f-x;
		if (diff>=3)
		{
			//turn left 6-diff steps
			turn_left();
			rotation=1;
			if ((left_motor_get_pos()<(-1)*(((float)53)/((float)41))*(((float)1000)/((float)6))*(6-diff)) &&
				(right_motor_get_pos()>(((float)53)/((float)41))*(((float)1000)/((float)6))*(6-diff)))
				//(53/41)=rapport entre le diametre du robot et les roues, 1000 nombre de microstep par rotation de roues, 6-diff nombre de 6e de cercle a effectuer
			{
				stop();
				rotation=0;
				left_motor_set_pos(0);
				right_motor_set_pos(0);
				x=f; //new position
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
				x=f; //new position
			}
		}
	}
	else if (f<x) // Cas de 2 de rotation
	{
		diff = x-f;
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
				x=f; //new position
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
				x=f; //new position
			}
		}
	}
	else stop();
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
