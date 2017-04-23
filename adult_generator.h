/**
 * @file adult_generator.h
 * @brief Adult processes generator interface.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 22.4.2017
 */


#ifndef PROJ2_ADULT_GENERATOR_H
#define PROJ2_ADULT_GENERATOR_H


#include <semaphore.h>


pid_t adult_generator_start(int count, int generate_time, int wait_time);


#endif //PROJ2_ADULT_GENERATOR_H
