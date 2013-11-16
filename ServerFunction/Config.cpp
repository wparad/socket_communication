#include "Config.h"

using namespace std;
//Constructor
Config::Config(string configFile){
  Config::load(configFile);
  if(port < 1024 || port > 65535) port=58058;
}

//Deconstructor
Config::~Config(){
}

//setup configuration parameters
void Config::load(string configFile){
  int colon;
  string line;
  ifstream confStream(configFile.c_str());
  if (!confStream.is_open()){
	  cout<<"Config File could not be opened!\n";
	  return;
  }
  while (confStream.good()){
      getline(confStream,line);
      colon=line.find(':');
	  if(     line.substr(0,colon).compare("Port")==0)	stringstream(line.substr(colon+1).c_str())>> port;
	  else if(line.substr(0,colon).compare("Log")==0)	logFile=line.substr(colon+1).c_str();
	 // else if(line.substr(0,colon).compare("Email")==0) ;
	 // else if(line.substr(0,colon).compare("User")==0) ;
	 // else if(line.substr(0,colon).compare("Password")==0) ;
  }
  confStream.close();
}

//generate a logging error
void Config::log(vector<string>message){
	ofstream LogStream;
	LogStream.open(logFile.c_str(), ios::out | ios::app);
	if (!LogStream.is_open()) {
		cout<<"Log File could not be opened!\n";
		cout<<logFile<<endl;
		return;  //if we cannot open that file for writing ignore it.
	}
	for(unsigned int ln=0; ln <message.size() ;++ln){
		LogStream << message[ln] <<endl;
	}
	LogStream.close();
}
int Config::getPort(){
	return port;
}
