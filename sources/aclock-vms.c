/*
 * aclock - ascii clock for OpenVMS
 *
 * Copyright (c) 2002 Antoni Sawicki <as@tenoware.com>
 * Version 2.4 (vms-smg); Mountain View, Jan 2026
 *
 * Compilation: $ cc aclock-vms
 *              $ link aclock-vms
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <descrip.h>
#include <smgdef.h>
#include <smg$routines.h>
#include <lib$routines.h>
#include <ssdef.h>

#define FONT_WH_RATIO 2

static unsigned long pb_id = 0;
static unsigned long disp_id = 0;
static unsigned long kb_id = 0;
static unsigned long rows, cols;

static void cleanup(void) {
    if (disp_id) smg$delete_virtual_display(&disp_id);
    if (pb_id) smg$delete_pasteboard(&pb_id, 0);
    if (kb_id) smg$delete_virtual_keyboard(&kb_id);
}

static void put_char(int row, int col, char c) {
    struct dsc$descriptor_s dsc;
    char buf[2];
    long r = row, c2 = col;
    buf[0] = c;
    buf[1] = '\0';
    dsc.dsc$w_length = 1;
    dsc.dsc$b_dtype = DSC$K_DTYPE_T;
    dsc.dsc$b_class = DSC$K_CLASS_S;
    dsc.dsc$a_pointer = buf;
    smg$put_chars(&disp_id, &dsc, &r, &c2, 0, 0, 0, 0);
}

static void put_str(int row, int col, char *str) {
    struct dsc$descriptor_s dsc;
    long r = row, c2 = col;
    dsc.dsc$w_length = strlen(str);
    dsc.dsc$b_dtype = DSC$K_DTYPE_T;
    dsc.dsc$b_class = DSC$K_CLASS_S;
    dsc.dsc$a_pointer = str;
    smg$put_chars(&disp_id, &dsc, &r, &c2, 0, 0, 0, 0);
}

static void draw_circle(int hand_max, int ycen, int xcen) {
    int r, x, y;
    char c;
    double angle;

    for (r = 0; r < 60; r++) {
        angle = r * M_PI / 30.0;
        x = (int)(cos(angle) * hand_max * FONT_WH_RATIO + xcen);
        y = (int)(sin(angle) * hand_max + ycen);
        c = (r % 5 == 0) ? 'o' : '.';
        put_char(y, x, c);
    }
}

static void draw_hand(int minute, int length, char c, int xcen, int ycen) {
    int n, x, y;
    double angle = (minute - 15) * M_PI / 30.0;

    for (n = 1; n < length; n++) {
        x = (int)(cos(angle) * n * FONT_WH_RATIO + xcen);
        y = (int)(sin(angle) * n + ycen);
        put_char(y, x, c);
    }
}

int main(int argc, char **argv) {
    unsigned long status;
    int smax, hand_max, xcen, ycen;
    time_t t;
    struct tm *ltime;
    char timebuf[16];
    long one = 1;

    status = smg$create_pasteboard(&pb_id, 0, &rows, &cols, 0);
    if (!(status & 1)) {
        printf("Failed to create pasteboard: %d\n", status);
        return status;
    }

    status = smg$create_virtual_display(&rows, &cols, &disp_id, 0, 0, 0);
    if (!(status & 1)) {
        cleanup();
        printf("Failed to create virtual display: %d\n", status);
        return status;
    }

    status = smg$create_virtual_keyboard(&kb_id, 0, 0, 0, 0);
    if (!(status & 1)) {
        cleanup();
        printf("Failed to create virtual keyboard: %d\n", status);
        return status;
    }

    status = smg$paste_virtual_display(&disp_id, &pb_id, &one, &one);
    if (!(status & 1)) {
        cleanup();
        printf("Failed to paste virtual display: %d\n", status);
        return status;
    }

    for (;;) {
        time(&t);
        ltime = localtime(&t);

        if ((int)cols / FONT_WH_RATIO <= (int)rows)
            smax = cols / FONT_WH_RATIO;
        else
            smax = rows;

        hand_max = (smax / 2) - 1;
        xcen = cols / 2;
        ycen = rows / 2;

        smg$erase_display(&disp_id, 0, 0, 0, 0);

        draw_circle(hand_max, ycen, xcen);
        draw_hand((ltime->tm_hour % 12) * 5 + ltime->tm_min / 12,
                  2 * hand_max / 3, 'h', xcen, ycen);
        draw_hand(ltime->tm_min, hand_max - 2, 'm', xcen, ycen);
        draw_hand(ltime->tm_sec, hand_max - 1, '.', xcen, ycen);

        put_str(ycen - (3 * hand_max / 5), xcen - 5, ".:ACLOCK:.");
        sprintf(timebuf, "[%02d:%02d:%02d]",
                ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
        put_str(ycen + (3 * hand_max / 5), xcen - 5, timebuf);

        smg$repaint_screen(&pb_id);

        sleep(1);
    }

    cleanup();
    return SS$_NORMAL;
}
