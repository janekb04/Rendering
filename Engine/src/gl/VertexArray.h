#pragma once

#include "../Vendor.h"
#include "GLObject.h"
#include "Buffer.h"

class VertexAttribute
{
private:
	GLuint m_index;
public:
	explicit VertexAttribute(unsigned int location) :
		m_index{ location }
	{
	}

	unsigned int index() const
	{
		return m_index;
	}
};

class VertexArray : public GLObject<VertexArray>
{
private:
	friend class GLObject<VertexArray>;
	static void create(GLsizei count, GLuint* handles)
	{
		glCreateVertexArrays(count, handles);
	}
	static void destroy(GLsizei count, GLuint* handles)
	{
		glDeleteVertexArrays(count, handles);
	}
public:
	void bind() const
	{
		glBindVertexArray(handle);
	}

	void vertex_buffer(unsigned int binding_index, const Buffer& buffer, ptrdiff_t offset, size_t stride)
	{
		glVertexArrayVertexBuffer(handle, binding_index, buffer, offset, stride);
	}

	void element_buffer(const Buffer& buffer)
	{
		glVertexArrayElementBuffer(handle, buffer);
	}

	void enable_attrib(VertexAttribute attribute)
	{
		glEnableVertexArrayAttrib(handle, attribute.index());
	}

	void attrib_binding(VertexAttribute attribute, unsigned int binding_index)
	{
		glVertexArrayAttribBinding(handle, attribute.index(), binding_index);
	}

	void attrib_format(VertexAttribute attribute, int size, GLenum type, bool normalized, unsigned int relative_offset)
	{
		glVertexArrayAttribFormat(handle, attribute.index(), size, type, normalized, relative_offset);
	}

	void attrib_format_i(VertexAttribute attribute, int size, GLenum type, unsigned int relative_offset)
	{
		glVertexArrayAttribIFormat(handle, attribute.index(), size, type, relative_offset);
	}

	void binding_divisor(unsigned int binding_index, int divisor) const
	{
		glVertexArrayBindingDivisor(handle, binding_index, divisor);
	}
};