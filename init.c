#include <pigpio.h>
#include <stdio.h>

#define SERVO1 17 // Servo trục xoay (góc mặc định 90)
#define SERVO2 18 // Servo bên phải
#define SERVO3 27 // Servo bên trái

void setServoAngle(int gpio, int angle, int offset) {
    int adjustedAngle = angle + offset; // Bù góc offset
    if (adjustedAngle < 0) adjustedAngle = 0;
    if (adjustedAngle > 180) adjustedAngle = 180;
    int pulseWidth = 500 + (adjustedAngle * 2000 / 180); // Từ 500µs đến 2500µs
    gpioServo(gpio, pulseWidth);
}

int main() {
    // Khởi tạo pigpio
    if (gpioInitialise() < 0) {
        printf("Không thể khởi tạo pigpio!\n");
        return 1;
    }
    printf("Khởi tạo pigpio thành công!\n");

    // Cài đặt GPIO cho các servo
    gpioSetMode(SERVO1, PI_OUTPUT);
    gpioSetMode(SERVO2, PI_OUTPUT);
    gpioSetMode(SERVO3, PI_OUTPUT);
    
    setServoAngle(SERVO1, 90, 0); // Servo1: Trái 45 (thực tế: 45 - 90)
    setServoAngle(SERVO2, 90, 0);            // Servo2: Góc 90
    setServoAngle(SERVO3, 30, 0);            // Servo3: Góc 30
    return 0;
}