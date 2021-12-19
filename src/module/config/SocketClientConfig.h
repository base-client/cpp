#pragma once

#include "Config.h"

class SocketClientConfig : public Config {
private:
	int iAdminPort;
	int iAdminTimeout;

	string strServerAddress;
	int iServerPort;
	int iServerTimeout;

	virtual bool InitializeDerived();
public:
	SocketClientConfig();
	virtual ~SocketClientConfig() = default;

	int GetAdminPort();
	int GetAdminTimeout();

	string GetServerAddress();
	int GetServerPort();
	int GetServerTimeout();
};
