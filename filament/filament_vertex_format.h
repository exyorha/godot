#ifndef FILAMENT_VERTEX_FORMAT_H
#define FILAMENT_VERTEX_FORMAT_H

#include <cstdint>
#include <vector>
#include <optional>

#include "core/templates/vector.h"

#include <filament/VertexBuffer.h>

struct AABB;

class FilamentVertexFormat {
public:
	struct Attribute {
		uint32_t offset;
		uint8_t bufferIndex;
		filament::VertexAttribute attribute;
		filament::VertexBuffer::AttributeType attributeType;
		uint8_t stride;
		bool normalized;
	};

	struct Buffer {
		size_t position;
		uint8_t interleavedBufferStride;
	};


	FilamentVertexFormat(uint64_t godotSurfaceFormat, size_t numberOfVertices);
	~FilamentVertexFormat();

	FilamentVertexFormat(const FilamentVertexFormat &other) = delete;
	FilamentVertexFormat &operator =(const FilamentVertexFormat &other) = delete;

	inline bool empty() const {
		return m_buffers.empty();
	}

	inline const std::vector<Attribute> &attributes() const {
		return m_attributes;
	}

	inline const std::vector<Buffer> &buffers() const {
		return m_buffers;
	}

	inline bool hasPositionDataBuffer() const {
		return m_positionDataBuffer.has_value();
	}

	inline size_t positionDataBuffer() const {
		return m_positionDataBuffer.value();
	}

	inline bool hasTangentDataBuffer() const {
		return m_tangentDataBuffer.has_value();
	}

	inline size_t tangentDataBuffer() const {
		return m_tangentDataBuffer.value();
	}

	inline bool hasAttributeDataBuffer() const {
		return m_attributeDataBuffer.has_value();
	}

	inline size_t attributeDataBuffer() const {
		return m_attributeDataBuffer.value();
	}

	inline bool hasSkinDataBuffer() const {
		return m_skinDataBuffer.has_value();
	}

	inline size_t skinDataBuffer() const {
		return m_skinDataBuffer.value();
	}

	void transcodeVertexDataForFilament(
		const Vector<uint8_t> &godotFormattedData, std::optional<Vector<uint8_t>> &positionBuffer, std::optional<Vector<uint8_t>> &tangentBuffer,
		const AABB &aabb) const;

private:
	Attribute &addAttributeToBuffer(std::optional<size_t> &buffer, filament::VertexAttribute attribute, filament::VertexBuffer::AttributeType attributeType, bool normalized, bool interleaved = false);

	static bool godotFormatHasNormals(uint64_t format);

	static filament::VertexBuffer::AttributeType convertCustomFormat(uint32_t type, bool &normalized);
	static size_t lengthForAttributeType(filament::VertexBuffer::AttributeType type);

	uint64_t m_godotFormat;
	size_t m_numberOfVertices;
	std::vector<Attribute> m_attributes;
	std::vector<Buffer> m_buffers;
	std::optional<size_t> m_positionDataBuffer;
	std::optional<size_t> m_tangentDataBuffer;
	std::optional<size_t> m_attributeDataBuffer;
	std::optional<size_t> m_skinDataBuffer;
};

#endif
