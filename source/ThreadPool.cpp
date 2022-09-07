#include "ThreadPool.h"

ThreadPool::ThreadPool( size_t threadCount )
	: m_IsActive{ true }
	, m_ActiveTasks{ }
{
	Start( std::max( threadCount, size_t{ 1 } ) );
}

void ThreadPool::AddTask( const Task& task )
{
	{
		std::unique_lock<std::mutex> lock{ m_EventMutex };
		m_Tasks.emplace( task );
	}

	m_EventVar.notify_one( );
}

void ThreadPool::AddTask( Task&& task )
{
	{
		std::unique_lock<std::mutex> lock { m_EventMutex };
		m_Tasks.emplace( std::move( task ) );
	}

	m_EventVar.notify_one( );
}

void ThreadPool::WaitForCompletion( )
{
	std::unique_lock<std::mutex> lock{ m_EventMutex };
	m_TaskCompleted.wait( lock, [=]( )
	{
		return m_Tasks.empty( ) && m_ActiveTasks == 0;
	} );
}

void ThreadPool::Start( size_t threadCount )
{
	for( size_t i{}; i < threadCount; ++i )
	{
		m_Threads.emplace_back( [=]
		{
			while( true )
			{
				Task task;
				{
					std::unique_lock<std::mutex> lock{ m_EventMutex };
					m_EventVar.wait( lock, [=]
					{
						return !m_IsActive || !m_Tasks.empty( );
					} );
					++m_ActiveTasks;
					if( !m_IsActive && m_Tasks.empty( ) )
						break;
					task = std::move( m_Tasks.front( ) );
					m_Tasks.pop( );
				}
				task( );
				{
					std::unique_lock<std::mutex> lock { m_EventMutex };
					--m_ActiveTasks;
				}
				m_TaskCompleted.notify_one( );
			}
		} );
	}
}

void ThreadPool::Stop( )
{
	{
		std::unique_lock<std::mutex> lock{ m_EventMutex };
		m_IsActive = false;
	}

	m_EventVar.notify_all( );
	for( std::thread& thread : m_Threads )
	{
		thread.join( );
	}
}
