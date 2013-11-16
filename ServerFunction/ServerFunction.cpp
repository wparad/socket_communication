// ServerFunction.cpp : Defines the entry point for the console application.
#include "stdafx.h"
#include "Config.h"
#include "Connection.h"
//#include "Socket.h"

using namespace std;
int main(int argc, char* argv[])
{
	char tmp[LOG_STRING_SIZE]; //,tmp_time[TIME_SIZE];  //save mode
	time_t rawtime;
	vector<string> message(3);
//	vector<Connection> Connections;
	string message_string;
	Config Configuration("/etc/ServerFunction/server.conf");

	
	time(&rawtime );
	message[0]="---Server Online with the following parameters---";
	sprintf(tmp,"Port: %d",Configuration.getPort());  //sprintf_s(tmp,LOG_STRING_SIZE,"Port: %d",Configuration.getPort());
	message[1]=tmp;
	sprintf(tmp,"Server Started At: %s",ctime(&rawtime));  //sprintf_s(tmp,LOG_STRING_SIZE,"Server Started At: %s",ctime_s(tmp_time,TIME_SIZE,&rawtime));
	message[2]=tmp;
	Configuration.log(message);
	message.clear();

	//Actually start the server now

	int sockfd, newsockfd, portno, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
		message_string="ERROR: Socket cannot be opened, quitting...";
		message.push_back(message_string);
		Configuration.log(message);
		message.clear();
		return ERROR_SOCKET_NOT_OPEN;
	}
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(Configuration.getPort());
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)  {
		message_string="ERROR: programm cannot bind to socket, quitting...";
		message.push_back(message_string);
		Configuration.log(message);
		message.clear();
		return ERROR_SOCKET_NOT_BIND;
	}
    listen(sockfd,5);  //queue 5 connections
    clilen = sizeof(cli_addr);
    for(;;) {
//		for(int ln=0;ln < Connections.size(); ++ln){
//			if(Connections[ln].timeout()){
				//kill PID
//				Connections.erase(ln);
//			}
//		}
		
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);	//waiting for client to connect
        if (newsockfd < 0){	//accepted a client connection
            message_string="ERROR: socket cannot accept connection, quitting...";
			message.push_back(message_string);
			Configuration.log(message);
			message.clear();
			close(sockfd);
			return ERROR_SOCKET_NOT_ACPT;
		}
        pid = fork();		//after a client connects we want to allow that connection to wait in its own process   (deal with the zombie later)
        if (pid < 0){		//we could not fork that means we will not get a new process to run this connection in
            message_string="ERROR: programm cannot fork new connection, quitting...";
			message.push_back(message_string);
			Configuration.log(message);
			message.clear();
			close(sockfd);
			close(newsockfd);
			return ERROR_SOCKET_NOT_FORK;
		}
        if (pid == 0)  {//SUBPROCESS		//after the client connects new process will close the main socket to allow another connection
			close(sockfd);
            Connection connect(newsockfd);  //last thing executed in this function, also need handle to stop the main process remotely
            exit(0);		//before doing this we need to deal with the zombie process  (DO NOT ALLOW THE ZOMBIE here to continue)
        }
        else {  //MAIN PROCESS
	//		Connections.push_back(newsockfd);
			close(newsockfd);  //after the client connect the main process thread will release the handle on the next connection.
			sprintf(tmp,"New Connection Created in PID: %d, Socket: %d",pid,newsockfd);
			message.push_back(tmp);
			Configuration.log(message);
			message.clear();
		}
    }
   
	
	close(sockfd);

	cout<<"Done...Now Exiting\n";  //Like we should ever get here
	return 0;
}

