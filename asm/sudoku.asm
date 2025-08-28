.text
main:
	push	sp
	push	lr
	bl	solve
	cmp	r0, 0
	beq	LBB4_2

	bl	print_grid
	b	LBB4_3
LBB4_2:
	msg	'No solution\n'
LBB4_3:
	mov	r0, 0
	pop	lr
	pop	sp
	halt

get_next_cell:
	push	sp
	push	lr
	mov	r0, 0
	lea	r1, (board)
LBB0_1:
	mov	r3, r1
	mov	r2, 0
LBB0_2:
	ldw	r4, 0(r3)
	cmp	r4, 0
	beq	LBB0_6

	add	r3, 4
	inc	r2
	cmp	r2, 9
	bne	LBB0_2

	add	r1, 36
	inc	r0
	cmp	r0, 9
	beq	LBB0_5
	b	LBB0_1
LBB0_5:
	mov	r0, 0
	b	LBB0_7
LBB0_6:
	lea	r1, (next_cell)
	stw	r2, 4(r1)
	stw	r0, 0(r1)
	mov	r0, 1
LBB0_7:
	pop	lr
	pop	sp
	ret

check_value:
	push	sp
	push	lr
	mov	r0, r4
	mul	r0, 36
	lea	r1, (board)
	mov	r2, r1
	add	r2, r0
	mov	r7, r5
	lsl	r7, 2
	mov	r3, r2
	add	r3, r7
	mov	r0, 1
	ldw	r3, 0(r3)
	cmp	r3, 0
	bne	LBB1_7

	mov	r3, r1
	add	r3, r7
	mov	r7, r5
	smod	r7, 3
	sub	r5, r7
	mov	r7, r4
	smod	r7, 3
	sub	r4, r7
	mov	r7, 0
LBB1_3:
	ldw	r8, 0(r2)
	cmp	r8, r6
	beq	LBB1_6

	ldw	r8, 0(r3)
	cmp	r8, r6
	beq	LBB1_6

	mov	r8, r7
	and	r8, 255
	mul	r8, 171
	lsr	r8, 9
	mov	r9, r4
	add	r9, r8
	mul	r8, 3
	mov	r10, r7
	sub	r10, r8
	and	r10, 255
	mov	r8, r5
	add	r8, r10
	mul	r9, 36
	mov	r10, r1
	add	r10, r9
	lsl	r8, 2
	add	r10, r8
	ldw	r8, 0(r10)
	cmp	r8, r6
	bne	LBB1_2
	b	LBB1_6
LBB1_2:
	add	r3, 36
	add	r2, 4
	inc	r7
	cmp	r7, 9
	beq	LBB1_7
	b	LBB1_3
LBB1_6:
	mov	r0, 0
LBB1_7:
	pop	lr
	pop	sp
	ret

solve:
	push	r16
	push	r17
	push	r18
	push	r19
	push	sp
	push	lr
	bl	get_next_cell
	cmp	r0, 0
	beq	LBB2_1

	lea	r0, (next_cell)
	ldw	r16, 0(r0)
	mov	r1, r16
	mul	r1, 36
	lea	r19, (board)
	add	r19, r1
	ldw	r17, 4(r0)
	mov	r0, r17
	lsl	r0, 2
	add	r19, r0
	mov	r18, 1
LBB2_3:
	mov	r4, r16
	mov	r5, r17
	mov	r6, r18
	bl	check_value
	cmp	r0, 0
	beq	LBB2_6

	stw	r18, 0(r19)
	bl	solve
	cmp	r0, 0
	bne	LBB2_1

	stw	0, 0(r19)
LBB2_6:
	inc	r18
	cmp	r18, 10
	bne	LBB2_3

	mov	r0, 0
	b	LBB2_8
LBB2_1:
	mov	r0, 1
LBB2_8:
	pop	lr
	pop	sp
	pop	r19
	pop	r18
	pop	r17
	pop	r16
	ret

print_grid:
	push	r16
	push	r17
	push	r18
	push	sp
	push	lr
	mov	r16, 0
	lea	r17, (board)
LBB3_1:
	mov	r18, 0
LBB3_2:
	mov	r0, r17
	add	r0, r18
	ldw	r4, 0(r0)
	msg	r4, ' '
	add	r18, 4
	cmp	r18, 36
	beq	LBB3_3
	b	LBB3_2
LBB3_3:
	msg	'\n'
	add	r17, 36
	inc	r16
	cmp	r16, 9
	beq	LBB3_4
	b	LBB3_1
LBB3_4:
	pop	lr
	pop	sp
	pop	r18
	pop	r17
	pop	r16
	ret

.data
board:
	.word	8
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	3
	.word	6
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	7
	.word	0
	.word	0
	.word	9
	.word	0
	.word	2
	.word	0
	.word	0
	.word	0
	.word	5
	.word	0
	.word	0
	.word	0
	.word	7
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	4
	.word	5
	.word	7
	.word	0
	.word	0
	.word	0
	.word	0
	.word	0
	.word	1
	.word	0
	.word	0
	.word	0
	.word	3
	.word	0
	.word	0
	.word	0
	.word	1
	.word	0
	.word	0
	.word	0
	.word	0
	.word	6
	.word	8
	.word	0
	.word	0
	.word	8
	.word	5
	.word	0
	.word	0
	.word	0
	.word	1
	.word	0
	.word	0
	.word	9
	.word	0
	.word	0
	.word	0
	.word	0
	.word	4
	.word	0
	.word	0
next_cell:
	.zero	8
