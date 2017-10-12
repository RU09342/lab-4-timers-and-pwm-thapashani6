## Shani Thapa
* Code added on 10/11
* Populated README on 10/11

### Code
The PWM lab had to be done again this time using Timer modules to directly output to the LED. This required the manipulation of a new resigister the PxSEL. The value of this register can shift the Port from GPIO to the pheripherals in the boards such as timer modules. Some boards have a PxSEL and PxSEL2 since they have more pheripherals to multiplex through. The following table is a general case on how the PxSEL register works found in the user : 
  Table 1: PxSEL Registers 
| PxSEL2 | PxSEL | Pin Function|
| --- | --- | --- | 
| 0 | 0 | I/O function is selected. |
| 0 | 1 | Primary pheripheral module function is selected. |
| 1 | 0 | Reserved. See device-specific data sheet. |
| 1 | 1 | Secondary peripheral mdoule function is selected | 

Furthermore, the PxDIR values of the Port can also influence the pheripherals selected from the PxSEL regsiters. Each board has different pheripherals on each pin, so it is nesscessary to check the datasheets to find which pin corresponds to which pheripheral.

### Differences
Besides the different values the PxSEL registers take, there were no differences between the boards. The lab is also similar to the Software PWM besides the fact that the Timers dont need ISRs since they directly output to the LEDs. 

#### Code Exmaple with FR2311 and FR6989
```
    // FR2311
    P2SEL0 = BIT0; // sets P2.0 to TB1.1(Timer1_B)
    P2SEL1 = ~BIT0; // sets P2.0 to TB1.1(Timer1_B)
    P2DIR = BIT0; // sets P2.0 to TB1.1(Timer1_B)
    P1DIR = BIT0; // sets P1.0 to an output for the second led
``` 
```
    //FR6989
    P1SEL0 = 0x01; //sets P1.0 to TA0.1
    P1SEL1 = 0x00; //sets P1.0 to TA0.1
    P1DIR = BIT0; //sets P1.0 to TA0.1
    P9DIR = BIT7; //sets P9.7 to output (2nd LED)
```