#include "SocketClientConfig.h"
#include <string>

using namespace std;

SocketClientConfig::SocketClientConfig()
	: Config("socket_client.config"), serverAddress(""), serverPort(0),
	  serverTimeout(0) {}

bool SocketClientConfig::InitializeDerived() {
	this->serverAddress = this->json->GetValue<string>({"server_address"});
	this->serverPort = this->json->GetValue<int64_t>({"server_port"});
	this->serverTimeout = this->json->GetValue<int64_t>({"server_timeout"});

	return true;
}

string SocketClientConfig::GetServerAddress() const {
	return this->serverAddress;
}

int SocketClientConfig::GetServerPort() const { return this->serverPort; }

int SocketClientConfig::GetServerTimeout() const { return this->serverTimeout; }
