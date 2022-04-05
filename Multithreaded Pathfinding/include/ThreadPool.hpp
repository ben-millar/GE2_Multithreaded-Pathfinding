#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>      // std::atomic
#include <cstdint>     // std::int_fast64_t, std::uint_fast32_t
#include <functional>  // std::function
#include <future>      // std::future, std::promise
#include <iostream>    // std::cout, std::ostream
#include <memory>      // std::shared_ptr, std::unique_ptr
#include <mutex>       // std::mutex, std::scoped_lock
#include <queue>       // std::queue
#include <thread>      // std::this_thread, std::thread
#include <type_traits> // std::common_type_t, std::decay_t, std::enable_if_t, std::is_void_v, std::invoke_result_t
#include <utility>     // std::move

class ThreadPool
{
	// Explicitly prefer fast performance from our uints over storage efficiency
	typedef std::uint_fast32_t ui32;
	typedef std::uint_fast64_t ui64;

public:

	/// <summary>
	/// Overloaded c'tor taking a number of threads to be initialized
	/// </summary>
	/// <param name="t_threadCount">Number of threads to create (defaults to max available)</param>
	ThreadPool(const ui32& t_threadCount = std::thread::hardware_concurrency())
		: m_threadCount(t_threadCount), m_threads(new std::thread[t_threadCount])
	{
		createThreads();
	}

	/// <summary>
	/// Waits for all tasks to complete, and then destroys all threads
	/// </summary>
	~ThreadPool()
	{
		waitForTasks();
		m_running = false;
		destroyThreads();
	}

	/// <summary>
	/// Gets the total number of tasks (running + queued)
	/// </summary>
	ui32 getTotalTasks() const { return m_totalTasks; }

	/// <summary>
	/// Gets the number of tasks currently running
	/// </summary>
	ui32 getRunningTasks() const { return m_totalTasks - getTasksQueued(); }

	/// <summary>
	/// Gets the number of tasks waiting to execute
	/// </summary>
	ui32 getTasksQueued() const
	{
		// Acquired a scoped lock on our queue mutex
		const std::scoped_lock lock(m_queueMutex);
		return m_tasks.size();
	}

	/// <summary>
	/// Gets the number of threads in the pool
	/// </summary>
	ui32 getThreadCount() const { return m_threadCount; }

	/// <summary>
	/// Push a void() function onto our tasks queue
	/// </summary>
	template <typename T>
	void pushTask(const T& t_task)
	{
		const std::scoped_lock lock(m_queueMutex);
		m_tasks.push(std::function<void()>(t_task));
		m_totalTasks++;
	}

	/// <summary>
	/// Push a void(args...) function onto our tasks queue
	/// </summary>
	template <typename T, typename...A>
	void pushTask(const T& t_task, const A& ...args)
	{
		// Wrap our task call in a lambda to hide the args from our queue
		push_task([t_task, args...]{ task(args...); });
	}

	/// <summary>
	/// Wait for all tasks to complete and resize our thread pool
	/// (Note: This will reallocate all threads from scratch)
	/// </summary>
	/// <param name="t_threadCount">Number of threads for resized pool</param>
	void resize(const ui32& t_threadCount = std::thread::hardware_concurrency())
	{
		waitForTasks();

		m_running = false;

		destroyThreads();
		m_threadCount = t_threadCount;
		m_threads.reset(new std::thread[m_threadCount]);

		m_running = true;

		createThreads();
	}

	/// <summary>
	/// Submits a void func to our queue taking 0...n args
	/// </summary>
	/// <param name="t_task">Function to submit</param>
	/// <param name="...args">Args to tie to the function call</param>
	/// <returns>An std::future which will contain a bool marking when our task has complete</returns>
	template <typename T, typename... A, typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<std::decay_t<T>, std::decay_t<A>...>>>>
	std::future<bool> submit(const T& t_task, const A& ...args)
	{
		std::shared_ptr<std::promise<bool>> promise(new std::promise<bool>);
		std::future<bool> future = promise->get_future();

		// Wrap our function call in error handling, and tie returns to promise
		pushTask([task, args..., promise]
		{
			try
			{
				task(args...);
				promise->set_value(true);
			}
			catch (...)
			{
				try
				{
					promise->set_exception(std::current_exception());
				}
				catch (...)
				{
				}
			}
		});

		return future;
	}

	/// <summary>
	/// Wait for all tasks to finish executing
	/// </summary>
	void waitForTasks()
	{
		while (true)
			if (!m_totalTasks)
				break;
			else
				sleep();
	}

	// How long our workers should sleep for if a task isn't available, in microseconds
	ui32 worker_sleep_duration = 1000;

private:
	/// <summary>
	/// Initialize our threads, with each running a worker function
	/// </summary>
	void createThreads()
	{
		for (ui32 i = 0; i < m_threadCount; ++i)
			m_threads[i] = std::thread(&ThreadPool::worker, this);
	}

	/// <summary>
	/// Destroy the threads in our pool by joining them
	/// </summary>
	void destroyThreads()
	{
		for (ui32 i = 0; i < m_threadCount; ++i)
			m_threads[i].join();
	}

	/// <summary>
	/// Pops a task from our queue into the passed param if one is available
	/// </summary>
	/// <param name="t_task">Empty function pointer to populate with task</param>
	/// <returns>True if task was available, otherwise false</returns>
	bool popTask(std::function<void()>& t_task)
	{
		// Acquire a lock on our queue mutex
		const std::scoped_lock lock(m_queueMutex);

		if (!m_tasks.empty())
		{
			t_task = std::move(m_tasks.front());
			m_tasks.pop();
			return true;
		}

		return false;
	}

	/// <summary>
	/// Have our thread sleep while waiting for work
	/// </summary>
	void sleep()
	{
		if (worker_sleep_duration)
			std::this_thread::sleep_for(std::chrono::microseconds(worker_sleep_duration));
		else
			std::this_thread::yield();
	}

	/// <summary>
	/// Worker function which will be assigned to a thread and look for work
	/// </summary>
	void worker()
	{
		while (m_running)
		{
			std::function<void()> task;

			if (popTask(task))
			{
				task();
				--m_totalTasks;
			}
			else
			{
				sleep();
			}
		}
	}

	// Synchronise access to our task queue
	mutable std::mutex m_queueMutex;

	// Let our worker threads know if they should keep running
	std::atomic<bool> m_running = true;

	// Our incoming tasks queue (type is void() as we wrap our tasks in a lambda)
	std::queue<std::function<void()>> m_tasks = {};

	// Number of threads in our pool
	ui32 m_threadCount;

	// Manage the memory allocated for our threads
	std::unique_ptr<std::thread[]> m_threads;

	// Total active tasks (queued + running)
	std::atomic<ui32> m_totalTasks;
};

#endif