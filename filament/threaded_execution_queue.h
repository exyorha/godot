#ifndef THREADED_EXECUTION_QUEUE_H
#define THREADED_EXECUTION_QUEUE_H

#include <functional>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <thread>
#include <atomic>

#include "atomic_queue/atomic_queue.h"

template<typename T>
class ThreadedExecutionQueueCallResultHolder {
public:
	ThreadedExecutionQueueCallResultHolder() = default;
	~ThreadedExecutionQueueCallResultHolder() = default;

	T finish() {
		std::unique_lock<std::mutex> locker(m_mutex);

		m_condvar.wait(locker, [this]() { return m_storage.has_value(); });

		return std::move(*m_storage);
	}

	void store(T &&value) {
		{
			std::unique_lock<std::mutex> locker(m_mutex);

			m_storage.emplace(std::move(value));

			m_condvar.notify_all();
		}
	}

private:
	std::mutex m_mutex;
	std::condition_variable m_condvar;
	std::optional<T> m_storage;
};

class BaseThreadedExecutionQueueBackend {
protected:
	BaseThreadedExecutionQueueBackend();
public:

	virtual ~BaseThreadedExecutionQueueBackend();

	BaseThreadedExecutionQueueBackend(const BaseThreadedExecutionQueueBackend &other) = delete;
	BaseThreadedExecutionQueueBackend &operator =(const BaseThreadedExecutionQueueBackend &other) = delete;

	virtual void runStepOnThread() = 0;
};

class BaseThreadedExecutionQueue {
protected:
	using BackendFactoryFunction = std::unique_ptr<BaseThreadedExecutionQueueBackend>();

	explicit BaseThreadedExecutionQueue(BackendFactoryFunction factory);
	~BaseThreadedExecutionQueue();

	BaseThreadedExecutionQueue(const BaseThreadedExecutionQueue &other) = delete;
	BaseThreadedExecutionQueue &operator =(const BaseThreadedExecutionQueue &other) = delete;

	template<typename ReturnType>
	void invoke(std::function<ReturnType()> &&function, ThreadedExecutionQueueCallResultHolder<ReturnType> &resultHolder) {
		return enqueue([function, &resultHolder]() { resultHolder.store(function()); });
	}

	void enqueue(std::function<void()> &&function);

	BaseThreadedExecutionQueueBackend *m_backend;

private:
	class BackendRegistration {
	public:
		explicit BackendRegistration(BaseThreadedExecutionQueue *onQueue, std::unique_ptr<BaseThreadedExecutionQueueBackend> &&backend);
		~BackendRegistration();

		BackendRegistration(const BackendRegistration &other) = delete;
		BackendRegistration &operator =(const BackendRegistration &other) = delete;

	private:
		BaseThreadedExecutionQueue *m_queue;
		std::unique_ptr<BaseThreadedExecutionQueueBackend> m_backend;
	};

	void queueThread(BackendFactoryFunction factory);

	void registerBackend(BaseThreadedExecutionQueueBackend *backend);
	inline void unregisterBackend() {
		m_backend = nullptr;
	}

	std::thread m_queueThread;
	std::mutex m_initializationMutex;
	std::condition_variable m_initializationCondvar;
	std::atomic_bool m_shutdown;
	atomic_queue::AtomicQueue2<std::function<void()>, 512> m_queue;
};

template<typename Backend>
class ThreadedExecutionQueue : public BaseThreadedExecutionQueue {
protected:
	inline ThreadedExecutionQueue() : BaseThreadedExecutionQueue(&factoryFunction) {

	}

	~ThreadedExecutionQueue() = default;

	ThreadedExecutionQueue(const ThreadedExecutionQueue &other) = delete;
	ThreadedExecutionQueue &operator =(const ThreadedExecutionQueue &other) = delete;

protected:
	template<typename ResultType, typename... Args>
	ResultType execute(ResultType (Backend::*pointer)(Args... args), Args... args) {
		ThreadedExecutionQueueCallResultHolder<ResultType> result;

		invoke<ResultType>(std::bind(
			static_cast<ResultType (BaseThreadedExecutionQueueBackend::*)(Args... args)>(pointer),
			m_backend, args...), result);

		return result.finish();
	}

	template<typename DummyResultType, typename... Args>
	void execute(void (Backend::*pointer)(Args... args), Args... args) {
		enqueue(std::bind(
			static_cast<void (BaseThreadedExecutionQueueBackend::*)(Args... args)>(pointer),
			m_backend, args...));
	}

	template<typename ResultType, typename... Args>
	ResultType execute(ResultType (Backend::*pointer) (Args... args) const, Args... args) const {
		ThreadedExecutionQueueCallResultHolder<ResultType> result;

		const_cast<ThreadedExecutionQueue<Backend> *>(this)->invoke<ResultType>(std::bind(
			static_cast<ResultType (BaseThreadedExecutionQueueBackend::*)(Args... args) const>(pointer),
			m_backend, args...), result);

		return result.finish();
	}

	template<typename DummyResultType, typename... Args>
	void execute(void (Backend::*pointer) (Args... args) const, Args... args) const {
		const_cast<ThreadedExecutionQueue<Backend> *>(this)->enqueue(std::bind(
			static_cast<void (BaseThreadedExecutionQueueBackend::*)(Args... args) const>(pointer),
			m_backend, args...));
	}

private:
	static std::unique_ptr<BaseThreadedExecutionQueueBackend> factoryFunction() {
		return std::make_unique<Backend>();
	}
};

#endif

