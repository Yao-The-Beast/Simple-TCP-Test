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

#include "PracticalSocket.h"  // For Socket, ServerSocket, and SocketException
#include <iostream>           // For cerr and cout
#include <vector>
#include <cstdlib>            // For atoi()
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

const unsigned int MESSAGES = 10000;

const unsigned int RCVBUFSIZE = 1024;    // Size of receive buffer

void HandleTCPClient(TCPSocket *sock); // TCP client handling function

int main(int argc, char *argv[]) {

  unsigned short echoServPort = 1111;  // First arg: local port

  try {
    TCPServerSocket servSock(echoServPort);     // Server Socket object
  
    for (;;) {   // Run forever
      HandleTCPClient(servSock.accept());       // Wait for a client to connect
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED

  return 0;
}

// TCP client handling function
void HandleTCPClient(TCPSocket *sock) {
  cout << "Handling client ";
  try {
    cout << sock->getForeignAddress() << ":";
  } catch (SocketException e) {
    cerr << "Unable to get foreign address" << endl;
  }
  try {
    cout << sock->getForeignPort();
  } catch (SocketException e) {
    cerr << "Unable to get foreign port" << endl;
  }
  cout << endl;

  // Send received string and receive again until the end of transmission
  char echoBuffer[RCVBUFSIZE];
  int messagesReceived = 0;
  std::vector<long long int> latencies;
  
  struct timeval tv;
  while (messagesReceived < MESSAGES){
    int recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE);
    if (recvMsgSize > 0){
      gettimeofday(&tv, NULL);
      std::string receivedMessage(echoBuffer);
      //std::cout << receivedMessage << std::endl;
      long long int sentTime = atoll(echoBuffer);
      long long int currentTime = tv.tv_sec * 1000000 + tv.tv_usec;
      long long int latency = currentTime - sentTime;
      if (latency > 100000){
        std::cout << "sentTime: " << receivedMessage <<  "; currentTime: " << currentTime << std::endl;
      }
      //std::cout << latency << std::endl;
      if (latencies.size() < 10000){
        latencies.push_back(latency);
      }
      messagesReceived++;
    }else{
      std::cout << "ERROR: RECEIVED MSG SIZE LESS THAN ZERO" << std::endl;
    }
  }
  
  std::ofstream of("latencies.txt");
  long long int sum = 0;
  for (std::vector<long long int>::iterator it = latencies.begin(); it != latencies.end(); it++){
    std::stringstream ss;
    ss << *it;
    std::string output = ss.str() + "\n";
    of << output;
    sum += *it;
  }
  of.close();
  
  std::sort(latencies.begin(), latencies.end());
  std::cout << "Median Latency is: " << latencies[latencies.size() / 2] << std::endl;
  std::cout << "Average Latency is: " << sum/latencies.size() << std::endl;
  
  delete sock;
  exit(0);
}
