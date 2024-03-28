#ifndef FILAMENT_CONTROLLED_OBJECT_REFERENCE_H
#define FILAMENT_CONTROLLED_OBJECT_REFERENCE_H

#include <memory>

#include "filament_linked_list_entry.h"

class FilamentObject;
class FilamentControlledObjectReferenceBase;

class FilamentControlledObjectReferenceOwner {
protected:
	FilamentControlledObjectReferenceOwner();
	~FilamentControlledObjectReferenceOwner();

	FilamentControlledObjectReferenceOwner(const FilamentControlledObjectReferenceOwner &other) = delete;
	FilamentControlledObjectReferenceOwner &operator =(const FilamentControlledObjectReferenceOwner &other) = delete;

public:
	virtual void controlledObjectAboutToInvalidate(FilamentControlledObjectReferenceBase *linkedViaReference) = 0;
};

class FilamentControlledObjectReferenceBase : public FilamentLinkedListEntry {
protected:
	FilamentControlledObjectReferenceBase(FilamentControlledObjectReferenceOwner *owner) noexcept;
	~FilamentControlledObjectReferenceBase();

	FilamentControlledObjectReferenceBase(const FilamentControlledObjectReferenceBase &other) = delete;
	FilamentControlledObjectReferenceBase &operator =(const FilamentControlledObjectReferenceBase &other) = delete;

	FilamentControlledObjectReferenceBase(FilamentControlledObjectReferenceBase &&other) noexcept;
	FilamentControlledObjectReferenceBase &operator =(FilamentControlledObjectReferenceBase &&other) noexcept;

	void assignReference(const std::shared_ptr<FilamentObject> &reference) noexcept;

public:

	inline operator bool() const noexcept {
		return static_cast<bool>(m_reference);
	}

	void reset() noexcept;

	template<typename U>
	inline bool operator ==(const std::shared_ptr<U> &other) const {
		return m_reference == other;
	}

	template<typename U>
	inline bool operator !=(const std::shared_ptr<U> &other) const {
		return m_reference != other;
	}

	inline void referencedObjectIsAboutToInvalidate() {
		m_owner->controlledObjectAboutToInvalidate(this);
	}

protected:
	std::shared_ptr<FilamentObject> m_reference;

private:
	FilamentControlledObjectReferenceOwner *m_owner;
};

template<typename T>
class FilamentControlledObjectReference final : public FilamentControlledObjectReferenceBase {
public:
	inline FilamentControlledObjectReference(FilamentControlledObjectReferenceOwner *owner) noexcept : FilamentControlledObjectReferenceBase(owner) {

	}

	inline ~FilamentControlledObjectReference() noexcept = default;

	inline FilamentControlledObjectReference(FilamentControlledObjectReference &&other) noexcept = default;
	inline FilamentControlledObjectReference &operator =(FilamentControlledObjectReference &&other) noexcept = default;

	inline FilamentControlledObjectReference &operator =(const std::shared_ptr<T> &ref) noexcept {
		assignReference(ref);

		return *this;
	}

	inline operator std::shared_ptr<T>() const noexcept {
		return std::static_pointer_cast<T>(m_reference);
	}

	inline T &operator *() const noexcept {
		return *get();
	}

	inline T *operator ->() const noexcept {
		return get();
	}

	inline T *get() const noexcept {
		return static_cast<T *>(m_reference.get());
	}
};

#endif
