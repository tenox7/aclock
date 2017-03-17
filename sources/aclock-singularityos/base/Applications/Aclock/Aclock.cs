//
//  Author:
//    Natalia Portillo claunia@claunia.com
//
//  Copyright (c) 2016, © Claunia.com
//
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in
//       the documentation and/or other materials provided with the distribution.
//     * Neither the name of the [ORGANIZATION] nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
using System;
using System.Threading;

using Microsoft.Singularity.Directory;
using Microsoft.Singularity.Io;
using Microsoft.Singularity.Channels;
using Microsoft.Singularity;
using Microsoft.SingSharp;

[assembly: System.Reflection.AssemblyVersionAttribute("2.4.0.0")]
[assembly: System.Reflection.AssemblyKeyFileAttribute("public.snk")]
[assembly: System.Reflection.AssemblyDelaySignAttribute(true)]

namespace aclock_sharp
{
    class MainClass
    {
        const int FontWH_Ratio = 1;

        static void DrawCircle(ConsoleDeviceContract.Imp! imp, int handMax, int sYcen, int sXcen)
        {
            int x, y, r;
            char c;

            for(r = 0; r < 60; r++)
            {
                x = (int)(Math.Cos(r * Math.PI / 180 * 6) * handMax * FontWH_Ratio + sXcen);
                y = (int)(Math.Sin(r * Math.PI / 180 * 6) * handMax + sYcen);
                switch(r)
                {
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
                        c = 'o';
                        break;
                    default:
                        c = '.';
                        break;
                }
                SetCursorPosition(imp, x, y);
                Console.Write(c);
            }
        }

        static void DrawHand(ConsoleDeviceContract.Imp! imp, int minute, int hlength, char c, int sXcen, int sYcen)
        {
            int x, y, n;
            double r = (minute - 15) * (Math.PI / 180) * 6;

            for(n = 1; n < hlength; n++)
            {
                x = (int)(Math.Cos(r) * n * FontWH_Ratio + sXcen);
                y = (int)(Math.Sin(r) * n + sYcen);
                SetCursorPosition(imp, x, y);
                Console.Write(c);
            }
        }

        static void PrintCopyr(ConsoleDeviceContract.Imp! imp)
        {
            ClearConsole(imp);
            Console.WriteLine("Copyright (c) 1994-2013 Antoni Sawicki <as@tenoware.com>");
            Console.WriteLine("Copyright (c) 2016-2017 Natalia Portillo <claunia@claunia.com>");
            Console.WriteLine("Version 2.4 (singularity); Canary Islands, January 2017");
        }

        static bool KeepRunning = true;

        public static void Main()
        {
			bool itRun = false;
            foreach(string s in new string[] { "/dev/video-text", "/dev/conout" } )
            {
                ConsoleDeviceContract.Imp imp = OpenConsole((!)s);
                if(imp != null)
				{
					itRun = true;
					RunAclock(imp);
					delete imp;
                    break;
				}
            }

            if(!itRun)
            {
                Console.WriteLine("Cannot open console");
                return;
            }	
		}
		
		public static void RunAclock(ConsoleDeviceContract.Imp! imp)
		{
            PrintCopyr(imp);
            Thread.Sleep(2000);

            int sXmax, sYmax, smax, handMax, sXcen, sYcen;
            DateTime ltime;

            sXmax = sYmax = handMax = sXcen = sYcen = 0;

            ClearConsole(imp);
            HideCursor(imp);

            while(true)
            {
                if(!KeepRunning)
                    break;

                ltime = DateTime.Now;
				GetConsoleDimensions(imp, out sXmax, out sYmax);

                if(sXmax / FontWH_Ratio <= sYmax)
                    smax = sXmax / FontWH_Ratio;
                else
                    smax = sYmax;

                handMax = (smax / 2) - 1;

                sXcen = sXmax / 2;
                sYcen = sYmax / 2;

                ClearConsole(imp);
                DrawCircle(imp, handMax, sYcen, sXcen);

                DrawHand(imp, (ltime.Hour * 5) + (ltime.Minute / 10), 2 * handMax / 3, 'h', sXcen, sYcen);
                DrawHand(imp, ltime.Minute, handMax - 2, 'm', sXcen, sYcen);
                DrawHand(imp, ltime.Second, handMax - 1, '.', sXcen, sYcen);
                SetCursorPosition(imp, sXcen - 5, sYcen - (3 * handMax / 5));
                Console.Write(".:ACLOCK:.");
                SetCursorPosition(imp, sXcen - 5, sYcen + (3 * handMax / 5));
                Console.Write("{0:D2}:{1:D2}:{2:D2}", ltime.Hour, ltime.Minute, ltime.Second);

                Thread.Sleep(1000);
            }

            PrintCopyr(imp);
            ShowCursor(imp);
        }

        #region Singularity methods
        private static ConsoleDeviceContract.Imp OpenConsoleInternal(DirectoryServiceContract.Imp! nsImp, [Claims] char[]! in ExHeap    deviceName)
        {
            ConsoleDeviceContract.Exp! exp;
            ConsoleDeviceContract.Imp! imp;
            ConsoleDeviceContract.NewChannel(out imp, out exp);

            nsImp.SendBind(deviceName, exp);
            switch receive {
                case nsImp.AckBind():
                    return imp;
                    break;
                case nsImp.NakBind(rejectImp, error):
                    delete rejectImp;
                    delete imp;
                    break;
                case nsImp.ChannelClosed():
                    throw new Exception("Channel closed during Console bind.");
                    delete imp;
                    break;
                case nsImp.NakBindReparse(path, rest, linked, backExp):
                    assert linked == true;
                    assert rest   == null;
                    delete backExp;
                    delete imp;

                    return OpenConsoleInternal(nsImp, path);
                    break;
            }
            return null;
        }
		
        private static ConsoleDeviceContract.Imp OpenConsole(string! devName)
        {
            DirectoryServiceContract.Imp ns = DirectoryService.NewClientEndpoint();
            ConsoleDeviceContract.Imp imp = OpenConsoleInternal(ns, Bitter.FromString2(devName));
            delete ns;

            if(imp != null)
            {
                switch receive
                {
                    case imp.Success():
                        break;
                    case imp.ContractNotSupported():
                        throw new Exception("Contract not supported");
                        break;
                    case imp.ChannelClosed():
                        throw new Exception("Channel is closed");
                        break;
                }
            }
            return imp;
        }

	    private static void SetCursorPosition(ConsoleDeviceContract.Imp! imp, int x, int y)
        {
            imp.SendSetCursorPosition(x, y);
            imp.RecvAckSetCursorPosition();
        }

        private static void HideCursor(ConsoleDeviceContract.Imp! imp)
        {
            imp.SendCursorHide();
            imp.RecvAckCursorHide();
        }

        private static void ShowCursor(ConsoleDeviceContract.Imp! imp)
        {
            imp.SendCursorShow();
            imp.RecvAckCursorShow();
        }

        private static void ClearConsole(ConsoleDeviceContract.Imp! imp)
        {
            imp.SendClear();
            imp.RecvAckClear();
        }
		
		private static void GetConsoleDimensions(ConsoleDeviceContract.Imp! imp, out int columns, out int rows)
		{
			imp.SendGetDisplayDimensions();
			imp.RecvDisplayDimensions(out columns, out rows);
		}
        #endregion Singularity methods
    }
}
