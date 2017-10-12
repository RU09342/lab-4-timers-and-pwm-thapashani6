## Shani Thapa
* Added Code on 10/10
* Populated README on 10/11

### Code 
The Software PWM was created by using the LEDs, Buttons, Interrupts and Timers used in previous labs. After setting up the LEDs, Buttons, and Interrupts; then the timer has to be set. This was the first time multiple CCTL were utilized. Those registers had to have their interrupts enabled, and one was given 1000Hz for its CCR value and the other was given half. This replicates a 50% duty cycle of a PWM. The interrupts for the two timers were simple, merely blink the LED and clear its IFG. The button interrupt went through two if statements that did the bulk of the work. The first if statement goes through if the button is pressed or not. When it's pressed, LED2 blinks and its edge is reversed immidietly setting the interrupt again to go through the first else statement. Here LED2 is set low, and its edge is reversed to await another button press. Meanwhile, the second if statement always checks when the interrupt is fired if the second CCR is greater than 1000Hz. If it is, then CCR is reset otherwise 10% duty cycle is added and the LED becomes brighter or dimmer. 

#### Differences 
There are no differences in the code of this lab other than the differences encoutered in the previous labs that have been documentsed

##### Code Exmaple of Button Interrupt of F5529 
```
#pragma vector=PORT1_VECTOR //ISR for Button P1.1
__interrupt void Port_1(void)
{

    if (! (P1IN & BIT1)) //if button is pressed
        {
            P1IES &= ~BIT1; //sets interrupt to trigger on rising edge (button release)
            P4OUT ^= BIT7; // toggle led
        }
    else //if button released
        {
            P1IES |= BIT1; //sets interrupt to trigger on falling edge (button press)
            P4OUT &= ~BIT7; // toggle led
        }
    if(TA0CCR1>= 1000) //checks if duty cycle is less than 100%
        {
            TA0CCR1 = 0; //if duty cycle is at 100% it is reset to 0%
        }
    else
        {
            TA0CCR1 += 100; // adds 10% to duty cycle
        }

    P1IFG &= ~BIT1; //Clears interrupt flag register needed otherwise will be stuck in infinite loop
}
```       