#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string.h>

#define BUFLEN 30

int main(int argc, char *argv[]) {
	int pipefd1[2], pipefd2[2];// pipe 1 is P-->C pipe 2 is C-->P
	//char message[2][BUFLEN] = {"Enkokilish\n", "Minawkeleh\n"};
	int pipe1stat, pipe2stat;

	if (!(((argc != 1) || strcmp(argv[1], "-child")!=0 || strcmp(argv[1], "-parent")!=0))){
		fprintf(stderr,"usage: %s -child or %s -parent\n", argv[0], argv[0]);
		exit(0);
	}

	char parentmessage[3][30] = {"Enkokilesh",  "tewat weta mata yemtgeba", "hahah, adelechm",};
	char childmessage[3][30] = {"Minawkeleh",  "gorebetachen", "mndnew eshi?"};

	pid_t pid, childpid;
	char bufp1[BUFLEN], bufp2[BUFLEN];
	int flag=0;
	int i=0;
	int j=0;
	ssize_t readLen, writeLen;

	pipe1stat = pipe(pipefd1);
	if (pipe1stat==-1) printf("Unable to create pipe1\n");
	pipe2stat = pipe(pipefd2);
	if (pipe2stat==-1) printf("Unable to create pipe2\n");


	pid = fork();

	switch (pid) {
		case -1:{
			printf("forking failed\n");
			return 1;
		}
		
		case 0: {
			childpid = getpid();
			if(!strcmp(argv[1], "-parent")) {
				// first close unused discrpters
				flag = close(pipefd1[1]);
				flag = close(pipefd2[0]);
			}
			else if (!strcmp(argv[1], "-child")) {
				flag = close(pipefd1[0]);
				flag = close(pipefd2[1]);
			}
			// read and write to respective pipes
			for(j=1;j<4;j++){
				//printf("--------------conversation %d\n",j );
				for(i=0;i<3;i++) {
					if(!strcmp(argv[1], "-parent")) {
						readLen = read(pipefd1[0], bufp1, sizeof(bufp1));

						printf("from parent:-[ %s ]\n",bufp1);

						writeLen = write(pipefd2[1], childmessage[i], sizeof(childmessage[i]));

						if ((readLen==0) || (writeLen==0)) {
							break;
						}
					}

					else if (!strcmp(argv[1], "-child")) {
						write(pipefd1[1], parentmessage[i], sizeof(parentmessage[i]));
						read(pipefd2[0], bufp2, sizeof(bufp2));

						printf("from parent:- [ %s ]\n\n",bufp2);
					}	
				}
				
			}
			break;
			
		}
		default: {
			// first close unused discrpters
			if(!strcmp(argv[1], "-parent")) {
				flag = close(pipefd1[0]);
				flag = close(pipefd2[1]);
			}
			else if (!strcmp(argv[1], "-child")){
				flag = close(pipefd1[1]);
				flag = close(pipefd2[0]);
			}
			
			for(j=1;j<4;j++){
				printf("conversation--------------- %d\n",j);
				for(i=0;i<3;i++) {
					if (!strcmp(argv[1], "-child")) {
						readLen = read(pipefd1[0], bufp1, sizeof(bufp1));

						printf("from child:-[ %s ]\n",bufp1);

						writeLen = write(pipefd2[1], childmessage[i], sizeof(childmessage[i]));

						if ((readLen==0) || (writeLen==0)) {
							break;
						}
					}
		
					else if(!strcmp(argv[1], "-parent")) {
						write(pipefd1[1], parentmessage[i], sizeof(parentmessage[i]));
						read(pipefd2[0], bufp2, sizeof(bufp2));

						printf("from child:- [ %s ]\n\n",bufp2);}
					}
				}
			}
				
			}
		
	
	return 0;
}