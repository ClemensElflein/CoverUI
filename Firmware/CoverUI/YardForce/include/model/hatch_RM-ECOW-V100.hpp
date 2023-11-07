/**
 * @file hatch_RM-ECOW-V100.hpp
 * @author Apehaenger (joerg@ebeling.ws)
 * @brief YardForce RM-ECOW-V1.0.0 CoverUI Hatch header for OpenMower https://github.com/ClemensElflein/OpenMower
 * @version 0.1
 * @date 2023-11-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef YARDFORCE_HATCH_RMECOWV100_HPP
#define YARDFORCE_HATCH_RMECOWV100_HPP

#include <Arduino.h>
#include "../Hatch.hpp"

class HatchRMECOWV100 : public Hatch
{
public:

    unsigned int handle_button(unsigned int button_id, unsigned int press_cnt);
};

extern HatchRMECOWV100 hatch;

#endif // YARDFORCE_HATCH_RMECOWV100_HPP
