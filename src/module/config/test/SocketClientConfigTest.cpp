#include "../SocketClientConfig.h"
#include "test.h"
#include "gtest/gtest.h"
#include <memory>

using namespace std;

static unique_ptr<SocketClientConfig> get_config() {
	auto config = make_unique<SocketClientConfig>();

	EXPECT_TRUE(config->Initialize(CONFIG_PATH));

	return config;
}

TEST(SocketClientConfigTest, Initialize) {
	SocketClientConfig socketClientConfig;

	EXPECT_TRUE(socketClientConfig.Initialize(CONFIG_PATH));
	EXPECT_FALSE(socketClientConfig.Initialize(""));
}

TEST(SocketClientConfigTest, GetServerAddress) {
	EXPECT_STREQ(get_config()->GetServerAddress().c_str(), "127.0.0.1");
}

TEST(SocketClientConfigTest, GetServerPort) {
	EXPECT_EQ(get_config()->GetServerPort(), 10001);
}

TEST(SocketClientConfigTest, GetServerTimeout) {
	EXPECT_EQ(get_config()->GetServerTimeout(), 2);
}
