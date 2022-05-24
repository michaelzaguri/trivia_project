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
using System.ComponentModel;
using System.Threading;
using System.Diagnostics;

namespace triviaClient
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class PlayersWindow : Window
    {
        private bool _isAdmin;
        public ClientCommunicator _clientCom;
        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;
        public const int MAX_PRECENTAGE = 100;
        public const int REFRESH_COOLDOWN = 3000;
        private BackgroundWorker background_worker = new BackgroundWorker();
        private int precentage = 1;
        public PlayersWindow(ClientCommunicator clientCom, bool isAdmin)
        {
            InitializeComponent();
            _clientCom = clientCom;
            _isAdmin = isAdmin;
            if (!isAdmin)  // a member sees the window differentry due to access gap 
            {
                close_leave_btn.Content = "Leave";
                start_btn.Visibility = Visibility.Hidden;
            }
            // setting up the backgrounf worker
            background_worker.WorkerSupportsCancellation = true;
            background_worker.WorkerReportsProgress = true;
            background_worker.DoWork += background_worker_DoWork;
            background_worker.ProgressChanged += background_worker_ProgressChanged;
            background_worker.RunWorkerCompleted += background_worker_RunWorkerCompleted;
            background_worker.RunWorkerAsync();
        }
        void background_worker_DoWork(object sender, DoWorkEventArgs e)
        {
            while (true)
            {
                if (background_worker.CancellationPending || precentage == MAX_PRECENTAGE)
                {
                    e.Cancel = true;
                    break;
                }
                background_worker.ReportProgress(precentage);
                Thread.Sleep(REFRESH_COOLDOWN);
            }
        }
        void background_worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            updateRoomState();
        }
        void background_worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            this.Hide();
            this.Close();
        }
        private void close_leave_btn_Click(object sender, RoutedEventArgs e)
        {
            string response = "", msg = "";
            if (_isAdmin)
            {
                msg = "1000101000000000000000000000000000000000";  // close room request
            }
            else
            {
                msg = "1000110100000000000000000000000000000000";  // leave room request
            }
            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00001101")  // leave room
            {
                LeaveRoomResponse leaveRoomResponse = JsonConvert.DeserializeObject<LeaveRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
            }
            else if (response.Substring(0, BYTE_SIZE) == "00001010")  // close room
            {
                CloseRoomResponse closeRoomResponse = JsonConvert.DeserializeObject<CloseRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }
        }
        private void start_btn_Click(object sender, RoutedEventArgs e)
        {
            string response = "";
            string msg = "1000101100000000000000000000000000000000";  // start game request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00001011")  // start game response
            {
                StartGameResponse startGameResponse = JsonConvert.DeserializeObject<StartGameResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
                //TO DO: in 400 adding games soooo in 400 to add a window
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }
        }

        private void updateRoomState()
        {
            string response = "";
            string msg = "1000110000000000000000000000000000000000";  // get room state request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00001100")  // get room state response
            {
                GetRoomStateResponse GetRoomStateResponse = JsonConvert.DeserializeObject<GetRoomStateResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                addPlayers(GetRoomStateResponse.players);
            }
            else if(response.Substring(0, BYTE_SIZE) == "00001010") // close room response
            {
                CloseRoomResponse closeRoomResponse = JsonConvert.DeserializeObject<CloseRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
            }
            else if (response.Substring(0, BYTE_SIZE) == "00001101") // leave room response
            {
                LeaveRoomResponse leaveRoomResponse = JsonConvert.DeserializeObject<LeaveRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
            }
            else if(response.Substring(0, BYTE_SIZE) == "00001011") // start game
            {
                StartGameResponse startGameResponse = JsonConvert.DeserializeObject<StartGameResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
                // adding connections to other windows in 400
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }
        }

        private void addPlayers(List<string> players)  // adds the players to the players list
        {
            int i = 0;
            List<Player> items = new List<Player>();
            for (i = 0; i < players.Count; i++)
            {
                items.Add(new Player()
                {
                    name = players[i]
                });
            }
            players_lb.ItemsSource = items;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            string response = "", msg = "";
            if(new StackTrace().GetFrames().Any(x => x.GetMethod().Name == "Close"))
            {
                
            }
            else if (new StackTrace().GetFrames().Any(x => x.GetMethod().Name != "Close"))
                // a method we found online that checks how the window was closed - by a Close() function
                // that was called from the code or a user click on the X button/alt + f4
            {
                // enters the function if the user clicked the X button or alt + f4
                e.Cancel = true;
                if (_isAdmin)
                {
                    msg = "1000101000000000000000000000000000000000";  // close room request
                }
                else
                {
                    msg = "1000110100000000000000000000000000000000";  // leave room request
                }
                _clientCom.sendMsgToServer(msg);
                response = _clientCom.readMsgFromServer();
                if (response.Substring(0, BYTE_SIZE) == "00001101")  // leave room
                {
                    LeaveRoomResponse leaveRoomResponse = JsonConvert.DeserializeObject<LeaveRoomResponse>
                        (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                    precentage = MAX_PRECENTAGE;
                }
                else if (response.Substring(0, BYTE_SIZE) == "00001010")  // close room
                {
                    CloseRoomResponse closeRoomResponse = JsonConvert.DeserializeObject<CloseRoomResponse>
                        (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                    precentage = MAX_PRECENTAGE;
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
    public class Player
    {
        public string name { get; set; }
    }
}
