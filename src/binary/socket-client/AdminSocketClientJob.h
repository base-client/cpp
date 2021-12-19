#pragma once

#include "SocketServer.h"

class AdminSocketClientJob {
private:
	SocketServer socketServer;
public:
	AdminSocketClientJob() = default;
	virtual ~AdminSocketClientJob() = default;

	bool Start();
	bool Stop();
};
