CAT=cat
CC=sdcc
ECHO=echo
HEXBIN=./hexbin.py
RM=rm

INFILE=aclock.c
OUTFILE=aclock

INCLUDE=-I../msx
PARAMS=-mz80 --no-std-crt0 --data-loc 0
NORMAL=--code-loc 0x107 ../msx/crt0msx_msxdos.rel
ADVANCED=--code-loc 0x178 ../msx/crt0msx_msxdos_advanced.rel
RELOCATE=../msx/putchar.rel ../msx/getchar.rel ../msx/dos.rel ../msx/conio.rel get_time.rel

.PHONY: normal advanced clean superclean

default:
	make normal

normal:
	${CC} ${INCLUDE} ${PARAMS} ${NORMAL} ${RELOCATE} ${INFILE}
	${HEXBIN} ${OUTFILE}.ihx >${OUTFILE}.com
advanced:
	${CC} ${INCLUDE} ${PARAMS} ${ADVANCED} ${RELOCATE} ${INFILE}
	${HEXBIN} ${OUTFILE}.ihx >${OUTFILE}.com

clean:
	${RM} ${OUTFILE}.com ${OUTFILE}.ihx

superclean:
	${RM} -f ${OUTFILE}.com ${OUTFILE}.ihx ${OUTFILE}.asm\
		${OUTFILE}.lk ${OUTFILE}.lst ${OUTFILE}.map\
		${OUTFILE}.noi ${OUTFILE}.rel ${OUTFILE}.sym

