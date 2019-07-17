#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include <stdint.h>
#include <linux/can.h>

#include "types.h"

typedef struct can_frame canframe_t;
typedef struct can_filter can_filter_t;


/**
 * @brief socketcan_init, initializes, bur not open, socketcan
 * @param name, Interface to use, ie can0
 * @param speed, the bitrate to use
 * @return 0 on error, 1 when ok
 */
int socketcan_init(const char *name, CAN_Speeds_t speed);

/**
 * @brief socketcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int socketcan_set_filter(canid_t mask, canid_t id, int extended);

/**
 * @brief socketcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int socketcan_open(void);

/**
 * @brief socketcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int socketcan_status(void);

/**
 * @brief socketcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int socketcan_set_fd_mode(void);

/**
 * @brief socketcan_close
 * @return 0 on error, 1 when ok
 */
int socketcan_close(void);

/**
 * @brief socketcan_set_abortvariable, abort deep loop if variable is set
 *          socketcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void socketcan_set_abortvariable(int *abortVar);

/**
 * @brief socketcan_send, send on can
 * @param frm, frame to send
 * @param timeoutms, timeout if not send within
 * @return true on success
 */
int socketcan_send(canframe_t *frm, uint32_t timeoutms);

/**
 * @brief socketcan_recv, recive fromcan
 * @param frm, frame to fill when recv occurs
 * @param timeoutms, timeout if no recieved frm within timeout
 * @return true if we have a frm
 */
int socketcan_recv(canframe_t *frm, uint32_t timeoutms);


#endif // SOCKETCAN_H
