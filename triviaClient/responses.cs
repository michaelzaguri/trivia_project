using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace triviaClient
{
    public class LoginResponse
    {
        public uint status { set; get; }   // the status of the response
    }

    public class ErrorResponse
    {
        public string message { set; get; }   // the error message
    };

    public class SignupResponse
    {
        public uint status { set; get; }   // the status of the response
    }

    public class GetPersonalStatsResponse
    {
        public uint status { set; get; }  // the status of the response
        public List<string> statistics { set; get; }
    };

    public class LogoutResponse
    {
        public uint status { set; get; }  // the status of the response
    };

    public class GetHighScoreResponse
    {
        public uint status { set; get; }  // the status of the response
        public List<string> statistics { set; get; }
    };
    public class CreateRoomResponse
    {
        public uint status { set; get; }  // the status of the response
    };
    public class GetRoomsResponse
    {
        public uint status { set; get; }  // the status of the response
        public List<RoomData> rooms { set; get; }
    };
    public class GetPlayersInRoomResponse
    {
        public List<string> players { set; get; }
    };
    public class JoinRoomResponse
    {
        public uint status { set; get; } // the status of the response
    };
    public class GetRoomStateResponse
    {
        public uint status { set; get; }  // the status of the response
        public bool hasGameBegun { set; get; }
        public List<string> players { set; get; }
        public uint questionCount { set; get; }
        public uint answerTimeout { set; get; }
    };
    public class StartGameResponse
    {
        public uint status { set; get; }  // the status of the response
    };
    public class LeaveRoomResponse
    {
        public uint status { set; get; }  // the status of the response
    };
    public class CloseRoomResponse
    {
        public uint status { set; get; }  // the status of the response
    };

}
