#include "test.h"

#include "../SocketClientConfig.h"

#include "gtest/gtest.h"

static SocketClientConfig get_config()
{
	SocketClientConfig socketClientConfig;

	EXPECT_TRUE(socketClientConfig.Initialize(GstrConfigPath));

	return socketClientConfig;
}

TEST(SocketClientConfigTest, Initialize)
{
	SocketClientConfig socketClientConfig;

	EXPECT_TRUE(socketClientConfig.Initialize(GstrConfigPath));
	EXPECT_FALSE(socketClientConfig.Initialize(""));
}

TEST(SocketClientConfigTest, GetAdminPort)
{
    EXPECT_EQ(get_config().GetAdminPort(), 10002);
}

TEST(SocketClientConfigTest, GetAdminTimeout)
{
    EXPECT_EQ(get_config().GetAdminTimeout(), 3);
}

TEST(SocketClientConfigTest, GetServerAddress)
{
	EXPECT_STREQ(get_config().GetServerAddress().c_str(), "127.0.0.1");
}

TEST(SocketClientConfigTest, GetServerPort)
{
	EXPECT_EQ(get_config().GetServerPort(), 10000);
}

TEST(SocketClientConfigTest, GetServerTimeout)
{
	EXPECT_EQ(get_config().GetServerTimeout(), 3);
}
