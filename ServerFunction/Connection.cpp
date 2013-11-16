#include "Connection.h"
using namespace std;

Connection::Connection(int _Socket){
	socket=_Socket;
	last_command=clock();
	Run(socket);
}

Connection::~Connection(){
	close(socket);
}
//Handle the subprocess connection indefinitely
void Connection::Run(int socket){
	int n,read_fail=0;
	char buffer[BUFFER_SIZE];
	vector<string> message(3);
	string message_string;
	for(;;){
		bzero(buffer,BUFFER_SIZE);
		n = read(socket,buffer,BUFFER_SIZE-1); //start blocking and wait for the read, we probably should implement a timeout here, run from the parent process to kill this one.
		if ((n<0)||(buffer == 0)) && read_fail<5){	//if we had a problem reading we probably should just try again
			read_fail++;
			continue;
	/*		message_string="ERROR: Cannot Read from write...";
			message.push_back(message_string);
			Configuration.log(message);
			message.clear();
			exit(ERROR_SOCKET_NOT_OPEN);	*/  //Not helpful since PID is going to be 0, we should grab the actual PID from the process if we want to track these errors
		}
		else if(read_fail > 4) break;
		else read_fail=0;

		cout<<"command in: "<<buffer<<endl;		//This is still displaying multiple times, I think there is something weird with the read command
		if(!Command(buffer)) break;
		last_command=clock();
		cout<<endl;
		n = write(socket,"Acknowledgement",15);
		if (n < 0){	//since we are just going to write here, a failure to do so should just immediately end the process.
			break;
		}
	}//end while
}
//Run an OS command on the server, return success
bool Connection::Command(char *buffer){
	//we need to parse buffer in here
	string osCMD(buffer);
	osCMD=osCMD.substr(0,100);
//	string CHKSUM(osCMD,101,100);	//Not yet implemented
//	osCMD="ls / > /tmp/serverOutput.txt"; 		//testing
	osCMD.append(" 2>&1");
/*	FILE *p = popen(osCMD.c_str(), "r");
	if(p == NULL) return false;
	while(fgets(data, sizeof(data), p) != NULL){
		output.append(data);
	}
	pclose(p);
*/
	if(osCMD.find(":q!")) return false;	//prevent PID leak
	cout<<osCMD.c_str())<<endl;
	//system(osCMD.c_str());	//I dont think I need the pipe here, we really should not be return data to the client.  Maybe in a later version of this program.
	return true; 
}
bool Connection::timeout() const{
  return (last_command + 60*20*CLOCKS_PER_SEC) > clock();		//timeout if we have past 20 minutes without a connection
}