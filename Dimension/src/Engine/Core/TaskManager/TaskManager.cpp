#include "TaskManager.h"

TaskManager::TaskManager() {}

template<class F, class... Args>
void TaskManager::CreateAsyncTask(F&& function, Args&&... args) {
	AsyncTasks.push_back(AsyncTasks.push_back(std::async(std::launch::async, function)));

	//auto make_call = std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...);

	//std::async(std::launch::async, [=]() -> decltype(make_call());
}


TaskManager::~TaskManager() {}