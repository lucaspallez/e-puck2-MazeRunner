#ifndef INC_PROXIMITY_CONTROL_H_
#define INC_PROXIMITY_CONTROL_H_

#define IR_FRONT_RIGHT 			0		//IR1
#define IR_FRONT_LEFT 			7		//IR8
#define IR_49D_RIGHT			1		//IR2
#define IR_49D_LEFT				6		//IR7
#define IR_LEFT					5		//IR6
#define IR_RIGHT				2		//IR3
#define NB_SENSOR				2		//Use of the 2 front sensors only
#define MAX_SCORE_PER_COUNT		1
#define PROX_THRESHOLD			1000 	//Empirically determined


/*
*	Starts wall_detection and score calculation thread
*/
void wall_detection_start(void);

#endif /* INC_PROXIMITY_CONTROL_H_ */
