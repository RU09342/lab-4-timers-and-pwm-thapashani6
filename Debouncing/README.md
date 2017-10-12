## Shani Thapa 
* Added code on 10/9
* Populated README on 10/10

### Code 
Switches were debounced using timers, and button interrupts. First, LEDs, buttons and timers had to be set similarly to the previous labs. However, the tiemr needed to be in stop mode since they need to start counting when the button is pressed. Also, the button should  be set to interrupt when its pressed so IES should be set to high. When the button is pressed the interrupt fires. At the ISR, the clock is now set to UP/Down mode. The interrupts for the button are disabledWhile the button is pressed, the if statement is true, the LED is set to high and the IES is set to low. Due to the IES being set to low, the interrupt fires again when the button is released, and this time the LED is set to low and IES is once again set to high to repeat the process. The timer is on now and re-enables the interrupts for the button while also stopping its own clock. The process can now be repeated, and the bouncing aftereffects of teh switch will be drowned out.

### Differences 
There were no differences between the boards for this debouncing labs, only thoses that have been encountered in the previous labs.

#### Code Example F5529
```
   #pragma vector=TIMER0_A0_VECTOR             // ISR for Timer
   __interrupt void Timer_A (void)
   {
       TA0CTL = TASSEL_2 + MC_0 + ID_3;        // SMCLK, stop, /8
       P2IE |= BIT1;
   }

   #pragma vector=PORT2_VECTOR                 // ISR for Button 2.3
   __interrupt void Port_2(void) 
   {

       P2IFG &= ~BIT1;                         // P2.1 IFG cleared
       P2IE &= ~BIT1;                          // disable interrupts on P2
       TA0CTL = TASSEL_2 + MC_1 + ID_3;        // SMCLK, UP DOWN, /8
       if (!(P2IN & BIT1))                     // button is pressed 
             {
               P1OUT |= BIT0;                  // Sets LED
               P2IES &= ~BIT1;                 // Clear edge detect to rising edge
             }
       else
             {
               P1OUT &= ~BIT0;                 // Resets LED
               P2IES |= BIT1;                  // Set edge detect to rising edge
             }
   }
 ```        
