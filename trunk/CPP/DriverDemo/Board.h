#ifndef Board_h
#define Board_h

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#ifndef outb
	#define	outb(addr, data)	addr = (data)
#endif
#ifndef inb
	#define	inb(addr)			(addr)
#endif

#define TIMER_CLK_STOP			0x00	///< Timer Stopped
#define TIMER_CLK_DIV1			0x01	///< Timer clocked at F_CPU
#define TIMER_CLK_DIV8			0x02	///< Timer clocked at F_CPU/8
#define TIMER_CLK_DIV64			0x03	///< Timer clocked at F_CPU/64
#define TIMER_CLK_DIV256		0x04	///< Timer clocked at F_CPU/256
#define TIMER_CLK_DIV1024		0x05	///< Timer clocked at F_CPU/1024
#define TIMER_CLK_T_FALL		0x06	///< Timer clocked at T falling edge
#define TIMER_CLK_T_RISE		0x07	///< Timer clocked at T rising edge
#define TIMER_PRESCALE_MASK		0x07	///< Timer Prescaler Bit-Mask

#define LED_PORT	PORTB
#define LED_DDR		DDRB
#define LED_YELLOW	2

#define LED_YELLOW_0 cbi(LED_PORT,LED_YELLOW)
#define LED_YELLOW_1 sbi(LED_PORT,LED_YELLOW)

#define SYS_TIMER_FREQ  10
#define SYS_TIMER_CNTR  65535 - (F_CPU/64/SYS_TIMER_FREQ)

#define VFD_CS_PIN   	12	//D4	//15 //PD7
#define VFD_SCLK_PIN 	7	//A7	//14 //PD6
#define VFD_DATA_PIN 	5	//A5	//13 //PD5

//#define STANDBY_PIN  12 //PD4

#define MAIN_LOOP_DELAY 200

enum enum_SysState
{
    sysSelfTest,
    sysStandby,
    sysFailure
};


#endif
