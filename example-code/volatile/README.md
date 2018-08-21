This section covers how to make your variables volatile. This is extremely important when using systick, and you likely want to have all variables you are using within systick be volatile.

Files included:
* `globals.cpp`. Where your global variables are defined held (see "globals" example)
* `globals.h`.  Where your global variables are declared, and accessed by the rest of your program.
* `v_class.cpp`. Volatile class impementation.
* `v_class.h`. Volatile class definition.
* `main.cpp`. Using the volatile class.

Fun notes about `volatile`:
* If in doubt, use `volatile` for anything that interacts with systick. At worst, it makes your mouse program run slower, at best it fixes extremely hard to catch problems.
* If your instance of a class is declared volatile (e.g., `volatile V_Class v_instance`), it can _only_ use member functions declared `volatile`.