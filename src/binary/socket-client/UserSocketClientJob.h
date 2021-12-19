#pragma once

#include "SocketServer.h"

class UserSocketClientJob {
private:
	atomic_bool bCondition;
public:
	UserSocketClientJob() = default;
	virtual ~UserSocketClientJob() = default;

	bool Start();
	bool Stop();
};
