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
    /// Interaction logic for CreateRoomWindow.xaml
    /// </summary>
    public partial class CreateRoomWindow : Window
    {
        public ClientCommunicator _clientCom;
        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int HEX_POWER = 16;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;
        public CreateRoomWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            string response = "";
            CreateRoomRequest request = new CreateRoomRequest
            {
                roomName = roomName_txtb.Text,
                maxUsers = Convert.ToUInt32(playerCount_txtb.Text, HEX_POWER) ,
                questionCount = Convert.ToUInt32(questionCount_txtb.Text, HEX_POWER),
                answerTimeout = Convert.ToUInt32(questionTime_txtb.Text, HEX_POWER)
            };
            string jsonPart = JsonConvert.SerializeObject(request);
            string binaryJsonPart = TextUtils.textToBinary(jsonPart);
            string msg = "10000111" + Convert.ToString(jsonPart.Length, BINARY_POWER).PadLeft(LEN_MSG_LENGTH, '0') + binaryJsonPart;
            // create room request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000111")  // create room response
            {
                CreateRoomResponse createRoomResponse = JsonConvert.DeserializeObject<CreateRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                this.Hide();
                PlayersWindow playersWindow = new PlayersWindow(_clientCom, true);
                playersWindow.ShowDialog();
                roomName_txtb.Clear();
                questionTime_txtb.Clear();
                playerCount_txtb.Clear();
                questionCount_txtb.Clear();
                this.Close();
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
                roomName_txtb.Clear();
                questionTime_txtb.Clear();
                playerCount_txtb.Clear();
                questionCount_txtb.Clear();
            }
        }

        private void back_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
