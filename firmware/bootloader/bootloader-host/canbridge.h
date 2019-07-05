/**
 * canbridge.h, this file provides abstraction between socketcan and slcan drivers
 *
 *
 *  Created on: 4 jul 2019
 *      Author: jof
 */

#ifndef CANBRIDGE_H
#define CANBRIDGE_H

// include drivers
#include "slcan.h" // lawcell protocol, over serialport

#ifdef BUILD_SOCKETCAN
# include "socketcan.h" // only available on linux
#else
# include "cantypes.h"
#endif // BUILD_SOCKETCAN


// global defines
#define CANBRIDGE_CLOSED 0
#define CANBRIDGE_INIT 1
#define CANBRIDGE_OPEN 2


/**
 * @brief CAN_Drivers_t, these are the available driver to choose from
 */
typedef enum {
    CAN_driver_invalid = 0,
#ifdef __linux__
    CAN_driver_socketcan = 1,
#endif
    CAN_driver_slcan = 2,
    // fill with more drivers here


    // end marker
    _CAN_driver_end // must be dead last in enum,
                    // used at stop marker in for loop
} CAN_Drivers_t;


#define CANBRIDGE_ERRORMSG_SZ 200
/**
 * @brief canbridge_errmsg, is set by last occured error in driver
 */
extern char canbridge_errmsg[CANBRIDGE_ERRORMSG_SZ];


/**
 * @brief canbridge_get_driverId, gets the currently used driver
 * @return id of current driver
 */
CAN_Drivers_t canbridge_get_driverId(void);

/**
 * @brief canbridge_set_driverId, sets the currently used driver
 * @param driverId, use this driver in the future
 */
void canbridge_set_driverId(CAN_Drivers_t driverId);

/**
 * @brief canbridge_get_driverName
 * @return get the string name for current driver
 */
const char *canbridge_get_driverName(void);

/**
 * @brief canbridge_get_driverNameForId
 * @param driverId get name for driverId
 * @return the name the driver or NULL
 */
const char *canbridge_get_driverNameForId(CAN_Drivers_t driverId);

/**
 * @brief canbridge_set_driverFromName
 * @param name, driver to lookup
 * @return 1 on success, 0 = failure to find name
 */
int canbridge_set_driverFromName(const char *name);

/**
 * @brief canbridge_get_driver_count, number of different drivers
 * @return count of all available drivers
 */
uint8_t canbridge_get_driver_count(void);

/**
 * @brief socketcan_init, initializes, bur not open, socketcan
 * @param name, Interface to use, ie can0
 * @return 0 on error, 1 when ok
 */
int canbridge_init(const char *idStr);

/**
 * @brief socketcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int canbridge_set_filter(canid_t mask, canid_t id);

/**
 * @brief socketcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int canbridge_open(void);

/**
 * @brief socketcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int canbridge_status(void);

/**
 * @brief socketcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int canbridge_set_fd_mode(void);

/**
 * @brief socketcan_close
 * @return 0 on error, 1 when ok
 */
int canbridge_close(void);

/**
 * @brief socketcan_set_abortvariable, abort deep loop if variable is set
 *          socketcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void canbridge_set_abortvariable(int *abortVar);

/**
 * @brief canbridge_send, send on can
 * @param frm, frame to send
 * @param timeoutms, timeout if not send within
 * @return 1 on success, 0 timeout, below 0 if socket errno is set
 */
extern int (*canbridge_send)(canframe_t *frm, int timeoutms);

/**
 * @brief canbridge_recv, recive fromcan
 * @param frm, frame to fill when recv occurs
 * @param timeoutms, timeout if no recieved frm within timeout
 * @return 1 if we have frm, 0 timeout, below 0 if socket errno is set
 */
extern int (*canbridge_recv)(canframe_t *frm, int timeoutms);



#endif // CANBRIDGE_H
