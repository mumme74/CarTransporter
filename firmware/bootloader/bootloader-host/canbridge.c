/**
 * canbridge.c, this file provides abstraction between socketcan and slcan drivers
 *
 *
 *  Created on: 4 jul 2019
 *      Author: jof
 */


#include <string.h>
#include <stdio.h>
#include "canbridge.h"

static CAN_Drivers_t _driverId = CAN_driver_invalid;

#define MAXLEN_DRIVERNAME_STR 15
// available drivers as string name
// must be in the order they were entered in CAN_Drivers_t
static const char CAN_Drivers[_CAN_driver_end][MAXLEN_DRIVERNAME_STR] = {
    "invalid", "slcan", "socketcan"
};


int setupDriver(CAN_Drivers_t driverId)
{
    _driverId = driverId;

    switch (_driverId) {
    case CAN_driver_slcan:
        return 1; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        canbridge_send = &socketcan_send;
        canbridge_recv = &socketcan_recv;
        return 1;
#endif
    default:
        canbridge_send = NULL;
        canbridge_recv = NULL;
        return 0;
    }
}

// ------------------------------------------------------------------
// begin exported variables
char canbridge_errmsg[CANBRIDGE_ERRORMSG_SZ];



// -------------------------------------------------------------------
// begin exported functions

/**
 * @brief canbridge_get_driverId, gets the currently used driver
 * @return id of current driver
 */
CAN_Drivers_t canbridge_get_driverId(void)
{
    return _driverId;
}

/**
 * @brief canbridge_set_driverId, sets the currently used driver
 * @param driverId, use this driver in the future
 */
void canbridge_set_driverId(CAN_Drivers_t driverId)
{
    setupDriver(driverId);
}

/**
 * @brief canbridge_get_driverName
 * @return get the string name for current driver
 */
const char *canbridge_get_driverName(void)
{
    return CAN_Drivers[_driverId];
}

/**
 * @brief canbridge_get_driverNameForId
 * @param driverId get name for driverId
 * @return the name the driver or NULL
 */
const char *canbridge_get_driverNameForId(CAN_Drivers_t driverId)
{
    if (driverId < CAN_driver_invalid || driverId >= _CAN_driver_end)
        return NULL;
    return CAN_Drivers[driverId];
}

/**
 * @brief canbridge_get_driver_count, number of different drivers
 * @return count of all available drivers
 */
uint8_t canbridge_get_driver_count(void)
{
    return (uint8_t)_CAN_driver_end;
}

/**
 * @brief canbridge_set_driverFromName
 * @param name, driver to lookup
 * @return 1 on success, 0 = failure to find name
 */
int canbridge_set_driverFromName(const char *name)
{
    for(CAN_Drivers_t d = CAN_driver_invalid +1; d < _CAN_driver_end; ++d) {
        if (strncmp(name, CAN_Drivers[d], MAXLEN_DRIVERNAME_STR) == 0) {
            setupDriver(d);
            return 1;
        }
    }
    sprintf(canbridge_errmsg, "Driver '%s' not valid.\n", name);
    return 0; // not set
}

/**
 * @brief socketcan_init, initializes, bur not open, socketcan
 * @param name, Interface to use, ie can0
 * @return 0 on error, 1 when ok
 */
int canbridge_init(const char *idStr)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return 0; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        return socketcan_init(idStr);
#endif
    default:
        sprintf(canbridge_errmsg, "Invalid CAN driver\n");
        return 0;
    }
}

/**
 * @brief socketcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int canbridge_set_filter(uint32_t mask, uint32_t id)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return 0; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        return socketcan_set_filter(mask, id);
#endif
    default:
        sprintf(canbridge_errmsg, "Invalid CAN driver\n");
        return 0;
    }
}

/**
 * @brief socketcan_open, open a previously initialized
 * @return 0 on error, 1 when ok, -1 when already was opened
 */
int canbridge_open(void)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return 0; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        return socketcan_open();
#endif
    default:
        sprintf(canbridge_errmsg, "Invalid CAN driver\n");
        return 0;
    }
}

/**
 * @brief socketcan_status
 * @return CANBRIDGE_CLOSED, CANBRIDGE_INIT (but not open), CANBRIDGE_OPEN
 */
int canbridge_status(void)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return 0; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        return socketcan_status();
#endif
    default:
        return 0;
    }
}

/**
 * @brief socketcan_set_fd_mode, tries to set FD mode,
 *          not all CAN controllers support this faster mode
 * @return 0 on error, 1 when ok
 */
int canbridge_set_fd_mode(void)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return 0; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        return socketcan_set_fd_mode();
#endif
    default:
        sprintf(canbridge_errmsg, "Invalid CAN driver\n");
        return 0;
    }
}

/**
 * @brief socketcan_close
 * @return 0 on error, 1 when ok
 */
int canbridge_close(void)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return 0; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        return socketcan_close();
#endif
    default:
        sprintf(canbridge_errmsg, "Invalid CAN driver\n");
        return 0;
    }
}

/**
 * @brief socketcan_set_abortvariable, abort deep loop if variable is set
 *          socketcan re-tries to send message, program can get stuck
 * @param abortVar, a variable that can get set from a sighandler funtion
 *                  such as SIGINT
 */
void canbridge_set_abortvariable(int *abortVar)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        break; // FIXME
#ifdef __linux__
    case CAN_driver_socketcan:
        socketcan_set_abortvariable(abortVar);
        break;
#endif
    default:
        break;
    }
}

// function pointers, gets set by setupDriver();
int (*canbridge_send)(canframe_t *frm, int timeoutms) = NULL;

int (*canbridge_recv)(canframe_t *frm, int timeoutms) = NULL;


