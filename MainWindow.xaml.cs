using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Threading; //dispatchertimer

namespace GamepadVibMapper
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        [DllImport("GamepadVibMapperDLL")] public static extern void readAllProfiles();
        [DllImport("GamepadVibMapperDLL")] public static extern int getNumProfiles();
        [DllImport("GamepadVibMapperDLL")] [return: MarshalAs(UnmanagedType.BStr)] public static extern string getProfileName(int index);
        [DllImport("GamepadVibMapperDLL")] public static extern void setProfile(int index);
        [DllImport("GamepadVibMapperDLL")] public static extern void vibrationLoop();
        [DllImport("GamepadVibMapperDLL")] public static extern void endVibLoop();
        [DllImport("GamepadVibMapperDLL")] public static extern bool isGamepadConnected();
        [DllImport("GamepadVibMapperDLL")] public static extern float getData(int field, int index);

        #region Declare_Global_Variables
        List<ProfileName> profiles = new List<ProfileName>();
        int numProfiles;
        int selectedProfileIndex = 0;
        Thread vibLoopThread;
        #endregion
        public MainWindow()
        {
            InitializeComponent();

            #region Initialize_Vibration_Loop
            readAllProfiles();
            numProfiles = getNumProfiles();
            vibLoopThread = new Thread(vibrationLoop);
            vibLoopThread.Start();
            setProfile(selectedProfileIndex);
            #endregion

            #region Initialize_ProfilesListBox
            for (int i = 0; i < numProfiles; i++)
            {
                profiles.Add(new ProfileName() { Name = getProfileName(i) });
            }
            ProfilesListBox.ItemsSource = profiles;
            #endregion

            #region Initialize_Connection_Indicator
            var timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(500);
            timer.Start();
            timer.Tick += Timer_Tick;
            #endregion
        }

        private void RunButton_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("button was clicked");
        }

        private void ProfilesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            selectedProfileIndex = ProfilesListBox.SelectedIndex;
            string? s = profiles[selectedProfileIndex].Name;
            setProfile(selectedProfileIndex);
            DebugTextBlock.Text = String.Format("Index is {0}, {1} was selected\n{2}", selectedProfileIndex, s, PrintProfile());
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            endVibLoop();
            vibLoopThread.Join();
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            bool isConnected = isGamepadConnected();
            if (isConnected == true)
            {
                ConnectionTextBlock.Text = "connected";
                ConnectionTextBlock.Foreground = Brushes.LightGreen;
            }
            else
            {
                ConnectionTextBlock.Text = "disconnected";
                ConnectionTextBlock.Foreground = Brushes.Red;
            }
        }

        private void NumberTextBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            if (string.IsNullOrEmpty(e.Text) == false)
            {
                e.Handled = !Char.IsNumber(e.Text[0]);
            }
        }

        private string PrintProfile()
        {
            string s = "Profiles:\n";
            for (int i = 0; i < 16; i++)
            {
                for(int j = 0; j < 6; j++)
                {
                    s = String.Format("{0}   {1}", s, getData(j, i));
                }
                s += "\n";
            }
            return s;
        }
    }
}

public class ProfileName
{
    public string? Name { get; set; }
}
