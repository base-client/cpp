#include "test.h"

#include "../UserSocketClientJob.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "EnvironmentVariable.h"

static const string strGreeting = "greeting\r\n";
static const string strWrite = "test message\r\n";
static const string strPrefixResponse = "[response] ";

class UserSocketClientJobTest : public ::testing::Test {
protected:
	void SetUp() override {
		extern int optind;
		optind = 1;

		int iArgc = 4;
		char *pcArgv[] = {(char *)"./UserSocketClientJobTest", (char *)"-c", (char *)GstrConfigPath.c_str(), (char *)"-s"};

		EXPECT_TRUE(Singleton<EnvironmentVariable>::Instance().Initialize(iArgc, pcArgv));
	}

	void TearDown() override {}
};

TEST_F(UserSocketClientJobTest, Start)
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

	UserSocketClientJob userSocketServerJob;

	future<bool> future = threadPool.AddJob([&]()->bool{return userSocketServerJob.Start();});
	this_thread::sleep_for(chrono::seconds(1));

	EXPECT_TRUE(userSocketServerJob.Stop());

	EXPECT_TRUE(future.get());

	EXPECT_TRUE(socketServer.Stop());
	serverThread.join();
	EXPECT_TRUE(bServerResult);
}

TEST_F(UserSocketClientJobTest, Stop)
{
	UserSocketClientJob userSocketServerJob;
	EXPECT_TRUE(userSocketServerJob.Stop());
}
