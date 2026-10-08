// Name: Miriam Valenzuela
// Red ID: 131786119

#include <stdio.h>
#include "log.h"

/* Handle C++ namespaces, ignore if compiled in C 
 * C++ usually uses this #define to declare the C++ standard.
 * It will not be defined if a C compiler is used.
 */
#ifdef __cplusplus
using namespace std;
#endif

/*
 * Data section - 
 */

/* Names of producer threads and request types */
const char *legStopReason[] = {"departs on delivery", "charge cap reached", "shift complete"};

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
                             LegStopReasonType stopReason) {

  // print according to this format
  // R0: charged minutes = 3, delivery minutes = 0, time elapsed = 3, departs on delivery
  printf("R%d: charged minutes = %d, delivery minutes = %d, time elapsed = %d, %s\n", 
         robotNumber, chargedMinutes, deliveryMinutes, totalElapsedTime,
         legStopReason[stopReason]);
}

/**
 * @brief 
 * 
 * @param legs - 1D array of integers
 */
void log_robot_legs (unsigned int legs[], size_t numOfLegs) {

  //printf("P%d: ", robotNumber);

  for (int i = 0; i < numOfLegs; i++) {
    // Print integers on one line.
    printf("%d ", legs[i]);
  }

  printf("\n");  
} 

/**
 * @brief log the estimated legs from exponential averaging
 * 
 * @param legs - 1D array of floating points
 */
void log_robot_estimated_legs (unsigned int robotNumber, float legs[], size_t numOfLegs) {

  printf("R%d: ", robotNumber);

  for (int i = 0; i < numOfLegs; i++) {
    // Print integers on one line.
    printf("%.2f ", legs[i]);
  }

  printf("\n");

  /* This is not really needed, but will be helpful for making sure that you
  * see output prior to a segmentation violation.  This is not usually a
  * good practice as we want to avoid ending the charge leg premaurely which
  * this will do, but it is a helpful technique.
  */
  // fflush(stdout);  
}

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
                             unsigned int totalQueueTime) {

  // print according to this format
  printf("R%d: shift time = %d, queue time = %d\n", 
         robotNumber, shiftTime, totalQueueTime);
}

