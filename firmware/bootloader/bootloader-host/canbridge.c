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

static CAN_Drivers_t _driverId = _CAN_driver_start_marker;

#define MAXLEN_DRIVERNAME_STR 15
// available drivers as string name
// must be in the order they were entered in CAN_Drivers_t
static const char CAN_Drivers[_CAN_driver_end_marker][MAXLEN_DRIVERNAME_STR] = {
    { "invalid" },
#ifdef BUILD_SOCKETCAN
    { "socketcan" },
#endif
    { "slcan" },
};

// available baudrates as string name
#define MAXLEN_SPEEDNAME_STR  15
static const char CAN_Speeds[_CAN_speed_end_marker][MAXLEN_SPEEDNAME_STR] = {
    { "invalid" },
    { "socketspeed" },
    { "10Kbit" },
    { "20Kbit" },
    { "50Kbit" },
    { "100Kbit" },
    { "125Kbit" },
    { "250Kbit" },
    { "800Kbit" },
    { "1Mbit" },
    { "invalid" },
};

int setupDriver(CAN_Drivers_t driverId)
{
    _driverId = driverId;

    switch (_driverId) {
    case CAN_driver_slcan:
        canbridge_send = &slcan_send;
        canbridge_recv = &slcan_recv;
        return 1; // FIXME
#ifdef BUILD_SOCKETCAN
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
CAN_Drivers_t canbridge_get_driver_id(void)
{
    return _driverId;
}

/**
 * @brief canbridge_set_driverId, sets the currently used driver
 * @param driverId, use this driver in the future
 */
void canbridge_set_driver_id(CAN_Drivers_t driverId)
{
    setupDriver(driverId);
}

/**
 * @brief canbridge_get_driverName
 * @return get the string name for current driver
 */
const char *canbridge_get_driver_name(void)
{
    return CAN_Drivers[_driverId];
}

/**
 * @brief canbridge_get_driverNameForId
 * @param driverId get name for driverId
 * @return the name the driver or NULL
 */
const char *canbridge_get_driver_name_for_id(CAN_Drivers_t driverId)
{
    if (driverId <= _CAN_driver_start_marker ||
        driverId >= _CAN_driver_end_marker)
    {
        return NULL;
    }
    return CAN_Drivers[driverId];
}

/**
 * @brief canbridge_get_driver_count, number of different drivers
 * @return count of all available drivers
 */
uint8_t canbridge_get_driver_count(void)
{
    return (uint8_t)_CAN_driver_end_marker;
}

/**
 * @brief canbridge_set_driver_from_name
 * @param name, driver to lookup
 * @return 1 on success, 0 = failure to find name
 */
int canbridge_set_driver_from_name(const char *name)
{
    for(CAN_Drivers_t d = _CAN_driver_start_marker +1; d < _CAN_driver_end_marker; ++d) {
        if (strncmp(name, CAN_Drivers[d], MAXLEN_DRIVERNAME_STR) == 0) {
            setupDriver(d);
            return 1;
        }
    }
    CANBRIDGE_SET_ERRMSG("Driver '%s' not valid driver\n", name);
    return 0; // not set
}

/**
 * @brief canbridge_get_speed_id_from_speed_name
 * @param name, get the ID for name
 * @return the id on success, _CAN_speed_start_marker on failure
 */
CAN_Speeds_t canbridge_get_speed_id_from_speed_name(const char *name)
{
    for(CAN_Speeds_t s = _CAN_speed_start_marker; s < _CAN_speed_end_marker; ++s) {
        if (strncmp(name, CAN_Speeds[s], MAXLEN_SPEEDNAME_STR) == 0)
            return s;
    }
    // return invalid
    return _CAN_speed_start_marker;
}

/**
 * @brief canbridge_get_speed_name_from_id
 * @param speedId, get the str name for speedId
 * @return the str name of speedId or NULL
 */
const char *canbridge_get_speed_name_from_id(CAN_Speeds_t speedId)
{
    if (speedId <= _CAN_speed_start_marker ||
        speedId >= _CAN_speed_end_marker)
    {
        return NULL;
    }
    return CAN_Speeds[speedId];
}

/**
 * @brief socketcan_init, initializes, bur not open, socketcan
 * @param name, Interface to use, ie can0
 * @param speed, the bitrate to use
 * @return 0 on error, 1 when ok
 */
int canbridge_init(const char *idStr, CAN_Speeds_t speed)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return slcan_init(idStr, speed); // FIXME
#ifdef BUILD_SOCKETCAN
    case CAN_driver_socketcan:
        return socketcan_init(idStr, speed);
#endif
    default:
        CANBRIDGE_SET_ERRMSG("Invalid CAN driver\n");
        return 0;
    }
}

/**
 * @brief socketcan_set_filter, sets a CAN filter
 * @param mask, use mask to filter against
 * @param id, use this can msg ID
 * @return 0 on error, 1 when ok
 */
int canbridge_set_filter(canid_t mask, canid_t id)
{
    switch (_driverId) {
    case CAN_driver_slcan:
        return slcan_set_filter(mask, id);
#ifdef BUILD_SOCKETCAN
    case CAN_driver_socketcan:
        return socketcan_set_filter(mask, id);
#endif
    default:
        CANBRIDGE_SET_ERRMSG("Invalid CAN driver\n");
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
        return slcan_open();
#ifdef BUILD_SOCKETCAN
    case CAN_driver_socketcan:
        return socketcan_open();
#endif
    default:
        CANBRIDGE_SET_ERRMSG("Invalid CAN driver\n");
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
        return slcan_status();
#ifdef BUILD_SOCKETCAN
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
        return slcan_set_fd_mode();
#ifdef BUILD_SOCKETCAN
    case CAN_driver_socketcan:
        return socketcan_set_fd_mode();
#endif
    default:
        CANBRIDGE_SET_ERRMSG("Invalid CAN driver\n");
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
        return slcan_close();
#ifdef BUILD_SOCKETCAN
    case CAN_driver_socketcan:
        return socketcan_close();
#endif
    default:
        CANBRIDGE_SET_ERRMSG("Invalid CAN driver\n");
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
        slcan_set_abortvariable(abortVar);
        break;
#ifdef BUILD_SOCKETCAN
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


