#ifndef __GP_DEBOUNCED_BUTTON_H_
#define __GP_DEBOUNCED_BUTTON_H_

#define GP_DEBOUNCEDBUTTON_ERRORS_NOERROR               0
#define GP_DEBOUNCEDBUTTON_ERRORS_ALREADYINITIALIZED     1
#define GP_DEBOUNCEDBUTTON_ERRORS_NOTINITIALIZED        2
#define GO_DEBOUNCEDBUTTON_ERRORS_ALLOCFAILED           3

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct gp_debounced_button gp_debounced_button_t;

typedef enum push_types {
    SHORT = 0,
    LONG = 1,
    DOUBLE = 2,
    DOUBLE_LONG = 3
} gp_push_types_t;

typedef enum push_state {
    RELEASED = 0,
    PUSHED = 1
} gp_push_state_t;

/// @brief initializes an debounced button
/// @param  button to be initialized
/// @return 0 on success, otherwise use UnbouncedButton_GetLastError() to retreive errorcode
uint8_t gp_debounced_button_init(gp_debounced_button_t**);

/// @brief destroys button
/// @param button button which is to be destroyed
/// @return 0 on success, otherwise errorcode of occured error
uint8_t gp_debounced_button_destroy(gp_debounced_button_t**);

/// @brief adds an action taken on specifed push pattern, if an action is already assigned it will be overwritten
/// @param  module module on which the action is performed
/// @param  type pushtype where the action is added
/// @param  action action which is added
/// @return 0 on success, otherwise errorcode of occured error
uint8_t gp_debounced_button_add_action(gp_debounced_button_t*, gp_push_types_t, uint32_t, void(*)());

/// @brief removes an action from specifed push pattern
/// @param  module module on which the action is performed
/// @param  type pushtype action is removed from
/// @return 0 on success, otherwise errorcode of occured error
uint8_t gp_debounced_button_remove_action(gp_debounced_button_t*, gp_push_types_t);

/// @brief non blocking handler for a button
/// @param module on which the operation is performed 
/// @param state state of the button read by any means
/// @param  systemtime systemtime in milliseeconds
/// @return 0 on success, otherwise errorcode of occured error
uint8_t gp_debounced_button_handle(gp_debounced_button_t*, gp_push_state_t,  uint64_t);

#ifdef __cplusplus
}
#endif

#endif //__GP_DEBOUNCED_BUTTON_H_
