/**
 * @file child_generator.h
 * @brief Child processes generator interface.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 22.4.2017
 */

#ifndef PROJ2_CHILD_GENERATOR_H
#define PROJ2_CHILD_GENERATOR_H


#include <semaphore.h>


pid_t child_generator_start(int count, int generate_time, int wait_time, int adult_count);


#endif //PROJ2_CHILD_GENERATOR_H
