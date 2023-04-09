#pragma once
#include <chrono>



class timer
{
public:
	timer();
	float get_frame_time();
	float peek_time() const;
private:
	std::chrono::steady_clock::time_point last;
};