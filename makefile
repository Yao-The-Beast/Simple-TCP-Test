CXX = g++ -std=c++11
CXXFLAGS = -Wall -ansi -pedantic -g

ifeq ($(shell uname),SunOS)
  LIBS = -lsocket -lnsl
endif

all: TCPEchoClient TCPEchoServer UDPEchoClient UDPEchoServer TCPEchoServer-Thread \
BroadcastSender BroadcastReceiver MulticastSender MulticastReceiver

TCPEchoClient: TCPEchoClient.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o TCPEchoClient TCPEchoClient.cpp PracticalSocket.cpp $(LIBS)

TCPEchoServer: TCPEchoServer.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o TCPEchoServer TCPEchoServer.cpp PracticalSocket.cpp $(LIBS)

UDPEchoClient: UDPEchoClient.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o UDPEchoClient UDPEchoClient.cpp PracticalSocket.cpp $(LIBS)

UDPEchoServer: UDPEchoServer.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o UDPEchoServer UDPEchoServer.cpp PracticalSocket.cpp $(LIBS)

TCPEchoServer-Thread: TCPEchoServer-Thread.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) -D_GNU_SOURCE -o TCPEchoServer-Thread TCPEchoServer-Thread.cpp PracticalSocket.cpp $(LIBS) -lpthread

BroadcastSender: BroadcastSender.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o BroadcastSender BroadcastSender.cpp PracticalSocket.cpp $(LIBS)

BroadcastReceiver: BroadcastReceiver.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o BroadcastReceiver BroadcastReceiver.cpp PracticalSocket.cpp $(LIBS)

MulticastSender: MulticastSender.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o MulticastSender MulticastSender.cpp PracticalSocket.cpp $(LIBS)

MulticastReceiver: MulticastReceiver.cpp PracticalSocket.cpp PracticalSocket.h
	$(CXX) $(CXXFLAGS) -o MulticastReceiver MulticastReceiver.cpp PracticalSocket.cpp $(LIBS)

clean:
	$(RM) TCPEchoClient TCPEchoServer UDPEchoClient UDPEchoServer TCPEchoServer-Thread \
        BroadcastSender BroadcastReceiver MulticastSender MulticastReceiver