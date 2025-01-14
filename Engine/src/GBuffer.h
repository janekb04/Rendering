#pragma once

#include "Vendor.h"
#include "gl/Framebuffer.h"
#include "gl/Texture2dArray.h"

// Format
//  Attachment        x        y        z         w  
//	         0    pos.x    pos.y    pos.z        AO
//	         1 normal.x normal.y normal.z  metallic
//	         2 albedo.r albedo.g albedo.b roughness
class GBuffer
{
private:
	const unsigned m_width, m_height;
	Framebuffer fbo;
	Texture2dArray attachments;
	Renderbuffer depth;
private:
	constexpr static unsigned ATTACHMENT_COUNT = 3;
public:
	GBuffer(unsigned width, unsigned height) :
		m_width{width},
		m_height{height}
	{
		attachments.storage(1, GL_RGBA16F, width, height, ATTACHMENT_COUNT);
		depth.storage(GL_DEPTH_COMPONENT, width, height);

		fbo.texture_layer(GL_COLOR_ATTACHMENT0, attachments, 0, 0);
		fbo.texture_layer(GL_COLOR_ATTACHMENT1, attachments, 0, 1);
		fbo.texture_layer(GL_COLOR_ATTACHMENT2, attachments, 0, 2);
		fbo.renderbuffer(GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

		GLenum buffers[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		fbo.draw_buffers(3, buffers);

		if (fbo.check_status(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			throw std::runtime_error("framebuffer is not complete");
	}

	void bind(GLenum target)
	{
		fbo.bind(target);
	}

	unsigned width() const
	{
		return m_width;
	}

	unsigned height() const
	{
		return m_height;
	}

	const Texture2dArray& get_texture() const
	{
		return attachments;
	}
};