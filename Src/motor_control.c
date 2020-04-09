
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
