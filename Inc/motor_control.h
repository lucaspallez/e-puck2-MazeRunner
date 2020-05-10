#ifndef INC_MOTOR_CONTROL_H_
#define INC_MOTOR_CONTROL_H_

//Frequency brackets [Hz]
#define DO3_L		250
#define DO3_H		270
#define RE3_L		270
#define RE3_H		310
#define MI3_L		310
#define MI3_H		340
#define FA3_L		340
#define FA3_H		365
#define SOL3_L		365
#define SOL3_H		410
#define LA3_L		410
#define LA3_H		480
#define DO4_L		480
#define DO4_H		540
#define RE4_L		540
#define RE4_H		610
#define MI4_L		610
#define MI4_H		675
#define FA4_L		675
#define FA4_H		740
#define SOL4_L		740
#define SOL4_H		830
#define LA4_L		830
#define LA4_H		900

//Define positions
#define DO			0
#define RE			1
#define MI			2
#define FA			3
#define SOL			4
#define LA			5
#define STOP		6

//Define robot
#define HALFWAY		3
#define WHEEL_D 	41		//Wheel diameter
#define WHEEL_SEP	53  	//distance between the wheels
#define TURN2STEP	1000	//1 turn = 1000 motor steps
#define NB_SECTIONS	6		//6 sections for 6 muscial notes

#define STEP2SECTION	(((float)WHEEL_SEP)/((float)WHEEL_D)*(((float)TURN2STEP)/((float)NB_SECTIONS)))	//Motor steps to section number



/*
*	Start motor_control thread
*/
void motor_control_start(void);

/*
*	Stop both motors
*/
void stop(void);

/*
*	Makes the robot turn left
*/
void turn_left(void);

/*
*	Makes the robot turn right
*/
void turn_right(void);

/*
*	Makes the robot go straight
*/
void go_straight(void);

#endif /* INC_MOTOR_CONTROL_H_ */
