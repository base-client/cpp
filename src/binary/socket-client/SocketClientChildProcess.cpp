#include "FileLog.h"

#include "SocketClientChildProcess.h"

bool SocketClientChildProcess::InitializeDerived()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	return true;
}

bool SocketClientChildProcess::FinalizeDerived()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	if(this->adminSocketClientJob.Stop() == false) {
		ERROR_L_G("adminSocketClientJob stop fail");
	}

	if(this->userSocketClientJob.Stop() == false) {
		ERROR_L_G("userSocketClientJob stop fail");
	}

	return true;
}

bool SocketClientChildProcess::Job()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	ThreadPool threadPool(2);

	auto adminSocketServerFunc = [&]() -> bool {
		return this->adminSocketClientJob.Start();
	};

	auto userSocketServerFunc = [&]() -> bool {
		return this->userSocketClientJob.Start();
	};

	vector<future<bool>> vecFuture;
	vecFuture.clear();

	vecFuture.push_back(threadPool.AddJob(adminSocketServerFunc));
	vecFuture.push_back(threadPool.AddJob(userSocketServerFunc));

	for(auto &iter : vecFuture) {
		if(iter.valid()) {
			iter.get();
		}
	}

	return true;
}
