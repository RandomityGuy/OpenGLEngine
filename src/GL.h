#pragma once
#include <glad/glad.h>

class GL
{
public:
	enum class BufferTarget
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

	enum class ClearTarget
	{
		Color = GL_COLOR_BUFFER_BIT,
		Depth = GL_DEPTH_BUFFER_BIT,
		Stencil = GL_STENCIL_BUFFER_BIT
	};

	enum class CompareFunc
	{
		Always = GL_ALWAYS, //	The depth test always passes.
		Never = GL_NEVER,	// The depth test never passes.
		LessThan = GL_LESS,	// Passes if the fragment's depth value is less than the stored depth value.
		Equals = GL_EQUAL,	// Passes if the fragment's depth value is equal to the stored depth value.
		LessThanEquals = GL_LEQUAL,	// Passes if the fragment's depth value is less than or equal to the stored depth value.
		GreaterThan = GL_GREATER,	// Passes if the fragment's depth value is greater than the stored depth value.
		NotEquals = GL_NOTEQUAL,	// Passes if the fragment's depth value is not equal to the stored depth value.
		GreaterThanEquals = GL_GEQUAL,	// Passes if the fragment's depth value is greater than or equal to the stored depth value.

	};

	enum class StencilOp
	{
		Keep = GL_KEEP,	// The currently stored stencil value is kept.
		Zero = GL_ZERO,	// The stencil value is set to 0.
		Replace = GL_REPLACE,	// The stencil value is replaced with the reference value set with glStencilFunc.
		Increment = GL_INCR,	// The stencil value is increased by 1 if it is lower than the maximum value.
		IncrementWrap = GL_INCR_WRAP,	// Same as GL_INCR, but wraps it back to 0 as soon as the maximum value is exceeded.
		Decrement = GL_DECR,	// The stencil value is decreased by 1 if it is higher than the minimum value.
		DecrementWrap = GL_DECR_WRAP,	// Same as GL_DECR, but wraps it to the maximum value if it ends up lower than 0.
		Invert = GL_INVERT,	// Bitwise inverts the current stencil buffer value.
	};

	enum class BlendMode
	{
		Zero = GL_ZERO, // Factor is equal to 0.
		One = GL_ONE, //  Factor is equal to 1.
		Src = GL_SRC_COLOR, //	Factor is equal to the source color vector C_source.
		OneMinusSrc = GL_ONE_MINUS_SRC_COLOR, // Factor is equal to 1 minus the source color vector : 1 - C_source.
		Dst = GL_DST_COLOR, // Factor is equal to the destination color vector C_destination
		OneMinusDst = GL_ONE_MINUS_DST_COLOR, // Factor is equal to 1 minus the destination color vector : 1 - C_destination.
		SrcAlpha = GL_SRC_ALPHA, // Factor is equal to the alpha component of the source color vector C_source.
		OneMinusSrcAlpha = GL_ONE_MINUS_SRC_ALPHA, // Factor is equal to 1 - alpha of the source color vector C_source.
		DstAlpha = GL_DST_ALPHA, // Factor is equal to the alpha component of the destination color vector C_destination.
		OneMinusDstAlpha = GL_ONE_MINUS_DST_ALPHA, // Factor is equal to 1 - alpha of the destination color vector C_destination.
		Constant = GL_CONSTANT_COLOR, // Factor is equal to the constant color vector C_constant.
		OneMinusConstant = GL_ONE_MINUS_CONSTANT_COLOR, // Factor is equal to 1 - the constant color vector C_constant.
		ConstantAlpha = GL_CONSTANT_ALPHA, // Factor is equal to the alpha component of the constant color vector C_constant.
		OneMinusConstantAlpha = GL_ONE_MINUS_CONSTANT_ALPHA, // Factor is equal to 1 - alpha of the constant color vector C_constant.
	};

	enum class BlendEquation
	{
		Add = GL_FUNC_ADD, // C = Src + Dst
		Subtract = GL_FUNC_SUBTRACT, // C = Src - Dst
		ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT, // C = Dst - Src
		Min = GL_MIN, // C = min(Src, Dst)
		Max = GL_MAX, // C = max(Src, Dst)
	};

	enum class CullFace
	{
		Front = GL_FRONT, // Front face
		Back = GL_BACK, // Back face
		Both = GL_FRONT_AND_BACK, // Both faces
	};

	enum class WindingOrder
	{
		Clockwise = GL_CW,
		CounterClockwise = GL_CCW
	};

	static void vertexAttribPointer(int location, int size, int type, int stride, int offset, bool normalize);
	static void enableVertexAttribArray(int location);
	static void bindBuffer(BufferTarget target, GLuint buffer);
	static void bindVertexArray(GLuint arr);

	// Clearing
	static void setClearColor(float r, float g, float b, float a);
	static void setClearDepth(double depth);
	static void setClearStencil(int s);
	static void clear(int target);

	// Depth testing
	static void setDepthTest(bool enabled);
	static void depthWrite(bool write);
	static void depthFunc(CompareFunc func);

	// Stencil testing
	static void setStencilTest(bool enabled);
	static void stencilMask(GLuint mask);
	static void stencilFunc(CompareFunc func, int ref, GLuint mask);
	static void stencilOp(StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass);

	// Blending
	static void setBlending(bool enabled);
	static void blendFunc(BlendMode src, BlendMode dst, BlendMode srcalpha, BlendMode dstalpha);
	static void blendEquation(BlendEquation eq);

	// Culling
	static void setCulling(bool enabled);
	static void cullFace(CullFace face);
	static void frontFace(WindingOrder order);
};