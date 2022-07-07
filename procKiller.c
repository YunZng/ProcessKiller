#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include "configreader.c"

//Used to print error message.
void errorMsg(char *string);
void errorMsg(char *string){
    perror(string);
    exit(0);
}

int main(void){
    //sockStat  : Socket status.
    //msg       : Holds UPD message to send to child.
    int sockStat, msg;

    //child[3]  : Contains the configuration for Child1, Child2, and Child3.
    //killer    : Contains the configuration for procKiller.
    struct sockaddr_in child[3], killer;
    
    //Configurate all children.
    configChild("Child1", &child[0]);
    configChild("Child2", &child[1]);
    configChild("Child3", &child[2]);
    
    //Configure procKiller.
    killer.sin_family = AF_INET;
    killer.sin_port = htons(23455);
    killer.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Creates a socket for the procKiller.
    if((sockStat = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        errorMsg("Killer socket failed");
    }

    //Combine the socket with this program/process.
    if(bind(sockStat, (struct sockaddr *)&killer, sizeof(killer)) == -1){
        errorMsg("Killer bind failed");
    }

    //Print the IP and Port of procKiller.
    printf("Killer address: %s\n", inet_ntoa(killer.sin_addr));
    printf("Killer port: %d\n", ntohs(killer.sin_port));


    //The main job of procKiller is to send assasin to kill the specified child.
    //Actually it only sends a message to the child and the child will die when the child sees the message.
    while(1){
        
        //Ask for a specific child to kill.
        puts("Which child do you wanna kill? (Enter Number)");
        puts("1. Child1");
        puts("2. Child2");
        puts("3. Child3");

        //procKiller takes note.
        scanf("%d", &msg);

        socklen_t size = sizeof(child[0]);
        //Send the data to the specified child.
        if(sendto(sockStat, &msg, sizeof(msg), 0, (struct sockaddr *)&child[msg-1], size) == -1){
            errorMsg("Killer send failed\n");
        }else{
            //Indicates successful sendto, and clear the value for msg.
            puts("Assasin on the way.");
            msg = 0;
        }
    }
    close(sockStat);
    return 0;
}
