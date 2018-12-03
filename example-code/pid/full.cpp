// Most of these variables are globals, or class
// variables, don't lose track of them!

void update() {
	get_sensor_feedback();
	x_controller();
	w_controller();
	update_motor_pwm();
}

void get_sensor_feedback() {
	// Do not want these values changing in between calculations
	int right = encoders.right();
	int left = encoders.left();
	
	distance_travelled = (right + left) / 2;
	angle_travelled = right - left;
}

void x_controller() {
	x_error = goal_distance - distance_travelled;
	pwm_x = KpX * x_error + KdX * (x_error - x_error_old) * dt;

	x_error_old = x_error;

	pwmX = pwm_x;
}

void w_controller() {
	w_error = goal_angle - angle_travelled;
	pwm_w = KpW * w_error + KdW * (w_error - w_error_old) * dt;

	w_error_old = w_error;

	pwmW = pwm_w;
}

void update_motor_pwm() {
	// You MUST do this. Otherwise, good luck :). But why?
	// (may or may not be a check-off question)
	pwm_x = limit_pwm(pwm_x);
	pwm_w = limit_pwm(pwm_w);

	float right = pwm_x + pwm_w;
	float left = pwm_x - pwm_w;

	if (right < min_speed && left < min_speed) {
		counter++;
	}

	motors.set_right_pwm(pwm_x + pwm_w);
	motors.set_left_pwm(pwm_x - pwm_w);
}

bool isDone() {
	return counter > threshold;
}

float limit_pwm(float pwm) {
	if (abs(pwm) > max_speed) {
		if (pwm > 0) pwm = max_speed;
		else if (pwm < 0) pwm = -max_speed;
	}
}