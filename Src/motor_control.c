
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

	static int x=0; //position note du robot
	static int f=0; //position de la frequence ecoutee
	static int diff = 0;
	static int rotation = 0;//1 si rotation en cours, 0 sinon
	////balblabla

	//Codage de la frequence ecoutee en fonction du schema //Violon:
	// condition nécessaire pour éviter changement de comportement avant la fin d'une rotation,
	//on rend le robot sourd jusqu'à la fin de la rotation
	if (rotation==0)
	{
		if ((frequency>250) && (frequency<280)) f=0; //Do: 265.625
		else if ((frequency>280) && (frequency<312)) f=1; //Re: 296.875
		else if ((frequency>312) && (frequency<358)) f=2; //Mi: 328.125
		else if ((frequency>358) && (frequency<390)) f=3; //Fa: 375
		else if ((frequency>390) && (frequency<430)) f=4; //Sol: 406.25
		else if ((frequency>430) && (frequency<480)) f=5; //La: 453.125
		else f=6;
	}

	// condition de deplacement
	if (f==x)
	{
		go_straight();
	}
	if (1)
	{
		if (f>x)
		{
			if ((diff=f-x)>=3)
			{
				//turn left 6-diff steps
				turn_left();
				rotation=1;
				if ((left_motor_get_pos()<(-1)*(1000/6)*(6-diff)) && (right_motor_get_pos()>(1000/6)*(6-diff)))
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
				if ((right_motor_get_pos()<(-1)*(1000/6)*diff) && (left_motor_get_pos()>(1000/6)*diff))
				{
					stop();
					rotation=0;
					left_motor_set_pos(0);
					right_motor_set_pos(0);
					x=f; //new position
				}
			}
		}
		if (f<x)
		{
			if ((diff = x-f)>=3)
			{
				//turn right 6-diff steps
				turn_right();
				rotation=1;
				if ((right_motor_get_pos()<(-1)*(1000/6)*(6-diff) && (left_motor_get_pos()>(1000/6)*(6-diff))
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
				if ((left_motor_get_pos()<(-1)*(1000/6)*diff) && (_motor_get_pos()>(1000/6)*diff))
				{
					stop();
					rotation=0;
					left_motor_set_pos(0);
					right_motor_set_pos(0);
					x=f; //new position
				}
			}
		}
	}
	if((f==6) stop();

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

void deplacement(float frequency) // dit au robot de tourner à droite, gauche ou de continuer tout
// droit en fonction du son écouté.
{
	left_motor_set_pos(0);
	right_motor_set_pos(0);
	  //Violon:


	int x=0; //position note du robot
	int f=0; //position de la fréquence écoutée
	int diff;
	int rotation; //1 si rotation en cours, 0 sinon

	//Codage de la frequence écoutée en fonction du schéma //Violon:
	if (frequency>250 & frequency<280) f=0; //Do: 265.625
	if (frequency>280 & frequency<312) f=1; //Ré: 296.875
	if (frequency>312 & frequency<358) f=2; //Mi: 328.125
	if (frequency>358 & frequency<390) f=3; //Fa: 375
	if (frequency>390 & frequency<430) f=4; //Sol: 406.25
	if (frequency>430 & frequency<480) f=5; //La: 453.125

	// condition de déplacement
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
					if (diff=f-x>3)
							{
								//turn left 6-diff steps
								turn_left();
								if (left_motor_get_pos<(-1)(1000/6)*(6-diff) & right_motor_get_pos>(1000/6)*(6-diff))
									{
										stop();
										rotation=0;
									}
							}
					else
							{
								//turn right diff steps
								turn_right();
								if (right_motor_get_pos<(-1)(1000/6)*diff & left_motor_get_pos>(1000/6)*diff)
									{
										stop();
										rotation=0;
									}
							}
				}
			if (f<x)
				{
					if (diff=x-f>3)
						{
							//turn right 6-diff steps
							turn_right();
							if (right_motor_get_pos<(-1)(1000/6)*diff & left_motor_get_pos>(1000/6)*diff)
								{
									stop();
									rotation=0;
								}
						}
					else // turn left diff steps
						{
							turn_right();
							if (right_motor_get_pos<(-1)(1000/6)*diff & left_motor_get_pos>(1000/6)*diff)
								{
									stop();
									rotation=0;
								}
					}
				}
			x=f;
		}
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
	left_motor_set_speed(-600);
	right_motor_set_speed(600);
}
void stop(void)
{
	left_motor_set_speed(0);
	right_motor_set_speed(0);
}

