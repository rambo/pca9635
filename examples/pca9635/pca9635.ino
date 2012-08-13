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
    /**
     * This will set the driver to open-drain mode, 0xff will set it to the default totem-pole mode
    PCA9635.set_driver_mode(0x0);
     */

    // Set device address and call I2c.begin() (note: your need to change the address to correspond to your device)
    driver.begin(0x70, false); // ******** You need to change the device address here, 0x70 is the 7-bit ALL-CALL address for PCA9635 ***********

    Serial.println(F("Booted"));
}

void loop()
{
    // Scan the bus, output addresses (just 
    I2c.scan();
    delay(1000);
    
    // Blink leds in turn on all-call
    Serial.println(F("Blinking all drivers leds' in turn (via ALL-CALL)"));
    for (byte ledno = 0; ledno < 16; ledno++)
    {
        // Using half-brightness to make the led light up regardless of whether the drver is wired to source or sink current through it (when in totem-pole mode, open drain naturally is sink-only)
        PCA9635.set_led_pwm(ledno, 0x80);
        delay(250);
        PCA9635.set_led_pwm(ledno, 0x0);
    }
    delay(1000);
    Serial.println(F("Blinking addressed drivers leds' in turn"));
    // Blink leds in turn on the addressed instance
    for (byte ledno = 0; ledno < 16; ledno++)
    {
        driver.set_led_pwm(ledno, 0x80);
        delay(250);
        driver.set_led_pwm(ledno, 0x0);
    }

    // Dump device registers
    driver.dump_registers(0x0, 0x1b);

    delay(5000);
}