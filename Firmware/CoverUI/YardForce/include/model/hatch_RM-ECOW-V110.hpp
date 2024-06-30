/**
 * @file hatch_RM-ECOW-V110.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.1.0 CoverUI Hatch header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2024-06-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef YARDFORCE_HATCH_RMECOWV110_HPP
#define YARDFORCE_HATCH_RMECOWV110_HPP

#include <Arduino.h>
#include "../Hatch.hpp"

class HatchRMECOWV110 : public Hatch
{
public:

    unsigned int handle_button(unsigned int button_id, unsigned int press_cnt);
};

extern HatchRMECOWV110 hatch;

#endif // YARDFORCE_HATCH_RMECOWV110_HPP
