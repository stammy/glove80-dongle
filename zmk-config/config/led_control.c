#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/kernel.h>
#include <zmk/split/bluetooth/central.h>

#define RED_NODE DT_ALIAS(red_pwm_led)
#define GREEN_NODE DT_ALIAS(green_pwm_led)
#define BLUE_NODE DT_ALIAS(blue_pwm_led)

static const struct pwm_dt_spec red_led = PWM_DT_SPEC_GET(RED_NODE);
static const struct pwm_dt_spec green_led = PWM_DT_SPEC_GET(GREEN_NODE);
static const struct pwm_dt_spec blue_led = PWM_DT_SPEC_GET(BLUE_NODE);

#define MAX_BRIGHTNESS 65535

void set_led_color(bool connected) {
    if (connected) {
        // Green when connected
        pwm_set_dt(&red_led, MAX_BRIGHTNESS, 0);           // OFF (0% duty cycle)
        pwm_set_dt(&green_led, MAX_BRIGHTNESS, MAX_BRIGHTNESS); // ON (100% duty cycle)
        pwm_set_dt(&blue_led, MAX_BRIGHTNESS, 0);          // OFF (0% duty cycle)
    } else {
        // Red when disconnected
        pwm_set_dt(&red_led, MAX_BRIGHTNESS, MAX_BRIGHTNESS); // ON (100% duty cycle)
        pwm_set_dt(&green_led, MAX_BRIGHTNESS, 0);         // OFF (0% duty cycle)
        pwm_set_dt(&blue_led, MAX_BRIGHTNESS, 0);         // OFF (0% duty cycle)
    }
}

void led_update_callback(bool connected) {
    set_led_color(connected);
}

void led_init(void) {
    if (!device_is_ready(red_led.dev) || 
        !device_is_ready(green_led.dev) || 
        !device_is_ready(blue_led.dev)) {
        return;
    }

    // Start with red (disconnected)
    set_led_color(false);
    
    zmk_split_bt_central_register_callback(led_update_callback);
}

SYS_INIT(led_init, APPLICATION, CONFIG_APPLICATION_INIT_PRIORITY);
