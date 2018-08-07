#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h>     /* for close() */
#include <iostream>
#include <assert.h>
#include "CalcFramer.hpp"
#include "CalcParser.hpp"
#include <inttypes.h>
#include <stdint.h>

using namespace std;

#define ADD 0
#define SUB 1
#define SET 2
#define END 3

void HandleTCPClient(int clntSocket) {
	CalcFramer framer;
	char buffer[512];
	if (connect(clntSocket, (struct sockaddr *)&buffer, sizeof(buffer)) < 0) {
		//printf("CONNECTION FAILED\n");
	}
	else {
		//printf("Connected to client socket!\n");
	}

	uint64_t totalBytes = 0;
	uint64_t value = 0;

	while (1) {
		uint64_t numBytes = recv(clntSocket, &buffer, 512, 0);
		//no more bytes to receive - close connection
		if (numBytes < 1) {
			break;
		}
		totalBytes += numBytes;

		// append bytes received to framer
		char* framerBuf = new char[numBytes];
		for (uint64_t i = 0; i < numBytes; i++) {
			framerBuf[i] = buffer[i];
		}
		framer.append(string(framerBuf));

		//process any complete instructions in buffer
		while (framer.hasMessage()) {
			string instruction = framer.topMessage();
			//remove message from buffer
			framer.popMessage();
			//printf("MESSAGE: %s\n", instruction.c_str());
			CalcInstruction instr = CalcParser::parse(instruction);
			//printf("OPCODE: %d\n", (int)instr.opcode);
			//printf("OPERAND: %d\n", (int)instr.operand);
			//printf("VALUE: %d\n", (int)value);
			if (instr.opcode == END) {
				string valueStr = to_string(value);
				//add CRLF terminators to string
				valueStr.push_back('\r');
				valueStr.push_back('\n');
				//printf("SEND VALUE: %s\n", valueStr.c_str());
				send(clntSocket, valueStr.c_str(), valueStr.length(), 0);
				//reset value
				value = 0;
			}
			else {
				//execute operation depending on opcode
				switch(instr.opcode) {
					case ADD : value += instr.operand; break;
					case SUB : value -= instr.operand; break;
					case SET : value = instr.operand; break;
					default: break;
				}
			}
		}
	}
	close(clntSocket);	/* Close client socket */
}

