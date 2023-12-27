#include "gp_debounced_button.h"
#include <malloc.h>


#define UNBOUNCEDBUTTON_ERROR_NOERROR                           0
#define UNBOUNCEDBUTTON_ERROR_ALREADYINITIALIZED                1
#define UNBOUNCEDBUTTON_ERROR_NOTINITIALIZED                    2
#define UNBOUNCEDBUTTON_ERROR_ALLOCFAILED                       3

#define __assertInitialized(x)     do { if(x == NULL) { return UNBOUNCEDBUTTON_ERROR_NOTINITIALIZED; }} while(0)
#define __assertNotInitialized(x)   do { if(x != NULL) { return UNBOUNCEDBUTTON_ERROR_ALREADYINITIALIZED; }} while(0)

struct flags {
    uint8_t longReset : 1;
    uint8_t shortReset : 1;
    uint8_t longPush : 1;
    uint8_t shortPush : 1;
};

struct actions {
    uint8_t used;
    uint32_t time_pushed;
    void(*action)();
    uint8_t reset;
};

struct gp_debounced_button {
    // times
    gp_push_state_t last_push_state;
    uint64_t time_pushed;
    uint64_t time_released;

    struct flags flags;
    struct actions actions[4];
};



uint8_t gp_debounced_button_init(gp_debounced_button_t** button_param) {
    __assertNotInitialized(button_param);


    if(*button_param = (gp_debounced_button_t*)malloc((sizeof(gp_debounced_button_t))) == NULL) {

        return UNBOUNCEDBUTTON_ERROR_ALLOCFAILED;
    };
    (*button_param)->time_released = 0;
    (*button_param)->time_pushed = 0;

    for (uint8_t i = 0; i < 4; i++) {
        (*button_param)->actions[i].used = 0;
        (*button_param)->actions[i].time_pushed = 0;
        (*button_param)->actions[i].reset = 1;
        (*button_param)->actions[i].action = NULL;
    }

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}

uint8_t gp_debounced_button_destroy(gp_debounced_button_t** button_param) {
    __assertInitialized(*button_param);


    free(*button_param);

    return 0;
}

uint8_t gp_debounced_button_add_action(gp_debounced_button_t* button_param, gp_push_types_t type_param, uint32_t push_time_param, void(*action_param)()) {
    __assertInitialized(button_param);


    button_param->actions[(uint8_t)type_param].used = 1;
    button_param->actions[(uint8_t)type_param].time_pushed = push_time_param;
    button_param->actions[(uint8_t)type_param].action = action_param;
    button_param->actions[(uint8_t)type_param].reset = 1;

    return GP_DEBOUNCEDBUTTON_ERRORS_NOERROR;
}

uint8_t gp_debounced_button_remove_action(gp_debounced_button_t* button_param, gp_push_types_t type_param) {
    __assertInitialized(button_param);


    button_param->actions[(uint8_t)type_param].used = 0;
    button_param->actions[(uint8_t)type_param].time_pushed = 0;
    button_param->actions[(uint8_t)type_param].action = NULL;

    return GP_DEBOUNCEDBUTTON_ERRORS_NOERROR;
}

/// @brief 
/// @param button_param 
/// @param push_state_param 
/// @param systemTimeParam 
/// @return 
uint8_t gp_debounced_button_handle(gp_debounced_button_t* button_param, gp_push_state_t push_state_param, uint64_t systemTimeParam) {
    __assertInitialized(button_param);

    if (push_state_param == PUSHED) {

        if (button_param->last_push_state != push_state_param) {

            
        }

    } else {

        if (button_param->last_push_state == PUSHED) {
            

        }
    }

    /*if(stateParam == button_param->statePushed) {
        
        if(button_param->flags.longPush) {

            if(button_param->flags.longReset) {

                if((systemTimeParam - button_param->lastMillis) > button_param->time_long_push) {
                    button_param->flags.longReset = 0;
                    button_param->flags.shortReset = 0;

                    button_param->button_pushed_long();
                }
            }

        } else if(button_param->flags.shortPush) {

            if(systemTimeParam - button_param->lastMillis > button_param->unbounceTime) {

                if(button_param->flags.shortReset) {
                    button_param->flags.shortReset = 0;

                    button_param->button_pushed_short();
                }
            }
        }

    } else {

        if(button_param->flags.shortPush) {
            
            if(button_param->flags.shortReset) {

                if(systemTimeParam - button_param->lastMillis > button_param->unbounceTime) {
                    button_param->button_pushed_short();
                }
            }
        }

        button_param->flags.shortReset = 1;
        button_param->flags.longReset = 1;
        button_param->lastMillis = systemTimeParam;
    }*/

    button_param->last_push_state = push_state_param;

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}
