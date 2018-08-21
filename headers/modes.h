#pragma once

const int MAX_MODES = 10;

extern int cur_mode;

void set_mode();

inline int mode(int n) { return cur_mode == n; }