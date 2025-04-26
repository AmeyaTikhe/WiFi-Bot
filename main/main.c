//Components
#include "sra_board.h"
//#define debug

// C Headers
#include <stdio.h>
#include <math.h>

#define MAX_PWM 80
#define MIN_PWM 50

#define TAG "COMMS_WEBSOCKET_SERVER"

#include "tuning_http_server.h"

void drive_task(void *args)
{
    motor_handle_t motor_a_0, motor_a_1;
	ESP_ERROR_CHECK(enable_motor_driver(&motor_a_0, MOTOR_A_0));
	ESP_ERROR_CHECK(enable_motor_driver(&motor_a_1, MOTOR_A_1));

    while(true){
        if(read_comms()->val_changed){
            // float speed = read_comms().speed;
            // //print to console
            // ESP_LOGI(TAG, "speed: %f", speed);
            // float motor_pwm = bound((speed), MIN_PWM, MAX_PWM);
            if(read_comms()->forward == true)
            {
                ESP_LOGI(TAG, "forward");
                set_motor_speed(motor_a_0, MOTOR_FORWARD, MAX_PWM);
                set_motor_speed(motor_a_1, MOTOR_FORWARD, MAX_PWM);
                read_comms()->forward = false;
            } 
            else if(read_comms()->backward == true)
            {
                ESP_LOGI(TAG, "backward");
                set_motor_speed(motor_a_0, MOTOR_BACKWARD, MAX_PWM);
                set_motor_speed(motor_a_1, MOTOR_BACKWARD, MAX_PWM);
                read_comms()->backward = false;
            } 
            else if(read_comms()->front_left == true) 
            {
                ESP_LOGI(TAG, "front and left");
                set_motor_speed(motor_a_0, MOTOR_FORWARD, MIN_PWM);
                set_motor_speed(motor_a_1, MOTOR_FORWARD, MAX_PWM);
                read_comms()->front_left = false;
            }
            else if(read_comms()->front_right == true) 
            {
                ESP_LOGI(TAG, "front and right");
                set_motor_speed(motor_a_0, MOTOR_FORWARD, MAX_PWM);
                set_motor_speed(motor_a_1, MOTOR_FORWARD, MIN_PWM);
                read_comms()->front_right = false;
            }
            else if(read_comms()->back_right == true) 
            {
                ESP_LOGI(TAG, "back and right");
                set_motor_speed(motor_a_0, MOTOR_BACKWARD, MAX_PWM);
                set_motor_speed(motor_a_1, MOTOR_BACKWARD, MIN_PWM);
                read_comms()->back_right = false;
            }  
            else if(read_comms()->back_left == true) 
            {
                ESP_LOGI(TAG, "back and left");
                set_motor_speed(motor_a_0, MOTOR_BACKWARD, MIN_PWM);
                set_motor_speed(motor_a_1, MOTOR_BACKWARD, MAX_PWM);
                read_comms()->back_left = false;
            } 
            else if(read_comms()->anticlockwise == true) 
            {
                ESP_LOGI(TAG, "anticlockwise");
                set_motor_speed(motor_a_0, MOTOR_BACKWARD, MIN_PWM);
                set_motor_speed(motor_a_1, MOTOR_FORWARD, MIN_PWM);
                read_comms()->anticlockwise = false;
            } 
            else if(read_comms()->clockwise == true) 
            {
                ESP_LOGI(TAG, "clockwise");
                set_motor_speed(motor_a_0, MOTOR_FORWARD, MIN_PWM);
                set_motor_speed(motor_a_1, MOTOR_BACKWARD, MIN_PWM);
                read_comms()->clockwise = false;
            } 
            else 
            {
                ESP_LOGI(TAG, "stop");
                set_motor_speed(motor_a_0, MOTOR_STOP, 0);
                set_motor_speed(motor_a_1, MOTOR_STOP, 0);
            }

            vTaskDelay(100 / portTICK_PERIOD_MS);
            set_motor_speed(motor_a_0, MOTOR_STOP, 0);
            set_motor_speed(motor_a_1, MOTOR_STOP, 0);
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