.text
main:
	push	r16
	push	r17
	push	sp
	push	lr
	lea	r16, (front)
	stw	1, 4188(r16)
	stw	1, 4184(r16)
	stw	1, 4180(r16)
	stw	1, 4176(r16)
	stw	1, 4172(r16)
	stw	1, 4168(r16)
	stw	1, 3872(r16)
	stw	1, 3848(r16)
	stw	1, 3528(r16)
	stw	1, 3232(r16)
	stw	1, 3212(r16)
	stw	1, 2904(r16)
	stw	1, 2900(r16)
	stw	1, 5528(r16)
	stw	1, 5524(r16)
	stw	1, 5520(r16)
	stw	1, 5516(r16)
	stw	1, 5512(r16)
	stw	1, 5508(r16)
	stw	1, 5212(r16)
	stw	1, 5188(r16)
	stw	1, 4868(r16)
	stw	1, 4572(r16)
	stw	1, 4552(r16)
	stw	1, 4244(r16)
	stw	1, 4240(r16)
	stw	1, 2968(r16)
	stw	1, 2964(r16)
	stw	1, 2960(r16)
	stw	1, 2956(r16)
	stw	1, 2952(r16)
	stw	1, 2948(r16)
	stw	1, 2652(r16)
	stw	1, 2628(r16)
	stw	1, 2308(r16)
	stw	1, 2012(r16)
	stw	1, 1992(r16)
	stw	1, 1684(r16)
	stw	1, 1680(r16)
	lea	r17, (back)
LBB0_1:
	bl	display
	bl	next
	mov	r0, 0
	mov	r1, r16
	mov	r2, r17
LBB0_2:
	mov	r3, 0
LBB0_3:
	mov	r4, r1
	add	r4, r3
	mov	r5, r2
	add	r5, r3
	ldw	r5, 0(r5)
	stw	r5, 0(r4)
	add	r3, 4
	cmp	r3, 320
	beq	LBB0_4
	b	LBB0_3
LBB0_4:
	add	r1, 320
	add	r2, 320
	inc	r0
	cmp	r0, 24
	beq	LBB0_5
	b	LBB0_2
LBB0_5:
	msg	'\e[24A\e[80D'
	mov	r0, 2000000
LBB0_6:
	dec	r0
	cmp	r0, 0
	beq	LBB0_1
	b	LBB0_6
display:
	push	r16
	push	r17
	push	r18
	push	sp
	push	lr
	mov	r16, 0
	lea	r17, (front)
LBB1_1:
	mov	r18, 0
LBB1_2:
	mov	r0, r17
	add	r0, r18
	ldw	r0, 0(r0)
	cmp	r0, 0
	beq	LBB1_6
	msg	'\e[1;32m#\e[0m'
	b	LBB1_7
LBB1_6:
	msg	' '
LBB1_7:
	add	r18, 4
	cmp	r18, 320
	beq	LBB1_4
	b	LBB1_2
LBB1_4:
	msg	'\n'
	add	r17, 320
	inc	r16
	cmp	r16, 24
	beq	LBB1_5
	b	LBB1_1
LBB1_5:
	pop	lr
	pop	sp
	pop	r18
	pop	r17
	pop	r16
	ret
next:
	push	r16
	push	r17
	push	r18
	push	r19
	push	r20
	push	r21
	push	sp
	push	lr
	mov	r16, 0
	lea	r18, (front)
	lea	r19, (back)
LBB2_1:
	mov	r20, r19
	mov	r21, r18
	mov	r17, 0
LBB2_2:
	mov	r4, r17
	mov	r5, r16
	bl	count_nbors
	mov	r1, 1
	cmp	r0, 3
	mov	r2, 1
	beq	LBB2_4
	mov	r2, 0
LBB2_4:
	and	r0, -2
	cmp	r0, 2
	beq	LBB2_6
	mov	r1, 0
LBB2_6:
	ldw	r0, 0(r21)
	cmp	r0, 0
	moveq	r1, r2
	stw	r1, 0(r20)
	add	r20, 4
	add	r21, 4
	inc	r17
	cmp	r17, 80
	beq	LBB2_7
	b	LBB2_2
LBB2_7:
	add	r19, 320
	add	r18, 320
	inc	r16
	cmp	r16, 24
	beq	LBB2_8
	b	LBB2_1
LBB2_8:
	pop	lr
	pop	sp
	pop	r21
	pop	r20
	pop	r19
	pop	r18
	pop	r17
	pop	r16
	ret
count_nbors:
	push	r16
	push	r17
	push	r18
	push	r19
	push	r20
	push	r21
	push	r22
	push	r23
	push	sp
	push	lr
	mov	r16, r5
	mov	r17, r4
	mov	r21, -1
	mov	r18, 0
	lea	r22, (front)
LBB3_1:
	mov	r19, r21
	add	r19, r17
	mov	r23, -1
LBB3_2:
	mov	r0, r23
	or	r0, r21
	cmp	r0, 0
	beq	LBB3_6
	mov	r4, r19
	mov	r5, 80
	bl	modulo
	mov	r20, r0
	mov	r4, r16
	add	r4, r23
	mov	r5, 24
	bl	modulo
	mul	r0, 320
	mov	r1, r22
	add	r1, r0
	lsl	r20, 2
	add	r1, r20
	ldw	r1, 0(r1)
	mov	r0, 1
	cmp	r1, 0
	bne	LBB3_5
	mov	r0, 0
LBB3_5:
	add	r18, r0
LBB3_6:
	inc	r23
	cmp	r23, 2
	beq	LBB3_7
	b	LBB3_2
LBB3_7:
	inc	r21
	cmp	r21, 2
	beq	LBB3_8
	b	LBB3_1
LBB3_8:
	mov	r0, r18
	pop	lr
	pop	sp
	pop	r23
	pop	r22
	pop	r21
	pop	r20
	pop	r19
	pop	r18
	pop	r17
	pop	r16
	ret
modulo:
	push	sp
	mov	r0, r4
	smod	r0, r5
	add	r0, r5
	lsl	r0, 16
	asr	r0, 16
	lsl	r5, 16
	asr	r5, 16
	smod	r0, r5
	pop	sp
	ret

.data
.align 4
front:
	.zero	7680
back:
	.zero	7680
