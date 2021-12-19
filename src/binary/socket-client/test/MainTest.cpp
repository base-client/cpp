#include <thread>
using namespace std;

#include "test.h"

#include "../Main.h"

#include "gtest/gtest.h"

#include "Singleton.h"
#include "ThreadPool.h"
#include "SocketServer.h"
#include "SocketClient.h"
#include "EnvironmentVariable.h"

static const string strGreeting = "greeting\r\n";
static const string strWrite = "test message\r\n";
static const string strPrefixResponse = "[response] ";

static void test(int iArgc, char *pcArgv[])
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

	extern int optind;
	optind = 1;

	if(iPid == 0) {
		EXPECT_TRUE(Main().Run(iArgc, pcArgv));

		exit(testing::Test::HasFailure());
	}
	this_thread::sleep_for(chrono::seconds(2));

	EXPECT_FALSE(Main().Run(iArgc, pcArgv));

	EXPECT_STREQ(send_command("stop", true).c_str(), "200 ok\r\n");

	this_thread::sleep_for(chrono::seconds(2));

	EXPECT_TRUE(socketServer.Stop());
	serverThread.join();
	EXPECT_TRUE(bServerResult);
}

TEST(MainTest, Run)
{
	EXPECT_FALSE(Main().Run(0, nullptr));
}

TEST(MainTest, StandAlone)
{
	int iArgc = 4;
	char *pcArgv[] = {(char *)"./MainTest", (char *)"-c", (char *)GstrConfigPath.c_str(), (char *)"-s"};

	test(iArgc, pcArgv);
}

TEST(MainTest, NonStandAlone)
{
	int iArgc = 3;
	char *pcArgv[] = {(char *)"./MainTest", (char *)"-c", (char *)GstrConfigPath.c_str()};

	test(iArgc, pcArgv);
}
