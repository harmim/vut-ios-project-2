/**
 * @file adult_generator.c
 * @brief Adult processes generator implementation.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 22.4.2017
 */


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "adult_generator.h"
#include "resources.h"


static void adult_process(int wait_time)
{
	sem_wait(mutex);
	(*adult_counter)++;
	(*current_adult_count)++;
	if (*child_waiting) {
		int n = *child_waiting;
		if (n > 3) {
			n = 3;
		}
		for (int i = 0; i < n; i++) {
			sem_post(child_queue);
			(*child_waiting)--;
			(*child_counter)++;
			(*current_child_count)++;
		}
	}

	printf(PRINT_FORMAT, ++(*action_counter), 'A', *adult_counter, "started");
	printf(PRINT_FORMAT, ++(*action_counter), 'A', *adult_counter, "enter");
	sem_post(mutex);


	if (wait_time > 0) {
		usleep((useconds_t) (random() % wait_time * 1000));
	}


	sem_wait(mutex);
	printf(PRINT_FORMAT, ++(*action_counter), 'A', *adult_counter, "trying to leave");

	if (*current_child_count <= 3 * ((*current_adult_count) - 1)) {
		printf(PRINT_FORMAT, ++(*action_counter), 'A', *adult_counter, "leave");
		(*current_adult_count)--;
		sem_post(mutex);
	} else {
		(*adult_waiting)++;
		int ca = *adult_counter, cc = *child_counter;
		printf(PRINT_FORMAT_WAITING, ++(*action_counter), 'A', ca, "waiting", ca, cc);
		sem_post(mutex);
		sem_wait(adult_queue);

		sem_wait(mutex);
		printf(PRINT_FORMAT, ++(*action_counter), 'A', *adult_counter, "leave");
		(*adult_waiting)--;
		(*current_adult_count)--;
		sem_post(mutex);
	}

	sem_wait(mutex);
	(*working_counter)--;
	sem_post(mutex);
	if (*working_counter == 0) {
		sem_post(finished_barrier);
	}
	sem_wait(finished_barrier);
	sem_post(finished_barrier);

	sem_wait(mutex);
	printf(PRINT_FORMAT, ++(*action_counter), 'A', *adult_counter, "finished");
	sem_post(mutex);

	exit(0);
}


pid_t adult_generator_start(int count, int generate_time, int wait_time)
{
	pid_t pid = fork();
	if (pid != 0) {
		return pid;
	}

	for (int i = 0; i < count; i++) {
		if (generate_time > 0) {
			usleep((useconds_t) (random() % generate_time * 1000));
		}

		pid_t adult_pid = fork();
		if (adult_pid < 0) {
			return adult_pid;

		} else if (adult_pid == 0) {
			adult_process(wait_time);
		}
	}

	while (wait(NULL)) {
		if (errno == ECHILD) {
			break;
		}
	}

	exit(0);
}
