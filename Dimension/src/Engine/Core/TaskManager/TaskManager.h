#pragma once
#include <vector>
#include <future>

class TaskManager {
public:
	TaskManager();

	template<class F, class... Args>
	void CreateAsyncTask(F&& function, Args&&... args);

	~TaskManager();
private:
	std::vector<std::future<void>> AsyncTasks;
};