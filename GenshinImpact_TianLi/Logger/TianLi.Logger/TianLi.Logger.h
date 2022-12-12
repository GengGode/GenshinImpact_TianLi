#pragma once
#ifdef TIANLI_LOGGER_EXPORTS
#define TIANLI_LOGGER_API __declspec(dllexport)
#else
#define TIANLI_LOGGER_API __declspec(dllimport)
#endif // TIANLI_LOGGER_EXPORTS


namespace TianLi::Logger
{
	class LoggerImpl;
	enum class LogLevel
	{
		Info,
		Debug,
		Warning,
		Error,
		Fatal
	};
	class TIANLI_LOGGER_API Logger
	{
		LoggerImpl* impl;
		Logger();
	public:
		static Logger& get_instance();
		~Logger();
	public:
		// 时间戳，日志等级，函数名，日志内容
		void log(int time, LogLevel level, const char* func, const char* log);
		void info(const char* log);
		void info(const char* func, const char* log);
	public:
		void set_callback(void(*callback_ptr)(const char*));
	};
	//TIANLI_LOGGER_API Logger& Log() { return Logger::get_instance(); }
}
