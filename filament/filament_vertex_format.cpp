#include "filament_vertex_format.h"

#include "core/variant/variant.h"
#include "filament/MaterialEnums.h"
#include "filament/VertexBuffer.h"
#include "servers/rendering_server.h"

#include <geometry/Transcoder.h>
#include <geometry/SurfaceOrientation.h>

#include <memory>

FilamentVertexFormat::FilamentVertexFormat(uint64_t format, size_t numberOfVertices) : m_godotFormat(format), m_numberOfVertices(numberOfVertices) {
	/*
	 * NOTE: all contents of the 'vertex' vertex buffer are rebuilt in
	 * transcodeVertexDataForFilament, and are not used directly. See that
	 * function for details.
	 */

	if(format & RenderingServer::ARRAY_FORMAT_VERTEX) {
		if(format & RenderingServer::ARRAY_FLAG_USE_2D_VERTICES) {
			addAttributeToBuffer(m_positionDataBuffer, filament::VertexAttribute::POSITION, filament::VertexBuffer::AttributeType::FLOAT2, false);
		} else {
			auto &attribute = addAttributeToBuffer(m_positionDataBuffer, filament::VertexAttribute::POSITION, filament::VertexBuffer::AttributeType::FLOAT3, false);

			if(format & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
				/*
				 * Positions derived from the compressed vertex data will have
				 * an extra float component. We keep that to avoid one more
				 * repacking.
				 */
				attribute.stride += sizeof(float);
				m_buffers.at(*m_positionDataBuffer).position += sizeof(float) * m_numberOfVertices;
			}
		}
	}

	if(godotFormatHasNormals(format)) {
		addAttributeToBuffer(m_tangentDataBuffer, filament::VertexAttribute::TANGENTS, filament::VertexBuffer::AttributeType::SHORT4, true);
	}

	if(format & RenderingServer::ARRAY_FORMAT_COLOR) {
		addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::COLOR, filament::VertexBuffer::AttributeType::UBYTE4, true);
	}

	if(format & RenderingServer::ARRAY_FORMAT_TEX_UV) {
		if(format & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
			addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::UV0, filament::VertexBuffer::AttributeType::USHORT2, true);
		} else {
			addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::UV0, filament::VertexBuffer::AttributeType::FLOAT2, false);
		}
	}

	if(format & RenderingServer::ARRAY_FORMAT_TEX_UV2) {
		if(format & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
			addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::UV1, filament::VertexBuffer::AttributeType::USHORT2, true);
		} else {
			addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::UV1, filament::VertexBuffer::AttributeType::FLOAT2, false);
		}
	}

	if(format & RenderingServer::ARRAY_FORMAT_CUSTOM0) {
		bool normalized;
		auto type = convertCustomFormat((format >> RenderingServer::ARRAY_FORMAT_CUSTOM0_SHIFT) & RenderingServer::ARRAY_FORMAT_CUSTOM_BITS, normalized);
		addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::CUSTOM0, type, normalized);
	}

	if(format & RenderingServer::ARRAY_FORMAT_CUSTOM1) {
		bool normalized;
		auto type = convertCustomFormat((format >> RenderingServer::ARRAY_FORMAT_CUSTOM1_SHIFT) & RenderingServer::ARRAY_FORMAT_CUSTOM_BITS, normalized);
		addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::CUSTOM1, type, normalized);
	}

	if(format & RenderingServer::ARRAY_FORMAT_CUSTOM2) {
		bool normalized;
		auto type = convertCustomFormat((format >> RenderingServer::ARRAY_FORMAT_CUSTOM2_SHIFT) & RenderingServer::ARRAY_FORMAT_CUSTOM_BITS, normalized);
		addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::CUSTOM2, type, normalized);
	}

	if(format & RenderingServer::ARRAY_FORMAT_CUSTOM3) {
		bool normalized;
		auto type = convertCustomFormat((format >> RenderingServer::ARRAY_FORMAT_CUSTOM3_SHIFT) & RenderingServer::ARRAY_FORMAT_CUSTOM_BITS, normalized);
		addAttributeToBuffer(m_attributeDataBuffer, filament::VertexAttribute::CUSTOM3, type, normalized);
	}

	if(format & RenderingServer::ARRAY_FORMAT_BONES) {
		addAttributeToBuffer(m_skinDataBuffer, filament::VertexAttribute::BONE_INDICES, filament::VertexBuffer::AttributeType::USHORT4, false);
	}

	if(format & RenderingServer::ARRAY_FORMAT_WEIGHTS) {
		addAttributeToBuffer(m_skinDataBuffer, filament::VertexAttribute::BONE_WEIGHTS, filament::VertexBuffer::AttributeType::USHORT4, true);
	}
}

FilamentVertexFormat::~FilamentVertexFormat() = default;

void FilamentVertexFormat::transcodeVertexDataForFilament(const Vector<uint8_t> &godotFormattedData, std::optional<Vector<uint8_t>> &positionBuffer,
														  std::optional<Vector<uint8_t>> &tangentBuffer, const AABB &aabb) const {


	/*
	 * NOTE: we skip over ARRAY_FORMAT_TANGENT because that never has any data
	 * written in the current Godot, but may still be set.
	 */

	auto hasNormals = godotFormatHasNormals(m_godotFormat);

	if(!hasNormals) {
		if(!(m_godotFormat & RenderingServer::ARRAY_FORMAT_VERTEX)) {
			/*
			 * If we don't have position nor normals, then there's nothing to transcode.
			 */
			return;
		}

		if(!(m_godotFormat & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES)) {

			/*
			* If the vertex buffer only has positions, and is not compressed,
			* then its format is compatible with the Filament definition we
			* have above, and we can use it as-is.
			*/

			positionBuffer.emplace(godotFormattedData);
			return;
		}
	}

	size_t offset = 0;
	std::optional<filament::geometry::Transcoder::Config> vertexAttribute;
	std::optional<filament::geometry::Transcoder::Config> normalAttribute;
	size_t vertexOffset = 0;
	size_t normalOffset = 0;

	if(m_godotFormat & RenderingServer::ARRAY_FORMAT_VERTEX) {
		auto &config = vertexAttribute.emplace();
		vertexOffset = offset;

		if(m_godotFormat & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
			/*
			 * Compressed positions are stored in R16G16B16A16_UNORM.
			 * XYZ: quantized position, scaled and translated by the surface's
			 *      AABB.
			 * W:   angle for the normal axis-angle encoding.
			 */

			config.componentType = filament::geometry::ComponentType::USHORT;
			config.normalized = true;
			config.componentCount = 4;
			config.inputStrideBytes = 0;
			offset += 8 * m_numberOfVertices;
		} else {
			/*
			 * Uncompressed positions are just FLOAT3.
			 */
			config.componentType = filament::geometry::ComponentType::FLOAT;
			config.normalized = false;
			config.componentCount = 3;
			config.inputStrideBytes = 0;
			offset += sizeof(float) * 3 * m_numberOfVertices;
		}
	}

	if(hasNormals) {
		auto &config = normalAttribute.emplace();
		normalOffset = offset;

		if(m_godotFormat & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
			/*
			 * Compressed normals are stored in an axis-angle representation
			 * produced via _get_axis_angle in rendering_server.cpp.
			 * The 'angle' is stored in the W component of the position.
			 *
			 * The 'axis', encoded via octahedron_encode, is stored in the XY
			 * components of the normal.
			 */

			config.componentType = filament::geometry::ComponentType::USHORT;
			config.normalized = true;
			config.componentCount = 2;
			config.inputStrideBytes = 0;
			offset += 2 * sizeof(uint16_t) * m_numberOfVertices;
		} else {
			/*
			 * Uncompressed normals are stored in the normal attribute:
			 * X, Y: Normal vector, encoded via Vector3.octahedron_encode,
			 * Z, W: Tangent vector, encoded via Vector3.octahedron_tangent_encode
			 */
			config.componentType = filament::geometry::ComponentType::USHORT;
			config.normalized = true;
			config.componentCount = 4;
			config.inputStrideBytes = 0;
			offset += 4 * sizeof(uint16_t) * m_numberOfVertices;
		}
	}

	float *positions = nullptr;

	if(vertexAttribute.has_value()) {
		auto &buffer = positionBuffer.emplace();

		buffer.resize(vertexAttribute->componentCount * sizeof(float) * m_numberOfVertices);

		filament::geometry::Transcoder transcoder(*vertexAttribute);
		positions = reinterpret_cast<float *>(buffer.ptrw());
		transcoder(positions, godotFormattedData.ptr() + vertexOffset, m_numberOfVertices);

		if(m_godotFormat & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
			/*
			 * If the positions were stored compressed, then we also need to
			 * undo the normalization.
			 *
			 * If not, then we're already all set.
			 */

			for(auto vertex = positions, limit = positions + 4 * m_numberOfVertices; vertex < limit; vertex += 4) {
				vertex[0] = vertex[0] * aabb.size.x + aabb.position.x;
				vertex[1] = vertex[1] * aabb.size.y + aabb.position.y;
				vertex[2] = vertex[2] * aabb.size.z + aabb.position.z;
			}
		}
	}

	if(normalAttribute.has_value()) {
		/*
		 * Godot-encoded normals (either float2 or float4)
		 */
		PackedFloat32Array godotNormals;
		godotNormals.resize(normalAttribute->componentCount * m_numberOfVertices);

		filament::geometry::Transcoder transcoder(*vertexAttribute);
		transcoder(godotNormals.ptrw(), godotFormattedData.ptr() + normalOffset, m_numberOfVertices);

		/*
		 * Straight normals (float3)
		 */
		std::vector<filament::math::float3> normals;
		normals.resize(m_numberOfVertices);

		/*
		 * Straight tangents (float4)
		 */
		std::vector<filament::math::float4> tangents;
		tangents.resize(m_numberOfVertices);

		auto outputNormals = normals.data();
		auto outputTangents = tangents.data();

		if(m_godotFormat & RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES) {
			for(const float *inputPosition    = positions,
				            *inputNormal      = godotNormals.ptr(),
				            *inputNormalLimit = inputNormal + godotNormals.size();
			    inputNormal < inputNormalLimit;
				inputPosition += 4,
				inputNormal += 2) {

				auto axis = Vector3::octahedron_decode(Vector2(inputNormal[0], inputNormal[1]));
				auto angle = inputPosition[3];

				Vector3 decodedNormal;
				Vector4 decodedTangent;
				RenderingServer::_get_tbn_from_axis_angle(axis, angle, decodedNormal, decodedTangent);

				*outputNormals++ = filament::math::float3(decodedNormal.x, decodedNormal.y, decodedNormal.z);
				*outputTangents++ = filament::math::float4(decodedTangent.x, decodedTangent.y, decodedTangent.z, decodedTangent.w);
			}

		} else {
			for(const float *inputNormal      = godotNormals.ptr(),
				            *inputNormalLimit = inputNormal + godotNormals.size();
			    inputNormal < inputNormalLimit;
				inputNormal += 4) {

				Vector3 decodedNormal = Vector3::octahedron_decode(Vector2(inputNormal[0], inputNormal[1]));

				float binormalSign;
				auto decodedTangent = Vector3::octahedron_tangent_decode(Vector2(inputNormal[2], inputNormal[3]), &binormalSign);

				*outputNormals++ = filament::math::float3(decodedNormal.x, decodedNormal.y, decodedNormal.z);
				*outputTangents++ = filament::math::float4(decodedTangent.x, decodedTangent.y, decodedTangent.z, binormalSign);
			}
		}

		std::unique_ptr<filament::geometry::SurfaceOrientation> orientation(
			filament::geometry::SurfaceOrientation::Builder().vertexCount(m_numberOfVertices).normals(normals.data()).tangents(tangents.data()).build());

		if(!orientation) {
			throw std::runtime_error("failed to build SurfaceOrientation");
		}

		auto &outputTangentBuffer = tangentBuffer.emplace();
		outputTangentBuffer.resize(sizeof(filament::math::short4) * m_numberOfVertices);

		orientation->getQuats(reinterpret_cast<filament::math::short4 *>(outputTangentBuffer.ptrw()), m_numberOfVertices);

	}

}

bool FilamentVertexFormat::godotFormatHasNormals(uint64_t format) {

	auto normalEncoding = format & (RenderingServer::ARRAY_FORMAT_NORMAL | RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES | RenderingServer::ARRAY_FLAG_USE_2D_VERTICES);

	/*
	 * ARRAY_FORMAT_NORMAL | ARRAY_FLAG_COMPRESS_ATTRIBUTES | ARRAY_FLAG_USE_2D_VERTICES is an invalid configuration and Godot
	 * straight up writes no normals, quietly, if that's set.
	 */
	return (normalEncoding & RenderingServer::ARRAY_FORMAT_NORMAL) &&
		normalEncoding != (RenderingServer::ARRAY_FORMAT_NORMAL | RenderingServer::ARRAY_FLAG_COMPRESS_ATTRIBUTES | RenderingServer::ARRAY_FLAG_USE_2D_VERTICES);
}

filament::VertexBuffer::AttributeType FilamentVertexFormat::convertCustomFormat(uint32_t type, bool &normalized) {
	switch(static_cast<RenderingServer::ArrayCustomFormat>(type)) {
		case RenderingServer::ARRAY_CUSTOM_RGBA8_UNORM:
			normalized = true;
			return filament::VertexBuffer::AttributeType::UBYTE4;

		case RenderingServer::ARRAY_CUSTOM_RGBA8_SNORM:
			normalized = true;
			return filament::VertexBuffer::AttributeType::BYTE4;

		case RenderingServer::ARRAY_CUSTOM_RG_HALF:
			normalized = false;
			return filament::VertexBuffer::AttributeType::HALF2;

		case RenderingServer::ARRAY_CUSTOM_RGBA_HALF:
			normalized = false;
			return filament::VertexBuffer::AttributeType::HALF4;

		case RenderingServer::ARRAY_CUSTOM_R_FLOAT:
			normalized = false;
			return filament::VertexBuffer::AttributeType::FLOAT;

		case RenderingServer::ARRAY_CUSTOM_RG_FLOAT:
			normalized = false;
			return filament::VertexBuffer::AttributeType::FLOAT2;

		case RenderingServer::ARRAY_CUSTOM_RGB_FLOAT:
			normalized = false;
			return filament::VertexBuffer::AttributeType::FLOAT3;

		case RenderingServer::ARRAY_CUSTOM_RGBA_FLOAT:
			normalized = false;
			return filament::VertexBuffer::AttributeType::FLOAT4;

		default:
			throw std::runtime_error("unsupported format of a custom vertex attribute");
	};
}

auto FilamentVertexFormat::addAttributeToBuffer(std::optional<size_t> &buffer, filament::VertexAttribute attribute, filament::VertexBuffer::AttributeType attributeType,
												bool normalized) -> Attribute & {

	Buffer *bufferDesc;
	if(buffer.has_value()) {
		bufferDesc = &m_buffers.at(*buffer);
	} else {
		buffer.emplace(m_buffers.size());
		bufferDesc = &m_buffers.emplace_back();
		bufferDesc->position = 0;
	}

	auto &attributeDesc = m_attributes.emplace_back();
	attributeDesc.bufferIndex = *buffer;
	attributeDesc.attribute = attribute;
	attributeDesc.attributeType = attributeType;
	attributeDesc.offset = bufferDesc->position;
	attributeDesc.stride = lengthForAttributeType(attributeType);
	attributeDesc.normalized = normalized;

	bufferDesc->position += attributeDesc.stride * m_numberOfVertices;

	return attributeDesc;
}

size_t FilamentVertexFormat::lengthForAttributeType(filament::VertexBuffer::AttributeType type) {
	switch(type) {
		case filament::VertexBuffer::AttributeType::BYTE:
		case filament::VertexBuffer::AttributeType::UBYTE:
			return sizeof(uint8_t);

		case filament::VertexBuffer::AttributeType::BYTE2:
		case filament::VertexBuffer::AttributeType::UBYTE2:
			return sizeof(uint8_t) * 2;

		case filament::VertexBuffer::AttributeType::BYTE3:
		case filament::VertexBuffer::AttributeType::UBYTE3:
			return sizeof(uint8_t) * 3;

		case filament::VertexBuffer::AttributeType::BYTE4:
		case filament::VertexBuffer::AttributeType::UBYTE4:
			return sizeof(uint8_t) * 4;

		case filament::VertexBuffer::AttributeType::SHORT:
		case filament::VertexBuffer::AttributeType::USHORT:
		case filament::VertexBuffer::AttributeType::HALF:
			return sizeof(uint16_t);

		case filament::VertexBuffer::AttributeType::SHORT2:
		case filament::VertexBuffer::AttributeType::USHORT2:
		case filament::VertexBuffer::AttributeType::HALF2:
			return sizeof(uint16_t) * 2;

		case filament::VertexBuffer::AttributeType::SHORT3:
		case filament::VertexBuffer::AttributeType::USHORT3:
		case filament::VertexBuffer::AttributeType::HALF3:
			return sizeof(uint16_t) * 3;

		case filament::VertexBuffer::AttributeType::SHORT4:
		case filament::VertexBuffer::AttributeType::USHORT4:
		case filament::VertexBuffer::AttributeType::HALF4:
			return sizeof(uint16_t) * 4;

		case filament::VertexBuffer::AttributeType::INT:
		case filament::VertexBuffer::AttributeType::UINT:
		case filament::VertexBuffer::AttributeType::FLOAT:
			return sizeof(uint32_t);

		case filament::VertexBuffer::AttributeType::FLOAT2:
			return sizeof(uint32_t) * 2;

		case filament::VertexBuffer::AttributeType::FLOAT3:
			return sizeof(uint32_t) * 3;

		case filament::VertexBuffer::AttributeType::FLOAT4:
			return sizeof(uint32_t) * 4;

		default:
			throw std::runtime_error("unsupported vertex attribute type");
	}
}

