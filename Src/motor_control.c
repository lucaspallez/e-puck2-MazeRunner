
#include <Inc/motor_control.h>
#include <motors.h>



//void motor_control(float frequency) //test
//{
//	if(frequency >= 300 && frequency <= 400)
//		{
//				left_motor_set_speed(600);
//				right_motor_set_speed(600);
//		}
//		else if(frequency > 400 && frequency <= 500)
//		{
//			left_motor_set_speed(-600);
//			right_motor_set_speed(-600);
//		}
//		else
//		{
//			left_motor_set_speed(0);
//			right_motor_set_speed(0);
//		}
//}

void motor_control(float frequency) // dit au robot de tourner a droite, gauche ou de continuer tout
// droit en fonction du son ecoute.
{
//	left_motor_set_pos(0);
//	right_motor_set_pos(0);
	  //Violon:


	int x=0; //position note du robot
	int f=0; //position de la frequence ecoutee
	int diff;
	int rotation; //1 si rotation en cours, 0 sinon

	//Codage de la frequence ecoutee en fonction du schema //Violon:
	if (frequency>250 && frequency<280) f=0; //Do: 265.625
	else if (frequency>280 && frequency<312) f=1; //Re: 296.875
	else if (frequency>312 && frequency<358) f=2; //Mi: 328.125
	else if (frequency>358 && frequency<390) f=3; //Fa: 375
	else if (frequency>390 && frequency<430) f=4; //Sol: 406.25
	else if (frequency>430 && frequency<480) f=5; //La: 453.125
	else f=6;

	// condition de deplacement
	if (f==x)
		{
			go_straight();
			rotation=0;
		}
	if (rotation==0)
		{
			rotation=1;
			if (f>x)
				{
					if ((diff=f-x)>3)
							{
								//turn left 6-diff steps
								turn_left();
								if ((left_motor_get_pos()<(-1)*(1000/6)*(6-diff)) && (right_motor_get_pos()>(1000/6)*(6-diff)))
									{
										stop();
										rotation=0;
										left_motor_set_pos(0);
										right_motor_set_pos(0);
									}
							}
					else
							{
								//turn right diff steps
								turn_right();
								if ((right_motor_get_pos()<(-1)*(1000/6)*diff) && (left_motor_get_pos()>(1000/6)*diff))
									{
										stop();
										rotation=0;
										left_motor_set_pos(0);
										right_motor_set_pos(0);
									}
							}
				}
			else if (f<x)
				{
					if ((diff = x-f) >3)
						{
							//turn right 6-diff steps
							turn_right();
							if ((right_motor_get_pos()<(-1)*(1000/6)*diff) && (left_motor_get_pos()>(1000/6)*diff))
								{
									stop();
									rotation=0;
									left_motor_set_pos(0);
									right_motor_set_pos(0);
								}
						}
					else // turn left diff steps
						{
							turn_right();
							if ((right_motor_get_pos()<(-1)*(1000/6)*diff) && (left_motor_get_pos()>(1000/6)*diff))
								{
									stop();
									rotation=0;
									left_motor_set_pos(0);
									right_motor_set_pos(0);
								}
					}
				}
			x=f; //new position
		}
	if(f==6)
		stop();

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
