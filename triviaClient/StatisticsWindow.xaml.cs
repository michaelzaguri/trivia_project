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
using System.Windows.Shapes;

namespace triviaClient
{
    /// <summary>
    /// Interaction logic for StatisticsWindow.xaml
    /// </summary>
    public partial class StatisticsWindow : Window
    {
        public ClientCommunicator _clientCom;

        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;

        public StatisticsWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
        }

        private void stats_btn_Click(object sender, RoutedEventArgs e)
        {
            PersonalStatsWindow personalStatsWindow = new PersonalStatsWindow(_clientCom);
            personalStatsWindow.ShowDialog();
        }

        private void highScore_btn_Click(object sender, RoutedEventArgs e)
        {
            HighScoreWindow highScoreWindow = new HighScoreWindow(_clientCom);
            highScoreWindow.ShowDialog();
        }

        private void back_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
