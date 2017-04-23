/**
 * @file resources.h
 * @brief Resources interface.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 22.4.2017
 */


#ifndef PROJ2_RESOURCES_H
#define PROJ2_RESOURCES_H


#include <stdbool.h>
#include <semaphore.h>
#include <stdio.h>

#define MUTEX_NAME "/xharmi00-ios2-mutex"
#define CHILD_QUEUE_NAME "/xharmi00-ios2-childq"
#define ADULT_QUEUE_NAME "/xharmi00-ios2-adultq"
#define FINISHED_BARRIER "/xharmi00-ios2-fbarr"
#define PRINT_FORMAT "%-6i : %c %-4i : %s\n"
#define PRINT_FORMAT_WAITING "%-6i : %c %-4i : %s : %i : %i\n"
#define OUTPUT_FILE_NAME "proj2.out"


extern sem_t *mutex,
		*child_queue,
		*adult_queue,
		*finished_barrier;

extern int *action_counter,
			action_counter_id;

extern int *child_counter,
			child_counter_id;

extern int *adult_counter,
			adult_counter_id;

extern int *child_waiting,
			child_waiting_id;

extern int *adult_waiting,
			adult_waiting_id;

extern int *current_child_count,
			current_child_count_id;

extern int *current_adult_count,
			current_adult_count_id;

extern int *working_counter,
			working_counter_id;

extern FILE *output_file;
extern int output_file_id;


bool set_resources(int processes_count);


void clean_resources();


#endif //PROJ2_RESOURCES_H
