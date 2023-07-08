#include "Main.h"
#include "CommonConfig.h"
#include "FileLog.h"
#include "Singleton.h"
#include "SocketClient.h"
#include "SocketClientConfig.h"
#include "define.h"
#include <cstdio>
#include <cstring>
#include <format>
#include <string>

using namespace std;

bool Main::Initialize(int argc, char *argv[]) {
	if (this->environmentVariable.Initialize(argc, argv) == false) {
		printf("%s\n",
			   Singleton<EnvironmentVariable>::Instance().Usage().c_str());
		return false;
	}

	CommonConfig commonConfig;
	if (commonConfig.Initialize(this->environmentVariable.GetConfigPath()) ==
		false) {
		printf(format("CommonConfig Initialize fail - errno : ({})({})\n",
					  errno, strerror(errno))
				   .c_str());
		return false;
	}
	if (Singleton<FileLog>::Instance().Initialize(
			commonConfig.GetLogLevel(), commonConfig.GetLogOutputPath(),
			commonConfig.GetLogFileName(), commonConfig.GetLogLinePrint(),
			commonConfig.GetLogThreadMode()) == false) {
		printf(format("FileLog Initialize fail - errno : ({})({})\n", errno,
					  strerror(errno))
				   .c_str());
		return false;
	}

	return true;
}

bool Main::Job() {
	SocketClientConfig socketClientConfig;
	if (socketClientConfig.Initialize(
			this->environmentVariable.GetConfigPath()) == false) {
		ERROR("SocketClientConfig Initialize fail - errno : ({})({})\n", errno,
			  strerror(errno));
		return false;
	}

	SocketClient socketClient(socketClientConfig.GetServerAddress(),
							  socketClientConfig.GetServerPort(),
							  socketClientConfig.GetServerTimeout());

	auto greeting = [&socketClient]() {
		bool end = false;
		const auto result = socketClient.Read(1024, end);
		if (get<0>(result) == false) {
			ERROR("socket read fail - error : ({})({})", errno,
				  strerror(errno));
			return false;
		}
		DEBUG("socket read greeting message : ({})", get<1>(result));

		return true;
	};

	auto send = [&socketClient](const string &messages) {
		if (socketClient.Write(messages) == false) {
			ERROR("socket write fail - messages : ({}), error : ({})({})",
				  messages, errno, strerror(errno));
			return false;
		}
		DEBUG("socket write message : ({})", messages);

		bool end = false;
		const auto result = socketClient.Read(1024, end);
		if (get<0>(result) == false) {
			ERROR("socket read fail - error : ({})({})", errno,
				  strerror(errno));
			return false;
		}
		DEBUG("socket read message : ({})", get<1>(result));

		return true;
	};

	if (greeting() && send("test message\r\n") && send("quit\r\n")) {
		return true;
	}

	return false;
}

bool Main::Run(int argc, char *argv[]) {
	if (this->Initialize(argc, argv) == false) {
		return false;
	}

	if (this->Job() == false) {
		return false;
	}

	return true;
}
