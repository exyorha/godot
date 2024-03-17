#include "threaded_execution_queue.h"

BaseThreadedExecutionQueueBackend::BaseThreadedExecutionQueueBackend() = default;

BaseThreadedExecutionQueueBackend::~BaseThreadedExecutionQueueBackend() = default;

BaseThreadedExecutionQueue::BaseThreadedExecutionQueue(BackendFactoryFunction factory) :
	m_backend(nullptr),
	m_queueThread(&BaseThreadedExecutionQueue::queueThread, this, factory),
	m_shutdown(false) {

	std::unique_lock<std::mutex> locker(m_initializationMutex);
	m_initializationCondvar.wait(locker, [this]() { return m_backend != nullptr; });
}

BaseThreadedExecutionQueue::~BaseThreadedExecutionQueue() {
	m_shutdown.store(true);
	m_queueThread.join();
}

void BaseThreadedExecutionQueue::queueThread(BackendFactoryFunction factory) {
	BackendRegistration registration(this, factory());

	while(!m_shutdown.load()) {
		std::function<void()> callable;
		while(m_queue.try_pop(callable)) {
			callable();
		}

		m_backend->runStepOnThread();
	}
}

void BaseThreadedExecutionQueue::enqueue(std::function<void()> &&function) {
	m_queue.push(std::move(function));
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
