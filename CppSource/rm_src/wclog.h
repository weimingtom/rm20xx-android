#ifndef WCLOG_H
#define WCLOG_H

/* Typedefs */
typedef void *elog;

/*
	Opens a new log.
*/
extern elog wclog_open(char *name);

/*
	Finishes and closes a log.
*/
extern void wclog_close(elog l);

/*
	Writes a string to a log.
*/
extern void wclog_write(elog l,char *str);

/*
	Line feeds a log.
*/
extern void wclog_enter(elog l);

/*
	Writes a number to a log.
*/
extern void wclog_write(elog l,int n);

#endif