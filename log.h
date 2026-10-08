// Name: Miriam Valenzuela
// Red ID: 131786119

#ifndef LOG_H
#define LOG_H

/*
 * Compilation notes:
 * C compilers:
 *    uses bool, must compile with -std=c99 or later (bool was introduced
 *    in the 1999 C standard.
 *
 * C++ compilers
 *    uses uint32_t, unsigned 32 bit integer type, introduced in C++11,
 *    The defaults in the g++ compiler on edoras should be fine with this
 */

/* C and C++ define some of their types in different places.
 * Check and see if we are using C or C++ and include appropriately
 * so that this will compile under C and C++
 */
#ifdef __cplusplus
/* C++ includes */
#include <stdint.h>
#else
/* C includes */
#include <inttypes.h>
#include <stdbool.h>
#endif 

/*
 * structure used for tracking execution stop reasons
 *
 * If compiled with a C compiler, make sure that the C99 dialect or later is used.
 * (-std=c99 with a GNU C compiler)
 */
typedef enum {
  DEPART_DELIVERY,
  CAP_REACHED, 
  SHIFT_COMPLETE,
} LegStopReasonType;

/**
 * @brief 
 * 
 * @param robotNumber 
 * @param chargedMinutes 
 * @param deliveryMinutes 
 * @param totalElapsedTime 
 * @param stopReason 
 */
void log_charge_leg (unsigned int robotNumber, 
                             unsigned int chargedMinutes,
                             unsigned int deliveryMinutes,
                             unsigned int totalElapsedTime,
                             LegStopReasonType stopReason);

/**
 * @brief 
 * 
 * @param legs - 1D array of integers
 */
void log_robot_legs (/*unsigned int robotNumber,*/ unsigned int legs[], size_t numOfLegs); 

/**
 * @brief log the estimated legs from exponential averaging
 * 
 * @param legs - 1D array of float numbers
 */
void log_robot_estimated_legs (unsigned int robotNumber, float legs[], size_t numOfLegs); 

/**
 * @brief 
 * 
 * @param robotNumber 
 * @param shiftTime 
 * @param totalQueueTime 
 */
void log_robot_completion (unsigned int robotNumber, 
                             unsigned int shiftTime,
                             // queue time is the time spent in the dock queue
                             // queue time = shiftTime - total charge legs - total delivery runs
                             unsigned int totalQueueTime);

#endif
