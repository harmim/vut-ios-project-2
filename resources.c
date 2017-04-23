/**
 * @file resources.c
 * @brief Resources implementation.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 22.4.2017
 */


#include <stdlib.h>
#include <sys/shm.h>
#include <sys/fcntl.h>
#include "resources.h"


sem_t *mutex,
	*child_queue,
	*adult_queue,
	*finished_barrier;

int *action_counter = NULL,
	action_counter_id = 0;

int *child_counter = NULL,
	child_counter_id = 0;

int *adult_counter = NULL,
	adult_counter_id = 0;

int *child_waiting = NULL,
	child_waiting_id = 0;

int *adult_waiting = NULL,
	adult_waiting_id = 0;

int *working_counter = NULL,
	working_counter_id = 0;

int *current_child_count = NULL,
	current_child_count_id = 0;

int *current_adult_count = NULL,
	current_adult_count_id = 0;


static bool set_semaphore(sem_t **sem, char *name, int value)
{
	if ((*sem = sem_open(name, O_CREAT | O_EXCL, 0666, value)) == SEM_FAILED) {
		return false;
	}

	return true;
}


static bool set_shm(int *id, int **mem, int value)
{
	if ((*id = shmget(IPC_PRIVATE, sizeof (int), IPC_CREAT | 0666)) == -1) {
		return false;
	}
	if ((*mem = (int *) shmat(*id, NULL, 0)) == NULL) {
		return false;
	}
	**mem = value;

	return true;
}


bool set_resources(int processes_count)
{
	if ( ! set_semaphore(&mutex, MUTEX_NAME, 1)) {
		return false;
	}
	if ( ! set_semaphore(&child_queue, CHILD_QUEUE_NAME, 0)) {
		return false;
	}
	if ( ! set_semaphore(&adult_queue, ADULT_QUEUE_NAME, 0)) {
		return false;
	}
	if ( ! set_semaphore(&finished_barrier, FINISHED_BARRIER, 0)) {
		return false;
	}

	if ( ! set_shm(&action_counter_id, &action_counter, 0)) {
		return false;
	}
	if ( ! set_shm(&child_counter_id, &child_counter, 0)) {
		return false;
	}
	if ( ! set_shm(&child_waiting_id, &child_waiting, 0)) {
		return false;
	}
	if ( ! set_shm(&adult_counter_id, &adult_counter, 0)) {
		return false;
	}
	if ( ! set_shm(&adult_waiting_id, &adult_waiting, 0)) {
		return false;
	}
	if ( ! set_shm(&working_counter_id, &working_counter, processes_count)) {
		return false;
	}
	if ( ! set_shm(&current_child_count_id, &current_child_count, 0)) {
		return false;
	}
	if ( ! set_shm(&current_adult_count_id, &current_adult_count, 0)) {
		return false;
	}

	return true;
}


static void clean_semaphore(sem_t *sem, char *name)
{
	sem_close(sem);
	sem_unlink(name);
}


static void clean_shm(int id)
{
	shmctl(id, IPC_RMID, NULL);
}


void clean_resources()
{
	clean_semaphore(mutex, MUTEX_NAME);
	clean_semaphore(child_queue, CHILD_QUEUE_NAME);
	clean_semaphore(adult_queue, ADULT_QUEUE_NAME);
	clean_semaphore(finished_barrier, FINISHED_BARRIER);

	clean_shm(action_counter_id);
	clean_shm(child_counter_id);
	clean_shm(child_waiting_id);
	clean_shm(adult_counter_id);
	clean_shm(adult_waiting_id);
	clean_shm(working_counter_id);
	clean_shm(current_child_count_id);
	clean_shm(current_adult_count_id);
}
