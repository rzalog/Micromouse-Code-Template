MainController::doStuff(int how_many) {
    // ALSO, make sure you are somehow dealing with resetting
    // encoder counts. You'll need to reset them every time 
    // you do something new with the main controller.

    pc.printf("Doing action.\n");

    // We need to convert everything from human units
    // to counts. Also, probably only need to convert
    // one of these things.
    int x_counts = convert_to_counts(how_many);
    int w_counts = convert_to_counts(how_many);

    m_pid->setXGoal(x_counts);
    m_pid->setWGoal(w_counts);

    // If you haven't implemented "isDone" yet, this
    // will just run forever.
    while (!m_pid->isDone()) {
        pc.printf(m_pid->getData());
    }

    pc.printf("Finished action.\n");
}