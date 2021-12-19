#include "FileLog.h"
#include "JsonManager.h"

#include "SocketClientConfig.h"

SocketClientConfig::SocketClientConfig()
	: Config("socket_client.config"),
		iAdminPort(0),
		iAdminTimeout(0),
		strServerAddress(""),
		iServerPort(0),
		iServerTimeout(0)
{
	DEBUG_G(__PRETTY_FUNCTION__);
}

bool SocketClientConfig::InitializeDerived()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	this->iAdminPort = this->jsonManager.GetValue<int>({"admin_port"});
	this->iAdminTimeout = this->jsonManager.GetValue<int>({"admin_timeout"});

	this->strServerAddress = this->jsonManager.GetValue<string>({"server_address"});
	this->iServerPort = this->jsonManager.GetValue<int>({"server_port"});
	this->iServerTimeout = this->jsonManager.GetValue<int>({"server_timeout"});

	return true;
}

int SocketClientConfig::GetAdminPort()
{
	return this->iAdminPort;
}

int SocketClientConfig::GetAdminTimeout()
{
	return this->iAdminTimeout;
}

string SocketClientConfig::GetServerAddress()
{
	return this->strServerAddress;
}

int SocketClientConfig::GetServerPort()
{
	return this->iServerPort;
}

int SocketClientConfig::GetServerTimeout()
{
	return this->iServerTimeout;
}
