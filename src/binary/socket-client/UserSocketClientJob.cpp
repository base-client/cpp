#include <cstring>
using namespace std;

#include "FileLog.h"
#include "SocketClient.h"
#include "SocketClientConfig.h"
#include "EnvironmentVariable.h"

#include "UserSocketClientJob.h"

bool UserSocketClientJob::Start()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	this->bCondition.store(true);

	while(this->bCondition) {
		SocketClientConfig socketClientConfig;
		if(socketClientConfig.Initialize(Singleton<EnvironmentVariable>::Instance().GetConfigPath()) == false) {
			ERROR_L_G("SocketClientConfig Initialize fail");
			continue;
		}

		SocketClient socketClient(socketClientConfig.GetServerAddress(), socketClientConfig.GetServerPort(), socketClientConfig.GetServerTimeout());

		bool bEnd = false;
		string strRead = "";

		if(socketClient.Read(strRead, 1024, bEnd) == false) {
			ERROR_L_G("socket read fail");
			continue;
		}
		DEBUG_G("socket read message : (%s)", strRead.c_str());

		if(socketClient.Write("test message\r\n") == false) {
			ERROR_L_G("socket write fail");
			continue;
		}

		if(socketClient.Read(strRead, 1024, bEnd) == false) {
			ERROR_L_G("socket read fail");
			continue;
		}
		DEBUG_G("socket read message : (%s)", strRead.c_str());
	}

	return true;
}

bool UserSocketClientJob::Stop()
{
	DEBUG_G(__PRETTY_FUNCTION__);

	this->bCondition.store(false);

	return true;
}
