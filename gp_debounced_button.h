#ifndef _UNBOUNCEDBUTTON_H_
#define _UNBOUNCEDBUTTON_H_

#include <stdint.h>

typedef struct gp_debounced_button gp_debounced_button_t;

/// @brief initializes an unbounce button
/// @param  button to be initialized
/// @return 0 on success, otherwise use UnbouncedButton_GetLastError() to retreive errorcode
uint8_t gp_debounced_button_init(gp_debounced_button_t**);



#endif //_UNBOUNCEDBUTTON_H_
