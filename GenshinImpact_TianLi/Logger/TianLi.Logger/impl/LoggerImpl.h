#pragma once
#include <string>
#include <queue>
namespace TianLi::Logger
{
	typedef void(*LoggerCallback)(const char* log);
	class LoggerImpl
	{
	public:
		LoggerImpl();
		~LoggerImpl();
	public:
		std::queue<std::string> log_buffer;
		LoggerCallback log_callback = nullptr;
	};
}