#pragma once
#include <string>
#include <queue>
namespace TianLi::Logger
{
	typedef void(*LoggerCallback)(const char* time, const char* type,const char* func, const char* log);
	typedef struct  {
		std::string time;
		std::string type;
		std::string func;
		std::string log;
	} LoggerInfo;
	class LoggerImpl
	{
	public:
		LoggerImpl();
		~LoggerImpl();
	public:
		std::queue<LoggerInfo> log_buffer;
		LoggerCallback log_callback = nullptr;
	};
}