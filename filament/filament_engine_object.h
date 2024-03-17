#ifndef FILAMENT_ENGINE_OBJECT_H
#define FILAMENT_ENGINE_OBJECT_H

#include <utility>

template<typename T>
class FilamentEngineObjectReleaser {
public:
	static void release(T *ptr);
};

template<class T>
class FilamentEngineObject {
public:
	explicit inline FilamentEngineObject(T *ptr = nullptr) noexcept : m_ptr(ptr) {

	}

	~FilamentEngineObject() {
		FilamentEngineObjectReleaser<T>::release(m_ptr);
	}

	inline FilamentEngineObject(const FilamentEngineObject &other) = delete;
	inline FilamentEngineObject &operator =(const FilamentEngineObject &other) = delete;

	inline FilamentEngineObject(FilamentEngineObject &&other) noexcept :FilamentEngineObject() {
		*this = std::move(other);
	}

	FilamentEngineObject &operator =(FilamentEngineObject &&other) noexcept {
		if(this != &other) {
			FilamentEngineObjectReleaser<T>::release(m_ptr);

			m_ptr = other.m_ptr;
			other.m_ptr = nullptr;
		}

		return *this;
	}

	inline explicit operator bool() const {
		return static_cast<bool>(m_ptr);
	}

	inline T *get() const {
		return m_ptr;
	}

	inline T *operator ->() const {
		return m_ptr;
	}

	inline T &operator *() const {
		return *m_ptr;
	}

private:
	T *m_ptr;
};

#endif
