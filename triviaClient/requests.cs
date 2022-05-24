using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace triviaClient
{
    public class LoginRequest
    {
        public string Username { set; get; }  // the username
        public string Password { set; get; }  // the password
    }

    public class SignupRequest
    {
        public string Username { set; get; }  // the username
        public string Password { set; get; }  // the password
        public string Email { set; get; }  // the email
    }

    public class CreateRoomRequest
    {
        public String roomName { set; get; }
        public uint maxUsers { set; get; }
        public uint questionCount { set; get; }
        public uint answerTimeout { set; get; }
    };

    public class JoinRoomRequest
    {
        public uint roomId { set; get; }
    };
    public class GetPlayersInRoomRequest
    {
        public uint roomId { set; get; }
    };
}
