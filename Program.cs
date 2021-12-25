using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace GamepadVibCSharpTest
{
    class Program
    {
        [DllImport("GamepadVIbDLL")] public static extern void readAllProfiles();
        [DllImport("GamepadVIbDLL")] public static extern int getNumProfiles();
        [DllImport("GamepadVIbDLL")] [return: MarshalAs(UnmanagedType.BStr)] public static extern string getProfileName(int index);
        [DllImport("GamepadVIbDLL")] public static extern void setProfile(int index);
        [DllImport("GamepadVIbDLL")] public static extern void vibrationLoop();
        [DllImport("GamepadVIbDLL")] public static extern void endVibLoop();

        static void Main(string[] args)
        {
            readAllProfiles();
            int numProfiles = getNumProfiles();
            Console.WriteLine("Number of profiles: {0}", numProfiles);
            List<string> profileList = new List<string>(numProfiles);
            for (int i = 0; i < numProfiles; i++)
            {   
                profileList.Add(getProfileName(i));
                Console.WriteLine("{0}. {1}", i, profileList[i]);
            }
            Console.WriteLine(" ");
            string s;
            int select;
            Console.WriteLine("Input Selection Below:");
            s = Console.ReadLine();
            select = int.Parse(s);
            setProfile(select);
            Console.WriteLine("profile {0} set", profileList[select]);
            Console.WriteLine("starting loop...");
            Thread secondThread = new Thread(vibrationLoop);
            secondThread.Start();
            secondThread.Join();
            Console.WriteLine("end of C#");
        }
    }
}