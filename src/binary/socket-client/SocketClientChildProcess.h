#pragma once

#include "UserSocketClientJob.h"
#include "AdminSocketClientJob.h"

#include "ChildProcess.h"

class SocketClientChildProcess : public ChildProcess {
private:
	UserSocketClientJob userSocketClientJob;
	AdminSocketClientJob adminSocketClientJob;

	virtual bool InitializeDerived();
	virtual bool FinalizeDerived();
	virtual bool Job();
public:
	SocketClientChildProcess() = default;
	virtual ~SocketClientChildProcess() = default;
};
