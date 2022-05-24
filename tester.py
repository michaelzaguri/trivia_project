import socket
import json
import time

SERVER_IP = "127.1.1.1"
SERVER_PORT = 2811

def cons():
    # signup request:
    #json_req = { "Username":"michael", "Password":"1234", "Email":"1@gmail.com"}
    #request = '10000000' + '{0:b}'.format(len(json.dumps(json_req))).zfill(32) +  \
    #          (''.join(format(ord(i), '08b') for i in json.dumps(json_req))).zfill(len(json.dumps(json_req)) * 8)
    #login request:
    json_req = { "Username":"michael", "Password":"1234"}
    request = '10000001' + '{0:b}'.format(len(json.dumps(json_req))).zfill(32) +  \
              (''.join(format(ord(i), '08b') for i in json.dumps(json_req))).zfill(len(json.dumps(json_req)) * 8)
    print(''.join(chr(int(request[40:][i*8:i*8+8],2)) for i in range(len(request[40:])//8)))
    #request = "1000000000000000000000000000001000010000011110110010001001010101011100110110010101110010011011100110000101101101011001010010001000111010001000000010001001100010011011110110001001101111001000100010110000100000001000100101000001100001011100110111001101110111011011110111001001100100001000100011101000100000001000100111001101101111011100110110111100100010001011000010000000100010010001010110110101100001011010010110110000100010001110100010000000100010011001010110010101100101010000000110011101101101011000010110100101101100001011110110001101101111011011010010001001111101"
    with socket.socket() as sock:
        server_address = (SERVER_IP, SERVER_PORT)
        sock.connect(server_address)
        sock.sendall(request.encode())
        string = sock.recv(1024).decode()
        r = ''.join(chr(int(string[40:][i*8:i*8+8],2)) for i in range(len(string[40:])//8))
        print(r)

def main():
    cons()

if "__main__" == __name__:
    main()
