#ifndef FILAMENT_ENGINE_OBJECT_H
#define FILAMENT_ENGINE_OBJECT_H

#include <utility>

template<typename T>
class FilamentEngineObjectReleaser {
public:
	static void release(T *ptr);
};

template<class T, class Releaser>
class FilamentEngineObjectBase {
public:
	explicit inline FilamentEngineObjectBase(T *ptr = nullptr) noexcept : m_ptr(ptr) {

	}

	~FilamentEngineObjectBase() {
		FilamentEngineObjectReleaser<T>::release(m_ptr);
	}

	inline FilamentEngineObjectBase(const FilamentEngineObjectBase &other) = delete;
	inline FilamentEngineObjectBase &operator =(const FilamentEngineObjectBase &other) = delete;

	inline FilamentEngineObjectBase(FilamentEngineObjectBase &&other) noexcept :FilamentEngineObjectBase() {
		*this = std::move(other);
	}

	FilamentEngineObjectBase &operator =(FilamentEngineObjectBase &&other) noexcept {
		if(this != &other) {
			Releaser::release(m_ptr);

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

template<typename T> using FilamentEngineObject = FilamentEngineObjectBase<T, FilamentEngineObjectReleaser<T>>;

#endif
