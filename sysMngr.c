#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

void startChild(int index, int status[], char **children[]);
int findChild(int status[], int pid);

int main(void) {

	//status: store the pid number for each child, status[0] = Child1, status[2] = Child2,...
	//pid 	: record the pid number of a dead child.
	//index	: holds the value returned by findChild function.
	int status[3], pid, index;

	//These data will be used for the execvp function.
	char *child1[] = {"./child", "Child1", NULL};
	char *child2[] = {"./child", "Child2", NULL};
	char *child3[] = {"./child", "Child3", NULL};
	char **children[] = {child1, child2, child3};

	//Use a for loop to create 3 children.
	for(int i = 0; i < 3; i++){
		//Record the PID.
		status[i] = fork();
		//Child process will turn into a ./child program.
		if(status[i] == 0){
			execvp(children[i][0], children[i]);
			//If execvp success, this will not be displayed.
			puts("execvp failed");
		//Parent notify user the activation of a child process with name and PID.
		}else{
			printf("Parent: %s with pid %d activated.\n", children[i][1], status[i]);
		}
	}

	puts("Parent activated");
  	while(1){
		//Parent forever waits for death of child
		//This is like a block call, it will only execute if any child dies,
		//and wait(NULL) returns the PID of the killed function.
  		pid = wait(NULL);
		if(pid > 0){
			//Helps find out the identity of the dead child.
			index = findChild(status, pid);
  			printf("Parent: %s with pid %d killed.\n", children[index][1], pid);
			printf("Reviving...\n");
			//Revives the dead child.
			startChild(index, status, children);
		}
	}
  	return 0;
}

//Function to restart child.
void startChild(int index, int status[], char **children[]){
	//Record PID of revived child.
	int newPid = fork();
	//Child will run the ./child program like mentioned earlier.
	if(newPid == 0){
		execvp(children[index][0], children[index]);
		puts("execvp failed");
	//Parent will update the PID on the PID list.
	}else{
		//Parent updates the child pid in the status array.
		printf("Parent: %s with pid %d revived.\n", children[index][1], newPid);
		status[index] = newPid;
	}
}

//Use a for loop to find out the index of a specific PID.
int findChild(int status[], int pid){
	for(int i = 0; i < 3; i++){
		if(pid == status[i]){
			return i;
		}
	}
	return -1;
}

