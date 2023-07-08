#include "../Main.h"
#include "SocketClient.h"
#include "SocketClientConfig.h"
#include "SocketServer.h"
#include "test.h"
#include "gtest/gtest.h"
#include <chrono>
#include <thread>

using namespace std;

TEST(MainTest, Run_1) {
	int argc = 2;
	char *argv[] = {(char *)"./MainTest", (char *)"-c",
					(char *)CONFIG_PATH.c_str()};
	EXPECT_FALSE(Main().Run(argc, argv));
}
TEST(MainTest, Run_2) {
	SocketClientConfig socketClientConfig;
	ASSERT_TRUE(socketClientConfig.Initialize(CONFIG_PATH));

	SocketServer socketServer;
	ASSERT_TRUE(socketServer.Start(
		socketClientConfig.GetServerPort(),
		socketClientConfig.GetServerTimeout() + 1, 1,
		[](const SocketClient &socketClient) {
			EXPECT_TRUE(socketClient.Write("=== greeting ===\r\n"));

			bool end = false;
			auto result = socketClient.Read(1024, end);
			EXPECT_TRUE(get<0>(result));
			EXPECT_STREQ(get<1>(result).c_str(), "test message\r\n");

			EXPECT_TRUE(socketClient.Write("[response] " + get<1>(result)));

			end = false;
			result = socketClient.Read(1024, end);
			EXPECT_TRUE(get<0>(result));
			EXPECT_STREQ(get<1>(result).c_str(), "quit\r\n");

			EXPECT_TRUE(socketClient.Write("200 ok\r\n"));
		}));
	this_thread::sleep_for(std::chrono::seconds(1));

	int argc = 3;
	char *argv[] = {(char *)"./MainTest", (char *)"-c",
					(char *)CONFIG_PATH.c_str()};
	EXPECT_TRUE(Main().Run(argc, argv));

	EXPECT_TRUE(socketServer.Stop());
}
