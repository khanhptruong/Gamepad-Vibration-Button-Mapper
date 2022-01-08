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
        #region DllImport P/Invoke Functions
        [DllImport("GamepadVibMapperDLL")] public static extern void readAllProfiles();
        [DllImport("GamepadVibMapperDLL")] public static extern int getNumProfiles();
        [DllImport("GamepadVibMapperDLL")] [return: MarshalAs(UnmanagedType.BStr)] public static extern string getProfileName(int index);
        [DllImport("GamepadVibMapperDLL")] public static extern void setProfile(int index);
        [DllImport("GamepadVibMapperDLL")] public static extern void vibrationLoop();
        [DllImport("GamepadVibMapperDLL")] public static extern void endVibLoop();
        [DllImport("GamepadVibMapperDLL")] public static extern bool isGamepadConnected();
        [DllImport("GamepadVibMapperDLL")] public static extern float getData(int field, int index);
        [DllImport("GamepadVibMapperDLL")] public static extern void setData(int field, int index, float data);
        #endregion

        #region Declare_Global_Variables
        List<ProfileName> profiles = new List<ProfileName>();
        int numProfiles;
        int buttonIndex;
        bool initialized = false;
        bool profileChanged = false;
        bool buttonChanged = false;
        bool settingsChanged = false;
        bool keepCurrentProfile = false;
        int prevProfileSelection = 0;
        Thread vibLoopThread;
        const int fieldButtonMap = 0;
        const int fieldNumPulses = 1;
        const int fieldPulseFreq = 2;
        const int fieldPulseOffFreq = 3;
        const int fieldLeftSpeed = 4;
        const int fieldRightSpeed = 5;
        #endregion

        public MainWindow()
        {
            InitializeComponent();

            #region Initialize_Vibration_Loop
            readAllProfiles();
            vibLoopThread = new Thread(vibrationLoop);
            vibLoopThread.Start();
            setProfile(0);
            #endregion

            #region Initialize_ProfilesListBox
            numProfiles = getNumProfiles();
            for (int i = 0; i < numProfiles; i++)
            {
                profiles.Add(new ProfileName() { Name = getProfileName(i) });
            }
            ProfilesListBox.ItemsSource = profiles;
            #endregion

            #region Initialize_Connection_Indicator
            var timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds(700);
            timer.Start();
            timer.Tick += Timer_Tick;
            #endregion

            initialized = true;
            loadButtonSettings();
            highlightActiveButtons();
        }

        private void ProfilesListBox_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            prevProfileSelection = ProfilesListBox.SelectedIndex;
        }

        private void ProfilesListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (settingsChanged == true && keepCurrentProfile == false)
            {
                if (MessageBox.Show("Profile has not been saved. Continue?", "Save Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
                {
                    keepCurrentProfile = true;
                    ProfilesListBox.SelectedIndex = prevProfileSelection;
                }
                else
                {
                    //save
                    loadProfile();
                }
            }
            else if (keepCurrentProfile == true)
            {
                keepCurrentProfile = false;
            }
            else
            {
                loadProfile();
            }
        }

        private void ButtonSelectStackPanel_Checked(object sender, RoutedEventArgs e)
        {
            var currentRadio = e.OriginalSource as RadioButton;
            string indexName = currentRadio.Name;
            switch (indexName)
            {
                case "ARadioButton":
                    buttonIndex = 0;
                    break;
                case "BRadioButton":
                    buttonIndex = 1;
                    break;
                case "XRadioButton":
                    buttonIndex = 2;
                    break;
                case "YRadioButton":
                    buttonIndex = 3;
                    break;
                case "UpRadioButton":
                    buttonIndex = 4;
                    break;
                case "DownRadioButton":
                    buttonIndex = 5;
                    break;
                case "LeftRadioButton":
                    buttonIndex = 6;
                    break;
                case "RightRadioButton":
                    buttonIndex = 7;
                    break;
                case "LBRadioButton":
                    buttonIndex = 8;
                    break;
                case "RBRadioButton":
                    buttonIndex = 9;
                    break;
                case "LSRadioButton":
                    buttonIndex = 10;
                    break;
                case "RSRadioButton":
                    buttonIndex = 11;
                    break;
                case "StartRadioButton":
                    buttonIndex = 12;
                    break;
                case "BackRadioButton":
                    buttonIndex = 13;
                    break;
                case "LTRadioButton":
                    buttonIndex = 14;
                    break;
                case "RTRadioButton":
                    buttonIndex = 15;
                    break;
            }
            buttonChanged = true;
            if (initialized == true) { loadButtonSettings(); }
            buttonChanged = false;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (settingsChanged == true)
            {
                if (MessageBox.Show("Profile has not been saved. Continue?", "Save Warning", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
                {
                    e.Cancel = true;
                }
                else
                {
                    endVibLoop();
                    vibLoopThread.Join();
                }
            }
            else
            {
                endVibLoop();
                vibLoopThread.Join();
            }
        }

        #region Button_Settings_Controls
        private void SettingStackPanel_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            settingsButtonsEnablement();
        }

        private void ButtonMapToggleButton_Click(object sender, RoutedEventArgs e)
        {
            if (ButtonMapToggleButton.IsChecked == true) { setData(fieldButtonMap, buttonIndex, 1.0f); }
            else { setData(fieldButtonMap, buttonIndex, 0.0f); }
            highlightActiveButtons();
            settingsButtonsEnablement();
        }

        private void NumPulsesSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            int x = (int)NumPulsesSlider.Value;
            setData(fieldNumPulses, buttonIndex, x);
            NumPulsesTextBlock.Text = NumPulsesSlider.Value.ToString();
        }

        private void PulseFreqSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            float x = (float)PulseFreqSlider.Value;
            setData(fieldPulseFreq, buttonIndex, x);
            PulseFreqTextBlock.Text = PulseFreqSlider.Value.ToString("0.00");
        }

        private void PulseOffFreqSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            float x = (float)PulseOffFreqSlider.Value;
            setData(fieldPulseOffFreq, buttonIndex, x);
            PulseOffFreqTextBlock.Text = PulseOffFreqSlider.Value.ToString("0.00");
        }

        private void LeftSpeedSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {       
            float x = (float)LeftSpeedSlider.Value;
            setData(fieldLeftSpeed, buttonIndex, x);
            LeftSpeedTextBlock.Text = LeftSpeedSlider.Value.ToString("0.00");
        }

        private void RightSpeedSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            float x = (float)RightSpeedSlider.Value;
            setData(fieldRightSpeed, buttonIndex, x);
            RightSpeedTextBlock.Text = RightSpeedSlider.Value.ToString("0.00");
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            loadProfile();
        }
        #endregion

        #region Internal_Functions
        private void loadButtonSettings()
        {
            if (getData(fieldButtonMap, buttonIndex) != 0.0f) { ButtonMapToggleButton.IsChecked = true; }
            else { ButtonMapToggleButton.IsChecked = false; }

            int x = (int)getData(fieldNumPulses, buttonIndex);
            NumPulsesSlider.Value = x;

            PulseFreqSlider.Value = getData(fieldPulseFreq, buttonIndex);
            PulseOffFreqSlider.Value = getData(fieldPulseOffFreq, buttonIndex);
            LeftSpeedSlider.Value = getData(fieldLeftSpeed, buttonIndex);
            RightSpeedSlider.Value = getData(fieldRightSpeed, buttonIndex);

            NumPulsesTextBlock.Text = NumPulsesSlider.Value.ToString();
            PulseFreqTextBlock.Text = PulseFreqSlider.Value.ToString("0.00");
            PulseOffFreqTextBlock.Text = PulseOffFreqSlider.Value.ToString("0.00");
            LeftSpeedTextBlock.Text = LeftSpeedSlider.Value.ToString("0.00");
            RightSpeedTextBlock.Text = RightSpeedSlider.Value.ToString("0.00");
        }

        private void highlightActiveButtons()
        {
            int x = 0;
            if (getData(fieldButtonMap, x) != 0.0f) { ARadioButton.Foreground = Brushes.LightGreen; }
            else { ARadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { BRadioButton.Foreground = Brushes.LightGreen; }
            else { BRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { XRadioButton.Foreground = Brushes.LightGreen; }
            else { XRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { YRadioButton.Foreground = Brushes.LightGreen; }
            else { YRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { UpRadioButton.Foreground = Brushes.LightGreen; }
            else { UpRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { DownRadioButton.Foreground = Brushes.LightGreen; }
            else { DownRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { LeftRadioButton.Foreground = Brushes.LightGreen; }
            else { LeftRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { RightRadioButton.Foreground = Brushes.LightGreen; }
            else { RightRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { LBRadioButton.Foreground = Brushes.LightGreen; }
            else { LBRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { RBRadioButton.Foreground = Brushes.LightGreen; }
            else { RBRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { LSRadioButton.Foreground = Brushes.LightGreen; }
            else { LSRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { RSRadioButton.Foreground = Brushes.LightGreen; }
            else { RSRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { StartRadioButton.Foreground = Brushes.LightGreen; }
            else { StartRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { BackRadioButton.Foreground = Brushes.LightGreen; }
            else { BackRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { LTRadioButton.Foreground = Brushes.LightGreen; }
            else { LTRadioButton.Foreground = Brushes.Black; }
            x++;
            if (getData(fieldButtonMap, x) != 0.0f) { RTRadioButton.Foreground = Brushes.LightGreen; }
            else { RTRadioButton.Foreground = Brushes.Black; }
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
            DebugTextBlock.Text = String.Format("Profile: {0}\nbuttonIndex: {1}\nprevProfileSelection: {2}\nSettings:\n{3}", profiles[ProfilesListBox.SelectedIndex].Name, buttonIndex, prevProfileSelection, PrintProfile());
        }

        private string PrintProfile()
        {
            string s = "";
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

        private void settingsButtonsEnablement()
        {
            if (profileChanged == true)
            {
                CancelButton.IsEnabled = false;
                SaveButton.IsEnabled = false;
            }
            else if (buttonChanged == true && settingsChanged == false)
            {
                CancelButton.IsEnabled = false;
                SaveButton.IsEnabled = false;
            }
            else
            {
                settingsChanged = true;
                CancelButton.IsEnabled = true;
                SaveButton.IsEnabled = true;
            }
        }

        private void loadProfile()
        {
            profileChanged = true;
            settingsChanged = false;
            setProfile(ProfilesListBox.SelectedIndex);
            if (initialized == true)
            {
                loadButtonSettings();
                highlightActiveButtons();
            }
            profileChanged = false;
        }
        #endregion
    }
}

public class ProfileName
{
    public string? Name { get; set; }
}
