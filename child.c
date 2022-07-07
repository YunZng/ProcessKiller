#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include "configreader.c"

//Print error message.
void errorMsg(char *string);
void errorMsg(char *string){
    perror(string);
    exit(0);
}

int main(int arg, char **argv){

	printf("Child: %s Activated ---- PID:%d\n", argv[1], getpid());

    //sockStat  : Socket status.
    //msg       : Holds UPD message sent from procKiller.
    int sockStat, msg;
    //Configuration structures for child and procKiller.
    struct sockaddr_in childInfo, killer;

    //Configurate killer.
    killer.sin_family = AF_INET;
    killer.sin_port = htons(23455);
    killer.sin_addr.s_addr = inet_addr("127.0.0.1");

    //Configurate child.
    configChild(argv[1], &childInfo);

    //Set up socket.
    if((sockStat = socket(AF_INET, SOCK_DGRAM, 0)) == -1){
        errorMsg("Child socket failed");
    }

    //Combine the socket and this program/process.
    if(bind(sockStat, (struct sockaddr *)&childInfo, sizeof(childInfo)) == -1){
        errorMsg("Child bind failed");
    }

	while(1){
        socklen_t size = sizeof(killer);

        //Child process will only sit and wait for death call.
        //The following will run if it receives something from the procKiller.
        if(recvfrom(sockStat, &msg, sizeof(msg), 0, (struct sockaddr *)&childInfo, &size) == -1){
            errorMsg("Child failed to receive");
        }else{
            //If the message is received, then print its info and exit/die.
            printf("Child: %s with PID %d terminated\n", argv[1], getpid());
            exit(0);
        }

		fflush(stdout);
	}
    close(sockStat);
	return 0;
}