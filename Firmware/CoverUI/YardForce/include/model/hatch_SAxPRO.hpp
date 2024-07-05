/**
 * @file hatch_SAxPRO.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce SAxPRO CoverUI Hatch header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_HATCH_SAXPRO_HPP
#define YARDFORCE_HATCH_SAXPRO_HPP

#include <Arduino.h>
#include "../Hatch.hpp"

class HatchSAXPRO : public Hatch
{
public:
    unsigned int handle_button(unsigned int button_id, unsigned int press_cnt);
};

extern HatchSAXPRO hatch;

#endif // YARDFORCE_HATCH_SAXPRO_HPP
