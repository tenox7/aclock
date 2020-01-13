
	.area _CODE

_get_time::

	push hl
	pop iy
	
	ld c,#0x2c
	call 5

	ld 0(iy),h
	ld 1(iy),l
	ld 2(iy),d
	ld 3(iy),e

	push iy
	pop hl

	ret

