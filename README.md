# XIOT Filtration Task
## Steps to set up This solution on your device
  1. Install atmel studio (or eclipse with avr plugin)
  2. Create a new project and open the files "main.c" , "xiot_task.h" , "xiot_task.h" 
  3. Build the project
  4. If you have atmega32 and connect it as follows (If not download proteus and open the design.DSN (simulation of the design))

  ![Design Connected](https://i.imgur.com/6NcEUUC.png) 
  
  5. burn the output binary file of the project to the MU (or load it if you are using simulation)
  6. **Congrats It's now working** You Should have output as follows
       * Temperature is displayed every 3 seconds in celsius
       * Pressing the switch initiate an interrupt to toggle the led and output its state to the screen
     
   ![Design Working](https://i.imgur.com/CbfEFR6.png)


**note: The code assumes the temperature sensor is LM35**
