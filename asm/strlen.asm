.text

main:
	lea	r24, (string)
	mov	r4, r24
	bl	strlen
	mov	r25, r0
	out	(r24), r25
	halt

strlen:
	mov	r0, 0
	cmp	r4, 0
	beq	done
cont:
	mov	r1, r4
	add	r1, r0
	inc	r0
	ldub	r1, 0(r1)
	cmp	r1, 0
	bne	cont
	dec	r0
done:
	ret


.data
string: .ascii 'lorem ipsum dolor sit amet\n'
