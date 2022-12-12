// TianLi.Logger.cpp : 定义静态库的函数。
//

#include "TianLi.Logger.h"
#include "impl/LoggerImpl.h"
#include <chrono>
namespace TianLi::Logger
{
	Logger::Logger()
	{
		this->impl = new LoggerImpl();
	}
	
	Logger& Logger::get_instance()
	{
		static Logger logger;
		return logger;
	}

	Logger::~Logger()
	{
		delete this->impl;
	}

	void Logger::log(const char* time, const char* type, const char* func, const char* msg)
	{
		if (this->impl->log_callback)
		{
			this->impl->log_callback(time, type, func, msg);
			if (this->impl->log_buffer.size() > 0)
			{
				while (this->impl->log_buffer.size() > 0)
				{
					LoggerInfo info = this->impl->log_buffer.front();
					this->impl->log_callback(info.time.c_str(), info.type.c_str(), info.func.c_str(),info.log.c_str());
					this->impl->log_buffer.pop();
				}
			}
		}
		else
		{
			this->impl->log_buffer.push({ time,type,func,msg });
		}
	}

	inline std::tm localtime_xp(std::time_t timer)
	{
		std::tm bt{};
#if defined(__unix__)
		localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
		localtime_s(&bt, &timer);
#else
		static std::mutex mtx;
		std::lock_guard<std::mutex> lock(mtx);
		bt = *std::localtime(&timer);
#endif
		return bt;
	}

	// default = "YYYY-MM-DD HH:MM:SS"
	inline std::string time_stamp(const std::string& fmt = "%F %T")
	{
		auto bt = localtime_xp(std::time(0));
		char buf[64];
		return { buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt) };
	}
    void Logger::log_now( const char* type,const char* func, const char* log)
	{
		std::string time = time_stamp();
		time = time.substr(11, 8);
		Logger::get_instance().log(time.c_str(), type, func, log);
	}
	void Logger::info(const char* func, const char* log)
	{
		this->log_now("Info", func, log);
	}

    void Logger::debug(const char *func, const char *log)
    {
		this->log_now("Debug", func, log);
    }

	void Logger::warning(const char *func, const char *log)
	{
		this->log_now("Warning", func, log);
	}

    void Logger::error(const char *func, const char *log)
    {
		this->log_now("Error", func, log);
    }

    void Logger::fatal(const char *func, const char *log)
    {
		this->log_now("Fatal", func, log);
    }

    void Logger::set_callback(void(*callback_ptr)(const char*, const char*, const char*, const char*))
	{
		this->impl->log_callback = callback_ptr;
	}
}
