#pragma once

#include <string>

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/TcpListener.hpp>
#include "NetworkPacket.h"

class Network
{
public:

	int m_port = 52000;
	sf::IpAddress m_ip = "25.89.219.2";

	std::string m_name = "";

	sf::TcpListener m_listener;
	sf::TcpSocket m_socket;
	sf::Packet m_packet;

public:
	void SetIp(std::string&& ip);
	void InitialiseServer();
	void AcceptConnection();
	bool InitialiseClient();

	void SendData(NetworkPacket& networkPacket);
	void ReceiveData(NetworkPacket& networkPacket);
};