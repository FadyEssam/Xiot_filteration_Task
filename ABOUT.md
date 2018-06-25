## Goals of this solution
   * Initiate an interrupt when the switch is pressed to toggle the LED and send its state over the UART
   * Send temperature measurement every 3 seconds through UART
   * Leave the super loop empty, so the user can use any additional code or functions he wants

## Some thoughts about the task

   * The project task doesn't require much processing power, that's why I went for avr (atmega32) instead of an arm microcontroller
   *  I used #define for the LED and switch ports and pin numbers to make it much easier to change their places if the code was edited afterwards
   * My goal in designing the code is to make everything run without continous polling inside the superloop, so that the microcontroller 
      is not kept busy without need



## My experience in this Field
 ### Embedded systems
  * I finished an embedded system diploma through which I worked on avr, arm & RTOS

  * I worked on many projects in avr and arm like :
    1. Led matrix showing moving words using avr (atmega32)
    2. Smart Garage project using avr (atmega32)
    3. Distance measuring device using arm (tm4c123gh6pm)
    4. RC car with Arduino uno (equipping it with many sensors)

   * I worked with RTOS (osek-vdx) on atmega32

 ### C
  * I have contributed in Google summer of code in Boost Libraries in C/C++ project using opencl
  * I have worked on many C projects in embedded projects and in college  

