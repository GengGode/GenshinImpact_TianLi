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
		void log(const char* time, const char* type, const char* func, const char* msg);
        void log_now(const char *type, const char *func, const char *log);

        void info(const char *func, const char *log);
        void debug(const char* func, const char* log);
		void warning(const char* func, const char* log);
		void error(const char* func, const char* log);
		void fatal(const char* func, const char* log);
	public:
		void set_callback(void(*callback_ptr)(const char*, const char*, const char*, const char*));
	};
}
#define LogInfo(msg) TianLi::Logger::Logger::get_instance().info(__FUNCTION__,msg)
#define LogDebug(msg) TianLi::Logger::Logger::get_instance().debug(__FUNCTION__,msg)
#define LogWarn(msg) TianLi::Logger::Logger::get_instance().warning(__FUNCTION__,msg)
#define LogError(msg) TianLi::Logger::Logger::get_instance().error(__FUNCTION__,msg)
#define LogFatal(msg) TianLi::Logger::Logger::get_instance().fatal(__FUNCTION__,msg)
