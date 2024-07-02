# CoverUI for [OpenMower](https://github.com/ClemensElflein/OpenMower)

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT) [![Discord](https://badgen.net/badge/icon/discord?icon=discord&label)](https://discord.gg/jE7QNaSxW7)

## About

This repository contains the firmware files required to use the mowers CoverUI (ButtonBoard).

## Variants

1. Custom DIY CoverUI-PCB which replaces the stock (top-cover) PCB.<br>
 Please check the [Cover UI board assembly instructions](<https://openmower.de/docs/cover-ui-assembly/>) for more detailed infos.<br>
 Also available in [Vermut's Shop](https://shop.devops.care/openmower/29-openmower-012x-assembled-board.html) (as part of his 'Upgrade Kit')
1. Stock YardForce CoverUI for the following models:
   1. Classic 500 (STM32 & GD32 MCU)
   2. Classic 500B (STM32 & GD32 MCU)
   3. SA/SC/NX-Type (10 Buttons, 12 LEDs), PCB labeled with RM-ECOW-V1.0.0 (STM32 MCU tested)
   4. SA/SC/NX Type (9 Buttons, 11 LEDs), PCB labeled with RM-ECOW-V1.1.0 (GD32 MCU tested)
   5. NX100i (18 Buttons, 3 LEDs, 256*64 Pixel LC-Display), PCB labeled with RM-EC3-V1.1 (STM32 MCU tested)
   6. SAxPro (Rev6) 6 Buttons 240*128 Pixel LC-Display (STM32 MCU tested)
   
    For this, you need to solder some cables and flash a modified firmware to it.<br>
    Please check the detailed [Stock CoverUI Readme](Firmware/CoverUI/YardForce/README.md) (as you've also the option to use your stock hall, Stop and rain cables).

## License

Distributed under the MIT License. See [LICENSE](LICENSE) for more information.
