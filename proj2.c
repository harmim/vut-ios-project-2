/**
 * @file proj3.c
 * @brief Child Care.
 * @author Dominik Harmim <xharmi00@stud.fit.vutbr.cz>
 * @date 21.4.2017
 * @version 1.0
 */


#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
#include "argument_processor.h"

#define EXPECTED_ARGUMENTS_COUNT 6 /// Expected number of program arguments.


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
 * @return True, if getting was seccessful, false otherwise.
 */
bool get_arguments(int argc, char *argv[], struct argument *arguments[])
{
	// inicialization of expected arguments structure
	struct expected_arguments expected_args;
	if ( ! init_expected_arguments(&expected_args, EXPECTED_ARGUMENTS_COUNT)) {
		perror(NULL);

		return false;
	}

	// setting expected arguments
	set_expected_arguments(&expected_args);
	// process arguments
	if ( ! process_arguments(argc, argv, arguments, &expected_args)) {
		fprintf(stderr, USAGE_STRING);

		return false;
	}

	// cleare expected arguments structure
	clear_expected_arguments(&expected_args);

	return true;
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
	// inicialization of input arguments structure
	struct argument *arguments;
	if ( ! init_arguments(&arguments, EXPECTED_ARGUMENTS_COUNT)) {
		perror(NULL);

		return EXIT_FAILURE;
	}
	// get arguments
	if ( ! get_arguments(argc, argv, &arguments)) {
		return EXIT_FAILURE;
	}

	// clear input arguments structure
	clear_arguments(&arguments);

	return EXIT_SUCCESS;
}
