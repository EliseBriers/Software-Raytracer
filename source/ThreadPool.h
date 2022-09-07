#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <functional>
#include <atomic>

class ThreadPool
{
public:
	using Task = std::function<void( )>;

	explicit ThreadPool( size_t threadCount );

	~ThreadPool( )
	{
		Stop( );
	}

	void AddTask( const Task& task );
	void AddTask( Task&& task );

	void WaitForCompletion( );

	ThreadPool( const ThreadPool& other ) = delete;
	ThreadPool( ThreadPool&& other ) noexcept = delete;
	ThreadPool& operator=( const ThreadPool& other ) = delete;
	ThreadPool& operator=( ThreadPool&& other ) noexcept = delete;
private:
	std::vector<std::thread> m_Threads;
	std::condition_variable m_EventVar; // Notifies that a task has been added or that the pool has been asked to stop
	std::condition_variable m_TaskCompleted; // Notifies that a task has been completed
	std::mutex m_EventMutex;
	bool m_IsActive;
	std::queue<Task> m_Tasks;
	int m_ActiveTasks;
	void Start( size_t threadCount );

	void Stop( );
};

