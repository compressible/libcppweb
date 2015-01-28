#ifndef __CPPWEB_H__
#define __CPPWEB_H__

#include "PacketImpl.h"
#include "PacketParserImpl.h"
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <AsyncTransport.h>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::thread;

class WebListener {
	public:
		virtual void onData(int fd, unsigned char *data, unsigned int length) = 0;
		virtual void onConnect(int fd) = 0;
		virtual void onClose(int fd) = 0;
};

class CppWeb {
	public:
		CppWeb( WebListener & );
		~CppWeb();
		
		void
		start( int port );

		void
		stop();

		void
		send( int fd, unsigned char *data, unsigned int size );

		void
		close( int fd );

	private:
		static
		string base64_encode( unsigned char* data, int size );
			
		static unsigned int
		Hash(const char *mode, const char* dataToHash, size_t dataSize, unsigned char* outHashed); 
		
		PacketParserImpl packetParser;
		AsyncTransport   asyncTransport;
		map<unsigned int, bool> upgraded;
		WebListener &webListener;

		static const string SecMagic;
		
		static void
		RecvThread( CppWeb &instance );

		volatile bool isRunning;

		thread recvThread;
};

#endif
