/*
 * system.h
 *
 *  Created on: 27 jul 2018
 *      Author: jof
 */

// this file has system specific things
// should hide architecture dependant things
// we simple compile a different c file for another architecture
//   (set in Makefile)

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <stddef.h>
#include <stdint.h>
#include <can_protocol.h>

#define WAIT_BEFORE_APPBOOT 100



#ifdef __cplusplus
extern "C" {
#endif

#if defined(ARDUINO) && defined(DEBUG_PRINT)
  // for cfiles in teensy
  void print_str(const char *str);
  void endl();
  void print_uint(uint32_t vlu);
# define Sprt(...) Serial.print(__VA_ARGS__)
#else
# define print_str(buf)
# define print_uint(vlu)
# define endl()
# define Sprt(...)
#endif

extern const uint16_t canId;
extern volatile uint32_t globalMsSinceStartup;

extern const uint16_t pageSize;

void systemInit(void);
void systemDeinit(void);
void systickInit(void);
void systickShutdown(void);
void systemToApplication(void);
void systemReset(void) __attribute__((noreturn));

can_bootloaderErrs_e systemFlashErase(uint8_t *startAddr, uint8_t *endAddr);
can_bootloaderErrs_e systemFlashWritePage(uint16_t *memPageBuf,
                                          volatile uint16_t *addr);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_H_ */
