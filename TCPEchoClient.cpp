/*
 *   C++ sockets on Unix and Windows
 *   Copyright (C) 2002
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "PracticalSocket.h"  // For Socket and SocketException
#include <iostream>           // For cerr and cout
#include <cstdlib>            // For atoi()
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

const unsigned int MESSAGES = 1000;

int main(int argc, char *argv[]) {

  //string servAddress = "127.0.0.1"; 
  string servAddress = "10.0.0.27";  
  int echoStringLen = 1024;   
  unsigned short echoServPort = 1111;

  try {
    // Establish connection with the echo server
    TCPSocket sock(servAddress, echoServPort);
  
    int messagesSent = 0;
    struct timeval tv;
    std::cout << "BEGIN PUMPINT" << std::endl;
    while (messagesSent < MESSAGES){
      gettimeofday(&tv,NULL);
      long long int currentTime = tv.tv_sec * 1000000 + tv.tv_usec;
      char echoString[1024];
      sprintf(echoString, "%lld", currentTime);
      sock.send(echoString, echoStringLen); 
      messagesSent++;
    }
  } catch(SocketException &e) {
    cerr << e.what() << endl;
    exit(-1);
  }
  std::cout << "END PUMPING" << std::endl;

  return 0;
}
