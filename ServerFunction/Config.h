// Configuration file loader, possible to have more than one configuration
#ifndef _CONFIG_H
#define _CONFIG_H

#include "stdafx.h"

class Config
{
 public:

  Config(std::string);
  Config (){};
  virtual ~Config();
  void load (std::string);
  void log(std::vector<std::string>);
  int getPort();
private:
  std::string logFile;
  int port;

};

#endif
