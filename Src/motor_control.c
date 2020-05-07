
#include <Inc/motor_control.h>
#include <motors.h>

void motor_control(float frequency) // dit au robot de tourner a droite, gauche ou de continuer tout
// droit en fonction du son ecoute.
{
	static uint8_t x=0; //position note du robot
	static uint8_t f=6; //position de la frequence ecoutee changement intialisation
	static uint8_t diff = 0;
	static uint8_t rotation = 0;//1 si rotation en cours, 0 sinon
	static uint16_t freq_temp = 0;


	//Codage de la frequence ecoutee en fonction du schema
	// condition nécessaire pour éviter changement de comportement avant la fin d'une rotation,
	//on rend le robot sourd jusqu'à la fin de la rotation
	if (rotation==0 && (freq_temp != frequency))
	{
		freq_temp = frequency;
		left_motor_set_pos(0);
		right_motor_set_pos(0);
		if ((frequency>500) && (frequency<540)) f=0; //Do: 523
		else if ((frequency>560) && (frequency<610)) f=1; //Re: 587
		else if ((frequency>630) && (frequency<670)) f=2; //Mi: 659
    	else if ((frequency>675) && (frequency<720)) f=3; //Fa: 698
		else if ((frequency>760) && (frequency<810)) f=4; //Sol:783
		else if ((frequency>850) && (frequency<900)) f=5; //La: 880
		else f=6;
	}
	// condition de deplacement
	if(f==x)
	{
		go_straight();
	}
	else if((f>x) && (f!=6))
	{
		diff=f-x;
		if (diff>=3)
		{
			//turn left 6-diff steps
			turn_left();
			rotation=1;
			if ((left_motor_get_pos()<(-1)*(((float)53)/((float)41))*(((float)1000)/((float)6))*(6-diff)) &&
				(right_motor_get_pos()>(((float)53)/((float)41))*(((float)1000)/((float)6))*(6-diff)))
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
			{
				stop();
				rotation=0;
				left_motor_set_pos(0);
				right_motor_set_pos(0);
				x=f; //new position
			}
		}
	}
	else if (f<x)
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
	left_motor_set_speed(600);
	right_motor_set_speed(-600);
}
void turn_left(void)
{
	left_motor_set_speed(-600);
	right_motor_set_speed(600);
}
void stop(void)
{
	left_motor_set_speed(0);
	right_motor_set_speed(0);
}
