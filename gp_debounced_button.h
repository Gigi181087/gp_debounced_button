#ifndef __GP_DEBOUNCED_BUTTON_H_
#define __GP_DEBOUNCED_BUTTON_H_

#include <stdint.h>

typedef struct gp_debounced_button gp_debounced_button_t;

/// @brief initializes an unbounce button
/// @param  button to be initialized
/// @return 0 on success, otherwise use UnbouncedButton_GetLastError() to retreive errorcode
uint8_t gp_debounced_button_init(gp_debounced_button_t**);

/// @brief non blocking handler for a button
/// @param module on which the operation is performed 
/// @param  systemtime systemtime in milliseeconds
/// @return 0 on success, otherwise errorcode of occured error
uint8_t gp_debounced_button_handle(gp_debounced_button_t*, uint64_t);



#endif //__GP_DEBOUNCED_BUTTON_H_