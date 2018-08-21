// Most of these variables are globals, or class
// variables, don't lose track of them!

void get_sensor_feedback() {
	// Do not want these values changing in between calculations
	int right = encoders.right();
	int left = encoders.left();
	
	distance_travelled = (right + left) / 2;
	angle_travelled = left - right;
}

float x_controller() {
	x_error = goal_distance - distance_travelled;
	pwm_x = KpX * x_error + KdX * (x_error - x_error_old) * dt;

	x_error_old = x_error;

	return pwm_x;
}

float w_controller() {
	w_error = goal_angle - angle_travelled;
	pwm_w = KpW * w_error + KdW * (w_error - w_error_old) * dt;

	w_error_old = w_error;

	return pwm_w;
}

void update_motor_pwm() {
	pwm_x = x_controller();
	pwm_w = w_controller();
	
	adjust_pwms(&pwm_x, &pwm_w);
	
	if (too_slow(pwm_x, pwm_w)) {
		if (has_been_going_slow_for_too_long()) {
			finish_pid();
		}
	}
	else {
		motors.set_right_pwm(pwm_x + pwm_w);
		motors.set_left_pwm(pwm_x - pwm_w);
	}
}