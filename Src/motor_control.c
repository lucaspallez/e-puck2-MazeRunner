
#include <Inc/motor_control.h>
#include <motors.h>



void motor_control(float frequency) //test
{
	if(frequency >= 300 && frequency <= 400)
		{
				left_motor_set_speed(600);
				right_motor_set_speed(600);
		}
		else if(frequency > 400 && frequency <= 500)
		{
			left_motor_set_speed(-600);
			right_motor_set_speed(-600);
		}
		else
		{
			left_motor_set_speed(0);
			right_motor_set_speed(0);
		}
}

void deplacment(float frequency) // dit au robot de tourner à droite, gauche ou de continuer tout
// droit en fonction du son écouté.
{
	  //Violon:


	int x=0; //position note du robot
	int f=0; //position de la fréquence écoutée
	int diff;
	int rotation; //1 si rotation en cours, 0 sinon

	//Codage de la frequence écoutée en fonction du schéma //Violon:
	if (f>250 & f<280) f=0; //Do: 265.625
	if (f>280 & f<312) f=1; //Ré: 296.875
	if (f>312 & f<358) f=2; //Mi: 328.125
	if (f>358 & f<390) f=3; //Fa: 375
	if (f>390 & f<430) f=4; //Sol: 406.25
	if (f>430 & f<480) f=5; //La: 453.125

	// condition de déplacement
	if (f==x)
		{
			go_straight();
			rotation=0;
		}
	if (rotation==0)
		{
			if (f>x)
				rotation=1;
				{
					if (diff=f-x>3)
							{
								//turn left 6-diff times
							}
					else // turn right diff times
				}
			if (f<x)
				rotation=1;
				{
					if (diff=x-f>3)
						{
							//turn right 6-diff times
						}
					else // turn left diff times
				}
			x=f;
		}
}

void go_straight(void)
{
	left_motor_set_speed(600);
	right_motor_set_speed(600);
}
void turn_right(void) // effectue 1 sixième de cercle
{
	
}


