#include <rtems.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

typedef struct {
    float x; float y; float z;
} Position3D;

rtems_task Sensor_Task(rtems_task_argument arg);
rtems_task Tracker_Task(rtems_task_argument arg);
rtems_task Actuator_Task(rtems_task_argument arg);

rtems_id sensor_id, tracker_id, actuator_id;
rtems_id sensor_queue_id, actuator_queue_id;

rtems_task Init(rtems_task_argument arg) {
    printf("\n==================================================\n");
    printf("   Advanced AI-Agent: Adaptive Tracking Engine    \n");
    printf("==================================================\n");

    rtems_message_queue_create(rtems_build_name('S', 'N', 'S', 'Q'), 10, sizeof(Position3D), RTEMS_DEFAULT_ATTRIBUTES, &sensor_queue_id);
    rtems_message_queue_create(rtems_build_name('A', 'C', 'T', 'Q'), 10, sizeof(Position3D), RTEMS_DEFAULT_ATTRIBUTES, &actuator_queue_id);

    rtems_task_create(rtems_build_name('S', 'E', 'N', 'S'), 1, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &sensor_id);
    rtems_task_create(rtems_build_name('T', 'R', 'A', 'K'), 1, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &tracker_id);
    rtems_task_create(rtems_build_name('A', 'C', 'T', 'U'), 1, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &actuator_id);

    rtems_task_start(sensor_id, Sensor_Task, 1);
    rtems_task_start(tracker_id, Tracker_Task, 2);
    rtems_task_start(actuator_id, Actuator_Task, 3);

    rtems_task_exit();
}

rtems_task Sensor_Task(rtems_task_argument arg) {
    float time = 0.0f;
    Position3D true_pos;
    Position3D noisy_pos;

    while(1) {
        /* Non-Linear Zig-Zag Path using Sine Wave */
        true_pos.x = time * 10.0f;
        true_pos.y = sinf(time * 0.5f) * 50.0f; 
        true_pos.z = 50.0f + cosf(time * 0.2f) * 20.0f;

        /* Adding noise */
        noisy_pos.x = true_pos.x + (((float)rand()/(float)RAND_MAX) * 2.0f - 1.0f);
        noisy_pos.y = true_pos.y + (((float)rand()/(float)RAND_MAX) * 2.0f - 1.0f);
        noisy_pos.z = true_pos.z + (((float)rand()/(float)RAND_MAX) * 2.0f - 1.0f);

        printf("[Sensor]   Drone Path: (X:%.1f, Y:%.1f, Z:%.1f)\n", noisy_pos.x, noisy_pos.y, noisy_pos.z);
        rtems_message_queue_send(sensor_queue_id, &noisy_pos, sizeof(Position3D));
        
        time += 0.5f;
        rtems_task_wake_after(rtems_clock_get_ticks_per_second() * 1);
    }
}

rtems_task Tracker_Task(rtems_task_argument arg) {
    Position3D measurement;
    Position3D estimated = {0.0, 0.0, 50.0};
    size_t size;
    
    while(1) {
        rtems_message_queue_receive(sensor_queue_id, &measurement, &size, RTEMS_WAIT, RTEMS_NO_TIMEOUT);

        /* Simplified Kalman Update */
        estimated.x += 0.7f * (measurement.x - estimated.x);
        estimated.y += 0.7f * (measurement.y - estimated.y);
        estimated.z += 0.7f * (measurement.z - estimated.z);

        float error_dist = sqrtf(powf(measurement.x - estimated.x, 2) + powf(measurement.y - estimated.y, 2));

        /* ANOMALY DETECTION LOGIC */
        if (error_dist > 5.0f) {
            printf("[Tracker]  !!! WARNING: HIGH VARIANCE / EVASIVE MANEUVER DETECTED !!!\n");
        }

        printf("[Tracker]  Tracking -> Est: (X:%.1f, Y:%.1f) | Error: %.2f\n", estimated.x, estimated.y, error_dist);
        rtems_message_queue_send(actuator_queue_id, &estimated, sizeof(Position3D));
    }
}

rtems_task Actuator_Task(rtems_task_argument arg) {
    Position3D data; size_t size;
    while(1) {
        rtems_message_queue_receive(actuator_queue_id, &data, &size, RTEMS_WAIT, RTEMS_NO_TIMEOUT);
        printf("[Actuator] Maintaining Lock -> Azimuth: %.1f deg\n", atan2f(data.y, data.x) * 57.3f);
        printf("--------------------------------------------------\n");
    }
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_MAXIMUM_TASKS 4
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 2
#define CONFIGURE_MESSAGE_BUFFER_MEMORY (CONFIGURE_MESSAGE_BUFFERS_FOR_QUEUE(10, sizeof(Position3D)) + CONFIGURE_MESSAGE_BUFFERS_FOR_QUEUE(10, sizeof(Position3D)))
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT
#include <rtems/confdefs.h>
