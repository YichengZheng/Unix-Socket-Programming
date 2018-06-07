/*---------------------------------------
 Name: Yicheng Zheng
 USC ID: yichengz
 Student ID: 9152433157
 Session ID: DEN Session
 File name: aws.cpp
 EE450_SocketProgramming
 Reference:
 GeeksforGeeks, Socket Programming in C/C++ https://www.geeksforgeeks.org/socket-programming-cc/
 Beej code
 Code Project, A simple UDP time server and client https://www.codeproject.com/Articles/11740/A-simple-UDP-time-server-and-client-for-beginners
 -----------------------------------------*/

#include <cstdlib>
#include <sstream>
#include <iostream>
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
#include <typeinfo>

using namespace std;


// Declare host address, socket type, connection number and send message default size, and port number used
const char* LOCAL_IP_ADDRESS = "127.0.0.1";
const unsigned short PORT_TCP_CLIENT = 25157;
const unsigned short PORT_TCP_MONITOR = 26157;
const unsigned short PORT_UDP = 24157;
const unsigned short PORT_UDP_C = 23157;
const unsigned short PORT_UDP_B = 22157;
const unsigned short PORT_UDP_A = 21157;
const int TYPE_TCP = SOCK_STREAM;
const int TYPE_UDP = SOCK_DGRAM;
const int MAX_CONNECTION = 5;
const int    BUFSIZE  = 101;

//void receiveResult(int socket_udp, char* numOneEdit, int resultint, int BUFSIZE, struct sockaddr_in udp_addr, socklen_t len, vector<string> wordServer, int compareFlagSear, vector<string> oneEditDefServer, vector<string> oneEditWordServer, string serverName, const unsigned short PORT_UDP);

int main(){
    /*---------------------------------------
     Create TCP client socket
     -----------------------------------------*/
    // Create client socket
    int socket_tcp_client = socket(AF_INET, TYPE_TCP, 0); // Beej code
    if (socket_tcp_client < 0) {
        perror("socket AWS cannot be created");
        close(socket_tcp_client);
        return 0;
    }
    
    // Create local address for TCP client
    struct sockaddr_in tcp_client_addr;
    tcp_client_addr.sin_family = AF_INET;
    tcp_client_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    tcp_client_addr.sin_port = htons(PORT_TCP_CLIENT);
    
    /*---------------------------------------
     Create TCP monitor socket
     -----------------------------------------*/
    // Create monitor socket
    int socket_tcp_monitor = socket(AF_INET, TYPE_TCP, 0);
    if (socket_tcp_monitor < 0) {
        perror("socket AWS cannot be created");
        close(socket_tcp_monitor);
        return 0;
    }
    
    //Create local address for TCP monitor
    struct sockaddr_in tcp_monitor_addr;
    tcp_monitor_addr.sin_family = AF_INET;
    tcp_monitor_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    tcp_monitor_addr.sin_port = htons(PORT_TCP_MONITOR);
    
    /*---------------------------------------
     Bind TCP client & monitor sockets with addresses
     -----------------------------------------*/
    int bindFlagClient = ::bind(socket_tcp_client, (struct sockaddr *)&tcp_client_addr, sizeof(tcp_client_addr));
    if (bindFlagClient < 0) {
        perror("socket AWS client cannot be bound ");
        close(socket_tcp_client);
        return 0;
    }
    
    int bindFlagMonitor = ::bind(socket_tcp_monitor, (struct sockaddr *)&tcp_monitor_addr, sizeof(tcp_monitor_addr));
    if (bindFlagMonitor < 0) {
        perror("socket AWS monitor cannot be bound ");
        close(socket_tcp_monitor);
        return 0;
    }
    
    std::cout << "The AWS is up and running." << std::endl;
    
    
    /*---------------------------------------
     Listen connections
     -----------------------------------------*/
    int listen_Flag = listen(socket_tcp_client, MAX_CONNECTION);
    int listen_Flag_Monitor = listen(socket_tcp_monitor, MAX_CONNECTION);
    char* function_namep = new char[101]; // function name suffix, search, and prefix are all 6 characters
    char* wordp = new char[101];
    
    while(true){
        socklen_t len = sizeof(tcp_client_addr);
        int socket_client = accept(socket_tcp_client, (struct sockaddr *)&tcp_client_addr, &len); //GeeksforGeeks, Socket Programming in C/C++
        if (socket_client < 0) {
            perror("AWS client cannot be bound");
            close(socket_client);
            return 0;
        }
        
        socklen_t lenMon = sizeof(tcp_monitor_addr);
        int socket_monitor = accept(socket_tcp_monitor, (struct sockaddr *)&tcp_monitor_addr, &lenMon);
        if (socket_monitor < 0) {
            perror("AWS monitor cannot be bound");
            close(socket_monitor);
            return 0;
        }
        
        // Receiving data from the client
        
        recv(socket_client, function_namep, 101, 0);//GeeksforGeeks, Socket Programming in C/C++
        recv(socket_client, wordp, 101, 0); //GeeksforGeeks, Socket Programming in C/C++
        std::string word = std:: string(wordp);
        std::string function_name = std:: string(function_namep);
        
        std::cout<< "The AWS received input "<< word << " and function " << function_name << " from the client"
        << " using TCP over port "<<PORT_TCP_CLIENT<< "." << std::endl;
        /*---------------------------------------
         Create UDP socket
         -----------------------------------------*/
        int socket_udp = socket(AF_INET, TYPE_UDP, 0);
        if (socket_udp < 0) {
            perror("socket UDP cannot be created");
            close(socket_udp);
            return 0;
        }
        
        struct sockaddr_in udp_addr;
        udp_addr.sin_family = AF_INET;
        udp_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
        udp_addr.sin_port = htons(PORT_UDP);
        
        // --------- Bind UDP socket
        int bindFlagUDP = ::bind(socket_udp, (struct sockaddr *)&udp_addr, sizeof(udp_addr));//Beej
        if (bindFlagUDP < 0) {
            perror("socket UDP cannot be bound ");
            close(socket_udp);
            return 0;
        }
        

        // Server A UDP address and port number
        struct sockaddr_in udp_a_addr;// Beej code
        udp_a_addr.sin_family = AF_INET;
        udp_a_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
        udp_a_addr.sin_port = htons(PORT_UDP_A);
        socklen_t len_A = sizeof(udp_a_addr);
        char* resultA = new char[101];

        


        // Server B UDP address and port number
        struct sockaddr_in udp_b_addr; // Beej code
        udp_b_addr.sin_family = AF_INET;
        udp_b_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
        udp_b_addr.sin_port = htons(PORT_UDP_B);
        socklen_t len_B = sizeof(udp_b_addr);
        char* resultB = new char[101];

        
        
        // Server C UDP address and port number
        struct sockaddr_in udp_c_addr;// Beej code
        udp_c_addr.sin_family = AF_INET;
        udp_c_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
        udp_c_addr.sin_port = htons(PORT_UDP_C);
        socklen_t len_C = sizeof(udp_c_addr);
        char* resultC = new char[101];
        
        /*---------------------------------------
         String vectors for storing results
         -----------------------------------------*/
        
        std::vector<std::string> wordServer; // string vector storing all matching words
        std::vector<std::string> oneEditDefServer; // string vector storing definition of one edit distance word
        std::vector<std::string> oneEditWordServer; // string vector storing one edit distance word
        
        /*---------------------------------------
         Send and Receive data from backend server A
         -----------------------------------------*/
        // Send data to Server A
        int sendFlagFcn = sendto(socket_udp, function_namep, 101, 0, (struct sockaddr *)&udp_a_addr, len_A);// Beej code
        // Send intput to Server A
        int sendFlagWord = sendto(socket_udp, wordp, 101, 0, (struct sockaddr *)&udp_a_addr, len_A);// Beej code
        if (sendFlagFcn < 0 || sendFlagWord < 0){
            perror("Message cannot be sent");
            return 0;
        }
        std::cout<< "The AWS sent "<< word << " and "<< function_name << " to Backend-Server A"<< std::endl;
        
        // convert variable function_name to string and determine if it's "search" function
        
        std::string funcSearch = "search";
        int compareFlagSear = funcSearch.compare(function_name);

        // Receive number of results from server A
        recvfrom(socket_udp, resultA, 101, 0, (struct sockaddr *)&udp_a_addr, &len_A);// GeeksforGeeks, Socket Programming in C/C++
        // Convert resultA from char to int
        std::string strResA(resultA);




        std::stringstream resultAss(strResA);
        int resultAint = 0;
        resultAss >> resultAint;


        char* numOneEditA = new char[101];

        // Store results from A in string vector
        if (resultAint != 0){           

            for (int i = 0; i < resultAint; i++){
                char* wordA =  new char[101]; 

                int recvFlag = recvfrom(socket_udp, wordA, 101, 0, (struct sockaddr *)&udp_a_addr, &len_A); //GeeksforGeeks, Socket Programming in C/C++

                string strWordA(wordA);

                std:: cout<< wordA << std:: endl;

                wordServer.push_back(strWordA);

                memset(wordA,0,101);

            }
            if (compareFlagSear == 0){

                // if it's search function, receive number of one edit word

                recvfrom(socket_udp, numOneEditA, 101, 0, (struct sockaddr *)&udp_a_addr, &len_A);
                
                int intnumOneEdit = atoi(numOneEditA);

                
                if (intnumOneEdit != 0){
                    // store one edit distance away words definitions
                    char* oneEditAdef = new char[101];
                    char* oneEditAword = new char[101];
                    
                    recvfrom(socket_udp, oneEditAword, 101, 0, (struct sockaddr *)&udp_a_addr, &len_A);
                    recvfrom(socket_udp, oneEditAdef, 101, 0, (struct sockaddr *)&udp_a_addr, &len_A);
                    
                    std::string strOneEditWordA(oneEditAword);
                    oneEditWordServer.push_back(strOneEditWordA);
                    
                    std::string strOneEditA(oneEditAdef);
                    oneEditDefServer.push_back(strOneEditA);
                    memset(oneEditAdef, 0, 101);
                    memset(oneEditAword, 0, 101);
                    
                }
                std::cout << "AWS receives "<< resultAint << " similar words from Backend-Server A using UDP port "<<PORT_UDP_A<<std::endl;
            }
            else{
                std::cout << "AWS receives " << resultAint<< " matches from Backend-Server A using UDP over port "<< PORT_UDP_A<< std::endl;
            }
        }else {
            std::cout<< "No match found on Server A"<< std::endl;
        }
        
        
        /*---------------------------------------
         Send and Receive data from backend server B
         -----------------------------------------*/
        // Send data to Server B
        int sendFlagFcnB = sendto(socket_udp, function_namep, 101, 0, (struct sockaddr *)&udp_b_addr, len_B);
        
        // Send intput to Server B
        int sendFlagWordB = sendto(socket_udp, wordp, 101, 0, (struct sockaddr *)&udp_b_addr, len_B);
        if (sendFlagFcnB < 0 || sendFlagWordB < 0){
            perror("Message cannot be sent");
            return 0;
        }
        std::cout<< "The AWS sent "<< word << " and "<< function_name << " to Backend-Server B"<< std::endl;
        
        // Receive number of results from server B
        
        recvfrom(socket_udp, resultB, 101, 0, (struct sockaddr *)&udp_b_addr, &len_B);// Beej code
        // Convert resultB from char to int
        std::string strResB(resultB);
        


        std::stringstream resultBss(strResB);
        int resultBint = 0;
        resultBss >> resultBint;
        //std::cout<< "Result from B: "<< resultBint<<std::endl;

        char* numOneEditB = new char[101]; 
     
        // Store results from B in string vector
        if (resultBint != 0){
            

            for (int i = 0; i < resultBint; i++){
                char* wordB = new char[101];
                //cout<<"Im here-1B, i= "<< i<<endl;

                int recvBFlag = recvfrom(socket_udp, wordB, 101, 0, (struct sockaddr *)&udp_b_addr, &len_B);
                //cout<<"Im here1B, i= "<< i<<recvBFlag<<endl;
                std::string strWordB(wordB);
                wordServer.push_back(strWordB);
                //delete[] wordB;
                memset(wordB,0,101);
            }
            if (compareFlagSear == 0){
                

                // if it's search function, receive number of one edit word
                recvfrom(socket_udp, numOneEditB, 101, 0, (struct sockaddr *)&udp_b_addr, &len_B);
                
                // std::stringstream numOneEditBSS(numOneEditB);
                // int intnumOneEditB = 0;
                // numOneEditBSS >> intnumOneEditB;

                int intnumOneEditB = atoi(numOneEditB);
                //cout<< "numOneEditB"<<intnumOneEditB<<endl;
                //delete[] wordB;

                
                if (intnumOneEditB != 0){
                    // store one edit distance away words definitions
                    char* oneEditBdef = new char[101];
                    char* oneEditBword = new char[101];
                    
                    recvfrom(socket_udp, oneEditBword, 101, 0, (struct sockaddr *)&udp_b_addr, &len_B);
                    recvfrom(socket_udp, oneEditBdef, 101, 0, (struct sockaddr *)&udp_b_addr, &len_B);
                    
                    std::string strOneEditWordB(oneEditBword);
                    oneEditWordServer.push_back(strOneEditWordB);
                    
                    std::string strOneEditB(oneEditBdef);
                    oneEditDefServer.push_back(strOneEditB);
                    memset(oneEditBdef, 0, 101);
                    memset(oneEditBword, 0, 101);
                    
                }
                std::cout << "AWS receives "<< resultBint << " similar words from Backend-Server B using UDP port "<<PORT_UDP_B<<std::endl;
            }
            else{
                std::cout << "AWS receives " << resultBint<< " matches from Backend-Server B using UDP over port "<< PORT_UDP_B<< std::endl;
            }
        }else {
            std::cout<< "No match found on Server B"<< std::endl;
        }
        
         /*---------------------------------------
         Send and Receive data from backend server C
         -----------------------------------------*/
        
        // Send data to Server C
        int sendFlagFcnC = sendto(socket_udp, function_namep, 101, 0, (struct sockaddr *)&udp_c_addr, len_C);
        
        // Send intput to Server C
        int sendFlagWordC = sendto(socket_udp, wordp, 101, 0, (struct sockaddr *)&udp_c_addr, len_C);
        if (sendFlagFcnC < 0 || sendFlagWordC < 0){
            perror("Message cannot be sent");
            return 0;
        }
        std::cout<< "The AWS sent "<< word << " and "<< function_name << " to Backend-Server C"<< std::endl;
        
        // Receive number of results from server C
        
        recvfrom(socket_udp, resultC, 101, 0, (struct sockaddr *)&udp_c_addr, &len_C);
        // Convert resultB from char to int
        std::string strResC(resultC);

        stringstream resultCss(strResC);
        int resultCint = 0;
        resultCss >> resultCint;
        //cout<< "Result from C: "<< resultCint<<endl;

        char* numOneEditC = new char[101];
        
        // Store results from C in string vector
        if (resultCint != 0){
            

            for (int i = 0; i < resultCint; i++){
                char* wordC = new char[101];
                recvfrom(socket_udp, wordC, 101, 0, (struct sockaddr *)&udp_c_addr, &len_C);
                std::string strWordC(wordC);
                wordServer.push_back(strWordC);
                memset(wordC, 0, 101);
            }
            if (compareFlagSear == 0){
                
                // if it's search function, receive number of one edit word
                recvfrom(socket_udp, numOneEditC, 101, 0, (struct sockaddr *)&udp_c_addr, &len_C);
                
                int intnumOneEditC = atoi(numOneEditC);
                
                if (intnumOneEditC != 0){
                    // store one edit distance away words definitions
                    char* oneEditCdef = new char[101];
                    char* oneEditCword = new char[101];
                    
                    recvfrom(socket_udp, oneEditCword, 101, 0, (struct sockaddr *)&udp_c_addr, &len_C);// Beej code
                    recvfrom(socket_udp, oneEditCdef, 101, 0, (struct sockaddr *)&udp_c_addr, &len_C);// Beej code
                    
                    std::string strOneEditWordC(oneEditCword);
                    
                    oneEditWordServer.push_back(strOneEditWordC);
                    
                    std::string strOneEditC(oneEditCdef);
                    oneEditDefServer.push_back(strOneEditC);
                    memset(oneEditCword, 0, 101);
                    memset(oneEditCdef, 0, 101);
                    
                }
                std::cout << "AWS receives "<< resultCint << " similar words from Backend-Server C using UDP port "<<PORT_UDP_C<<std::endl;
            }
            else{
                std::cout << "AWS receives " << resultCint<< " matches from Backend-Server C using UDP over port "<< PORT_UDP_C<< std::endl;
            }
        }else {
            std::cout<< "No match found on Server C"<< std::endl;
        }
        
        // Close UDP Socket
        close(socket_udp);
        
       // total number of results
  
        int totalRes = resultAint + resultBint + resultCint;
        std::string totalResStr = std::to_string(totalRes);
        
        /*---------------------------------------
         Send data to client & monitor via TCP
         -----------------------------------------*/
        // send function name

        send(socket_monitor, function_namep, 101, 0);// send input words
        send(socket_monitor, wordp, 101, 0);// send input words
        
        // if function is search, send number of oneEdit word and number of exact match separately
        
        if (compareFlagSear == 0){

            int intnumOneEdit = atoi(numOneEditA) + atoi(numOneEditB)+ atoi(numOneEditC);
            std::string totalOneEdit = std::to_string(intnumOneEdit);
            
            int numMatch = totalRes - intnumOneEdit; // get number of exact match
            
            send(socket_monitor, totalOneEdit.c_str(), 27, 0); // send number of oneEdit word

            std::string strNumMatch = std::to_string(numMatch);
            send(socket_monitor, strNumMatch.c_str(), 27, 0); // send number of exact match word
            
            if (totalRes !=0){

                if(intnumOneEdit!= 0){
                    send(socket_monitor, oneEditWordServer[0].c_str(), 101, 0); // send oneEdit word

                    send(socket_monitor, oneEditDefServer[0].c_str(), 101, 0); // send definition of oneEdit word

                }

                if (numMatch != 0 && intnumOneEdit!= 0){
                    std::cout<< "The AWS sent " <<word << " and " << string(oneEditWordServer[0]) << " to the monitor via TCP port " << PORT_TCP_MONITOR<< " for search"<< std::endl;
                    
                }else{
                    std::cout<< "The AWS sent " <<word << " to the monitor via TCP port " << PORT_TCP_MONITOR<< " for search"<< std::endl;
                }
                
            }
            send(socket_client, totalResStr.c_str(), 27, 0);// send number of results to client
            for (int i = 0; i < wordServer.size(); i++){
                send(socket_client, wordServer[i].c_str(), 101, 0);
            }
            
        }else{
            send(socket_monitor, totalResStr.c_str(), 27, 0);// send number of results to monitor
            
            send(socket_client, totalResStr.c_str(), 27, 0);// send number of results to client
            
            for (int i = 0; i < wordServer.size(); i++){
                send(socket_client, wordServer[i].c_str(), 101, 0);
                send(socket_monitor, wordServer[i].c_str(), 101, 0);
            }
            std::cout<< "The AWS sent "<< totalResStr << " matches to client"<< std::endl;
            std::cout<< "The AWS sent "<< totalResStr << " matches to the monitor via TCP port "<<PORT_TCP_MONITOR << " for "<<function_name<<std::endl;
        }
        

        // Reset all lingering pointers and arrays 
        memset(wordp,0,101);
        memset(function_namep,0,101);
        memset(numOneEditB,0,101);
        memset(numOneEditA,0,101);
        memset(numOneEditC, 0, 101);
        memset(resultB,0,101);
        memset(resultA,0,101);
        memset(resultC, 0, 101);

        wordServer = vector<string>();
        oneEditDefServer = vector<string>();
        oneEditWordServer = vector<string>();

    }
}
