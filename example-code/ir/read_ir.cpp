float flash_ir(DigitalOut ir, AnalogIn rec) volatile {
    float init = rec.read();

    ir = 1;
    // "US" is microseconds
    wait_us(WARM_UP_US);

    int n = 10;
    float total = 0;
       
    for (int i = 0; i < n; i++) {
        total += rec.read();   
    }

    ir = 0;
    wait_us(COOL_DOWN_US);
    
    return (total / n) - init;
}
