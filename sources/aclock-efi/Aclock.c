/** @file
  Aclock is an analog clock program for text mode console displays, terminals, or terminal emulators.
  This program is obviously absolutely useless, except for turning your old, expensive mainframe or
  supercomputer into a wall clock.

  Copyright (c) 1994-2016 Antoni Sawicki <as@tenoware.com>
  Copyright (c) 2016 Natalia Portillo <claunia@claunia.com>
**/

#ifdef GNU_EFI

#include <efi.h>
#include <efilib.h>

#else

#include <Uefi.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <math.h>
#include <unistd.h>

#endif

// GNU EFI does not include libm, so use precomputed values
#ifdef GNU_EFI

int cosa[]={    0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995,-1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105 };
int sina[]={ -1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105,   0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995 };

#else

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#endif

#define FontWH_Ratio 2

// Global variables
SIMPLE_TEXT_OUTPUT_INTERFACE *ConOut;
EFI_SYSTEM_TABLE *eST;

void draw_circle(int hand_max, int sYcen, int sXcen){
    int x,y,r;
    char c;

    for(r=0;r<60;r++){
#ifdef GNU_EFI
        x=(cosa[r]*hand_max*FontWH_Ratio/1000)+sXcen;
        y=(sina[r]*hand_max/1000)+sYcen;
#else
        x=cos(r*M_PI/180*6)*hand_max*FontWH_Ratio+sXcen;
        y=sin(r*M_PI/180*6)*hand_max+sYcen;
#endif
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
#ifdef GNU_EFI
        uefi_call_wrapper(ConOut->SetCursorPosition, 3, ConOut, x, y);
#else
        ConOut->SetCursorPosition(ConOut, x, y);
#endif
        Print(L"%c", c);
    }
}

void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen){
    int x,y,n;

    for(n=1; n<hlenght; n++){
#ifdef GNU_EFI
        x=(cosa[minute]*n*FontWH_Ratio/1000)+sXcen;
        y=(sina[minute]*n/1000)+sYcen;
        uefi_call_wrapper(ConOut->SetCursorPosition, 3, ConOut, x, y);
#else
        float r=(minute-15)*(M_PI/180)*6;
        x=cos(r)*n*FontWH_Ratio+sXcen;
        y=sin(r)*n+sYcen;
        ConOut->SetCursorPosition(ConOut, x, y);
#endif
        Print(L"%c", c);
    }
}

/**
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
#ifdef GNU_EFI
efi_main (EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE * SystemTable)
#else
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
#endif
{
#ifdef GNU_EFI
    InitializeLib(ImageHandle, SystemTable);
#endif
    ConOut = SystemTable->ConOut;
    eST = SystemTable;
    EFI_TIME ltime;

    /*char INFO[]="Copyright (c) 1994-2016 Antoni Sawicki <as@tenoware.com>\n"
                "Copyright (c) 2016 Natalia Portillo <claunia@claunia.com>\n"
                "Version 1.1 (efi); Canary Islands, October 2016\n";*/
    int sXmax, sYmax, smax, hand_max, sXcen, sYcen;

    sXmax=sYmax=hand_max=sXcen=sYcen=0;
#ifdef GNU_EFI
    uefi_call_wrapper(ConOut->ClearScreen, 1, ConOut);
#else
    ConOut->ClearScreen(ConOut);
#endif

    while(1){
#ifdef GNU_EFI
        uefi_call_wrapper(eST->RuntimeServices->GetTime, 2, &ltime, NULL);
#else
        eST->RuntimeServices->GetTime(&ltime, NULL);
#endif

	UINTN Columns, Rows;
#ifdef GNU_EFI
        uefi_call_wrapper(ConOut->QueryMode, 4, ConOut, ConOut->Mode->Mode, &Columns, &Rows);
#else
        ConOut->QueryMode(ConOut, ConOut->Mode->Mode, &Columns, &Rows);
#endif
        sYmax = (int)Rows;
        sXmax = (int)Columns;

        if(sXmax/FontWH_Ratio<=sYmax)
            smax=sXmax/FontWH_Ratio;
        else
            smax=sYmax;

        hand_max = (smax/2)-1;

        sXcen = sXmax/2;
        sYcen = sYmax/2;

#ifdef GNU_EFI
        uefi_call_wrapper(ConOut->ClearScreen, 1, ConOut);
#else
        ConOut->ClearScreen(ConOut);
#endif
        draw_circle(hand_max, sYcen, sXcen);

        draw_hand((ltime.Hour*5)+(ltime.Minute/10), 2*hand_max/3, 'h', sXcen, sYcen);
        draw_hand(ltime.Minute, hand_max-2, 'm', sXcen, sYcen);
        draw_hand(ltime.Second, hand_max-1, '.', sXcen, sYcen);

#ifdef GNU_EFI
        uefi_call_wrapper(ConOut->SetCursorPosition, 3, ConOut, sXcen-5, sYcen-(3*hand_max/5));
#else
        ConOut->SetCursorPosition(ConOut, sXcen-5, sYcen-(3*hand_max/5));
#endif
        Print(L".:ACLOCK:.");
#ifdef GNU_EFI
        uefi_call_wrapper(ConOut->SetCursorPosition, 3, ConOut, sXcen-5, sYcen+(3*hand_max/5));
#else
        ConOut->SetCursorPosition(ConOut, sXcen-5, sYcen+(3*hand_max/5));
#endif
        Print(L"[%02d:%02d:%02d]", ltime.Hour, ltime.Minute, ltime.Second);

#ifdef GNU_EFI
        uefi_call_wrapper(eST->BootServices->Stall, 1, 1000000);
#else
        sleep(1);
#endif
    }

    return EFI_SUCCESS;
}

