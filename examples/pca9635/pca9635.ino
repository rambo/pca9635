// Get this from https://github.com/rambo/I2C
#include <I2C.h> // For some weird reason including this in the relevant .h file does not work
#include <i2c_device.h> // For some weird reason including this in the relevant .h file does not work
#include <pca9635.h>
#define I2C_DEVICE_DEBUG
// Container for the device
pca9635 driver;

void setup()
{
    Serial.begin(115200);
    // Use the SW-Reset address to reset all pca9635 ICs on the bus

    // Initialize I2C library manually
    I2c.begin();
    I2c.timeOut(500);
    PCA9635.reset(); // Incidentally this global name is a pca9635 instance bound to the generla "all-call" address so it can be used to set global parameters
    // Wake oscillators on all-call
    PCA9635.set_sleep(0x0);
    delayMicroseconds(500);
    // Enable PWM controls for all leds on all-call
    PCA9635.set_led_mode(3);

    // Set device address and call I2c.begin() (note: your need to change the address to correspond to your device)
    driver.begin(0xfe, false); // ******** You need to change the device address here ***********

    Serial.println("Booted");
}

void loop()
{
    // Dump device registers and wait 15sek
    driver.dump_registers(0x0, 0x1b);
    // Blink leds in turn on all-call
    Serial.println("Blinking all drivers leds' in turn");
    for (byte ledno = 0; ledno < 16; ledno++)
    {
        PCA9635.set_led_pwm(ledno, 0x80);
        delay(250);
        PCA9635.set_led_pwm(ledno, 0x0);
    }
    Serial.println("Blinking addressed drivers leds' in turn");
    // Blink leds in turn on the addressed instance
    for (byte ledno = 0; ledno < 16; ledno++)
    {
        driver.set_led_pwm(ledno, 0x80);
        delay(250);
        driver.set_led_pwm(ledno, 0x0);
    }


    delay(15000);
}
