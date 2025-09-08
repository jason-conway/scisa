.text
rand:
	lea	r1, (rand_seed)
	ldw	r0, 0(r1)
	mul	r0, 1103515245
	add	r0, 12345
	stw	r0, 0(r1)
	ret

.data
.align	4
rand_seed:
	.word	375226057
