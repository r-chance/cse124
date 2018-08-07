#include <assert.h>
#include "CalcFramer.hpp"

using namespace std;

void CalcFramer::append(string chars)
{
	buffer.append(chars);
}

bool CalcFramer::hasMessage() const
{
	int lenBuffer = buffer.size();
	for(int i = 0; i < lenBuffer; i++) {
		if(buffer[i] == '\n')
			return true;
        }
	return false;
}

string CalcFramer::topMessage() const
{
	std::string temp;
	int lenBuffer = buffer.size();
	for(int i = 0; i < lenBuffer; i++) {
		//check for CRLF terminator
		if (buffer[i] == '\r') {
			if (buffer[(i+1)] == '\n')
				return temp;
		}
		temp.append(1, buffer[i]);
	}
	return temp;
}

void CalcFramer::popMessage()
{
	int lenBuffer = buffer.size();
	for (int i = 0; i < lenBuffer; i++) {
		if (buffer[i] == '\n') {
			buffer.erase(0, i+1);
			return;
		}
	}
}

void CalcFramer::printToStream(ostream& stream) const
{
	// PUT YOUR CODE HERE
}
