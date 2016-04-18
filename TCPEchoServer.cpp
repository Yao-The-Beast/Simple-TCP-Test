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
#include <pthread.h>

using namespace std;

const unsigned int MESSAGES = 100000;
const int NUM_THREADS = 5;
pthread_t threads[NUM_THREADS];
int connections = 0;
int id = 0;
bool flag = true;
const unsigned int RCVBUFSIZE = 128;    // Size of receive buffer

void HandleTCPClient(TCPSocket *sock); // TCP client handling function

unsigned short echoServPort = 1234;  // First arg: local port
TCPServerSocket servSock(echoServPort);     // Server Socket object

int main(int argc, char *argv[]) {

  try {
    for (;;) {   // Run forever
      //if reach the exact amount
      if (connections == NUM_THREADS){
        for (int i = 0; i < NUM_THREADS; i++){
          pthread_join(threads[i],NULL);
        }
        servSock.closeSocket();
        break;
      }else{
        HandleTCPClient(servSock.accept());       // Wait for a client to connect
      }
    }
  } catch (SocketException &e) {
    cerr << e.what() << endl;
    exit(1);
  }
  // NOT REACHED

  return 0;
}

void* worker(void* socket){
  
  // Send received string and receive again until the end of transmission
  int messagesReceived = 0;
  std::vector<long long int> latencies;
  
  TCPSocket* sock = (TCPSocket *)socket;
  
  struct timeval tv;
  while (messagesReceived < MESSAGES){
    char echoBuffer[RCVBUFSIZE];
    int recvMsgSize = sock->recv(echoBuffer, RCVBUFSIZE);
    if (recvMsgSize == RCVBUFSIZE){
      gettimeofday(&tv, NULL);
      std::string receivedMessage(echoBuffer);
      //std::cout << receivedMessage << std::endl;
      long long int sentTime = atoll(echoBuffer);
      long long int currentTime = tv.tv_sec * 1000000 + tv.tv_usec;
      long long int latency = currentTime - sentTime;
      if (latency > 1000000){
        std::cout << "latency: " << latency << std::endl;
      }
      //std::cout << latency << std::endl;
      if (latencies.size() < 10000){
        latencies.push_back(latency);
      }
      messagesReceived++;
    }else if (recvMsgSize > 0){
      std::cout << "SHIT " << recvMsgSize << std::endl;
    }
  }
  
  if (flag == true){
    flag = false;
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
  }
  std::cout << "!" << std::endl;
  delete sock;
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
  std::cout << std::endl;
  int thisId = id;
  id++;
  connections++;
  pthread_create(&threads[thisId],NULL,worker,(void*)sock);
}
