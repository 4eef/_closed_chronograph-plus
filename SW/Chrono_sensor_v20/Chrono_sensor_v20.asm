;Chronograph+ sensor firmware based on ATtiny10

.INCLUDE "tn10def.inc"

;Variables
.equ	CSGNHH	= RAMEND-24
.equ	CSGNHL	= RAMEND-25
.equ	CSGNLH	= RAMEND-26
.equ	CSGNLL	= RAMEND-27
.equ	VELO4H	= RAMEND-22
.equ	VELO4L	= RAMEND-23
.equ	VELO3H	= RAMEND-20
.equ	VELO3L	= RAMEND-21
.equ	VELO2H	= RAMEND-18
.equ	VELO2L	= RAMEND-19
.equ	VELO1H	= RAMEND-16
.equ	VELO1L	= RAMEND-17
.equ	VELOH	= RAMEND-12
.equ	VELOL	= RAMEND-13
.equ	SIGNH	= RAMEND-14
.equ	SIGNL	= RAMEND-15
.equ	FSTSH	= RAMEND-3
.equ	FSTSL	= RAMEND-2
.equ	NULLH	= RAMEND-1
.equ	NULLL	= RAMEND

	rjmp	RESET
	reti	;INT0
	rjmp	PC0
	rjmp	T0CAPT
	reti	;T0OVF
	rjmp	T0CMA
	reti	;T0CMB
	reti	;ACOMP
	reti	;WDT
	reti	;VLM
	reti	;ADC

;************************INTERRUPT HANDLERS************************

;External interrupt
PC0:	out	PCICR,	r16
	reti

;Capture timer value to SRAM
T0CAPT:	in	r17,	ICR0L
	st	-z,	r17
	in	r17,	ICR0H
	st	-z,	r17
	reti

;Stop timer on compare match and switch clock frequency
T0CMA:	out	TCCR0B,	r16
	out	TIMSK0,	r16
	ldi	r17,	0b00100111
	out	TIFR0,	r17
	ldi	r17,	0xD8			;1 MHz
	ldi	r18,	0b00000011
	out	CCP,	r17
	out	CLKPSR,	r18
	ret

RESET:	ldi	r16,	low(RAMEND-28)
	out	SPL,	r16
	ldi	r16,	high(RAMEND-28)
	out	SPH,	r16
	;IO
	ldi	r16,	0b00000100		;PB0, PB1, PB3 - in; PB2 - out
	out	DDRB,	r16
	ldi	r16,	0b00001010		;PB1, PB3 - PU
	out	PUEB,	r16
	out	PORTB,	r16
	;Power consumption
	ldi	r16,	0b00000010		;Shut down ADC
	out	PRR,	r16
	;Miscellaneous
	clr	r16				;Clear SRAM
	ldi	zl,	low(RAMEND-31)
	ldi	zh,	high(RAMEND-31)
	ldi	r17,	32
	st	z+,	r16
	dec	r17
	brne	PC-2

;************************MAIN ROUTINE************************

	rjmp	main
bfrovf:	out	TIMSK0,	r16
	out	TCCR0B,	r16
	ldi	r17,	0b00100111
	out	TIFR0,	r17
main:	cbi	PORTB,	2
	cli
	clr	r16
	ldi	r17,	5			;Timer preload (6 clocks for exit from sleep)
	out	TCNT0H,	r16
	out	TCNT0L,	r17
	ldi	r17,	32			;When timer is stops (8192)
	out	OCR0AH,	r17
	out	OCR0AL,	r16
	ldi	r17,	0b00000101		;Power Down
;	ldi	r17,	0b00000001		;Idle (debug only)
	out	SMCR,	r17
	ldi	zl,	low(RAMEND-1)		;Load buffer address to Z
	ldi	zh,	high(RAMEND-1)
	ldi	r17,	0b00000010		;PCINT1
	out	PCMSK,	r17
	ldi	r17,	0b00000001		;PCINT interrupt
	out	PCIFR,	r17
	out	PCICR,	r17
	ldi	r17,	0xD8
	out	CCP,	r17
	out	CLKPSR,	r16			;8 MHz
	out	CCP,	r17
	out	WDTCSR,	r16			;Shut down WDT
	sei
	ldi	r17,	0b00000001		;Timer clocking
	out	TCCR0B,	r17
	sleep
	ldi	r17,	0b00100010		;ICU, OCR1A interrupts
	out	TIFR0,	r17
	out	TIMSK0,	r17
	wdr
	ldi	r17,	0b01001110		;WDIE, WDE, Tovf=1 s
	out	WDTCSR,	r17
	ldi	r17,	0b00000001		;Idle
	out	SMCR,	r17
t0cnt:	sleep
	ldi	r18,	high(RAMEND-15)		;Buffer upper border
	cpi	zl,	low(RAMEND-15)
	cpc	zh,	r18
	brcs	bfrovf
	in	r18,	TCCR0B
	sbrc	r18,	0
	rjmp	t0cnt
	;Find time on first hit of second sensor
	ld	r17,	z+			;MSB
	ld	r16,	z+			;LSB
	mov	r18,	r16
	mov	r19,	r17
	mov	r20,	r16
	mov	r21,	r17
	mov	r22,	r16
	mov	r23,	r17
	lsr	r21				;Divide by 4
	ror	r20
	lsr	r21
	ror	r20
	sub	r16,	r20			;75% of overall timer value
	sbc	r17,	r21
	rjmp	smplcp
smplst:	mov	r22,	r20			;Store sample
	mov	r23,	r21
	mov	r20,	r18
	mov	r21,	r19
	ld	r19,	z+			;Load (next) prevoius sample
	ld	r18,	z+
smplcp:	cp	r16,	r18			;Compare, load next if larger
	cpc	r17,	r19
	brcs	smplst
	sub	r22,	r20			;Pellet signature (one sensor used)
	sbc	r23,	r21
;	lds	r18,	FSTSL
;	lds	r19,	FSTSH
;	add	r22,	r18
;	adc	r23,	r19
	sts	VELOL,	r20
	sts	VELOH,	r21
;	sts	SIGNL,	r22
;	sts	SIGNH,	r23
	;Sensor signature
	clr	r20
	lds	r19,	CSGNHH
	lds	r18,	CSGNHL
	lds	r17,	CSGNLH
	lds	r16,	CSGNLL
	cp	r20,	r16
	cpc	r20,	r17
	cpc	r20,	r18
	cpc	r20,	r19
	brcs	irtx
	;Make particular chronograph signature
mkcsgn:	lds	r17,	VELO4H			;Check if there is enough data in buffer
	lds	r16,	VELO4L
	clr	r31
	cp	r31,	r16
	cpc	r31,	r17
	brcs	PC+2
	rjmp	dshft
	lds	r29,	VELO3H			;Load data
	lds	r28,	VELO3L
	lds	r27,	VELO2H
	lds	r26,	VELO2L
	lds	r25,	VELO1H
	lds	r24,	VELO1L
	lds	r23,	VELOH
	lds	r22,	VELOL
	lds	r21,	SIGNH
	lds	r20,	SIGNL
	clr	r19
	clr	r18
	ldi	r30,	5			;Combine data to get a random number
	lsl	r16
	rol	r17
	rol	r18
	rol	r19
dadd:	lsl	r16
	rol	r17
	rol	r18
	rol	r19
	lsl	r16
	rol	r17
	rol	r18
	rol	r19
	lsl	r16
	rol	r17
	rol	r18
	rol	r19
	add	r16,	r28
	adc	r17,	r29
	adc	r18,	r31
	adc	r19,	r31
	dec	r30
	brne	dadd
	sts	CSGNLL,	r16			;Store data
	sts	CSGNLH,	r17
	sts	CSGNHL,	r18
	sts	CSGNHH,	r19
	;NEC IR protocol implementation (load of next byte takes 5 clocks (microseconds))
irtx:	ldi	xl,	low(RAMEND-11)		;Buffer beginning
	ldi	xh,	high(RAMEND-11)
prmbl:	ldi	r17,	126			;3.15 ms active
	ldi	r18,	252			;6.3 ms total
	ldi	r19,	1			;Send it once
	ldi	r20,	17			;Buffer length (preamble + 16 bytes)
	rjmp	psw-1
byte:	ld	r21,	-x
	ldi	r19,	8			;Bits
bit:	nop
	ldi	r17,	22			;Active period - 550 us
	ldi	r18,	88			;Periods per phase "0" - 2.2 ms
	lsr	r21
	brcc	PC+2
	ldi	r18,	44			;"1" - 1.1 ms
	ldi	r16,	0b00000100
psw:	out	PINB,	r16			;"1", 10 clocks
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	out	PINB,	r16			;"0", 15 clocks
	nop
	dec	r18
	breq	PC+11
	nop
	nop
	nop
	nop
	nop
	nop
	dec	r17				;Shut off the active period
	brne	PC+2
	clr	r16
	rjmp	psw
	dec	r19
	brne	bit
	dec	r20
	brne	byte
	brts	dshft
endp:	ldi	r17,	255			;End of packet
	ldi	r18,	44
	ldi	r19,	1
	ldi	r20,	1
	set
	rjmp	psw-1
	;Shift data
dshft:	lds	r16,	VELO3L
	sts	VELO4L,	r16
	lds	r16,	VELO3H
	sts	VELO4H,	r16
	lds	r16,	VELO2L
	sts	VELO3L,	r16
	lds	r16,	VELO2H
	sts	VELO3H,	r16
	lds	r16,	VELO1L
	sts	VELO2L,	r16
	lds	r16,	VELO1H
	sts	VELO2H,	r16
	lds	r16,	VELOL
	sts	VELO1L,	r16
	lds	r16,	VELOH
	sts	VELO1H,	r16
	;Clear buffer
	clr	r16
	ldi	zl,	low(RAMEND-11)
	ldi	zh,	high(RAMEND-11)
	ldi	r17,	12
	st	z+,	r16
	dec	r17
	brne	PC-2
	clt
	rjmp	main
