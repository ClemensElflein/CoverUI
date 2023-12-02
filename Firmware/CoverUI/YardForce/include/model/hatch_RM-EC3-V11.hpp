/**
 * @file hatch_RM-EC3-V11.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-EC3-V1.1 CoverUI Hatch header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-16
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_HATCH_RMEC3V11_HPP
#define YARDFORCE_HATCH_RMEC3V11_HPP

#include <Arduino.h>
#include "../Hatch.hpp"

class HatchRMEC3V11 : public Hatch
{
public:

    unsigned int handle_button(unsigned int button_id, unsigned int press_cnt);
};

extern HatchRMEC3V11 hatch;

#endif // YARDFORCE_HATCH_RMEC3V11_HPP
