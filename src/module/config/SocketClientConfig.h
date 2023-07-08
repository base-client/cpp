#pragma once

#include "Config.h"
#include <string>

using namespace std;

class SocketClientConfig : public Config {
	private:
		string serverAddress;
		int serverPort;
		int serverTimeout;

		virtual bool InitializeDerived() final;

	public:
		SocketClientConfig();
		virtual ~SocketClientConfig() = default;

		string GetServerAddress() const;
		int GetServerPort() const;
		int GetServerTimeout() const;
};
