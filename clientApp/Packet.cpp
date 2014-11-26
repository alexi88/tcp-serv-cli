#include "Packet.h"


cPacket::cPacket(int packetType)
{
	this->packetType = packetType;
	buff = NULL;
}
cPacket::~cPacket()
{
	if (buff)
		delete[] buff;
}
byte* cPacket::GetBuff()
{
	return buff;
}

int   cPacket::GetPacketType()
{
	return packetType;
}
int	  cPacket::GetLength()
{
	return packetLength;
}
/*
byte* cPacket::Serialize(int& buffSize)
{

buffSize = (2 * sizeof(int)) + packetLength;
byte* newBuff = new byte[buffSize];
memcpy(newBuff, &packetType, sizeof(int));
memcpy(newBuff + sizeof(int), &packetLength, sizeof(int));
memcpy(newBuff + 2 * sizeof(int), buff, packetLength);
return newBuff;
}
*/
std::unique_ptr<byte[]> cPacket::Serialize(int& buffSize)
{
	buffSize = (2 * sizeof(int)) + packetLength;
	std::unique_ptr<byte[]> buff(new(byte[buffSize]));
	memcpy(buff.get(), &packetType, sizeof(int));
	memcpy(buff.get() + sizeof(int), &packetLength, sizeof(int));
	memcpy(buff.get() + 2 * sizeof(int), this->buff, packetLength);

	return buff;
}

void cPacket::Deserialize(byte* content, int length)
{
	this->packetLength = length;
	buff = new byte[length];
	ZeroMemory(buff, length);
	memcpy(buff, content, length);
}

