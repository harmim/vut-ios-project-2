/**
 * @file child_generator.c
 * @brief Child processes generator implementation.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 22.4.2017
 */


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "child_generator.h"
#include "resources.h"


static void child_process(int wait_time, int adult_count)
{
	sem_wait(mutex);
	printf(PRINT_FORMAT, ++(*action_counter), 'C', *child_counter + 1, "started");

	if (*current_child_count < 3 * (*current_adult_count) || adult_count == *adult_counter) {
		(*child_counter)++;
		(*current_child_count)++;
		printf(PRINT_FORMAT, ++(*action_counter), 'C', *child_counter, "enter");
		sem_post(mutex);
	} else {
		(*child_waiting)++;
		int ca = *adult_counter, cc = *child_counter + 1;
		printf(PRINT_FORMAT_WAITING, ++(*action_counter), 'C', cc, "waiting", ca, cc);
		sem_post(mutex);
		sem_wait(child_queue);

		sem_wait(mutex);
		printf(PRINT_FORMAT, ++(*action_counter), 'C', *child_counter, "enter");
		sem_post(mutex);
	}


	if (wait_time > 0) {
		usleep((useconds_t) (random() % wait_time * 1000));
	}


	sem_wait(mutex);
	printf(PRINT_FORMAT, ++(*action_counter), 'C', *child_counter, "trying to leave");
	printf(PRINT_FORMAT, ++(*action_counter), 'C', *child_counter, "leave");

	(*current_child_count)--;
	if (*adult_waiting && *current_child_count <= 3 * ((*current_adult_count) - 1)) {
		sem_post(adult_queue);
	}
	sem_post(mutex);


	sem_wait(mutex);
	(*working_counter)--;
	sem_post(mutex);
	if (*working_counter == 0) {
		sem_post(finished_barrier);
	}
	sem_wait(finished_barrier);
	sem_post(finished_barrier);

	sem_wait(mutex);
	printf(PRINT_FORMAT, ++(*action_counter), 'C', *child_counter, "finished");
	sem_post(mutex);

	exit(0);
}


pid_t child_generator_start(int count, int generate_time, int wait_time, int adult_count)
{
	pid_t pid = fork();
	if (pid != 0) {
		return pid;
	}

	for (int i = 0; i < count; i++) {
		if (generate_time > 0) {
			usleep((useconds_t) (random() % generate_time * 1000));
		}

		pid_t child_pid = fork();
		if (child_pid < 0) {
			return child_pid;

		} else if (child_pid == 0) {
			child_process(wait_time, adult_count);
		}
	}

	while (wait(NULL)) {
		if (errno == ECHILD) {
			break;
		}
	}

	exit(0);
}
