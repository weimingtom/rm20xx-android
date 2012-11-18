#ifndef WCPERFORM_H
#define WCPERFORM_H

#define WCPERFORM_SIZE 512
#define WCPERFORM_HEIGHT 80

/*
	Initialize performance
*/
extern void wcperform_init();

/*
	Finish performance measure
*/
extern void wcperform_exit();

/*
	Handler
*/
extern void wcperform_handle();

/*
	Get number of measures
*/
extern int wcperform_get_measure_count();

#endif