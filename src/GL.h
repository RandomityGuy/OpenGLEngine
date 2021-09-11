#pragma once
#include <glad/glad.h>

class GL
{
public:
	enum BufferTarget
	{
		ArrayBuffer = GL_ARRAY_BUFFER, // 	Vertex attributes
		AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER, // Atomic counter storage
		CopyReadBuffer = GL_COPY_READ_BUFFER,	// Buffer copy source
		CopyWriteBuffer = GL_COPY_WRITE_BUFFER,	// Buffer copy destination
		DispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,	// Indirect compute dispatch commands
		DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,	// Indirect command arguments
		ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,	// Vertex array indices
		PixelPackBuffer = GL_PIXEL_PACK_BUFFER,	// Pixel read target
		PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,	// Texture data source
		QueryResultBuffer = GL_QUERY_BUFFER, // Query result buffer
		ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,	// Read - write storage for shaders
		TextureBuffer = GL_TEXTURE_BUFFER,	// Texture data buffer
		TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,	// Transform feedback buffer
		UniformBuffer = GL_UNIFORM_BUFFER // Uniform block storage
	};

	enum ClearTarget
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};

	static void vertexAttribPointer(int location, int size, int type, int stride, int offset, bool normalize);
	static void enableVertexAttribArray(int location);
	static void bindBuffer(BufferTarget target, GLuint buffer);
	static void bindVertexArray(GLuint arr);
	static void setClearColor(float r, float g, float b, float a);
	static void setClearDepth(double depth);
	static void setClearStencil(int s);
	static void clear(int target);
	static void setDepthTest(bool enabled);
};