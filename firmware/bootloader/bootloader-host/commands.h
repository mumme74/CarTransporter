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


#define TEMP_FAIL_RETRY( expression) { \
  int var;  \
  do { \
    var = (expression); \
  } while (var == -1 && errno == EINTR && running); \
}

typedef struct {
    uint32_t lowerbound;
    uint32_t upperbound;
} memoptions_t;


// gets set in main.c
extern int running;
extern int running;
extern int cansock;
extern uint32_t canIdx;
extern struct sockaddr_can addr;


void doReadCmd(memoptions_t *mopt, char *storeName);
void doPrintMemorySetupCmd(void);
void doChecksumCmd(memoptions_t *mopt);
void doChecksumLocalCmd(const char *binName);
void doCompareCmd(const char *binName);
void doResetCmd(void);
void doEraseCmd(memoptions_t *mopt);
void doWriteCmd(memoptions_t *mopt, char *binName);

void errExit(char *errStr)  __attribute__ ((noreturn));
void cleanExit(void) __attribute__ ((noreturn));
void cleanup(void);

#endif // COMMANDS_H
