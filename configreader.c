#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define MAX_LINE 50

void configChild(char * childName, struct sockaddr_in * childInfo);

void configChild(char * childName, struct sockaddr_in * childInfo){
    //configFile now represents the sysconfig.ini file
    FILE * configFile = fopen("sysconfig.ini", "r");
    //lineRead  : contains a line in the ini as a string.
    //token     : holds a segment of the lineRead.
    char lineRead[MAX_LINE], * token;

    while(1){
        //Reads the ini file line by line
        fgets(lineRead, MAX_LINE, configFile);
        //Separate(Tokenize) the line read using the ":" token
        token = strtok(lineRead, ":");
        //If the first segment contains info for server_address, then...
        if(!strcmp(token, "server_address")){
            //Take the second segment which contains the data
            token = strtok(NULL, ":");
            //And set the childInfo which contains address configuration.
            childInfo -> sin_addr.s_addr = inet_addr(token);
        //If the first segment matches the current child process name, then...
        }else if(!strcmp(token, childName)){
            //Take the second segment which contains the data
            token = strtok(NULL, ":");
            //And set the childInfo which contains port configuration.
            childInfo -> sin_port = htons(atoi(token));
        }
        //End the loop when the program reaches the end of file.
        if(feof(configFile)){
            break;
        }
    }
    //By default ipv4.
    childInfo -> sin_family = AF_INET;
}
