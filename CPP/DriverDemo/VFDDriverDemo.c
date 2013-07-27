/*
  VFDDriverDemo.c - VFD driver test for  
  ATMega328p and 15-segment 7-digit VFD tube from old DVD player
  driven by HT16512 VFD controller.

  Copyright (c) 2012 Dmitry Pakhomenko.

  http://atmega.magictale.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <avr/eeprom.h>

#include "MVFD_16S8D.h"
//#include "HT16515.h"
#include "Board.h"
#include "wiring.h"

volatile uint16_t sysCntr;      //incremented every timer tick
volatile uint8_t sysState;      //current system state (mode)
volatile uint8_t stateCntr;     //counter for displaying current state (in sec) - when becomes
                                //zero display state changes to the next one

MVFD_16S8D vfd(VFD_CS_PIN, VFD_SCLK_PIN, VFD_DATA_PIN);    //VFD display

void setup()
{
    //Define powersave configuration
    set_sleep_mode(SLEEP_MODE_IDLE);

//    pinMode(STANDBY_PIN, OUTPUT);
//    digitalWrite(STANDBY_PIN, HIGH);

    //Set up LED pin
    cbi(LED_PORT, LED_YELLOW);
    sbi(LED_DDR, LED_YELLOW);

    LED_YELLOW_1;

#if defined(__AVR_ATmega328P__)
//#if defined(__AVR_ATmega32__)
    cbi(TIMSK1,TOIE1);
    sbi(TIFR1, TOV1);
#else
    cbi(TIMSK,TOIE1);
    sbi(TIFR, TOV1);
#endif

    TCCR1A = 0;
    outb(TCCR1B, (inb(TCCR1B) & ~TIMER_PRESCALE_MASK) | TIMER_CLK_DIV64);
    cbi(TCCR1B, WGM12);
    cbi(TCCR1B, WGM13);
    TCNT1 = SYS_TIMER_CNTR;

#if defined(__AVR_ATmega328P__)
//#if defined(__AVR_ATmega32__)
    sbi(TIMSK1,TOIE1);
#else
    sbi(TIMSK, TOIE1);
#endif

    sysCntr = 0;
    sysState = sysSelfTest;

#if defined(__AVR_ATmega328P__)
//#if defined(__AVR_ATmega32__)
    sbi(EIMSK, INT1);
#else
    sbi(GICR, INT1);
#endif

    //Enable VFD power supply
//    digitalWrite(STANDBY_PIN, LOW);
//    _delay_ms(100);
//    digitalWrite(STANDBY_PIN, HIGH);

    //Initialise VFD tube
    vfd.reset();

//    vfd.setCur(0);
//    vfd.print_p(PSTR("01234567"));


    sei();
}


//===============================================================
//===============================================================
/* main program starts here */
int main(void)
{    
    uint8_t flashCntr = 0;

    setup();

vfd.initScroll_p(PSTR("        THIS IS A TEST SCROLLING LINE."));

    while (1)
    {
        {
            switch (sysState)
            {
                case sysSelfTest:
                    {
//                        if (vfd.testStep() == vfd.COMPLETED)
                            ;//sysState = sysStandby;
if (!vfd.scrollStep())
vfd.initScroll_p(PSTR("        THIS IS THE SECOND TEST SCROLLING LINE."));

                    }
                    break;

                default:
                    ;       
            }
        }

        _delay_ms(MAIN_LOOP_DELAY);
        flashCntr++;
    }
}

/* IR receiver */
ISR(INT0_vect)
{
//    if (sysState == sysStandby)
//    {
//        sysState = sysSelfTest;
//    }
}

/* PIR sensor */
ISR(INT1_vect)
{
//    if (sysState == sysStandby)
//    {
//        sysState = sysSelfTest;
//    }
}

/* 10Hz system timer */
ISR(TIMER1_OVF_vect)
{
    TCNT1 = SYS_TIMER_CNTR;
    sysCntr++;

    if (sysCntr % SYS_TIMER_FREQ == 0)
    {
	    if (stateCntr != 0) stateCntr--; 
        LED_YELLOW_1;
    }else

    if (sysCntr % (SYS_TIMER_FREQ / 2) == 0)
    {
        LED_YELLOW_0;
    }
}
