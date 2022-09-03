#pragma once
#include <thread>
#include <functional>


class core_thread
{
	//std::function<bool> init_function;
	std::thread* init_thread = nullptr;
	bool init_is_finised = false;
public:
	void init();
	//void match();
	
	
};

