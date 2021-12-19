#include <thread>
using namespace std;

#include "test.h"

#include "../SocketClientChildProcess.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "ThreadPool.h"
#include "SocketServer.h"
#include "SocketClient.h"
#include "EnvironmentVariable.h"

static const string strGreeting = "greeting\r\n";
static const string strWrite = "test message\r\n";
static const string strPrefixResponse = "[response] ";

class SocketClientChildProcessTest : public ::testing::Test {
protected:
	void SetUp() override {
		extern int optind;
		optind = 1;

		int iArgc = 3;
		char *pcArgv[] = {(char *)"./SocketClientChildProcessTest", (char *)"-c", (char *)GstrConfigPath.c_str()};

		EXPECT_TRUE(Singleton<EnvironmentVariable>::Instance().Initialize(iArgc, pcArgv));
	}

	void TearDown() override {}
};

TEST_F(SocketClientChildProcessTest, Start)
{
	auto serverJobFunc = [](const SocketClient &socketClient) {
		EXPECT_TRUE(socketClient.Write(strGreeting));

		bool bEnd = false;
		string strRead = "";

		EXPECT_TRUE(socketClient.Read(strRead, 1024, bEnd));
		EXPECT_STREQ(strRead.c_str(), strWrite.c_str());

		EXPECT_TRUE(socketClient.Write(strPrefixResponse + strRead));
	};

	SocketServer socketServer;

	bool bServerResult = false;
	auto serverThreadFunc = [&]() {
		bServerResult = socketServer.Start(10000, 3, 1, serverJobFunc);
	};

	thread serverThread(serverThreadFunc);

	this_thread::sleep_for(std::chrono::seconds(1));

	ThreadPool threadPool(1);
	SocketClientChildProcess socketClientChildProcess;
	future<bool> future = threadPool.AddJob([&]()->bool{return socketClientChildProcess.Start();});
	this_thread::sleep_for(chrono::seconds(1));

	EXPECT_TRUE(socketClientChildProcess.Stop());
	EXPECT_TRUE(future.get());

	EXPECT_TRUE(socketServer.Stop());
	serverThread.join();
	EXPECT_TRUE(bServerResult);
}

TEST_F(SocketClientChildProcessTest, Stop)
{
	SocketClientChildProcess socketClientChildProcess;
	EXPECT_TRUE(socketClientChildProcess.Stop());
}

TEST_F(SocketClientChildProcessTest, SigTerm)
{
	auto serverJobFunc = [](const SocketClient &socketClient) {
		EXPECT_TRUE(socketClient.Write(strGreeting));

		bool bEnd = false;
		string strRead = "";

		EXPECT_TRUE(socketClient.Read(strRead, 1024, bEnd));
		EXPECT_STREQ(strRead.c_str(), strWrite.c_str());

		EXPECT_TRUE(socketClient.Write(strPrefixResponse + strRead));
	};

	SocketServer socketServer;

	bool bServerResult = false;
	auto serverThreadFunc = [&]() {
		bServerResult = socketServer.Start(10000, 3, 1, serverJobFunc);
	};

	thread serverThread(serverThreadFunc);
	this_thread::sleep_for(std::chrono::seconds(1));

	const int iPid = fork();
	ASSERT_NE(iPid, -1);

	if(iPid == 0) {
		ThreadPool threadPool(1);
		future<bool> future = threadPool.AddJob([&]()->bool{return SocketClientChildProcess().Start();});
		this_thread::sleep_for(chrono::seconds(1));

		EXPECT_EQ(raise(SIGTERM), 0);

		EXPECT_TRUE(future.get());

		exit(testing::Test::HasFailure());
	}

	int iStatus = -1;
	EXPECT_EQ(waitpid(iPid, &iStatus, 0), iPid);
	EXPECT_NE(WIFEXITED(iStatus), 0);
	EXPECT_EQ(WIFSIGNALED(iStatus), 0);
	EXPECT_EQ(WTERMSIG(iStatus), 0);

	EXPECT_TRUE(socketServer.Stop());
	serverThread.join();
	EXPECT_TRUE(bServerResult);
}
