/*---------------------------------------
 Name: Yicheng Zheng
 USC ID: yichengz
 Student ID: 9152433157
 Session ID: DEN Session
 File name: client.cpp
 EE450_SocketProgramming
 Reference used:
 GeeksforGeeks, Socket Programming in C/C++ https://www.geeksforgeeks.org/socket-programming-cc/
 Beej code
 Code Project, A simple UDP time server and client https://www.codeproject.com/Articles/11740/A-simple-UDP-time-server-and-client-for-beginners
 -----------------------------------------*/
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <string.h>
#include <vector>

using namespace std;

// Declare host address, socket type, connection number and send message default size, and port number used
const char* LOCAL_IP_ADDRESS = "127.0.0.1";
const int TYPE_TCP = SOCK_STREAM;
const unsigned short PORT_SERVER = 25157;
const int    BUFSIZE  = 101;

int main(int argc, char **argv){
    
    std:: string strword = argv[2];
    std:: string strFun = argv[1];
    char word[strword.length() + 1];
    for (int i = 0; i < strword.length(); i++){
        word[i] = strword[i];
    }
    word[strword.length()] = '\0';
    
    // if it's prefix and search function, convert first character of word to upper case
    std::string funcSearch = "search";
    std::string funcPrefi = "prefix";

    // if the user provided function is search or prefix, convert the first character of the word to upper case
    if (funcSearch.compare(strFun) == 0 || funcPrefi.compare(strFun )== 0){
        word[0] = toupper(word[0]);
    }
    strword = string(word);
    //cout<<"word is "<<strword<<endl;
    //cout<< "word length is "<<strword.length()<<endl;
    
    /*--------------------------------------------
     Create client socket
     --------------------------------------------*/
    int socket_client = socket(AF_INET, TYPE_TCP, 0);// Beej code
    
    if ( socket_client < 0) { // if fail, return
        perror("socket_client cannot be created");
        return 0;
    }
    /*--------------------------------------------
     Connect to AWS Server
     --------------------------------------------*/
    struct sockaddr_in aws_addr;
    aws_addr.sin_family = AF_INET;
    aws_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    aws_addr.sin_port = htons(PORT_SERVER);
    
    int connectFlag = connect(socket_client, (struct sockaddr*)&aws_addr, sizeof(aws_addr));// Beej code
    
    if (connectFlag < 0) {
        perror("socket_client cannot connect to AWS Server ");
        return 0;
    }
    std::cout << "The client is up and running. \n";
    
    /*--------------------------------------------
     Send data to AWS Server
     --------------------------------------------*/
    
    send(socket_client, strFun.c_str(), 101, 0);
    send(socket_client, strword.c_str(), 101, 0);
    
    std::cout << "The client sent " << strword << " and " << strFun << " to AWS."<< std::endl;
    
    /*--------------------------------------------
     Receive data from AWS Server
     --------------------------------------------*/
    std::vector<std::string> wordReceive;
    char* resultNum = new char[27];
    recv(socket_client, resultNum, 27 , 0 ); // number of results received
    
    // convert resultNum to int
    int intNum = atoi(resultNum);
    
    
    // convert variable function_name to string and determine if it's "search" function
    int compareFlagSear = funcSearch.compare(strFun);
    
    // Store results in string vector
    
    if (intNum != 0){
        char* wordrecv = new char[101];
        for (int i = 0; i < intNum; i++){
            recv(socket_client, wordrecv, 101 , 0 );
            std::string strWord(wordrecv);
            wordReceive.push_back(strWord);
        }
        if (compareFlagSear == 0){
            std::cout<< "Found a match for "<< strword <<std::endl;
            std::cout << wordReceive[0] << std::endl;
        }else {
            std::cout<< "Found "<< intNum << " matches for "<< strword <<std::endl;
            for (int i = 0; i < wordReceive.size(); i++){
                std::cout<< wordReceive[i]<< std::endl;
            }
        }
        memset(wordrecv,0,101);
    }else {
        std::cout << "Found no matches for "<< strword <<std::endl;
    }
    close(socket_client);
    return 0;
    
}

