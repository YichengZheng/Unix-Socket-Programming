/*---------------------------------------
 Name: Yicheng Zheng
 USC ID: yichengz
 Student ID: 9152433157
 Session ID: DEN Session
 File name: serverA.cpp
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
const int TYPE_UDP = SOCK_DGRAM; // UDP Datagram Socket
const unsigned short PORT_SERVER = 21157; // Port number for Server A
const unsigned short PORT_CLIENT = 24157; // Port number for AWS
const int    BUFSIZE  = 101;

bool isEditDistanceOne (std::string s1, std::string s2);
bool isPrefix (std::string s1, std::string s2);
bool isSuf (std::string s1, std::string s2);
void sendResult(int socket_a, int total, struct sockaddr_in client_addr, socklen_t addrlen, std::vector<int> exactIndex, std::vector<int> wordIndex, std::vector<std::string> keyword);

int main(){
    
    /*---------------------------------------
     Create UDP socket
     -----------------------------------------*/
    int socket_a = socket(AF_INET, TYPE_UDP, 0);// Beej code
    
    if (socket_a < 0) {
        perror("socket A cannot be created");
        close(socket_a);
        return 0;
    }
    /*---------------------------------------
     Bind socket with address and port
     -----------------------------------------*/
    struct sockaddr_in server_addr; // Beej code
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(LOCAL_IP_ADDRESS);
    server_addr.sin_port = htons(PORT_SERVER);
    
    int bindFlag = :: bind(socket_a, (struct sockaddr*)&server_addr, sizeof(server_addr));// Beej code
    if (bindFlag < 0) {
        perror("socket A cannot be bound ");
        close(socket_a);
        return 0;
    }else{
        std::cout<<"The Server A is up and running using UDP on port "
        <<PORT_SERVER <<std::endl;
    }
    
    /*---------------------------------------
     Read the backend Server dictionary text file
     Using vector to store each line of the text file
     -----------------------------------------*/
    std::vector<std::string> txtData;
    std::ifstream file("backendA.txt");
    std::string line;
    int lineNum = 0;
    if (!file.is_open()){
        perror ("backendA dictionary file cannot be opened");
        return 0;
    }else {
        while (!file.eof()){
            getline(file,line); // read each line of the text file
            if (line !=""){ //discard empty line
                lineNum ++;
                txtData.push_back(line);
            }
        }
    }
    file.close();
    //cout<<linenum<<endl;
    
    // Parse the txtdata vector into two separate vectors to store word and its definition
    std::vector<std::string> keyword;
    std::vector<std::string> worddef;
    for (int i = 0; i <= lineNum; i++){
        int pos = txtData[i].find("::");
        //cout<<pos<<endl;
        if (pos != -1){
            std::string dictword = txtData[i].substr(0,pos - 1);
            // convert words to lower cases
            // transform(dictword.begin(), dictword.end(), dictword.begin(), ::tolower);
            std::string def = txtData[i].substr(pos + 3);
            keyword.push_back(dictword);
            worddef.push_back(def);
        }
    }
    
    /*---------------------------------------
     Receiving Data from AWS
     -----------------------------------------*/
    struct sockaddr_in client_addr;             // remote address
    socklen_t addrlen = sizeof(server_addr);    // length of addresses
    char* function = new char[101];         // receive buffer
    char* word = new char[101];             // receive word buffer
    
    while (true) {
        
        int recvFlagFcn = recvfrom(socket_a, function, 101, 0, (struct sockaddr *)&client_addr, &addrlen);// Beej code
        int recvFlagWord = recvfrom(socket_a, word, 101, 0, (struct sockaddr *)&client_addr, &addrlen);// Beej code
        if(recvFlagFcn < 0 || recvFlagWord < 0 ){
            perror("Error in receiving messages from AWS");
            close(socket_a);
            return 0;
        }
        std::cout<< "The Server A received input "<< word << " and operation "<< function<< std::endl;
        
        // convert variable function from char to string
        std::string strFun(function);
        
        // convert variable word from char to string
        std::string strWord(word);
        
        /*---------------------------------------
         Implements Search function
         -----------------------------------------*/
        
        std::string funcSearch = "search";
        int compareFlagSear = funcSearch.compare(strFun);
        std::vector<int> exactIndex;         // int vector storing indices of words in the dictionary that match exactly
        std::vector<int> wordIndex;          // int vector storing indices of words in the dictionary that match function criteria
        
        std::vector<std::string> exactWordDef;    // string vector storing the definition of words matching exactly
        std::vector<std::string> matchWordDef;         // string vector storing the definition of words that match function criteria
        
        int numMatches = 0;             // int counter counting number of words that match exactly
        int numOneEdit = 0;
        
        if (compareFlagSear == 0){
            for (int i = 0; i < lineNum; i++){
                // find exact matches
                if (strWord.compare(keyword[i]) == 0){
                    numMatches++; // increment the number of matches
                    exactIndex.push_back(i); // push the corresponding indices into searchIndex
                    exactWordDef.push_back(worddef[i]);
                }
                // search one edit distance word
                if(isEditDistanceOne(strWord, keyword[i])){
                    numOneEdit++;
                    wordIndex.push_back(i);
                    matchWordDef.push_back(worddef[i]);
                }
            }
            
            std::cout << "The Server A has found " << std::to_string(numMatches) << " matches and " << std::to_string(numOneEdit) << " similar words" << std::endl;
            
            /*---------------------------------------
             Send output results to AWS
             -----------------------------------------*/
            int total = numMatches + numOneEdit;
            
            sendResult(socket_a, total, client_addr, addrlen, exactIndex, wordIndex, keyword);
        
            if (total != 0){
            // send numOneEdit to AWS
                sendto(socket_a ,std::to_string(numOneEdit).c_str(), 101,
                   0, (struct sockaddr *)&client_addr, addrlen );
            }
            
            // send word and definition of one distance away word to AWS
            if (numOneEdit !=0){
                std::string oneEditdef = worddef[wordIndex[0]];
                std::string oneEditWord = keyword[wordIndex[0]];
                    
                    // sendto(socket_a, oneEditWord.c_str(), strlen(oneEditWord.c_str()),
                    //        0, (struct sockaddr *)&client_addr, addrlen );
                    // sendto(socket_a, oneEditdef.c_str(), strlen(oneEditdef.c_str()),
                    //        0, (struct sockaddr *)&client_addr, addrlen );

                    sendto(socket_a, oneEditWord.c_str(), 101,
                           0, (struct sockaddr *)&client_addr, addrlen );
                    sendto(socket_a, oneEditdef.c_str(), 101,
                           0, (struct sockaddr *)&client_addr, addrlen );
                    //cout <<oneEditdef.c_str()<<endl;
                
            }
        }
        
        /*---------------------------------------
         Implements prefix function
         -----------------------------------------*/
        std::string funcPrefix = "prefix";
        int compareFlagPre = funcPrefix.compare(strFun);
        int numPrefix = 0;
        if (compareFlagPre == 0){
            
            for (int i = 0; i < lineNum; i++){
                // find exact matches
                if (strWord.compare(keyword[i]) == 0){
                    numMatches++; // increment the number of matches
                    exactIndex.push_back(i); // push the corresponding indices into searchIndex
                    exactWordDef.push_back(worddef[i]);
                }
                // find prefix words
                if(isPrefix(strWord, keyword[i])){
                    numPrefix++;
                    wordIndex.push_back(i);
                    matchWordDef.push_back(worddef[i]);
                }
            }
            int total = numPrefix + numMatches;
            std::cout << "The Server A has found "<< std::to_string(total) << " matches" << std::endl;
            
            /*---------------------------------------
             Send output results to AWS
             -----------------------------------------*/
            sendResult(socket_a, total, client_addr, addrlen, exactIndex, wordIndex, keyword);
            

        }
        
        /*---------------------------------------
         Implements suffix function
         -----------------------------------------*/
        std::string funcSuf = "suffix";
        int compareFlagSuf = funcSuf.compare(strFun);
        int numSuf = 0;
        if (compareFlagSuf == 0){
            for (int i = 0; i < lineNum; i++){
                // find exact matches
                if (strWord.compare(keyword[i]) == 0){
                    numMatches++; // increment the number of matches
                    exactIndex.push_back(i); // push the corresponding indices into searchIndex
                    exactWordDef.push_back(worddef[i]);
                }
                // find suffix words
                if(isSuf(strWord, keyword[i])){
                    numSuf++;
                    wordIndex.push_back(i);
                    matchWordDef.push_back(worddef[i]);
                }
            }
            int total = numSuf + numMatches;
            std::cout << "The Server A has found "<< std::to_string(total) << " matches" << std::endl;
            
            /*---------------------------------------
             Send output results to AWS
             -----------------------------------------*/
            sendResult(socket_a, total, client_addr, addrlen, exactIndex, wordIndex, keyword);

            
        }
        matchWordDef.clear();
        exactWordDef.clear();
        exactIndex.clear();
        wordIndex.clear();
    }
}

/*---------------------------------------
 Helper function to send results to AWS
 -----------------------------------------*/
void sendResult(int socket_a, int total, struct sockaddr_in client_addr, socklen_t addrlen, std::vector<int> exactIndex, std::vector<int> wordIndex, std::vector<std::string> keyword){

    // convert int total to string
    std::string tot = std::to_string(total);
    // send to AWS
    sendto(socket_a ,tot.c_str(), 101,
           0, (struct sockaddr *)&client_addr, addrlen );// Beej code
    if(total != 0){

        for (int i = 0; i < exactIndex.size(); i++){
            int sendFlag1 = sendto(socket_a ,keyword[exactIndex[i]].c_str(), 101,
                   0, (struct sockaddr *)&client_addr, addrlen );// Beej code
            // std::cout<<keyword[exactIndex[i]]<< " "<< i<<std::endl;
            // std::cout<<i<<" "<<sendFlag1<<std::endl;
        }

        for (int i = 0; i < wordIndex.size(); i++){
            int sendFlag2 = sendto(socket_a ,keyword[wordIndex[i]].c_str(), 101,
                   0, (struct sockaddr *)&client_addr, addrlen );// Beej code
            // std::cout<<keyword[wordIndex[i]]<< " "<< i<<std::endl;
            // std::cout<<i<<" "<<sendFlag2<<std::endl;
        }
    }
    std::cout << "The Server A finished sending the output to AWS"<< std::endl;
}


/*---------------------------------------
 Helper function to determine if two strings are
 one edit distance away from each other
 - Change a character
 -----------------------------------------*/

bool isEditDistanceOne (std::string s1, std::string s2){
    // find lengths of given strings
    int len1 = s1.length();
    int len2 = s2.length();
    
    if (len1 != len2){
        return false;
    }
    
    int count = 0; // count the number of edits
    int i = 0;
    while (i < len1){
        if (s1[i] != s2[i]){
            count ++;
            i++;
        }else{
            i++;
        }
    }
    return (count == 1);
}

/*---------------------------------------
 Helper function to determine if string s1 is
 prefix of string s2
 -----------------------------------------*/

bool isPrefix (std::string s1, std::string s2){
    int len1 = s1.length();
    int len2 = s2.length();
    if (len1 > len2){
        return false;
    }
    for (int i = 0; i < len1; i++){
        if (s1[i] != s2[i]){
            return false;
        }
    }
    return true;
}


/*---------------------------------------
 Helper function to determine if string s1 is
 suffix of string s2
 -----------------------------------------*/

bool isSuf (std::string s1, std::string s2){
    int len1 = s1.length();
    int len2 = s2.length();
    if(len1 > len2){
        return false;
    }
    for (int i = 0; i < len1; i++){
        if (s1[i] != s2[len2 - len1 + i]){
            return false;
        }
    }
    return true;
}
