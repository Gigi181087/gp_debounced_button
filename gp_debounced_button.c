#include "unbouncedButton.h"
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

struct unbouncedButton {
    // times
    uint64_t lastMillis;
    uint16_t unbounceTime;
    uint32_t longPushTime;

    uint8_t statePushed;

    void (*buttonPushedShort)();
    void (*buttonPushedLong)();

    struct flags flags;
};

uint8_t Unbouncedbutton_Init(Unbouncedbutton_t** buttonParam, uint8_t statePushedParam) {
    __assertNotInitialized(buttonParam);

    if(*buttonParam = (Unbouncedbutton_t*)(sizeof(Unbouncedbutton_t)) == NULL) {

        return UNBOUNCEDBUTTON_ERROR_ALLOCFAILED;
    };

    (*buttonParam)->lastMillis = 0;
    (*buttonParam)->statePushed = statePushedParam;

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}

uint8_t UnbouncedButton_AddActionToShortPush(Unbouncedbutton_t* buttonParam, uint32_t millisecondsParam, void (*actionParam)()) {
    __assertInitialized(buttonParam);

    buttonParam->unbounceTime = millisecondsParam;
    buttonParam->buttonPushedShort = actionParam;
    buttonParam->flags.shortPush = 1;

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}

uint8_t UnbouncedButton_RemoveActionFromShortPush(Unbouncedbutton_t* buttonParam) {
    __assertInitialized(buttonParam);

    buttonParam->unbounceTime = 0;
    buttonParam->buttonPushedShort = NULL;
    buttonParam->flags.shortPush = 0;

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}

uint8_t UnbouncedButton_AddActionToLongPush(Unbouncedbutton_t* buttonParam, uint32_t millisecondsParam, void (*actionParam)()) {
    __assertInitialized(buttonParam);

    buttonParam->longPushTime = millisecondsParam;
    buttonParam->buttonPushedLong = actionParam;
    buttonParam->flags.longPush = 1;

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}

uint8_t UnbouncedButton_RemoveActionFromLongPush(Unbouncedbutton_t* buttonParam) {
    __assertInitialized(buttonParam);

    buttonParam->longPushTime = 0;
    buttonParam->buttonPushedLong = NULL;
    buttonParam->flags.longPush = 0;

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}

uint8_t Unbouncedbutton_Handle(Unbouncedbutton_t* buttonParam, uint8_t stateParam, uint64_t systemTimeParam) {
    __assertInitialized(buttonParam);

    if(stateParam == buttonParam->statePushed) {
        
        if(buttonParam->flags.longPush) {

            if(buttonParam->flags.longReset) {

                if((systemTimeParam - buttonParam->lastMillis) > buttonParam->longPushTime) {
                    buttonParam->flags.longReset = 0;
                    buttonParam->flags.shortReset = 0;

                    buttonParam->buttonPushedLong();
                }
            }

        } else if(buttonParam->flags.shortPush) {

            if(systemTimeParam - buttonParam->lastMillis > buttonParam->unbounceTime) {

                if(buttonParam->flags.shortReset) {
                    buttonParam->flags.shortReset = 0;

                    buttonParam->buttonPushedShort();
                }
            }
        }

    } else {

        if(buttonParam->flags.shortPush) {
            
            if(buttonParam->flags.shortReset) {

                if(systemTimeParam - buttonParam->lastMillis > buttonParam->unbounceTime) {
                    buttonParam->buttonPushedShort();
                }
            }
        }

        buttonParam->flags.shortReset = 1;
        buttonParam->flags.longReset = 1;
        buttonParam->lastMillis = systemTimeParam;
    }

    return UNBOUNCEDBUTTON_ERROR_NOERROR;
}
