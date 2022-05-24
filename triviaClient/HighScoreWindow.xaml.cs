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
    /// Interaction logic for HighScoreWindow.xaml
    /// </summary>
    public partial class HighScoreWindow : Window
    {
        public ClientCommunicator _clientCom;

        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;
        public const int FIRST_PLACE_INDEX = 0;
        public const int SEC_PLACE_INDEX = 1;
        public const int THIRD_PLACE_INDEX = 2;

        public HighScoreWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
            getHighScore();
        }

        private void getHighScore()
        {
            string response = "";
            string msg = "1000100000000000000000000000000000000000";
            // get high score request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00001000")  // get high score response
            {
                GetHighScoreResponse highScoreResponse = JsonConvert.DeserializeObject<GetHighScoreResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                firstPlace_lbl.Content = highScoreResponse.statistics[FIRST_PLACE_INDEX];
                secPlace_lbl.Content = highScoreResponse.statistics[SEC_PLACE_INDEX];
                thirdPlace_lbl.Content = highScoreResponse.statistics[THIRD_PLACE_INDEX];
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
                this.Close();
            }
        }

        private void back_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
