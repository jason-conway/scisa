.text

main:
	lea	r4, (ro__str_data)
	call	svg2rgb
	mov	r12, r0
	mov	r4, r12
	call	rgb2svg
	mov	r5, r12
	lsr	r5, 16
	mov	r7, r12
	and	r7, 255
	lsr	r12, 8
	and	r12, 255
	mov	r4, r0
	mov	r6, r12
	call	print
	mov	r0, 0
	halt

svg2rgb:
	mov	r0, 0
	ldub	r1, 0(r4)
	cmp	r1, 0
	beq	LBB0_4

	inc	r4
LBB0_2:
	or	r1, 32
	and	r1, 255
	xor	r0, r1
	mul	r0, 1351300757
	ldub	r1, 0(r4)
	inc	r4
	cmp	r1, 0
	bne	LBB0_2

	lsr	r0, 22
LBB0_4:
	lsl	r0, 2
	lea	r1, (ro__t_data)
	add	r1, r0
	ldw	r0, 0(r1)
	and	r0, 16777215
	ret

rgb2svg:
	mov	r1, r4
	mul	r1, 191060119
	lsr	r1, 23
	mov	r0, r1
	lsl	r0, 2
	lea	r2, (ro__k_data)
	add	r2, r0
	mov	r0, 0
	ldw	r2, 0(r2)
	cmp	r2, r4
	bne	LBB1_2

	lsl	r1, 1
	lea	r0, (ro__v_data)
	add	r0, r1
	ldsh	r1, 0(r0)
	lea	r0, (ro__n_data)
	add	r0, r1
LBB1_2:
	ret

print:
	; r4 str
	; r5 red
	; r6 green
	; r7 blue
	lea r12, (ro__str_sz)
	ldw r12, (r12)
	out r4, r12 ; ??
	msg ' (', r5, ', ', r6, ', ', r7, ')\n'
	ret

.data
ro__t_sz:
	.word 4096

ro__t_data:
	.word	10145074                        ; 0x9acd32
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11119017                        ; 0xa9a9a9
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8421504                         ; 0x808080
	.word	0                               ; 0x0
	.word	14315734                        ; 0xda70d6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4251856                         ; 0x40e0d0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16770229                        ; 0xffe4b5
	.word	16121850                        ; 0xf5fffa
	.word	0                               ; 0x0
	.word	10506797                        ; 0xa0522d
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16445670                        ; 0xfaf0e6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3100495                         ; 0x2f4f4f
	.word	0                               ; 0x0
	.word	6970061                         ; 0x6a5acd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11674146                        ; 0xb22222
	.word	0                               ; 0x0
	.word	32768                           ; 0x8000
	.word	0                               ; 0x0
	.word	16119285                        ; 0xf5f5f5
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9109643                         ; 0x8b008b
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16448210                        ; 0xfafad2
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14381203                        ; 0xdb7093
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9498256                         ; 0x90ee90
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16113331                        ; 0xf5deb3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	6591981                         ; 0x6495ed
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9127187                         ; 0x8b4513
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16711935                        ; 0xff00ff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11403055                        ; 0xadff2f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14474460                        ; 0xdcdcdc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15761536                        ; 0xf08080
	.word	3050327                         ; 0x2e8b57
	.word	16777184                        ; 0xffffe0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16775920                        ; 0xfffaf0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16752762                        ; 0xffa07a
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8190976                         ; 0x7cfc00
	.word	6908265                         ; 0x696969
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	205                             ; 0xcd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	13808780                        ; 0xd2b48c
	.word	0                               ; 0x0
	.word	139                             ; 0x8b
	.word	4620980                         ; 0x4682b4
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	64154                           ; 0xfa9a
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11393254                        ; 0xadd8e6
	.word	16729344                        ; 0xff4500
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12433259                        ; 0xbdb76b
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16775885                        ; 0xfffacd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15132410                        ; 0xe6e6fa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15792383                        ; 0xf0f8ff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	2142890                         ; 0x20b2aa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16716947                        ; 0xff1493
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15631086                        ; 0xee82ee
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	2003199                         ; 0x1e90ff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	255                             ; 0xff
	.word	0                               ; 0x0
	.word	9055202                         ; 0x8a2be2
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	65407                           ; 0xff7f
	.word	13047173                        ; 0xc71585
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4734347                         ; 0x483d8b
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	2263842                         ; 0x228b22
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16768685                        ; 0xffdead
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8900346                         ; 0x87cefa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	13882323                        ; 0xd3d3d3
	.word	32896                           ; 0x8080
	.word	14596231                        ; 0xdeb887
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	7833753                         ; 0x778899
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	6737322                         ; 0x66cdaa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16737095                        ; 0xff6347
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11529966                        ; 0xafeeee
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	65535                           ; 0xffff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	6266528                         ; 0x5f9ea0
	.word	4286945                         ; 0x4169e1
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	7833753                         ; 0x778899
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	65535                           ; 0xffff
	.word	16119260                        ; 0xf5f5dc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3100495                         ; 0x2f4f4f
	.word	1644912                         ; 0x191970
	.word	0                               ; 0x0
	.word	16416882                        ; 0xfa8072
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3978097                         ; 0x3cb371
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16775930                        ; 0xfffafa
	.word	65280                           ; 0xff00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	128                             ; 0x80
	.word	0                               ; 0x0
	.word	5597999                         ; 0x556b2f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15308410                        ; 0xe9967a
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	10040012                        ; 0x9932cc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16316671                        ; 0xf8f8ff
	.word	52945                           ; 0xced1
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8421376                         ; 0x808000
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16643558                        ; 0xfdf5e6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16776960                        ; 0xffff00
	.word	0                               ; 0x0
	.word	13468991                        ; 0xcd853f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	7372944                         ; 0x708090
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11584734                        ; 0xb0c4de
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14423100                        ; 0xdc143c
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14329120                        ; 0xdaa520
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16772045                        ; 0xffebcd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16770273                        ; 0xffe4e1
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16774638                        ; 0xfff5ee
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4772300                         ; 0x48d1cc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16767673                        ; 0xffdab9
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15787660                        ; 0xf0e68c
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	7372944                         ; 0x708090
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	10824234                        ; 0xa52a2a
	.word	8388352                         ; 0x7fff00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	7048739                         ; 0x6b8e23
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16766720                        ; 0xffd700
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14745599                        ; 0xe0ffff
	.word	0                               ; 0x0
	.word	6908265                         ; 0x696969
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	49151                           ; 0xbfff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8388608                         ; 0x800000
	.word	12632256                        ; 0xc0c0c0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9662683                         ; 0x9370db
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16738740                        ; 0xff69b4
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14204888                        ; 0xd8bfd8
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3329330                         ; 0x32cd32
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12092939                        ; 0xb8860b
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12211667                        ; 0xba55d3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16711680                        ; 0xff0000
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16032864                        ; 0xf4a460
	.word	0                               ; 0x0
	.word	13458524                        ; 0xcd5c5c
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16444375                        ; 0xfaebd7
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4915330                         ; 0x4b0082
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15657130                        ; 0xeee8aa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	13882323                        ; 0xd3d3d3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12357519                        ; 0xbc8f8f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11591910                        ; 0xb0e0e6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16758465                        ; 0xffb6c1
	.word	8900331                         ; 0x87ceeb
	.word	0                               ; 0x0
	.word	14524637                        ; 0xdda0dd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8388564                         ; 0x7fffd4
	.word	0                               ; 0x0
	.word	8087790                         ; 0x7b68ee
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16773365                        ; 0xfff0f5
	.word	13789470                        ; 0xd2691e
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9699539                         ; 0x9400d3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16777215                        ; 0xffffff
	.word	0                               ; 0x0
	.word	16775388                        ; 0xfff8dc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16770244                        ; 0xffe4c4
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16747520                        ; 0xff8c00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16761035                        ; 0xffc0cb
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	25600                           ; 0x6400
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15794175                        ; 0xf0ffff
	.word	10025880                        ; 0x98fb98
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16773077                        ; 0xffefd5
	.word	16744272                        ; 0xff7f50
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9109504                         ; 0x8b0000
	.word	0                               ; 0x0
	.word	15794160                        ; 0xf0fff0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16711935                        ; 0xff00ff
	.word	0                               ; 0x0
	.word	8388736                         ; 0x800080
	.word	0                               ; 0x0
	.word	16777200                        ; 0xfffff0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16753920                        ; 0xffa500
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	35723                           ; 0x8b8b
	.word	0                               ; 0x0
	.word	9419919                         ; 0x8fbc8f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11119017                        ; 0xa9a9a9
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8421504                         ; 0x808080
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0

ro__k_sz:
	.word 2048
ro__k_data:
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16729344                        ; 0xff4500
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	5597999                         ; 0x556b2f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11591910                        ; 0xb0e0e6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	10145074                        ; 0x9acd32
	.word	16767673                        ; 0xffdab9
	.word	9662683                         ; 0x9370db
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3329330                         ; 0x32cd32
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16744272                        ; 0xff7f50
	.word	0                               ; 0x0
	.word	16768685                        ; 0xffdead
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4772300                         ; 0x48d1cc
	.word	0                               ; 0x0
	.word	9109504                         ; 0x8b0000
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8900331                         ; 0x87ceeb
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	205                             ; 0xcd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	35723                           ; 0x8b8b
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12092939                        ; 0xb8860b
	.word	16777184                        ; 0xffffe0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12433259                        ; 0xbdb76b
	.word	139                             ; 0x8b
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	10506797                        ; 0xa0522d
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16776960                        ; 0xffff00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	2263842                         ; 0x228b22
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14596231                        ; 0xdeb887
	.word	0                               ; 0x0
	.word	52945                           ; 0xced1
	.word	0                               ; 0x0
	.word	16711680                        ; 0xff0000
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4286945                         ; 0x4169e1
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3978097                         ; 0x3cb371
	.word	16416882                        ; 0xfa8072
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	10824234                        ; 0xa52a2a
	.word	15308410                        ; 0xe9967a
	.word	0                               ; 0x0
	.word	16775388                        ; 0xfff8dc
	.word	16444375                        ; 0xfaebd7
	.word	16753920                        ; 0xffa500
	.word	0                               ; 0x0
	.word	16773365                        ; 0xfff0f5
	.word	0                               ; 0x0
	.word	9109643                         ; 0x8b008b
	.word	13882323                        ; 0xd3d3d3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14474460                        ; 0xdcdcdc
	.word	8388608                         ; 0x800000
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	65535                           ; 0xffff
	.word	11674146                        ; 0xb22222
	.word	0                               ; 0x0
	.word	6266528                         ; 0x5f9ea0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	4251856                         ; 0x40e0d0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	1644912                         ; 0x191970
	.word	8388564                         ; 0x7fffd4
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	255                             ; 0xff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16032864                        ; 0xf4a460
	.word	0                               ; 0x0
	.word	15794175                        ; 0xf0ffff
	.word	13468991                        ; 0xcd853f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	32896                           ; 0x8080
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16775885                        ; 0xfffacd
	.word	16775930                        ; 0xfffafa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15631086                        ; 0xee82ee
	.word	14524637                        ; 0xdda0dd
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16316671                        ; 0xf8f8ff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	6737322                         ; 0x66cdaa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15761536                        ; 0xf08080
	.word	14315734                        ; 0xda70d6
	.word	10040012                        ; 0x9932cc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	3100495                         ; 0x2f4f4f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16770244                        ; 0xffe4c4
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16758465                        ; 0xffb6c1
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	49151                           ; 0xbfff
	.word	16773077                        ; 0xffefd5
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14204888                        ; 0xd8bfd8
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	12632256                        ; 0xc0c0c0
	.word	9055202                         ; 0x8a2be2
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11529966                        ; 0xafeeee
	.word	0                               ; 0x0
	.word	8087790                         ; 0x7b68ee
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15787660                        ; 0xf0e68c
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16716947                        ; 0xff1493
	.word	16777215                        ; 0xffffff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16772045                        ; 0xffebcd
	.word	14423100                        ; 0xdc143c
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16747520                        ; 0xff8c00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11119017                        ; 0xa9a9a9
	.word	16711935                        ; 0xff00ff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9498256                         ; 0x90ee90
	.word	0                               ; 0x0
	.word	9699539                         ; 0x9400d3
	.word	0                               ; 0x0
	.word	2003199                         ; 0x1e90ff
	.word	65407                           ; 0xff7f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15792383                        ; 0xf0f8ff
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16119260                        ; 0xf5f5dc
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8421504                         ; 0x808080
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	32768                           ; 0x8000
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16766720                        ; 0xffd700
	.word	4620980                         ; 0x4682b4
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15794160                        ; 0xf0fff0
	.word	0                               ; 0x0
	.word	7048739                         ; 0x6b8e23
	.word	0                               ; 0x0
	.word	128                             ; 0x80
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8190976                         ; 0x7cfc00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	2142890                         ; 0x20b2aa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9127187                         ; 0x8b4513
	.word	0                               ; 0x0
	.word	6970061                         ; 0x6a5acd
	.word	3050327                         ; 0x2e8b57
	.word	0                               ; 0x0
	.word	16770273                        ; 0xffe4e1
	.word	13458524                        ; 0xcd5c5c
	.word	4915330                         ; 0x4b0082
	.word	12211667                        ; 0xba55d3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	13808780                        ; 0xd2b48c
	.word	13789470                        ; 0xd2691e
	.word	6908265                         ; 0x696969
	.word	0                               ; 0x0
	.word	16119285                        ; 0xf5f5f5
	.word	0                               ; 0x0
	.word	16752762                        ; 0xffa07a
	.word	7833753                         ; 0x778899
	.word	0                               ; 0x0
	.word	10025880                        ; 0x98fb98
	.word	0                               ; 0x0
	.word	4734347                         ; 0x483d8b
	.word	0                               ; 0x0
	.word	16770229                        ; 0xffe4b5
	.word	0                               ; 0x0
	.word	8900346                         ; 0x87cefa
	.word	0                               ; 0x0
	.word	16761035                        ; 0xffc0cb
	.word	0                               ; 0x0
	.word	14381203                        ; 0xdb7093
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	9419919                         ; 0x8fbc8f
	.word	0                               ; 0x0
	.word	25600                           ; 0x6400
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14745599                        ; 0xe0ffff
	.word	0                               ; 0x0
	.word	16737095                        ; 0xff6347
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	7372944                         ; 0x708090
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11584734                        ; 0xb0c4de
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16643558                        ; 0xfdf5e6
	.word	16121850                        ; 0xf5fffa
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	64154                           ; 0xfa9a
	.word	0                               ; 0x0
	.word	16448210                        ; 0xfafad2
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16777200                        ; 0xfffff0
	.word	0                               ; 0x0
	.word	16445670                        ; 0xfaf0e6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	14329120                        ; 0xdaa520
	.word	16113331                        ; 0xf5deb3
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8388352                         ; 0x7fff00
	.word	16774638                        ; 0xfff5ee
	.word	0                               ; 0x0
	.word	12357519                        ; 0xbc8f8f
	.word	0                               ; 0x0
	.word	11403055                        ; 0xadff2f
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	11393254                        ; 0xadd8e6
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	13047173                        ; 0xc71585
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16775920                        ; 0xfffaf0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	15657130                        ; 0xeee8aa
	.word	65280                           ; 0xff00
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8421376                         ; 0x808000
	.word	15132410                        ; 0xe6e6fa
	.word	6591981                         ; 0x6495ed
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	8388736                         ; 0x800080
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	0                               ; 0x0
	.word	16738740                        ; 0xff69b4

ro__v_sz:
	.word	1024
.align 2
ro__v_data:
	.hword	58                              ; 0x3a
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1025                            ; 0x401
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	253                             ; 0xfd
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1130                            ; 0x46a
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1360                            ; 0x550
	.hword	1105                            ; 0x451
	.hword	841                             ; 0x349
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	777                             ; 0x309
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	142                             ; 0x8e
	.hword	0                               ; 0x0
	.hword	977                             ; 0x3d1
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	903                             ; 0x387
	.hword	0                               ; 0x0
	.hword	290                             ; 0x122
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1234                            ; 0x4d2
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	817                             ; 0x331
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	189                             ; 0xbd
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	198                             ; 0xc6
	.hword	760                             ; 0x2f8
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	231                             ; 0xe7
	.hword	180                             ; 0xb4
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1220                            ; 0x4c4
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1353                            ; 0x549
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	437                             ; 0x1b5
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	101                             ; 0x65
	.hword	0                               ; 0x0
	.hword	350                             ; 0x15e
	.hword	0                               ; 0x0
	.hword	1148                            ; 0x47c
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1162                            ; 0x48a
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	854                             ; 0x356
	.hword	1184                            ; 0x4a0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	95                              ; 0x5f
	.hword	298                             ; 0x12a
	.hword	0                               ; 0x0
	.hword	163                             ; 0xa3
	.hword	10                              ; 0xa
	.hword	1018                            ; 0x3fa
	.hword	0                               ; 0x0
	.hword	571                             ; 0x23b
	.hword	0                               ; 0x0
	.hword	241                             ; 0xf1
	.hword	660                             ; 0x294
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	457                             ; 0x1c9
	.hword	793                             ; 0x319
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	23                              ; 0x17
	.hword	415                             ; 0x19f
	.hword	0                               ; 0x0
	.hword	111                             ; 0x6f
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1313                            ; 0x521
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	935                             ; 0x3a7
	.hword	28                              ; 0x1c
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	79                              ; 0x4f
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1191                            ; 0x4a7
	.hword	0                               ; 0x0
	.hword	39                              ; 0x27
	.hword	1115                            ; 0x45b
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1293                            ; 0x50d
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	595                             ; 0x253
	.hword	1262                            ; 0x4ee
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1323                            ; 0x52b
	.hword	1125                            ; 0x465
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	467                             ; 0x1d3
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	800                             ; 0x320
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	618                             ; 0x26a
	.hword	1035                            ; 0x40b
	.hword	279                             ; 0x117
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	336                             ; 0x150
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	51                              ; 0x33
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	681                             ; 0x2a9
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	384                             ; 0x180
	.hword	1094                            ; 0x446
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1298                            ; 0x512
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1227                            ; 0x4cb
	.hword	84                              ; 0x54
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1066                            ; 0x42a
	.hword	0                               ; 0x0
	.hword	869                             ; 0x365
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	556                             ; 0x22c
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	375                             ; 0x177
	.hword	1336                            ; 0x538
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	64                              ; 0x40
	.hword	172                             ; 0xac
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	268                             ; 0x10c
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	212                             ; 0xd4
	.hword	449                             ; 0x1c1
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	670                             ; 0x29e
	.hword	0                               ; 0x0
	.hword	364                             ; 0x16c
	.hword	0                               ; 0x0
	.hword	404                             ; 0x194
	.hword	1267                            ; 0x4f3
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	45                              ; 0x2d
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	493                             ; 0x1ed
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	498                             ; 0x1f2
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	478                             ; 0x1de
	.hword	1279                            ; 0x4ff
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	516                             ; 0x204
	.hword	0                               ; 0x0
	.hword	1008                            ; 0x3f0
	.hword	0                               ; 0x0
	.hword	989                             ; 0x3dd
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	585                             ; 0x249
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	703                             ; 0x2bf
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1172                            ; 0x494
	.hword	0                               ; 0x0
	.hword	1242                            ; 0x4da
	.hword	1202                            ; 0x4b2
	.hword	0                               ; 0x0
	.hword	958                             ; 0x3be
	.hword	533                             ; 0x215
	.hword	543                             ; 0x21f
	.hword	828                             ; 0x33c
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1289                            ; 0x509
	.hword	132                             ; 0x84
	.hword	396                             ; 0x18c
	.hword	0                               ; 0x0
	.hword	1342                            ; 0x53e
	.hword	0                               ; 0x0
	.hword	691                             ; 0x2b3
	.hword	730                             ; 0x2da
	.hword	0                               ; 0x0
	.hword	1056                            ; 0x420
	.hword	0                               ; 0x0
	.hword	322                             ; 0x142
	.hword	0                               ; 0x0
	.hword	968                             ; 0x3c8
	.hword	0                               ; 0x0
	.hword	717                             ; 0x2cd
	.hword	0                               ; 0x0
	.hword	1120                            ; 0x460
	.hword	0                               ; 0x0
	.hword	1080                            ; 0x438
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	309                             ; 0x135
	.hword	0                               ; 0x0
	.hword	221                             ; 0xdd
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	629                             ; 0x275
	.hword	0                               ; 0x0
	.hword	1306                            ; 0x51a
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1252                            ; 0x4e4
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	745                             ; 0x2e9
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	994                             ; 0x3e2
	.hword	948                             ; 0x3b4
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	885                             ; 0x375
	.hword	0                               ; 0x0
	.hword	639                             ; 0x27f
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	550                             ; 0x226
	.hword	0                               ; 0x0
	.hword	787                             ; 0x313
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	483                             ; 0x1e3
	.hword	1330                            ; 0x532
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	121                             ; 0x79
	.hword	1211                            ; 0x4bb
	.hword	0                               ; 0x0
	.hword	1152                            ; 0x480
	.hword	0                               ; 0x0
	.hword	504                             ; 0x1f8
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	608                             ; 0x260
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	919                             ; 0x397
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	425                             ; 0x1a9
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1042                            ; 0x412
	.hword	772                             ; 0x304
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1002                            ; 0x3ea
	.hword	562                             ; 0x232
	.hword	148                             ; 0x94
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	1141                            ; 0x475
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	0                               ; 0x0
	.hword	525                             ; 0x20d

.align 4
ro__n_sz:
	.word	1372

.align 1
ro__n_data:
	.byte	97
	.byte	108
	.byte	105
	.byte	99
	.byte	101
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	97
	.byte	110
	.byte	116
	.byte	105
	.byte	113
	.byte	117
	.byte	101
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	0
	.byte	97
	.byte	113
	.byte	117
	.byte	97
	.byte	0
	.byte	97
	.byte	113
	.byte	117
	.byte	97
	.byte	109
	.byte	97
	.byte	114
	.byte	105
	.byte	110
	.byte	101
	.byte	0
	.byte	97
	.byte	122
	.byte	117
	.byte	114
	.byte	101
	.byte	0
	.byte	98
	.byte	101
	.byte	105
	.byte	103
	.byte	101
	.byte	0
	.byte	98
	.byte	105
	.byte	115
	.byte	113
	.byte	117
	.byte	101
	.byte	0
	.byte	98
	.byte	108
	.byte	97
	.byte	99
	.byte	107
	.byte	0
	.byte	98
	.byte	108
	.byte	97
	.byte	110
	.byte	99
	.byte	104
	.byte	101
	.byte	100
	.byte	97
	.byte	108
	.byte	109
	.byte	111
	.byte	110
	.byte	100
	.byte	0
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	118
	.byte	105
	.byte	111
	.byte	108
	.byte	101
	.byte	116
	.byte	0
	.byte	98
	.byte	114
	.byte	111
	.byte	119
	.byte	110
	.byte	0
	.byte	98
	.byte	117
	.byte	114
	.byte	108
	.byte	121
	.byte	119
	.byte	111
	.byte	111
	.byte	100
	.byte	0
	.byte	99
	.byte	97
	.byte	100
	.byte	101
	.byte	116
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	99
	.byte	104
	.byte	97
	.byte	114
	.byte	116
	.byte	114
	.byte	101
	.byte	117
	.byte	115
	.byte	101
	.byte	0
	.byte	99
	.byte	104
	.byte	111
	.byte	99
	.byte	111
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	0
	.byte	99
	.byte	111
	.byte	114
	.byte	97
	.byte	108
	.byte	0
	.byte	99
	.byte	111
	.byte	114
	.byte	110
	.byte	102
	.byte	108
	.byte	111
	.byte	119
	.byte	101
	.byte	114
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	99
	.byte	111
	.byte	114
	.byte	110
	.byte	115
	.byte	105
	.byte	108
	.byte	107
	.byte	0
	.byte	99
	.byte	114
	.byte	105
	.byte	109
	.byte	115
	.byte	111
	.byte	110
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	99
	.byte	121
	.byte	97
	.byte	110
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	103
	.byte	111
	.byte	108
	.byte	100
	.byte	101
	.byte	110
	.byte	114
	.byte	111
	.byte	100
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	107
	.byte	104
	.byte	97
	.byte	107
	.byte	105
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	109
	.byte	97
	.byte	103
	.byte	101
	.byte	110
	.byte	116
	.byte	97
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	111
	.byte	108
	.byte	105
	.byte	118
	.byte	101
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	111
	.byte	114
	.byte	97
	.byte	110
	.byte	103
	.byte	101
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	111
	.byte	114
	.byte	99
	.byte	104
	.byte	105
	.byte	100
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	114
	.byte	101
	.byte	100
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	115
	.byte	97
	.byte	108
	.byte	109
	.byte	111
	.byte	110
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	115
	.byte	101
	.byte	97
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	115
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	115
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	116
	.byte	117
	.byte	114
	.byte	113
	.byte	117
	.byte	111
	.byte	105
	.byte	115
	.byte	101
	.byte	0
	.byte	100
	.byte	97
	.byte	114
	.byte	107
	.byte	118
	.byte	105
	.byte	111
	.byte	108
	.byte	101
	.byte	116
	.byte	0
	.byte	100
	.byte	101
	.byte	101
	.byte	112
	.byte	112
	.byte	105
	.byte	110
	.byte	107
	.byte	0
	.byte	100
	.byte	101
	.byte	101
	.byte	112
	.byte	115
	.byte	107
	.byte	121
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	100
	.byte	105
	.byte	109
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	100
	.byte	111
	.byte	100
	.byte	103
	.byte	101
	.byte	114
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	102
	.byte	105
	.byte	114
	.byte	101
	.byte	98
	.byte	114
	.byte	105
	.byte	99
	.byte	107
	.byte	0
	.byte	102
	.byte	108
	.byte	111
	.byte	114
	.byte	97
	.byte	108
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	0
	.byte	102
	.byte	111
	.byte	114
	.byte	101
	.byte	115
	.byte	116
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	102
	.byte	117
	.byte	99
	.byte	104
	.byte	115
	.byte	105
	.byte	97
	.byte	0
	.byte	103
	.byte	97
	.byte	105
	.byte	110
	.byte	115
	.byte	98
	.byte	111
	.byte	114
	.byte	111
	.byte	0
	.byte	103
	.byte	104
	.byte	111
	.byte	115
	.byte	116
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	0
	.byte	103
	.byte	111
	.byte	108
	.byte	100
	.byte	0
	.byte	103
	.byte	111
	.byte	108
	.byte	100
	.byte	101
	.byte	110
	.byte	114
	.byte	111
	.byte	100
	.byte	0
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	121
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	119
	.byte	0
	.byte	104
	.byte	111
	.byte	110
	.byte	101
	.byte	121
	.byte	100
	.byte	101
	.byte	119
	.byte	0
	.byte	104
	.byte	111
	.byte	116
	.byte	112
	.byte	105
	.byte	110
	.byte	107
	.byte	0
	.byte	105
	.byte	110
	.byte	100
	.byte	105
	.byte	97
	.byte	110
	.byte	114
	.byte	101
	.byte	100
	.byte	0
	.byte	105
	.byte	110
	.byte	100
	.byte	105
	.byte	103
	.byte	111
	.byte	0
	.byte	105
	.byte	118
	.byte	111
	.byte	114
	.byte	121
	.byte	0
	.byte	107
	.byte	104
	.byte	97
	.byte	107
	.byte	105
	.byte	0
	.byte	108
	.byte	97
	.byte	118
	.byte	101
	.byte	110
	.byte	100
	.byte	101
	.byte	114
	.byte	0
	.byte	108
	.byte	97
	.byte	118
	.byte	101
	.byte	110
	.byte	100
	.byte	101
	.byte	114
	.byte	98
	.byte	108
	.byte	117
	.byte	115
	.byte	104
	.byte	0
	.byte	108
	.byte	97
	.byte	119
	.byte	110
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	108
	.byte	101
	.byte	109
	.byte	111
	.byte	110
	.byte	99
	.byte	104
	.byte	105
	.byte	102
	.byte	102
	.byte	111
	.byte	110
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	99
	.byte	111
	.byte	114
	.byte	97
	.byte	108
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	99
	.byte	121
	.byte	97
	.byte	110
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	103
	.byte	111
	.byte	108
	.byte	100
	.byte	101
	.byte	110
	.byte	114
	.byte	111
	.byte	100
	.byte	121
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	119
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	112
	.byte	105
	.byte	110
	.byte	107
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	115
	.byte	97
	.byte	108
	.byte	109
	.byte	111
	.byte	110
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	115
	.byte	101
	.byte	97
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	115
	.byte	107
	.byte	121
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	115
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	115
	.byte	116
	.byte	101
	.byte	101
	.byte	108
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	108
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	121
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	119
	.byte	0
	.byte	108
	.byte	105
	.byte	109
	.byte	101
	.byte	0
	.byte	108
	.byte	105
	.byte	109
	.byte	101
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	108
	.byte	105
	.byte	110
	.byte	101
	.byte	110
	.byte	0
	.byte	109
	.byte	97
	.byte	114
	.byte	111
	.byte	111
	.byte	110
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	97
	.byte	113
	.byte	117
	.byte	97
	.byte	109
	.byte	97
	.byte	114
	.byte	105
	.byte	110
	.byte	101
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	111
	.byte	114
	.byte	99
	.byte	104
	.byte	105
	.byte	100
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	112
	.byte	117
	.byte	114
	.byte	112
	.byte	108
	.byte	101
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	115
	.byte	101
	.byte	97
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	115
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	115
	.byte	112
	.byte	114
	.byte	105
	.byte	110
	.byte	103
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	116
	.byte	117
	.byte	114
	.byte	113
	.byte	117
	.byte	111
	.byte	105
	.byte	115
	.byte	101
	.byte	0
	.byte	109
	.byte	101
	.byte	100
	.byte	105
	.byte	117
	.byte	109
	.byte	118
	.byte	105
	.byte	111
	.byte	108
	.byte	101
	.byte	116
	.byte	114
	.byte	101
	.byte	100
	.byte	0
	.byte	109
	.byte	105
	.byte	100
	.byte	110
	.byte	105
	.byte	103
	.byte	104
	.byte	116
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	109
	.byte	105
	.byte	110
	.byte	116
	.byte	99
	.byte	114
	.byte	101
	.byte	97
	.byte	109
	.byte	0
	.byte	109
	.byte	105
	.byte	115
	.byte	116
	.byte	121
	.byte	114
	.byte	111
	.byte	115
	.byte	101
	.byte	0
	.byte	109
	.byte	111
	.byte	99
	.byte	99
	.byte	97
	.byte	115
	.byte	105
	.byte	110
	.byte	0
	.byte	110
	.byte	97
	.byte	118
	.byte	97
	.byte	106
	.byte	111
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	0
	.byte	110
	.byte	97
	.byte	118
	.byte	121
	.byte	0
	.byte	111
	.byte	108
	.byte	100
	.byte	108
	.byte	97
	.byte	99
	.byte	101
	.byte	0
	.byte	111
	.byte	108
	.byte	105
	.byte	118
	.byte	101
	.byte	0
	.byte	111
	.byte	108
	.byte	105
	.byte	118
	.byte	101
	.byte	100
	.byte	114
	.byte	97
	.byte	98
	.byte	0
	.byte	111
	.byte	114
	.byte	97
	.byte	110
	.byte	103
	.byte	101
	.byte	0
	.byte	111
	.byte	114
	.byte	97
	.byte	110
	.byte	103
	.byte	101
	.byte	114
	.byte	101
	.byte	100
	.byte	0
	.byte	111
	.byte	114
	.byte	99
	.byte	104
	.byte	105
	.byte	100
	.byte	0
	.byte	112
	.byte	97
	.byte	108
	.byte	101
	.byte	103
	.byte	111
	.byte	108
	.byte	100
	.byte	101
	.byte	110
	.byte	114
	.byte	111
	.byte	100
	.byte	0
	.byte	112
	.byte	97
	.byte	108
	.byte	101
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	112
	.byte	97
	.byte	108
	.byte	101
	.byte	116
	.byte	117
	.byte	114
	.byte	113
	.byte	117
	.byte	111
	.byte	105
	.byte	115
	.byte	101
	.byte	0
	.byte	112
	.byte	97
	.byte	108
	.byte	101
	.byte	118
	.byte	105
	.byte	111
	.byte	108
	.byte	101
	.byte	116
	.byte	114
	.byte	101
	.byte	100
	.byte	0
	.byte	112
	.byte	97
	.byte	112
	.byte	97
	.byte	121
	.byte	97
	.byte	119
	.byte	104
	.byte	105
	.byte	112
	.byte	0
	.byte	112
	.byte	101
	.byte	97
	.byte	99
	.byte	104
	.byte	112
	.byte	117
	.byte	102
	.byte	102
	.byte	0
	.byte	112
	.byte	101
	.byte	114
	.byte	117
	.byte	0
	.byte	112
	.byte	105
	.byte	110
	.byte	107
	.byte	0
	.byte	112
	.byte	108
	.byte	117
	.byte	109
	.byte	0
	.byte	112
	.byte	111
	.byte	119
	.byte	100
	.byte	101
	.byte	114
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	112
	.byte	117
	.byte	114
	.byte	112
	.byte	108
	.byte	101
	.byte	0
	.byte	114
	.byte	101
	.byte	100
	.byte	0
	.byte	114
	.byte	111
	.byte	115
	.byte	121
	.byte	98
	.byte	114
	.byte	111
	.byte	119
	.byte	110
	.byte	0
	.byte	114
	.byte	111
	.byte	121
	.byte	97
	.byte	108
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	115
	.byte	97
	.byte	100
	.byte	100
	.byte	108
	.byte	101
	.byte	98
	.byte	114
	.byte	111
	.byte	119
	.byte	110
	.byte	0
	.byte	115
	.byte	97
	.byte	108
	.byte	109
	.byte	111
	.byte	110
	.byte	0
	.byte	115
	.byte	97
	.byte	110
	.byte	100
	.byte	121
	.byte	98
	.byte	114
	.byte	111
	.byte	119
	.byte	110
	.byte	0
	.byte	115
	.byte	101
	.byte	97
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	115
	.byte	101
	.byte	97
	.byte	115
	.byte	104
	.byte	101
	.byte	108
	.byte	108
	.byte	0
	.byte	115
	.byte	105
	.byte	101
	.byte	110
	.byte	110
	.byte	97
	.byte	0
	.byte	115
	.byte	105
	.byte	108
	.byte	118
	.byte	101
	.byte	114
	.byte	0
	.byte	115
	.byte	107
	.byte	121
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	115
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	115
	.byte	108
	.byte	97
	.byte	116
	.byte	101
	.byte	103
	.byte	114
	.byte	97
	.byte	121
	.byte	0
	.byte	115
	.byte	110
	.byte	111
	.byte	119
	.byte	0
	.byte	115
	.byte	112
	.byte	114
	.byte	105
	.byte	110
	.byte	103
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0
	.byte	115
	.byte	116
	.byte	101
	.byte	101
	.byte	108
	.byte	98
	.byte	108
	.byte	117
	.byte	101
	.byte	0
	.byte	116
	.byte	97
	.byte	110
	.byte	0
	.byte	116
	.byte	101
	.byte	97
	.byte	108
	.byte	0
	.byte	116
	.byte	104
	.byte	105
	.byte	115
	.byte	116
	.byte	108
	.byte	101
	.byte	0
	.byte	116
	.byte	111
	.byte	109
	.byte	97
	.byte	116
	.byte	111
	.byte	0
	.byte	116
	.byte	117
	.byte	114
	.byte	113
	.byte	117
	.byte	111
	.byte	105
	.byte	115
	.byte	101
	.byte	0
	.byte	118
	.byte	105
	.byte	111
	.byte	108
	.byte	101
	.byte	116
	.byte	0
	.byte	119
	.byte	104
	.byte	101
	.byte	97
	.byte	116
	.byte	0
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	0
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	115
	.byte	109
	.byte	111
	.byte	107
	.byte	101
	.byte	0
	.byte	121
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	119
	.byte	0
	.byte	121
	.byte	101
	.byte	108
	.byte	108
	.byte	111
	.byte	119
	.byte	103
	.byte	114
	.byte	101
	.byte	101
	.byte	110
	.byte	0

.align 4
ro__str_sz:
	.word	6 ;15

.align 1
ro__str_data:
	.byte	119
	.byte	104
	.byte	105
	.byte	116
	.byte	101
	.byte	0

	; .byte	67
	; .byte	79
	; .byte	82
	; .byte	78
	; .byte	102
	; .byte	108
	; .byte	111
	; .byte	119
	; .byte	101
	; .byte	114
	; .byte	66
	; .byte	76
	; .byte	85
	; .byte	69
	; .byte	0
