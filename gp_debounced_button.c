#include "gp_debounced_button.h"
#include <malloc.h>

#define __assert_initialized(x)     do { if(x == NULL) { return GP_DEBOUNCEDBUTTON_ERRORS_NOTINITIALIZED; }} while(0)
#define __assertNotInitialized(x)   do { if(x != NULL) { return GP_DEBOUNCEDBUTTON_ERRORS_ALREADYINITIALIZED; }} while(0)
#define BOOL    uint8_t
#define TRUE    1
#define FALSE   0

struct flags {
    BOOL short_used : 1;
    BOOL long_used : 1;
    BOOL double_used : 1;
    BOOL double_long_used : 1;
    BOOL handled : 1;
    BOOL short_pushed : 1;
    gp_push_state_t last_button_state : 1;
};

struct gp_debounced_button {
    // times
    uint64_t time_stamp;
    uint32_t delay_time;
    uint32_t push_long_time;
    uint32_t debounce_time;

    // actions
    void(*short_action)();
    void(*long_action)();
    void(*double_action)();
    void(*double_long_action)();

    //flags
    struct flags flags;
};

uint8_t gp_debounced_button_init(gp_debounced_button_t** button_param) {
    __assertNotInitialized(button_param);


    if(*button_param = (gp_debounced_button_t*)malloc((sizeof(gp_debounced_button_t))) == NULL) {

        return GO_DEBOUNCEDBUTTON_ERRORS_ALLOCFAILED;
    }
    (*button_param)->time_stamp = 0;
    (*button_param)->delay_time = 500;
    (*button_param)->push_long_time = 1000;
    (*button_param)->debounce_time = 50;
    (*button_param)->short_action = NULL;
    (*button_param)->long_action = NULL;
    (*button_param)->double_action = NULL;
    (*button_param)->double_long_action = NULL;
    (*button_param)->flags.short_used = FALSE;
    (*button_param)->flags.long_used = FALSE;
    (*button_param)->flags.double_used = FALSE;
    (*button_param)->flags.double_long_used = FALSE;
    (*button_param)->flags.handled = TRUE;
    (*button_param)->flags.short_pushed = FALSE;
    (*button_param)->flags.last_button_state = RELEASED;

    return GP_DEBOUNCEDBUTTON_ERRORS_NOERROR;
}

uint8_t gp_debounced_button_destroy(gp_debounced_button_t** button_param) {
    __assert_initialized(*button_param);


    free(*button_param);

    return 0;
}

uint8_t gp_debounced_button_add_action(gp_debounced_button_t* button_param, gp_debounced_button_push_types_t type_param, void(*action_param)()) {
    __assert_initialized(button_param);


    switch (type_param) {

        case SHORT:
            button_param->flags.short_used = TRUE;
            button_param->short_action = action_param;
            
            break;

        case LONG:
            button_param->flags.long_used = TRUE;
            button_param->long_action = action_param;
            
            break;

        case DOUBLE:
            button_param->flags.double_used = TRUE;
            button_param->double_action = action_param;
            
            break;

        case DOUBLE_LONG:
            button_param->flags.double_long_used = TRUE;
            button_param->double_long_action = action_param;
            
            break;

        default:
            
            return GP_DEBOUNCEDBUTTON_ERRORS_PARAMETER;
    }

    return GP_DEBOUNCEDBUTTON_ERRORS_NOERROR;
}

uint8_t gp_debounced_button_remove_action(gp_debounced_button_t* button_param, gp_debounced_button_push_types_t type_param) {
    __assert_initialized(button_param);


    switch (type_param) {

        case SHORT:
            button_param->flags.short_used = FALSE;
            button_param->short_action = NULL;
            
            break;

        case LONG:
            button_param->flags.long_used = FALSE;
            button_param->long_action = NULL;
            
            break;

        case DOUBLE:
            button_param->flags.double_used = FALSE;
            button_param->double_action = NULL;
            
            break;

        case DOUBLE_LONG:
            button_param->flags.double_long_used = FALSE;
            button_param->double_long_action = NULL;
            
            break;

        default:
            
            return GP_DEBOUNCEDBUTTON_ERRORS_PARAMETER;
    }

    return GP_DEBOUNCEDBUTTON_ERRORS_NOERROR;
}

uint8_t gp_debounced_button_set_setting(gp_debounced_button_t* button_param, gp_debounced_button_settings_t setting_param, uint32_t value_param) {
    __assert_initialized(button_param);


    switch (setting_param) {

        case DOUBLE_PUSH_DELAY_TIME:
            button_param->delay_time = value_param;

            break;

        case PUSH_LONG_TIME:
            button_param->push_long_time = value_param;

            break;

        case DEBOUNCE_TIME:
            button_param->debounce_time = value_param;

            break;

        default:

            return GP_DEBOUNCEDBUTTON_ERRORS_PARAMETER;
    }

    return GP_DEBOUNCEDBUTTON_ERRORS_NOERROR;
}


uint8_t gp_debounced_button_handle(gp_debounced_button_t* button_param, gp_push_state_t push_state_param, uint64_t system_time_param) {
    __assert_initialized(button_param);

    
    if (push_state_param != button_param->flags.last_button_state) {

        if (push_state_param == RELEASED) {

            if (button_param->flags.handled = FALSE) {

                if (system_time_param - button_param->time_stamp >= button_param->push_long_time) {

                    if (button_param->flags.short_pushed == TRUE) {
                        button_param->double_long_action();
                        button_param->flags.short_pushed = FALSE;

                    } else {
                        button_param->long_action();
                    }

                } else if (system_time_param - button_param->time_stamp >= button_param->debounce_time) {

                    if (button_param->flags.short_pushed == TRUE) {
                        button_param->double_action();
                        button_param->flags.short_pushed = FALSE;

                    } else {

                        if (button_param->flags.double_used == TRUE) {
                            button_param->flags.short_pushed = TRUE;

                        } else {
                            button_param->short_action();
                        }
                    }


                }
                button_param->flags.handled = TRUE;
            }

        } else {
            button_param->flags.handled = FALSE;
        }
        button_param->time_stamp = system_time_param;

    } else {

        if (push_state_param == RELEASED) {

            if (button_param->flags.short_pushed == TRUE) {

                if (system_time_param - button_param->time_stamp >= button_param->delay_time) {
                    button_param->flags.short_pushed = TRUE;
                }
            }

        } else {

            if (button_param->flags.handled == FALSE) {

                if (button_param->flags.long_used || button_param->flags.double_long_used) {

                    if ((system_time_param - button_param->time_stamp) >= button_param->push_long_time) {

                        if (button_param->flags.short_pushed == TRUE) {
                            button_param->double_long_action();
                            button_param->flags.short_pushed = FALSE;

                        } else {
                            button_param->long_action();
                        }
                    button_param->flags.handled = TRUE;
                    }

                } else {

                    if ((system_time_param - button_param->time_stamp) >= button_param->debounce_time) {

                        if (button_param->flags.short_pushed = TRUE) {
                            button_param->double_action();
                            button_param->flags.short_pushed = FALSE;

                        } else {

                            if (button_param->flags.double_used == TRUE) {
                                button_param->flags.short_pushed = TRUE;

                            } else {
                                button_param->short_action();
                            }
                        }
                        button_param->flags.handled = TRUE;
                    }
                }
            }
        }
    }

    return GO_DEBOUNCEDBUTTON_ERRORS_ALLOCFAILED;
}
