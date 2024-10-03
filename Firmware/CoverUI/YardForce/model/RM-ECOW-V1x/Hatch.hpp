/**
 * @file Hatch.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.x CoverUI concrete- Hatch header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-10-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_RMECOWV1X_HATCH_HPP
#define YARDFORCE_RMECOWV1X_HATCH_HPP

#include <Arduino.h>

#include "../../include/ABC_Hatch.hpp"
#include "../../include/main.h"

class HatchRMECOWV1x : public AbstractHatch {
   public:
    unsigned int handle_button(unsigned int button_id, unsigned int press_cnt) {
        if (button_id == BTN_LOCK_NUM && press_cnt >= 2) {
            leds.sequence_start(&LEDcontrol::sequence_countdown_handler);  // Close hatch countdown
            queue_button(BTN_LOCK_NUM, 2, 4500);                           // Send long-press Lock button in 4.5 sec
            return 0;                                                      // Don't return button now (hatch is still open)
        }

        if (button_id == BTN_HOME_NUM || button_id == BTN_PLAY_NUM || button_id == BTN_S1_NUM || button_id == BTN_S2_NUM) {
            if (press_cnt > 10 || true)  // Be sure that the queue isn't filled with "wait for release" loops
            {
                leds.sequence_start(&LEDcontrol::sequence_countdown_handler);  // Close hatch countdown
                queue_button(BTN_LOCK_NUM, 2, 4500);                           // Send long-press Lock button in 4.5 sec
                queue_button(button_id, 0, 5000);
                return 0;  // Don't return button now (hatch is still open)
            }
        }

        if (button_id == BTN_4H_NUM)
            return BTN_MON_NUM;  // Volume up

        if (button_id == BTN_6H_NUM)
            return BTN_TUE_NUM;  // Volume down

        return button_id;
    }
};

extern HatchRMECOWV1x hatch;

#endif  // YARDFORCE_RMECOWV1X_HATCH_HPP
