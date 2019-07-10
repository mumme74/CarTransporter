/*
 * commands.c
 *
 *  Created on: 29 jul 2018
 *      Author: jof
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef struct {
    uint32_t lowerbound;
    uint32_t upperbound;
} memoptions_t;


// gets set in main.c
extern int abortVar;
extern int abortVar;
extern uint32_t canIdx;


void cmdInit(void);
void doReadCmd(memoptions_t *mopt, char *storeName);
void doPrintMemorySetupCmd(void);
void doChecksumCmd(memoptions_t *mopt);
void doChecksumLocalCmd(const char *binName);
void doCompareCmd(const char *binName);
void doResetCmd(void);
void doEraseCmd(memoptions_t *mopt);
void doWriteCmd(memoptions_t *mopt, char *binName);
void doBootloaderModeCmd(void);

void printCanError(void);
void cleanup(void);


#ifdef _WIN32
__declspec(noreturn) void errExit(char *errStr);
__declspec(noreturn) void cleanExit(void);
#else
void errExit(char *errStr)  __attribute__ ((noreturn));
void cleanExit(void) __attribute__ ((noreturn));
#endif

#endif // COMMANDS_H
