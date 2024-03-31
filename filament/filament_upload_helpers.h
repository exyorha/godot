#ifndef FILAMENT_UPLOAD_HELPERS_H
#define FILAMENT_UPLOAD_HELPERS_H

#include "core/templates/vector.h"

#include <backend/BufferDescriptor.h>

#include <memory>

template<typename T>
void filamentBufferDescriptorCallback(void* buffer, size_t size, void* user) {
	delete static_cast<Vector<T> *>(user);
}

template<typename T>
filament::backend::BufferDescriptor makeFilamentDescriptor(const Vector<T> &data) {
	auto vectorCopy = std::make_unique<Vector<T>>(data);

	/*
	 * BufferDescriptor constructor is noexcept.
	 */
	auto rawVectorCopy = vectorCopy.release();
	return filament::backend::BufferDescriptor(rawVectorCopy->ptr(), rawVectorCopy->size() * sizeof(T), nullptr,
											   filamentBufferDescriptorCallback<T>, rawVectorCopy);
}

#endif
