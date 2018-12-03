// Start here. All this can be run in systick!
// This will just keep going straight forever.

void update() {
	get_sensor_feedback();
	x_controller();
	w_controller();
	update_motor_pwm();
}

void get_sensor_feedback() {
	angle_travelled = encoders.left() - encoders.right();
}

float x_controller() {
	return 0;
}

float w_controller() {
	w_error = goal_angle - angle_travelled;
	pwm_w = KpW * w_error + KdW * (w_error - w_error_old) * dt;

	w_error_old = w_error;
}

void update_motor_pwm() {
	pwm_x = x_controller();
	pwm_w = w_controller();

	right = pwm_x + pwm_w;
	left = pwm_x - pwm_w;

	if (right < min_speed && left < min_speed) {
		counter++;
	}

	motors.set_right_pwm(right);
	motors.set_left_pwm(left);
}

bool is_done() {
	return counter > threshold;
}
