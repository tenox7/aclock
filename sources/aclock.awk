#!/usr/bin/nawk -f
#
# aclock.awk Version 1.4 AWK-VT100 15 Jul 2001 by Antoni Sawicki
#

BEGIN {
	REFRESH=1
	FONTHW=2

	DATECMD="date '+%H %M %S'"

	"tput lines" | getline 
	close("tput lines")
	SYMAX=int($0)

	"tput cols" | getline 
	close("tput lines")
	SXMAX=int($0)

	if(SXMAX<=SYMAX)
		SMAX=SXMAX
	else
		SMAX=SYMAX

	AMAX=SMAX/2-2
	SYCEN=int(SYMAX/2)
	SXCEN=int(SXMAX/2)

	for(;;){
		DATECMD|getline
		close(DATECMD)

		clear_screen()
		draw_circle()

		draw_hand($3,AMAX-1,".")
		draw_hand($1*5+int($2/10),2*AMAX/3,"H")
		draw_hand($2,AMAX-2,"M")

		printf("\033[%d;%dH.:ACLOCK:.",int(SYMAX/3),SXCEN-5)
		printf("\033[%d;%dH%s:%s:%s",int(2*SYMAX/3),SXCEN-4,$1,$2,$3)

		printf("\033[H")

		system("sleep " REFRESH)
	}

}

function clear_screen() {
	system("tput clear")
}

function draw_point(x,y,s) {
 	printf("\033[%d;%dH%s", y, x, s)
}

function draw_hand(c,al,s) {
		r=(c-15)*(3.14/180)*6

		for(l=1; l<al; l++) {
			Y=int(sin(r)*l+SYCEN)
			X=int(cos(r)*l*FONTHW+SXCEN)
			draw_point(X,Y,s)
		}
}

function draw_circle() {
	for(r=0;r<60;r++) {
		Y=int(sin(r*3.14/180*6)*AMAX+SYCEN)
		X=int(cos(r*3.14/180*6)*AMAX*FONTHW+SXCEN)
		if(r==0  || r==5  || r==10 || r==15 || r==20 || r==25\
		|| r==30 || r==35 || r==40 || r==45 || r==50 || r==55)
			draw_point(X,Y,"*")
		else
			draw_point(X,Y,".")

	}
}


