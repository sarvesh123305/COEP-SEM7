#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include "EncryptionAndDecryption.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cerr << "Usage: ip_address port" << endl;
        exit(0);
    }
    
    char *serverIp = argv[1];
    int port = atoi(argv[2]);
    char msg[1500];

    ImprovedTransform it;

    struct hostent* host = gethostbyname(serverIp);
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(port);

    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    int status = connect(clientSd, (sockaddr*)&sendSockAddr, sizeof(sendSockAddr));
    if (status < 0) {
        cout << "Error connecting to socket!" << endl;
        return 0;
    }
    cout << "Connected to the server! : " << endl;

    int bytesRead = 0, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    while (true) {
        cout << ">";
        string data;
        getline(cin, data);

        string encryptedMsg = it.encrypt(data);
        string decrypted = it.decrypt(encryptedMsg);
        cout << "The Encrypted Message " << data << " is " << encryptedMsg <<  endl;
        memset(&msg, 0, sizeof(msg)); // Clear the buffer
        strcpy(msg, encryptedMsg.c_str());

        if (data == "exit") {
            send(clientSd, (char*)&msg, strlen(msg), 0);
            break;
        }
        
        bytesWritten += send(clientSd, (char*)&msg, strlen(msg), 0);

        cout << "Awaiting server response..." << endl;
        memset(&msg, 0, sizeof(msg)); // Clear the buffer
        bytesRead += recv(clientSd, (char*)&msg, sizeof(msg), 0);

        if (strcmp(msg, "exit") == 0) {
            cout << "Server has quit the session" << endl;
            break;
        }

        string receivedMsg(msg);

        string decryptedMsg = it.decrypt(receivedMsg);
        cout << "Server: " << decryptedMsg << endl;
    }

    gettimeofday(&end1, NULL);
    close(clientSd);

    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << endl;
    cout << "Connection closed" << endl;

    return 0;
}
