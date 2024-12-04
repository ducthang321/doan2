#include <pigpio.h>
#include <stdio.h>

#define SERVO1 17 // Servo trục xoay (góc mặc định 90)
#define SERVO2 18 // Servo bên phải
#define SERVO3 27 // Servo bên trái

// Hàm đặt góc cho servo với offset
void setServoAngle(int gpio, int angle, int offset) {
    int adjustedAngle = angle + offset; // Bù góc offset
    if (adjustedAngle < 0) adjustedAngle = 0;
    if (adjustedAngle > 180) adjustedAngle = 180;
    int pulseWidth = 500 + (adjustedAngle * 2000 / 180); // Từ 500µs đến 2500µs
    gpioServo(gpio, pulseWidth);
}

// Hàm di chuyển servo mượt mà
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

int control_red() {
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

    int offsetServo1 = -90; // Bù góc cho servo1 (90 là 0)

    // Đặt servo1 ở góc 0 thực tế (90 độ vật lý)
    printf("Đặt Servo1 ở góc 0 thực tế (90 độ vật lý)...\n");
    smoothServoMove(SERVO1, 90, 135, 20, 0);
    gpioDelay(1000000); // Chờ 1 giây

    // Xoay servo2 từ 0 đến 90
    printf("Xoay Servo2 từ 0 đến 90...\n");
    smoothServoMove(SERVO2, 90, 90, 20, 0);

    // Xoay servo3 từ 0 đến 90
    printf("Xoay Servo3 từ 0 đến 90...\n");
    smoothServoMove(SERVO3, 90, 30, 20, 0);
    
    gpioDelay(1000000); // Chờ 1 giây
    // Trả servo 1 2 và servo3 về góc 0
    printf("Trả Servo2 và Servo3 về 0...\n");
    smoothServoMove(SERVO1, 135, 90, 20, 0);
    smoothServoMove(SERVO2, 90, 90, 20, 0);
    smoothServoMove(SERVO3, 30, 90, 20, 0);

    // Ngắt tín hiệu servo và kết thúc pigpio
    gpioServo(SERVO1, 0); // Ngắt tín hiệu PWM
    gpioServo(SERVO2, 0);
    gpioServo(SERVO3, 0);
    gpioTerminate(); // Tắt thư viện pigpio

    return 0;
}
