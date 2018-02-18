

	.area _CODE


_get_time::

	push hl
	pop ix

	ld c,#0x2c
	call 5

	ld (ix),h
	ld 1(ix),l
	ld 2(ix),d
	ld 3(ix),e

	push ix
	pop hl

	ret

