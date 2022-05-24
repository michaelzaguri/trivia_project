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

namespace triviaClient
{
    /// <summary>
    /// Interaction logic for JoinRoomWindow.xaml
    /// </summary>
    public partial class JoinRoomWindow : Window
    {
        public ClientCommunicator _clientCom;
        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int HEX_POWER = 16;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;
        public const int MAX_PRECENTAGE = 100;
        public const int REFRESH_COOLDOWN = 3000;
        private BackgroundWorker background_worker = new BackgroundWorker();
        private int precentage = 1;
        public JoinRoomWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
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
            while(true)
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
            getAllRooms();
        }
        void background_worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            
        }
        private void rooms_lb_DoubleClick(object sender, System.EventArgs e)
        {
            string roomId = ((RoomDataItem)rooms_lb.SelectedItem).Id.ToString();
            if (roomId == "Id")
            {
                return;
            }
            string response = "";
            JoinRoomRequest request = new JoinRoomRequest
            {
                roomId = Convert.ToUInt32(roomId, HEX_POWER)
            };
            string jsonPart = JsonConvert.SerializeObject(request);
            string binaryJsonPart = TextUtils.textToBinary(jsonPart);
            string msg = "10000110" + Convert.ToString(jsonPart.Length, BINARY_POWER).PadLeft(LEN_MSG_LENGTH, '0') + binaryJsonPart;
            // join room request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000110")  // join room response
            {
                JoinRoomResponse joinRoomResponse = JsonConvert.DeserializeObject<JoinRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                precentage = MAX_PRECENTAGE;
                this.Hide();
                PlayersWindow playersWindow = new PlayersWindow(_clientCom, false);
                playersWindow.ShowDialog();
                this.Close();
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }
        }

        private void getAllRooms()
        {
            string response = "";
            string msg = "1000010000000000000000000000000000000000";  // get rooms request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000100")  // get rooms response
            {
                GetRoomsResponse getRoomsResponse = JsonConvert.DeserializeObject<GetRoomsResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                addRooms(getRoomsResponse.rooms);
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
                this.Close();
            }
        }

        private void addRooms(List<RoomData> rooms)
        {
            int i = 0;
            List<RoomDataItem> items = new List<RoomDataItem>();
            items.Add(new RoomDataItem()
            {
                Id = "Id",
                Name = "room name",
                MaxPlayers = "max players",
                AmountQuestion = "question count",
                TimePerQuestion = "time per question"
            });
            for (i = 0; i < rooms.Count; i++)
            {
                if(canAddPlayer(rooms[i].id, rooms[i].maxPlayers))
                {
                    items.Add(new RoomDataItem()
                    {
                        Id = rooms[i].id.ToString(),
                        Name = rooms[i].name,
                        MaxPlayers = rooms[i].maxPlayers.ToString(),
                        AmountQuestion = rooms[i].numOfQuestionsInGame.ToString(),
                        TimePerQuestion = rooms[i].timePerQuestion.ToString()
                    });
                }
            }
            rooms_lb.ItemsSource = items;
        }
        
        private bool canAddPlayer(uint roomId, uint maxPlayers)  // adds a player if there is room for them
        {
            string response = "";
            GetPlayersInRoomRequest request = new GetPlayersInRoomRequest
            {
                roomId = roomId
            };
            string jsonPart = JsonConvert.SerializeObject(request);
            string binaryJsonPart = TextUtils.textToBinary(jsonPart);
            string msg = "10000101" + Convert.ToString(jsonPart.Length, BINARY_POWER).PadLeft(LEN_MSG_LENGTH, '0') + binaryJsonPart;
            // get players in room request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00000101")  // get players in room response
            {
                GetPlayersInRoomResponse getPlayersInRoomResponse = JsonConvert.DeserializeObject<GetPlayersInRoomResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                return getPlayersInRoomResponse.players.Count < maxPlayers;
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
            }
            return false;
        }

        private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void back_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
    public class RoomDataItem
    {
        public string Id { get; set; }
        public string Name { get; set; }
        public string MaxPlayers { get; set; }
        public string AmountQuestion { get; set; }
        public string TimePerQuestion { get; set; }
    }
    public class RoomData
    {
        public uint id { get; set; }
        public String name { get; set; }
        public uint maxPlayers { get; set; }
        public uint numOfQuestionsInGame { get; set; }
        public uint timePerQuestion { get; set; }
        public uint isActive { get; set; }
    }

}
