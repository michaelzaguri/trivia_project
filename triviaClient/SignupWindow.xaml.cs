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
    /// Interaction logic for SignupWindow.xaml
    /// </summary>
    public partial class SignupWindow : Window
    {
        public ClientCommunicator _clientCom;

        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;

        public SignupWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
        }

        private void password_psb_PasswordChanged(object sender, RoutedEventArgs e)
        {

        }

        private void signup_btn_Click(object sender, RoutedEventArgs e)
        {
            string response = "", jsonPart = "", binaryJsonPart = "", msg = "";
            if(password_psb.Password != passwordConf_psb.Password)
            {
                password_psb.Clear();
                passwordConf_psb.Clear();
                MessageBox.Show("Your passwords do not match - to sign in, you have to write two IDENICAL passwords!"); 
                return;
            }
            SignupRequest request = new SignupRequest
            {
                Username = username_txtb.Text,
                Password = password_psb.Password,
                Email = email_txtb.Text
            };
            jsonPart = JsonConvert.SerializeObject(request);
            binaryJsonPart = TextUtils.textToBinary(jsonPart);
            msg = "10000000" + Convert.ToString(jsonPart.Length, BINARY_POWER).PadLeft(LEN_MSG_LENGTH, '0') + binaryJsonPart;
            // signup request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000000")  // signup response
            {
                SignupRequest signupRequest = JsonConvert.DeserializeObject<SignupRequest>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));

                this.Hide();
                MenuWindow menuWindow = new MenuWindow(_clientCom);
                menuWindow.ShowDialog();
                this.Close();
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }

        }

        private void cancel_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
