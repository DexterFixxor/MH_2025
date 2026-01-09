/*
 * position.h
 *
 *  Created on: 09.01.2026.
 *      Author: lazar
 */

#ifndef LIB_MODULE_POSITION_POSITION_H_
#define LIB_MODULE_POSITION_POSITION_H_

typedef enum
{
	IDLE,
	ROTATE_TO_GOAL,
	TRANSLATE_TO_GOAL,
	ROTATE_TO_THETA,
	GOAL_REACHED
}MotionState_t;

extern volatile MotionState_t current_motion_state;

extern volatile float
x_ref,
y_ref,
theta_ref;

void robot_set_pose_ref(float x, float y, float theta);
void position_control_loop();

#endif /* LIB_MODULE_POSITION_POSITION_H_ */
