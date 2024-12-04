#include <pigpio.h>
#include <stdio.h>
#define SERVO1 17
void setServoAngle(int gpio, int angle) {
    int adjustedAngle = angle; // Bù góc offset
    if (adjustedAngle < 0) adjustedAngle = 0;
    if (adjustedAngle > 180) adjustedAngle = 180;
    int pulseWidth = 500 + (adjustedAngle * 2000 / 180); // Từ 500µs đến 2500µs
    gpioServo(gpio, pulseWidth);
}

void smoothServoMove(int gpio, int startAngle, int endAngle, int delayMs, int offset) {
    if (startAngle < endAngle) {
        for (int angle = startAngle; angle <= endAngle; angle++) {
            setServoAngle(gpio, angle, offset);
            gpioDelay(delayMs * 1000); // ms -> µs
        }
    } else {
        for (int angle = startAngle; angle >= endAngle; angle--) {
            setServoAngle(gpio, angle, offset);
            gpioDelay(delayMs * 1000); // ms -> µs
        }
    }
}

int route(int angle){
    gpioSetMode(SERVO1, PI_OUTPUT);

    setServoAngle(SERVO1, 90);

    smoothServoMove(SERVO1, 90,90 + angle, 20, 0);
    gpioDelay(1000000);
    printf("Xoay goc %d", angle);
    return 0;
}