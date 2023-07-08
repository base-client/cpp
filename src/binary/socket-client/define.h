#pragma once

#include "FileLog.h"
#include "Singleton.h"
#include <format>

using namespace std;

#define DEBUG(args...) Singleton<FileLog>::Instance().Debug(format(args))
#define INFO(args...) Singleton<FileLog>::Instance().Info(format(args))
#define WARNING(args...) Singleton<FileLog>::Instance().Warning(format(args))
#define ERROR(args...) Singleton<FileLog>::Instance().Error(format(args))
#define CRITICAL(args...) Singleton<FileLog>::Instance().Critical(format(args))
