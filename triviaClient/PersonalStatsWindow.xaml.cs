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
    /// Interaction logic for PersonalStatsWindow.xaml
    /// </summary>
    public partial class PersonalStatsWindow : Window
    {
        public ClientCommunicator _clientCom;

        public const int BYTE_SIZE = 8;
        public const int BINARY_POWER = 2;
        public const int LEN_MSG_LENGTH = 32;
        public const int MESSAGE_CONTENT_INDEX = 40;
        public const int CORRECT_ANSWERS_NUM = 0;
        public const int TOTAL_ANSWERS_NUM = 1;
        public const int AVRG_ANSWER_TIME = 2;
        public const int TOTAL_GAMES_NUM = 3;

        public PersonalStatsWindow(ClientCommunicator clientCom)
        {
            InitializeComponent();
            _clientCom = clientCom;
            getStats();
        }

        private void getStats()
        {
            string response = "";
            string msg = "1000100100000000000000000000000000000000";
            // get stats request

            _clientCom.sendMsgToServer(msg);
            response = _clientCom.readMsgFromServer();
            if (response.Substring(0, BYTE_SIZE) == "00001001")  // get stats response
            {
                GetPersonalStatsResponse personalStatsloginResponse = JsonConvert.DeserializeObject<GetPersonalStatsResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                addStats(personalStatsloginResponse.statistics);
            }
            else if (response.Substring(0, BYTE_SIZE) == "00000010")  // error response
            {
                ErrorResponse errorResponse = JsonConvert.DeserializeObject<ErrorResponse>
                    (TextUtils.binaryStrToTxt(response.Substring(MESSAGE_CONTENT_INDEX)));
                MessageBox.Show(errorResponse.message);
                this.Close();
            }
        }

        private void addStats(List<string> stats)
        {

            List<TodoItem> items = new List<TodoItem>();
            items.Add(new TodoItem() { Title = "The number of correct answers:  ", Content = stats[CORRECT_ANSWERS_NUM] });
            items.Add(new TodoItem() { Title = "The number of total answers:    ", Content = stats[TOTAL_ANSWERS_NUM] });
            items.Add(new TodoItem() { Title = "The average answer time:    ", Content = stats[AVRG_ANSWER_TIME] });
            items.Add(new TodoItem() { Title = "The number of total games:  ", Content = stats[TOTAL_GAMES_NUM] });

            stats_lb.ItemsSource = items;
        }

        private void stats_lb_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void back_btn_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }

    public class TodoItem
    {
        public string Title { get; set; }
        public string Content { get; set; }
    }
}
