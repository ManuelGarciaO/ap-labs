#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <syslog.h>
#include "logger.h"

#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

int log_stdout = 1;

int initLogger(char *logType) {
    printf("Initializing Logger on: %s\n", logType);
    if(!strcmp(logType, "syslog")) {
        log_stdout = 0;
    } 
    return 0;
}

void textcolor(int attr, int fg, int bg)
{	char command[13];

	/* Command is the control command to the terminal */
	sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
	printf("%s", command);
}

int infof(const char *format, ...) {
    textcolor(BRIGHT, GREEN, BLACK);
    va_list arg;
	va_start(arg, format);
    if(log_stdout==1) {
	    vprintf(format, arg);
    } 
    else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_INFO, format, arg);
        closelog();
    }
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int warnf(const char *format, ...) {
    textcolor(BRIGHT, YELLOW, BLACK);
    va_list arg;
	va_start(arg, format);
    if(log_stdout==1) {
	    vprintf(format, arg);
    } 
    else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_WARNING, format, arg);
        closelog();
    }
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

int errorf(const char *format, ...) {
    textcolor(BRIGHT, MAGENTA, BLACK);
    va_list arg;
	va_start(arg, format);
    if(log_stdout==1) {
	    vprintf(format, arg);
    } 
    else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_ERR, format, arg);
        closelog();
    }
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}
int panicf(const char *format, ...) {
    textcolor(BRIGHT, RED, BLACK);
    va_list arg;
	va_start(arg, format);
    if(log_stdout==1) {
	    vprintf(format, arg);
    } 
    else {
        openlog("SYSLOG", 0, 0);
        vsyslog(LOG_EMERG, format, arg);
        closelog();
    }
    va_end(arg);
    textcolor(RESET, WHITE, BLACK);
    return 0;
}

