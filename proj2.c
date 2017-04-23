/**
 * @file proj3.c
 * @brief Child Care.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 21.4.2017
 * @version 1.0
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include "argument_processor.h"
#include "resources.h"
#include "adult_generator.h"
#include "child_generator.h"

#define EXPECTED_ARGUMENTS_COUNT 6 /// Expected number of program arguments.
#define EXIT_FAILURE_SYS_CALL 2 /// System call exit code.
#define SYS_CALL_ERROR_MESSAGE "System call error"


/// Usage string (help).
const char USAGE_STRING[] =
		"usage:\n"
		"    proj2 A C AGT CGT AWT CWT\n"
		"        - A is number of adult process; A > 0.\n"
		"        - C is number of child process; C > 0.\n"
		"        - AGT max time after which a new adult process is generated; AGT >= 0 && AGT < 5001.\n"
		"        - CGT max time after which a new child process is generated; CGT >= 0 && CGT < 5001.\n"
		"        - AWT max time for which the adult process simulates activity in the center; AWT >= 0 && AWT < 5001.\n"
		"        - CWT max time for which the child process simulates activity in the center; CWT >= 0 && CWT < 5001.\n"
		"        - Time is in milliseconds.\n"
		"        - All parameters are integers.\n"
		"\n";


/**
 * Condition callback for testing number of processes (arguments: A and C).
 *
 * @param arg Argument to check.
 * @return True if argument is valid, false otherwise.
 */
static bool number_of_processes_condition(struct argument *arg)
{
	return arg->value.i_value > 0;
}


/**
 * Condition callback for testing max delay for processes (arguments: AGT, CGT, AWT and CWT).
 *
 * @param arg Argument to check.
 * @return True if argument is valid, false otherwise.
 */
static bool max_delay_for_process_condition(struct argument *arg)
{
	return arg->value.i_value >= 0 && arg->value.i_value < 5001;
}


/**
 * Set expected program arguments.
 *
 * @param expected Output structure of expected arguments.
 */
void set_expected_arguments(struct expected_arguments *expected)
{
	struct expected_argument *arg;

	// A
	arg = &expected->args[0];
	arg->type = ARG_TYPE_INTEGER;
	arg->condition = &number_of_processes_condition;

	// C
	arg = &expected->args[1];
	arg->type = ARG_TYPE_INTEGER;
	arg->condition = &number_of_processes_condition;

	// AGT
	arg = &expected->args[2];
	arg->type = ARG_TYPE_INTEGER;
	arg->condition = &max_delay_for_process_condition;

	// CGT
	arg = &expected->args[3];
	arg->type = ARG_TYPE_INTEGER;
	arg->condition = &max_delay_for_process_condition;

	// AWT
	arg = &expected->args[4];
	arg->type = ARG_TYPE_INTEGER;
	arg->condition = &max_delay_for_process_condition;

	// CWT
	arg = &expected->args[5];
	arg->type = ARG_TYPE_INTEGER;
	arg->condition = &max_delay_for_process_condition;
}


/**
 * Get program arguments.
 *
 * @param argc Number of arguments.
 * @param argv Argument values.
 * @param arguments Output arguments sctructure.
 * @return 0 if getting was seccessful, error code otherwise.
 */
int get_arguments(int argc, char *argv[], struct argument *arguments[])
{
	// inicialization of expected arguments structure
	struct expected_arguments expected_args;
	if ( ! init_expected_arguments(&expected_args, EXPECTED_ARGUMENTS_COUNT)) {
		perror(SYS_CALL_ERROR_MESSAGE);

		return EXIT_FAILURE_SYS_CALL;
	}

	// setting expected arguments
	set_expected_arguments(&expected_args);
	// process arguments
	if ( ! process_arguments(argc, argv, arguments, &expected_args)) {
		fprintf(stderr, USAGE_STRING);
		// cleare expected arguments structure
		clear_expected_arguments(&expected_args);

		return EXIT_FAILURE;
	}

	// cleare expected arguments structure
	clear_expected_arguments(&expected_args);

	return EXIT_SUCCESS;
}


/**
 * Main function.
 *
 * @param argc Number of arguments.
 * @param argv Argument values.
 * @return EXIT_SUCCESS, if run was seccessful, EXIT_FAILURE otherwise.
 */
int main(int argc, char *argv[])
{
	/****************** Processing arguments ******************/
	// inicialization of input arguments structure
	struct argument *arguments;
	if ( ! init_arguments(&arguments, EXPECTED_ARGUMENTS_COUNT)) {
		perror(SYS_CALL_ERROR_MESSAGE);

		return EXIT_FAILURE_SYS_CALL;
	}
	// get arguments
	int error_code = get_arguments(argc, argv, &arguments);
	if (error_code) {
		clear_arguments(&arguments); // clear input arguments structure

		return error_code;
	}
	int arg_a = arguments[0].value.i_value,
		arg_c = arguments[1].value.i_value,
		arg_agt = arguments[2].value.i_value,
		arg_cgt = arguments[3].value.i_value,
		arg_awt = arguments[4].value.i_value,
		arg_cwt = arguments[5].value.i_value;
	clear_arguments(&arguments); // clear input arguments structure


	/****************** Inicialize ******************/
	// disable output buffering
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	srandom((unsigned) time(NULL)); // initialize random number generator
	if ( ! set_resources(arg_a + arg_c)) {
		perror(SYS_CALL_ERROR_MESSAGE);
		clean_resources();

		return EXIT_FAILURE_SYS_CALL;
	}


	/****************** Process generating ******************/
	pid_t adult_pid, child_pid = -1;
	if (
		(adult_pid = adult_generator_start(arg_a, arg_agt, arg_awt)) < 0
		|| (child_pid = child_generator_start(arg_c, arg_cgt, arg_cwt, arg_a)) < 0
	) {
		perror(SYS_CALL_ERROR_MESSAGE);
		kill(adult_pid, SIGTERM);
		if (child_pid != -1) {
			kill(child_pid, SIGTERM);
		}
		clean_resources();

		return EXIT_FAILURE_SYS_CALL;
	}


	/****************** Waiting for exit processes ******************/
	waitpid(adult_pid, NULL, 0);
	waitpid(child_pid, NULL, 0);


	/****************** Clear resources ******************/
	clean_resources();


	return EXIT_SUCCESS;
}
