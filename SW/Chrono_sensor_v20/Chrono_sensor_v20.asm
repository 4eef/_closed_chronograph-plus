;������ ��� ������� ���������+ v2.0 �� ATtiny10

.INCLUDE "tn10def.inc"

;����������
.equ	DISTHH	= RAMEND-24
.equ	DISTHL	= RAMEND-25
.equ	DISTLH	= RAMEND-26
.equ	DISTLL	= RAMEND-27
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

;************************����������� ����������************************

;������� ����������
PC0:	out	PCICR,	r16
	reti

;���� ������������ �������� �������� � SRAM
T0CAPT:	in	r17,	ICR0L
	st	-z,	r17
	in	r17,	ICR0H
	st	-z,	r17
	reti

;���������� ������, ����������� ��������
T0CMA:	out	TCCR0B,	r16
	out	TIMSK0,	r16
	ldi	r17,	0b00100111
	out	TIFR0,	r17
	ldi	r17,	0xD8			;1 ���
	ldi	r18,	0b00000011
	out	CCP,	r17
	out	CLKPSR,	r18
	ret

RESET:	ldi	r16,	low(RAMEND-28)
	out	SPL,	r16
	ldi	r16,	high(RAMEND-28)
	out	SPH,	r16

	ldi	r16,	0b00000100		;PB0, PB1, PB3 - in; PB2 - out
	out	DDRB,	r16
	ldi	r16,	0b00001010		;PB1, PB3 - PU
	out	PUEB,	r16
	out	PORTB,	r16

	ldi	r16,	0b00000010		;��������� ADC
	out	PRR,	r16

	clr	r16				;�������� RAM (��� �������)
	ldi	zl,	low(RAMEND-31)
	ldi	zh,	high(RAMEND-31)
	ldi	r17,	32
	st	z+,	r16
	dec	r17
	brne	PC-2


;************************������� ����************************

	rjmp	main
bfrovf:	out	TIMSK0,	r16
	out	TCCR0B,	r16
	ldi	r17,	0b00100111
	out	TIFR0,	r17
main:	cbi	PORTB,	2
	cli
	clr	r16
	ldi	r17,	5			;������������ � ������ (����������� 6 ������)
	out	TCNT0H,	r16
	out	TCNT0L,	r17
	ldi	r17,	32			;����� ������ ������� (8192)
	out	OCR0AH,	r17
	out	OCR0AL,	r16
	ldi	r17,	0b00000101		;Power Down
;	ldi	r17,	0b00000001		;Idle
	out	SMCR,	r17
	sts	NULLH,	r16			;������� ��� ���������� ������ ������
	sts	NULLL,	r16
	ldi	zl,	low(RAMEND-1)		;�������� ������ � ��������� Z
	ldi	zh,	high(RAMEND-1)
	ldi	r17,	0b00000010		;PCINT1
	out	PCMSK,	r17
	ldi	r17,	0b00000001		;���������� �� PCINT
	out	PCIFR,	r17
	out	PCICR,	r17
	ldi	r17,	0xD8
	out	CCP,	r17
	out	CLKPSR,	r16			;8 ���
	out	CCP,	r17
	out	WDTCSR,	r16			;��������� WDT
	sei
	ldi	r17,	0b00000001		;������������ �������
	out	TCCR0B,	r17
	sleep
	ldi	r17,	0b00100010		;���������� �� ICU, OCR1A
	out	TIFR0,	r17
	out	TIMSK0,	r17
	wdr
	ldi	r17,	0b01001110		;WDIE, WDE, Tovf=1 �
	out	WDTCSR,	r17
	ldi	r17,	0b00000001		;Idle
	out	SMCR,	r17
t0cnt:	sleep
	ldi	r18,	high(RAMEND-15)		;������������� ������
	cpi	zl,	low(RAMEND-15)
	cpc	zh,	r18
	brcs	bfrovf
	in	r18,	TCCR0B
	sbrc	r18,	0
	rjmp	t0cnt

	;���������� ������� ������� ���� ����� ���������
	ld	r17,	z+			;MSB
	ld	r16,	z+			;LSB
	mov	r18,	r16
	mov	r19,	r17
	mov	r20,	r16
	mov	r21,	r17
	mov	r22,	r16
	mov	r23,	r17
	lsr	r21				;������ �� 4
	ror	r20
	lsr	r21
	ror	r20
	sub	r16,	r20			;75% �� ����� ����� �������
	sbc	r17,	r21
	rjmp	smplcp
smplst:	mov	r22,	r20			;��������� �����
	mov	r23,	r21
	mov	r20,	r18
	mov	r21,	r19
	ld	r19,	z+			;��������� (���������) ���������� �����
	ld	r18,	z+
smplcp:	cp	r16,	r18			;�������� � �������, ��������� ���������, ���� ������
	cpc	r17,	r19
	brcs	smplst
	sub	r22,	r20			;��������� ���� (������ � ���� ��������)
	sbc	r23,	r21
	lds	r18,	FSTSL
	lds	r19,	FSTSH
	add	r22,	r18
	adc	r23,	r19
	sts	VELOL,	r20
	sts	VELOH,	r21
	sts	SIGNL,	r22
	sts	SIGNH,	r23

	;����������� �������� ����
	ldi	r19,	0x01			;��������� - 22000000 (27.5*100000/0,125)
	ldi	r18,	0x4F
	ldi	r17,	0xB1
	ldi	r16,	0x80
	sts	DISTHH,	r19			;������������ ��������� ��� ��������� ����������
	sts	DISTHL,	r18
	sts	DISTLH,	r17
	sts	DISTLL,	r16
	rcall	div32u				;V=K/N, [�*100/�]
	tst	r18				;������� ������ - 655,36 �/�
	breq	PC+3
	ldi	r16,	0xFF
	ldi	r17,	0xFF
	sts	VELOL,	r16
	sts	VELOH,	r17
	;����������� ��������� ����
	ldi	r17,	0x6B			;���������� - l=27500 (27,5 ��)
	ldi	r16,	0x6C
	lds	r22,	SIGNL
	lds	r23,	SIGNH
	rcall	mpy16u				;L=l*N2/N1, [���]
	rcall	div32u
	sts	SIGNL,	r16
	sts	SIGNH,	r17

	;�������� ������ �� �� ������
irtx:	ldi	xl,	low(RAMEND-11)		;������ ������
	ldi	xh,	high(RAMEND-11)
	ldi	r19,	16			;������� ���� (����� ������������� ������)
byte:	ld	r20,	-x			;���������� ����
	com	r20				;�������������
	ldi	r18,	10			;������� ��� (8N1)
	rjmp	PC+6
bit:	clr	r16
	cpi	r18,	1			;����-���
	breq	PC+4
	ror	r20
	brcc	PC+2
	ldi	r16,	0b00000100		;PB2
	ldi	r17,	20			;�������, 40000/r17 = baud/s
psw:	out	PINB,	r16			;"1", 10 ������
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	out	PINB,	r16			;"0", 15 ������
	nop
	dec	r17
	breq	PC+11
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	rjmp	psw
	dec	r18
	brne	bit
	dec	r19
	brne	byte

	;���������� ������
	lds	r16,	VELO3L
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

	rjmp	main

;************************������������************************

;r19:r18:r17:r16 <- r17:r16 x r23:r22
mpy16u:	clr	r18
	clr	r19
	ldi	r20,	16
	lsr	r17
	ror	r16
m16u_1:	brcc	noad16
	add	r18,	r22
	adc	r19,	r23
noad16:	ror	r19
	ror	r18
	ror	r17
	ror	r16
	dec	r20
	brne	m16u_1
	ret

;r19:r18:r17:r16 <- r19:r18:r17:r16 / r21:r20
div32u:	clr	r22
	ldi	r23,	33
	clr	r24
	clr	r25
	clr	r26
	sub	r27,	r27
d32u_1:	rol	r16
	rol	r17
	rol	r18
	rol	r19
	dec	r23
	brne	d32u_2
	ret
d32u_2:	rol	r24
	rol	r25
	rol	r26
	rol	r27
	sub	r24,	r20
	sbc	r25,	r21
	sbc	r26,	r22
	sbc	r27,	r22
	brcc	d32u_3
	add	r24,	r20
	adc	r25,	r21
	adc	r26,	r22
	adc	r27,	r22
	clc
	rjmp	d32u_1
d32u_3:	sec
	rjmp	d32u_1
