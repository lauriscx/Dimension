#pragma once
#include <chrono>

class Timer {
public:
	Timer();
	
	void Start();
	float Stop();
	float DeltaTime();

	~Timer();

private:
	std::chrono::time_point<std::chrono::steady_clock> t_end = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> t_start = std::chrono::high_resolution_clock::now();
	double delta_time = 0;
};

Timer::Timer() {}

void Timer::Start() {
	t_start = std::chrono::high_resolution_clock::now();
}
float Timer::Stop() {
	t_end = std::chrono::high_resolution_clock::now();
	delta_time = (std::chrono::duration<double, std::milli>(t_end - t_start).count()) / 1000;
	return delta_time;
}
float Timer::DeltaTime() {
	return delta_time;
}
Timer::~Timer() {}