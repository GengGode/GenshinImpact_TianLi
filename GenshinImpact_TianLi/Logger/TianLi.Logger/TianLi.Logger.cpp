// TianLi.Logger.cpp : 定义静态库的函数。
//

#include "TianLi.Logger.h"
#include "impl/LoggerImpl.h"

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

	void Logger::info(const char* log)
	{
		if (this->impl->log_callback)
		{
			this->impl->log_callback(log);
			if(this->impl->log_buffer.size() > 0)
			{
				while (this->impl->log_buffer.size() > 0)
				{
					this->impl->log_callback(this->impl->log_buffer.front().c_str());
					this->impl->log_buffer.pop();
				}
			}
		}
		else
		{
			this->impl->log_buffer.push(log);
		}
	}
	void Logger::info(const char* func, const char* log)
	{
		if (this->impl->log_callback)
		{
			this->impl->log_callback(log);
			if (this->impl->log_buffer.size() > 0)
			{
				while (this->impl->log_buffer.size() > 0)
				{
					this->impl->log_callback(this->impl->log_buffer.front().c_str());
					this->impl->log_buffer.pop();
				}
			}
		}
		else
		{
			this->impl->log_buffer.push(log);
		}
	}
	
	void Logger::set_callback(void(*callback_ptr)(const char*))
	{
		this->impl->log_callback = callback_ptr;
	}
}
