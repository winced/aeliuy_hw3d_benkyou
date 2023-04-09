#include "timer.h"

timer::timer()
{
	last = std::chrono::steady_clock::now();
}

float timer::get_frame_time()
{
	const auto old = last;
	last = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frame_t = last - old;
	return frame_t.count();
}

float timer::peek_time() const
{
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - last).count();
}
