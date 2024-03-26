#include "threaded_execution_queue.h"

#include <cstdio>

BaseThreadedExecutionQueueBackend::BaseThreadedExecutionQueueBackend() = default;

BaseThreadedExecutionQueueBackend::~BaseThreadedExecutionQueueBackend() = default;

BaseThreadedExecutionQueue::BaseThreadedExecutionQueue(BackendFactoryFunction factory) :
	m_backend(nullptr),
	m_queueThread(&BaseThreadedExecutionQueue::queueThread, this, factory),
	m_shutdown(false),
	m_jobWakeupTokens(0) {

	std::unique_lock<std::mutex> locker(m_initializationMutex);
	m_initializationCondvar.wait(locker, [this]() { return m_backend != nullptr; });
}

BaseThreadedExecutionQueue::~BaseThreadedExecutionQueue() {
	m_shutdown.store(true);
	flush();
	m_queueThread.join();
}

void BaseThreadedExecutionQueue::flush() {
	{
		std::unique_lock<std::mutex> locker(m_jobWaitMutex);

		m_jobWakeupTokens++;
	}

	m_jobCondvar.notify_one();
}

void BaseThreadedExecutionQueue::queueThread(BackendFactoryFunction factory) {
	BackendRegistration registration(this, factory());

	while(!m_shutdown.load()) {
		unsigned int iterations = 0;
		while(iterations < 1000) {
			std::function<void()> callable;
			if(m_queue.try_pop(callable)) {
				iterations = 0;
				callable();
			} else {
				_mm_pause();
				iterations++;
			}
		}

		{
			std::unique_lock<std::mutex> locker(m_jobWaitMutex);
			m_jobCondvar.wait(locker, [this]() { return m_jobWakeupTokens != 0 || m_shutdown.load(); });
			if(m_jobWakeupTokens) {
				m_jobWakeupTokens -= 1;
			}
		}

		//m_backend->runStepOnThread();
	}
}

void BaseThreadedExecutionQueue::enqueue(std::function<void()> &&function) {
	bool woken = false;
	while(!m_queue.try_push(std::move(function))) {
		if(!woken) {
			flush();
			woken = true;
		}
		_mm_pause();
	}
}

BaseThreadedExecutionQueue::BackendRegistration::BackendRegistration(
	BaseThreadedExecutionQueue *onQueue,
	std::unique_ptr<BaseThreadedExecutionQueueBackend> &&backend) :

	m_queue(onQueue),
	m_backend(std::move(backend)) {

	m_queue->registerBackend(m_backend.get());
}

BaseThreadedExecutionQueue::BackendRegistration::~BackendRegistration() {

	m_queue->unregisterBackend();
}

void BaseThreadedExecutionQueue::registerBackend(BaseThreadedExecutionQueueBackend *backend) {
	{
		std::unique_lock<std::mutex> locker(m_initializationMutex);
		m_backend = backend;
	}

	m_initializationCondvar.notify_all();
}
