#pragma once
#ifndef TIMER_H
#define TIMER_H

#include <chrono>
class Timer {
public:
	Timer() {}
	
	inline void Start() {
		t_start = std::chrono::high_resolution_clock::now();
	}
	inline float Stop() {
		t_end = std::chrono::high_resolution_clock::now();
		delta_time = (std::chrono::duration<double, std::milli>(t_end - t_start).count()) / 1000;
		return delta_time;
	}
	inline float DeltaTime() {
		return delta_time;
	}
	inline float AverageDeltaTime(int sampels) {
		TimerHistory.push_back(delta_time);

		if (TimerHistory.size() > sampels) {//remove old data/samples
			TimerHistory.erase(TimerHistory.begin(), TimerHistory.begin() + 1);
		}

		float timeSum = 0;
		for (int i = 0; i < TimerHistory.size(); i++) {
			timeSum += TimerHistory[i];
		}
		return timeSum / TimerHistory.size();// Time average in seconds
	}
	inline float DeltaTimeMls() {
		return delta_time * 1000;
	}

	~Timer() {}

private:
	std::chrono::time_point<std::chrono::steady_clock> t_end = std::chrono::high_resolution_clock::now();
	std::chrono::time_point<std::chrono::steady_clock> t_start = std::chrono::high_resolution_clock::now();
	double delta_time = 0.00001;
	std::vector<float> TimerHistory;
};
#endif //TIMER_H