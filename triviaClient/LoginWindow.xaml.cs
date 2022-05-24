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
using System.Net.Sockets;
using System.Net;
using Newtonsoft.Json;

namespace triviaClient
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class LoginWindow : Window
    {
        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;

        public ClientCommunicator clientCom;

        public LoginWindow()
        {
            InitializeComponent();
            clientCom = new ClientCommunicator();
            clientCom.connectToServer();
        }

        private void cancel_btn_Click(object sender, RoutedEventArgs e)
        {
            clientCom.endSession();
            this.Close();
        }

        private void signup_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
            SignupWindow signupWindow = new SignupWindow(clientCom);
            signupWindow.ShowDialog();
            this.Close();
        }

        private void login_btn_Click(object sender, RoutedEventArgs e)
        {
            string response = "";
            LoginRequest request = new LoginRequest
            {
                Username = usernameTextBox.Text,
                Password = password_psb.Password
            };
            string jsonPart = JsonConvert.SerializeObject(request);
            string binaryJsonPart = TextUtils.textToBinary(jsonPart);
            string msg = "10000001" + Convert.ToString(jsonPart.Length, BINARY_POWER).PadLeft(LEN_MSG_LENGTH, '0') + binaryJsonPart;
            // login request

            clientCom.sendMsgToServer(msg);
            response = clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000001")  // login response
            {
                LoginResponse loginResponse = JsonConvert.DeserializeObject<LoginResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                this.Hide();
                MenuWindow menuWindow = new MenuWindow(clientCom);
                menuWindow.ShowDialog();
                this.Close();
                // TO DO: add connection to the next window
            }
            else if(response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
                usernameTextBox.Clear();
                password_psb.Clear();
            }
        }

        private void usernameTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

    }
}
