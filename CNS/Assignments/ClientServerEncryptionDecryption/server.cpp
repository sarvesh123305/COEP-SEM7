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
    ImprovedTransform it;

    if (argc != 2) {
        cerr << "Usage: port" << endl;
        exit(0);
    }

    int port = atoi(argv[1]);
    char msg[1500];
     
    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(port);
 
    int serverSd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSd < 0) {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    int bindStatus = bind(serverSd, (struct sockaddr*) &servAddr, sizeof(servAddr));
    if (bindStatus < 0) {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }

    cout << "Waiting for a client to connect..." << endl;
    listen(serverSd, 5);

    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
    int newSd = accept(serverSd, (sockaddr *)&newSockAddr, &newSockAddrSize);
    if (newSd < 0) {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }

    cout << "Connected with client!" << endl;

    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    int bytesRead = 0, bytesWritten = 0;
    while (true) {
        cout << "Awaiting client response..." << endl;
        memset(&msg, 0, sizeof(msg)); // Clear the buffer
        bytesRead += recv(newSd, (char*)&msg, sizeof(msg), 0);
        
        if (strcmp(msg, "exit") == 0) {
            cout << "Client has quit the session" << endl;
            break;
        }
        
         string receivedMsg(msg);
        string decryptedMsg = it.decrypt(receivedMsg);
        cout << "Client: " << decryptedMsg << endl;
        
        cout << ">";
        string data;
        getline(cin, data);
        

        if (data == "exit") {
            string encryptedMsg = it.encrypt(data);
            send(newSd, encryptedMsg.c_str(), encryptedMsg.size(), 0);
            break;
        }

        string encryptedMsg = it.encrypt(data);
        cout << "The Encrypted Message " << data << " is " << encryptedMsg <<  endl;
        send(newSd, encryptedMsg.c_str(), encryptedMsg.size(), 0);
        bytesWritten += encryptedMsg.size();
    }

    gettimeofday(&end1, NULL);
    close(newSd);
    close(serverSd);

    cout << "********Session********" << endl;
    cout << "Bytes written: " << bytesWritten << " Bytes read: " << bytesRead << endl;
    cout << "Elapsed time: " << (end1.tv_sec - start1.tv_sec) << " secs" << endl;
    cout << "Connection closed..." << endl;

    return 0;
}
