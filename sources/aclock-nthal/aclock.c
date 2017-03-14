//
// aclock - ascii clock for Windows NT HAL
//
// Copyright (c) 2002-2012 by Antoni Sawicki <as@tenoware.com>
// Copyright (c) 2012 by Tomasz Nowak <tn@tenoware.com>
// Version 2.0 (NTHAL); Mountain View, October 2012
//
// Compilation: build -Z from NTDDK cmd
//

#include "ntddk.h"
#include "stdio.h"

#define FontWH_Ratio 1  

int cosa[]={    0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995,-1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105 };
int sina[]={ -1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105,   0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995 };

VOID HalDisplayString(PSZ text);
VOID HalSetDisplayParameters (IN ULONG CursorColumn, IN ULONG CursorRow);
VOID HalQueryDisplayParameters (OUT PULONG WidthInCharacters, OUT PULONG HeightInLines, OUT PULONG CursorColumn, OUT PULONG CursorRow);

NTSTATUS 
draw_point(
    IN int x, 
    IN int y, 
    IN char p
) 
{
  char text[2];

  HalSetDisplayParameters(x,y);
  text[0]=p;
  text[1]='\0';
  HalDisplayString(text);

  return STATUS_SUCCESS;
}



NTSTATUS 
draw_text(
    IN PSZ text, 
    IN int x,
    IN int y
    ) 
{
  HalSetDisplayParameters(x,y);
  HalDisplayString(text);

  return STATUS_SUCCESS;
}



NTSTATUS 
draw_circle(
    IN int hand_max, 
    IN int sYcen, 
    IN int sXcen
    )
{
  int x,y,r;
  char c;

  for(r=0;r<60;r++){
    x=(cosa[r]*hand_max*FontWH_Ratio/1000)+sXcen;
    y=(sina[r]*hand_max/1000)+sYcen;
    switch (r) {
      case 0:
      case 5:
      case 10:
      case 15:
      case 20:
      case 25:
      case 30:
      case 35:
      case 40:
      case 45:
      case 50:
      case 55:
        c='o';
        break;
      default:
        c='.';
        break;
    }
    draw_point(x,y,c);
  }

  return STATUS_SUCCESS;
}



NTSTATUS 
draw_hand(
    IN int minute, 
    IN int hlenght, 
    IN char c, 
    IN int sXcen, 
    IN int sYcen
    )
{
  int x,y,n;

  for(n=1; n<hlenght; n++)
  {
    x=(cosa[minute]*n*FontWH_Ratio/1000)+sXcen;
    y=(sina[minute]*n/1000)+sYcen;
    draw_point(x,y,c);
  }
  
  return STATUS_SUCCESS;
}



NTSTATUS 
DriverEntry( 
    IN PDRIVER_OBJECT  *DriverObject,
    IN PUNICODE_STRING  RegistryPath 
    )
{
  char digital_time[15];
  int sXmax, sYmax, smax, hand_max, sXcen, sYcen, x, y;
  LARGE_INTEGER interval;
  LARGE_INTEGER st, lt;
  TIME_FIELDS tf;
  
  interval.QuadPart = -10000000;

  HalQueryDisplayParameters(&sXmax, &sYmax, &x, &y);
  HalDisplayString("\n");

  if(sXmax/FontWH_Ratio <= sYmax)
    smax=sXmax/FontWH_Ratio;
  else
    smax=sYmax;

  hand_max = (smax/2)-1;

  sXcen = sXmax/2;
  sYcen = sYmax/2;

  while(1) 
  {
    KeQuerySystemTime(&st);
    ExSystemTimeToLocalTime(&st, &lt);
    RtlTimeToTimeFields(&lt, &tf);

    draw_circle(hand_max, sYcen, sXcen);
    
    draw_hand(tf.Hour%12*5+tf.Minute/12, 2*hand_max/3, 'h', sXcen, sYcen);
    draw_hand(tf.Minute, hand_max-2, 'm', sXcen, sYcen);
    draw_hand(tf.Second, hand_max-1, '.', sXcen, sYcen);

    draw_text(".:ACLOCK:.", sXcen-5, sYcen-(3*hand_max/5));
    sprintf(digital_time, "[%02d:%02d:%02d]", tf.Hour, tf.Minute, tf.Second);
    draw_text(digital_time, sXcen-5, sYcen+(3*hand_max/5));
  
    KeDelayExecutionThread(KernelMode, FALSE, &interval);

    draw_hand(tf.Hour%12*5+tf.Minute/12, 2*hand_max/3, ' ', sXcen, sYcen);
    draw_hand(tf.Minute, hand_max-2, ' ', sXcen, sYcen);
    draw_hand(tf.Second, hand_max-1, ' ', sXcen, sYcen);
  }
  
  return STATUS_SUCCESS;
}
