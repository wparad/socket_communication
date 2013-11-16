#ifndef _CONNECTION_H
#define _CONNECTION_H

#include "stdafx.h"

class Connection
{
 public:

  Connection(int);
  Connection (){};
  virtual ~Connection();
  void Run(int);
  bool Command(char*);
  bool timeout() const;
private:
  int socket;
  clock_t last_command;
//  int pid;

};

#endif