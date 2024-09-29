<a name="readme-top"></a>

([back to README][README])

## YardForce Classic 500, RM-ECOW-V1.3.0 MODs

![Classic 500](images/IMG_Overview.jpg)


### Buttons (*mandatory*)

With the stock component placement, Button-Home and Button-Play get routed to the main (black) 16 pin connector (JP2/J6).
But we need them routed to the MCU.
Luckily the PCB is already prepared for this.

[![Mandatory Hardware Modification](images/IMG_PCB_Changes.jpg "Open in browser")](images/IMG_PCB_Changes.jpg)
<sup>(The yellow cable (to R5/C2) should not harm you. This is the NRST signal which is only required if you bug the code (like me) during development)</sup>

Move the 0 Ohm (bridge) resistors:
1. R33 to the NonPlaced R34 position (Button-Home)
2. R37 to (NP) R42 (Button-Play)
   
You might also simply bridge R34 and R42, but for me it was simpler to move them

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Rain Sensor (*optional*)

If you also like to use the Stock-Rain-Sensor cabling, you need to solder a separate cable from FB2 to the non-placed R79.
See the yellow cable on the following images:

[![Optional Rain Hardware Modification](images/IMG_Stock-Cables-overview.jpg "Open in browser")](images/IMG_Stock-Cables-overview.jpg)
[![Optional Rain Hardware Modification](images/IMG_Stock-Cables-detail2.jpg "Open in browser")](images/IMG_Stock-Cables-detail2.jpg)
[![Optional Rain Hardware Modification](images/IMG_Stock-Cables-rain1.jpg "Open in browser")](images/IMG_Stock-Cables-rain1.jpg)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


### Emergency Hall Sensors (*optional*)

If you also like to use your Stock-Hall-Sensor cabling (2 \* stop-button & 2 \* wheel-lift):
![Optional Hall Cables](images/IMG_Stock-Cables_500B.jpg)
Then you need to solder one more R- bridge as well as 3 more separate cables.
Take into **attention** that you need to make all 4 steps:

1. Move R52 to (non-placed) R57 (see green arrow on image), or simple bridge R57 (i.e. with 32kg solder)
2. Solder a cable-bridge from Q4/R61 to U5/Pin3 (see left green cable)
3. Solder a cable-bridge from the bottom end of R76 to U5/Pin2 (see white cable)
4. Solder a cable-bridge from Q6/R83 to U5/Pin10 (see right green cable)

[![Optional Hall Hardware Modification](images/IMG_Stock-Cables-overview-hall.jpg "Open in browser")](images/IMG_Stock-Cables-overview-hall.jpg)
[![Optional Hall Hardware Modification](images/IMG_Stock-Cables-detail3.jpg "Open in browser")](images/IMG_Stock-Cables-detail3.jpg)
[![Optional Hall Hardware Modification](images/IMG_Stock-Cables-detail4.jpg "Open in browser")](images/IMG_Stock-Cables-detail4.jpg)

I only made it with colored cables for a more clear documentation.
If you've repair-wire like [this](https://de.farnell.com/roadrunner/rrp-a-105/draht-0-15mm-sortiert-pk-4/dp/5017233), it might look better, like this:

![Hardware Changes Hall nice](images/IMG_Stock-Cables-nice.jpg)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

([back to README][README])

[README]: README.md
