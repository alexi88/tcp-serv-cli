#pragma once 
#include<Windows.h>
#include<memory>

enum PacketTypes
{
	INIT_CONNECTION_CMD = 0,
	SEND_INFO_CMD
};


class cPacket
{

	int packetType;
	int packetLength;
	byte* buff;

public:

	cPacket(int packetType);
	~cPacket();

	//byte* Serialize(int& buffSize);
	void  Deserialize(byte* content, int length);
	std::unique_ptr<byte[]> cPacket::Serialize(int& buffSize);

	byte* GetBuff();
	int   GetPacketType();
	int	  GetLength();
};