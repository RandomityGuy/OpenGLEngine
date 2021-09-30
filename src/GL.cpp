#include "GL.h"
#include <glad/glad.h>

void GL::vertexAttribPointer(int location, int size, int type, int stride, int offset, bool normalize)
{
	glVertexAttribPointer(location, size, type, normalize ? GL_TRUE : GL_FALSE, stride, (void*)offset);
}

void GL::enableVertexAttribArray(int location)
{
	glEnableVertexAttribArray(location);
}

void GL::bindBuffer(GL::BufferTarget target, GLuint buffer)
{
	glBindBuffer(target, buffer);
}

void GL::bindVertexArray(GLuint arr)
{
	glBindVertexArray(arr);
}

void GL::setClearColor(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void GL::setClearDepth(double depth)
{
	glClearDepth(depth);
}

void GL::setClearStencil(int s)
{
	glClearStencil(s);
}

void GL::clear(int target)
{
	glClear(target);
}

void GL::setDepthTest(bool enabled)
{
	if (enabled) {
		glEnable(GL_DEPTH_TEST);
	}
	else 
	{
		glDisable(GL_DEPTH_TEST);
	}
}

void GL::depthWrite(bool write)
{
	glDepthMask(write);
}

void GL::depthFunc(CompareFunc func)
{
	glDepthFunc(func);
}

void GL::setStencilTest(bool enabled)
{
	if (enabled) {
		glEnable(GL_STENCIL_TEST);
	}
	else
	{
		glDisable(GL_STENCIL_TEST);
	}
}

void GL::stencilMask(GLuint mask)
{
	glStencilMask(mask);
}

void GL::stencilFunc(CompareFunc func, int ref, GLuint mask)
{
	glStencilFunc(func, ref, mask);
}

void GL::stencilOp(StencilOp stencilFail, StencilOp depthFail, StencilOp depthPass)
{
	glStencilOp(stencilFail, depthFail, depthPass);
}
