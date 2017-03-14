#ifndef lint
static char sccsid[] = "@(#)semaphore.c	(ISTS) 8/27/90";
#endif /* not lint */

# include	<vxWorks.h>
# include	<semLib.h>
# include	<taskLib.h>

# include	"curses.ext"

/*
 * These are the semaphore P (take) and V (give) routines.vxman semClear
 The extra handling
 * of lock_id and lock_skp allows "locked" routines to be called from
 * within other "locked" routines, even recursive calls.
 */

static SEM_ID  curses_lock;     /* Curses global locking semaphore */

int curses_seminit()
{
   curses_lock=semMCreate(SEM_Q_FIFO|SEM_DELETE_SAFE);
   if (curses_lock==NULL)
      return(ERR);

   return(OK);
}

int curses_semshutdown()
{
   semDelete(curses_lock);
   return(OK);
}

STATUS curses_P()
{
    return(semTake(curses_lock,WAIT_FOREVER));
}

STATUS curses_V()
{
   return(semGive(curses_lock));
}
