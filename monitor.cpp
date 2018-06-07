/*---------------------------------------
 Name: Yicheng Zheng
 USC ID: yichengz
 Student ID: 9152433157
 Session ID: DEN Session
 File name: monitor.cpp
 EE450_SocketProgramming
 Reference used:
 GeeksforGeeks, Socket Programming in C/C++ https://www.geeksforgeeks.org/socket-programming-cc/
 Beej code
 Code Project, A simple UDP time server and client https://www.codeproject.com/Articles/11740/A-simple-UDP-time-server-and-client-for-beginners
 -----------------------------------------*/
#include <cstdlib>
#include <sstream>
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
const unsigned short PORT_SERVER = 26157;
const int    BUFSIZE  = 2048;

int main(){
    
    while(true){
        /*--------------------------------------------
         Create monitor socket
         --------------------------------------------*/
        
        int socket_monitor = socket(AF_INET, TYPE_TCP, 0);// Beej code
        
        if ( socket_monitor < 0) { // if fail, return
            perror("socket_monitor cannot be created");
            return 0;
        }
        
        /*--------------------------------------------
         Connect to AWS Server
         --------------------------------------------*/
        struct sockaddr_in aws_addr;
        aws_addr.sin_family = AF_INET;
        aws_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
        aws_addr.sin_port = htons(PORT_SERVER);
        
        int connectFlag = connect(socket_monitor, (struct sockaddr*)&aws_addr, sizeof(aws_addr));// Beej code
        
        if (connectFlag < 0) {
            perror("socket_monitor cannot connect to AWS Server ");
            return 0;
        }
        std::cout << "The Monitor is up and running. \n";
        
        /*--------------------------------------------
         Receive data from AWS Server
         --------------------------------------------*/
        
        char* function = new char[101];         // receive buffer
        char* word = new char[101];             // receive word buffer
    
//        char* resultNum = new char[BUFSIZE];
        char resultNum[BUFSIZE];
        char oneEditNum[BUFSIZE];
        char exactMatchNum[BUFSIZE];
        char oneEditA[BUFSIZE];
        char oneEditWordA[BUFSIZE];
        
        
        recv(socket_monitor, function, 101 , 0 ); //function name
        recv(socket_monitor, word, 101 , 0 ); //input words
        
        
        // convert variable function_name to string and determine if it's "search" function
        std::string strFun(function);
        string wordstr(word);
        std::string funcSearch = "search";
        int compareFlagSear = funcSearch.compare(strFun);
        
        if (compareFlagSear == 0){
            recv(socket_monitor, oneEditNum, 27 , 0 ); // number of one edit away word
            recv(socket_monitor, exactMatchNum, 27 , 0 ); // number of results received
            std:: string oneEditNumstr(oneEditNum);
            std:: string exactMatchNumstr(exactMatchNum);
            
            if (atoi(oneEditNum) + atoi(exactMatchNum) != 0){
                if (atoi(oneEditNum) !=0){
                    recv(socket_monitor, oneEditWordA, 101 , 0 ); // one edit distance match received if it exists
                    recv(socket_monitor, oneEditA, 101 , 0 ); // definition of one edit distance match received if it exists
                }
                // display results
                if (atoi(exactMatchNum) != 0){
                    std::cout << "Found a match for "<< wordstr << std::endl;
                    std::cout << word << std::endl;
                }
                if (atoi(oneEditNum) != 0){
                    std::cout << "One edit distance match is "<< std::string(oneEditWordA)<<std::endl;
                    std::cout << std::string(oneEditA) <<std::endl;
                }
            }else{
                std::cout << "Found no matches for "<< wordstr << std::endl;
            }
        }else {
            // receiving for function other than search
            
            recv(socket_monitor, resultNum, 27 , 0 ); // number of results received
            std::vector<std::string> wordReceive;
            
            // convert resultNum to int
            std::string resultNumstr(resultNum);
            int numRec = atoi(resultNum);
            
            // Store results in string vector
            if (numRec != 0){
                char* wordrecv = new char[101];
                
                for (int i = 0; i < numRec; i++){
                    recv(socket_monitor, wordrecv, 101 , 0 );
                    std::string strWord(wordrecv);
                    wordReceive.push_back(strWord);
                }
                
                std::cout<< "Found "<< numRec << " matches for "<< word <<std::endl;
                for (int i = 0; i < wordReceive.size(); i++){
                    std::cout<< wordReceive[i]<< std::endl;
                }
            }else {
                std::cout << "Found no matches for "<< wordstr<< std::endl;
            }
        }
        // clear character array
        memset(resultNum, 0, BUFSIZE);
        memset(oneEditNum, 0, BUFSIZE);
        memset(exactMatchNum, 0, BUFSIZE);
        memset(oneEditA, 0, BUFSIZE);
        memset(oneEditWordA, 0, BUFSIZE);

    }
}

