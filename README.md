BFFL
====

Code and hardware specs for Big F'n FlashLight (BFFL)


Concept
====

The BFFL represents the extension of commercially available high power LED flashlights to their illogical conclusion. The BFFL uses a 150W LED emitter, and is powered by 24 18650 lithium ion batteries. The BFFL easily outshines typical home outdoor spotlights. 


Control
====
Control is provided by an ATtiny84 microcontroller. Control inputs consist of a trigger, brightness control, and standby light.  Brightness is adjustable down to 1/32 power via PWM, and it can set to run at 1/32 to 1/8 power when the trigger is not depressed (so you can find your way with it pointed down, and then press the trigger to get a much brighter search light) Battery status is displayed via an RGB led, and the temperature of the emitter is monitored to prevent overheating in the event of a cooling failure. The fan used has PWM control, and the speed of the fan is controlled by the measured temperature. 

