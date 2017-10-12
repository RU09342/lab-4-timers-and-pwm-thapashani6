#include <msp430.h> 

/**
 * main.c
 * Change the frequency of the LED blinking depending on time button is pressed
 * MSP430F5529
 * Shani Thapa
 * 10/7/17
 * Button Based Delay
 */

int count = 0;
int freq = 10;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   //Stop Watch Dog Timer

P1SEL &= ~BIT0;
P4SEL &= ~BIT7;

P1DIR |= BIT0;          //LED output
P4DIR |= BIT7;          //Test LED Output
P4OUT = BIT7;


P2DIR &= ~(BIT1);       //Button input
P2IE |= BIT1;           //P1.3 interrupt enable
P2IES |= BIT1;          //high low edge
P2REN |= BIT1;          //enable pull up resistor on button
P2IFG &= ~(BIT1);          // IFG cleared
P2OUT |= BIT1;

P1DIR &= ~(BIT1);       //Button input
P1IE |= BIT1;           //P1.3 interrupt enable
P1IES |= BIT1;          //high low edge
P1REN |= BIT1;          //enable pull up resistor on button
P1IFG &= ~BIT1;          // IFG cleared
P1OUT |= BIT1;

TA0CCTL0 = CCIE;
TA0CTL = TASSEL_2 + MC_1 + ID_3;//SMCLK/8, UP
TA0CCR0 = 125000/freq;               // 100 Hz divided by the frequency

TA1CCTL0 = CCIE;
TA1CTL = TASSEL_2 + MC_0 + ID_3;//SMCLK/8, STOP
TA1CCR0 = 12500;               // 10 Hz

_BIS_SR(GIE);


}
#pragma vector = TIMER1_A0_VECTOR           //Timer1_A ISR
__interrupt void TA1_ISR(void)
{
    count ++;
    P4OUT ^= BIT7;     //Test LED
}

#pragma vector = TIMER0_A0_VECTOR           //Timer0_A ISR
__interrupt void TA0_ISR(void)
{
    P1OUT ^= BIT0;
}

#pragma vector = PORT2_VECTOR               //Button Delay
__interrupt void Port_2(void)
{
 if((P2IN & BIT1) == BIT1)                  //button released
 {
     TA1CTL = MC_0;                         //stop the second clock

     if(count >= 30 )                       //when the button is quickly released
     {
             TA0CCR0=count*125;
             freq = count/100;
     }
     else
     {
         freq++;
         TA0CCR0 = 125000/freq;
     }
 }
else                                        //Button is  Fully Pressed
    {
    TA1R = 0;
    count = 0;
    TA1CTL = TASSEL_2 + MC_1 + ID_3;        //Starts the clock
    }
    P2IES ^= BIT1;
    P2IFG &= ~(BIT1);

 }


#pragma vector = PORT1_VECTOR               //Reset
__interrupt void Port_1(void)
{
    TA0CCR0 = 12500; //10Hz
    P1IFG &= ~(BIT1);
}
