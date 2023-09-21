#ifndef _UNBOUNCEDBUTTON_H_
#define _UNBOUNCEDBUTTON_H_

#include <stdint.h>

typedef struct unbouncedButton Unbouncedbutton_t;

/// @brief initializes an unbounce button
/// @param  button to be initialized
/// @return 0 on success, otherwise use UnbouncedButton_GetLastError() to retreive errorcode
uint8_t Unbouncedbutton_Init(Unbouncedbutton_t**);



#endif //_UNBOUNCEDBUTTON_H_