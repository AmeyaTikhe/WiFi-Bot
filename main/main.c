//Components
#include "sra_board.h"
//#define debug

// C Headers
#include <stdio.h>
#include <math.h>

#define MAX_PWM (50.0f)
#define MIN_PWM (30.0f)

#define TAG "COMMS_WEBSOCKET_SERVER"

#include "tuning_http_server.h"

void drive_task(void *args)
{
    motor_handle_t motor_a_0, motor_a_1;
	ESP_ERROR_CHECK(enable_motor_driver(&motor_a_0, MOTOR_A_0));
	ESP_ERROR_CHECK(enable_motor_driver(&motor_a_1, MOTOR_A_1));

    while(true){
        if(read_comms().val_changed){
            // float speed = read_comms().speed;
            // //print to console
            // ESP_LOGI(TAG, "speed: %f", speed);
            // float motor_pwm = bound((speed), MIN_PWM, MAX_PWM);
            if(read_comms().forward)
            {
                ESP_LOGI(TAG, "forward");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_FORWARD, MAX_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_FORWARD, MAX_PWM));
            } 
            else if(read_comms().backward)
            {
                ESP_LOGI(TAG, "backward");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_BACKWARD, MAX_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_BACKWARD, MAX_PWM));
            } 
            else if(read_comms().front_left) 
            {
                ESP_LOGI(TAG, "front and left");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_FORWARD, MIN_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_FORWARD, MAX_PWM));
            }
            else if(read_comms().front_right) 
            {
                ESP_LOGI(TAG, "front and right");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_FORWARD, MAX_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_FORWARD, MIN_PWM));
            }
            else if(read_comms().back_right) 
            {
                ESP_LOGI(TAG, "back and right");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_BACKWARD, MAX_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_BACKWARD, MIN_PWM));
            }  
            else if(read_comms().back_left) 
            {
                ESP_LOGI(TAG, "back and left");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_BACKWARD, MIN_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_BACKWARD, MAX_PWM));
            } 
            else if(read_comms().anticlockwise) 
            {
                ESP_LOGI(TAG, "anticlockwise");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_BACKWARD, MIN_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_FORWARD, MIN_PWM));
            } 
            else if(read_comms().clockwise) 
            {
                ESP_LOGI(TAG, "clockwise");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_FORWARD, MIN_PWM));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_BACKWARD, MIN_PWM));
            } 
            else 
            {
                ESP_LOGI(TAG, "stop");
                ESP_ERROR_CHECK(set_motor_speed(motor_a_0, MOTOR_STOP, 0));
                ESP_ERROR_CHECK(set_motor_speed(motor_a_1, MOTOR_STOP, 0));
            }
            reset_val_changed_coms();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

void app_main()
{
    start_websocket_server();

    xTaskCreate(&drive_task, "drive task", 4096, NULL, 1, NULL);

}