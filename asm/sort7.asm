.text
main:
	stw	29, -36(sp)
	stw	53, -40(sp)
	stw	79, -44(sp)
	stw	6, -48(sp)
	stw	96, -52(sp)
	stw	41, -56(sp)
	stw	11, -60(sp)
	msg	'Unsorted: \n'
	mov	r4, sp
	add	r4, -60
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r16, sp
	add	r16, -56
	mov	r4, r16
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r17, sp
	add	r17, -52
	mov	r4, r17
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r18, sp
	add	r18, -48
	mov	r4, r18
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r19, sp
	add	r19, -44
	mov	r4, r19
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r20, sp
	add	r20, -40
	mov	r4, r20
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r21, sp
	add	r21, -36
	mov	r4, r21
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, sp
	add	r4, -60
	bl	sort7
	msg	'\nSorted: \n'
	mov	r4, sp
	add	r4, -60
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, r16
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, r17
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, r18
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, r19
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, r20
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	mov	r4, r21
	ldw	r4, 0(r4)
	msg	'val: ', r4, '\n'
	halt

sort7:
	push	sp
	ldw	r3, 16(r4)
	ldw	r0, 12(r4)
	cmp	r0, r3
	mov	r5, r3
	movlt	r5, r0
	xor	r3, r0
	ldw	r1, 24(r4)
	ldw	r2, 20(r4)
	cmp	r2, r1
	mov	r0, r1
	movlt	r0, r2
	xor	r1, r2
	xor	r1, r0
	xor	r3, r5
	cmp	r5, r0
	mov	r2, r0
	movlt	r2, r5
	xor	r0, r5
	ldw	r5, 8(r4)
	ldw	r6, 4(r4)
	cmp	r6, r5
	mov	r7, r5
	movlt	r7, r6
	xor	r5, r6
	xor	r0, r2
	cmp	r3, r1
	mov	r6, r1
	movlt	r6, r3
	cmp	r6, r0
	mov	r8, r0
	movlt	r8, r6
	xor	r0, r6
	xor	r0, r8
	xor	r5, r7
	ldw	r9, 0(r4)
	cmp	r9, r5
	mov	r10, r5
	movlt	r10, r9
	cmp	r10, r7
	mov	r11, r10
	xor	r11, r7
	movlt	r7, r10
	xor	r11, r7
	cmp	r11, r0
	mov	r12, r0
	movlt	r12, r11
	xor	r0, r11
	xor	r1, r3
	cmp	r7, r8
	mov	r3, r8
	movlt	r3, r7
	xor	r8, r7
	xor	r1, r6
	xor	r5, r9
	xor	r5, r10
	mov	r6, r1
	xor	r6, r5
	cmp	r5, r1
	movlt	r1, r5
	xor	r0, r12
	cmp	r1, r0
	mov	r5, r0
	movlt	r5, r1
	xor	r0, r1
	xor	r8, r3
	xor	r0, r5
	cmp	r5, r8
	mov	r7, r8
	movlt	r7, r5
	xor	r5, r8
	xor	r6, r1
	cmp	r3, r2
	mov	r1, r2
	movlt	r1, r3
	xor	r3, r2
	stw	r6, 24(r4)
	xor	r3, r1
	stw	r1, 0(r4)
	stw	r0, 20(r4)
	xor	r5, r7
	stw	r5, 16(r4)
	mov	r0, r3
	xor	r0, r12
	cmp	r12, r3
	movlt	r3, r12
	stw	r3, 4(r4)
	xor	r0, r3
	mov	r1, r0
	xor	r1, r7
	cmp	r7, r0
	movlt	r0, r7
	stw	r0, 8(r4)
	xor	r1, r0
	stw	r1, 12(r4)
	pop	sp
	ret
