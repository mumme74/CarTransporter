/*
 * commands.c
 *
 *  Created on: 5 jul 2019
 *      Author: jof
 */
#ifndef SLCAN_H
#define SLCAN_H

#include <stdint.h>

#ifdef BUILD_SOCKETCAN
# include "socketcan.h" // for canframe_t
#else
# include "cantypes.h" // custom canframe_t
#endif


/**
 * @brief slcan_init, initializes, bur not open, slcan
 * @param name, Interface to use, ie can0
 * @return 0 on error, 1 when ok
 */
int slcan_init(const char *name);

/**
 * @brief slcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int slcan_set_filter(uint32_t mask, uint32_t id);

/**
 * @brief slcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int slcan_open(void);

/**
 * @brief slcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int slcan_status(void);

/**
 * @brief slcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int slcan_set_fd_mode(void);

/**
 * @brief slcan_close
 * @return 0 on error, 1 when ok
 */
int slcan_close(void);

/**
 * @brief slcan_set_abortvariable, abort deep loop if variable is set
 *          slcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void slcan_set_abortvariable(int *abortVar);

/**
 * @brief slcan_send, send on can
 * @param frm, frame to send
 * @param timeoutms, timeout if not send within
 * @return true on success
 */
int slcan_send(canframe_t *frm, int timeoutms);

/**
 * @brief slcan_recv, recive fromcan
 * @param frm, frame to fill when recv occurs
 * @param timeoutms, timeout if no recieved frm within timeout
 * @return true if we have a frm
 */
int slcan_recv(canframe_t *frm, int timeoutms);


#endif // SLCAN_H
