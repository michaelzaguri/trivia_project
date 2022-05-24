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
using Newtonsoft.Json;

namespace triviaClient
{
    /// <summary>
    /// Interaction logic for MenuWindow.xaml
    /// </summary>
    public partial class MenuWindow : Window
    {
        public ClientCommunicator _clientCom;

        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;

        public MenuWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
        }

        private void createRoom_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
            CreateRoomWindow menuWindow = new CreateRoomWindow(_clientCom);
            menuWindow.ShowDialog();
            this.Show();
        }

        private void joinRoom_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
            JoinRoomWindow joinWindow = new JoinRoomWindow(_clientCom);
            joinWindow.ShowDialog();
            this.Show();
        }

        private void stats_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
            StatisticsWindow menuWindow = new StatisticsWindow(_clientCom);
            menuWindow.ShowDialog();
            this.Show();
        }

        private void room_btn_Click(object sender, RoutedEventArgs e)
        {
            ContextMenu cm = this.FindResource("cmRoom") as ContextMenu;
            cm.PlacementTarget = sender as Button;
            cm.IsOpen = true;
        }

        private void logout_btn_Click(object sender, RoutedEventArgs e)
        {
            string response = "";
            string msg = "1000001100000000000000000000000000000000";
            // logout request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000011")  // logout response
            {
                LogoutResponse logoutResponse = JsonConvert.DeserializeObject<LogoutResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                _clientCom.endSession();
                MessageBox.Show("Thank you for using our app!! see you next time :))");
                this.Close();
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }
        }
    }
}
