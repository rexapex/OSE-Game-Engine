#pragma once

namespace ose::rendering
{
	class FramebufferGL
	{
	public:
		FramebufferGL(int fbwidth, int fbheight)
		{
			if(fbwidth <= 0 || fbheight <= 0)
				throw std::invalid_argument("Framebuffer width/height out of bounds");

			// Create the fbo
			glGenFramebuffers(1, &fbo_);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo_);

			// Create a position buffer attachment
			glGenTextures(1, &pos_buffer_);
			glBindTexture(GL_TEXTURE_2D, pos_buffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pos_buffer_, 0);

			// Create a normal buffer attachment
			glGenTextures(1, &norm_buffer_);
			glBindTexture(GL_TEXTURE_2D, norm_buffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, norm_buffer_, 0);

			// Create a colour + specular buffer attachment
			glGenTextures(1, &col_buffer_);
			glBindTexture(GL_TEXTURE_2D, col_buffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, col_buffer_, 0);

			// Tell OpenGL which attachments to use for rendering
			GLenum attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
			glDrawBuffers(3, attachments);

			// Create the rbo for storing rendering depth info
			glGenRenderbuffers(1, &depth_rbo_);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo_);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbwidth, fbheight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_rbo_);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			// Check the framebuffer was created successfully
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
				LOG("Deferred shading framebuffer creation - SUCCESS");
			else
				LOG_ERROR("Deferred shading framebuffer creation - FAILURE -", glGetError());
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		~FramebufferGL()
		{
			if(fbo_)
				glDeleteFramebuffers(1, &fbo_);
			if(pos_buffer_)
				glDeleteTextures(1, &pos_buffer_);
			if(norm_buffer_)
				glDeleteTextures(1, &norm_buffer_);
			if(col_buffer_)
				glDeleteTextures(1, &col_buffer_);
			if(depth_rbo_)
				glDeleteRenderbuffers(1, &depth_rbo_);
		}

		void Resize(int fbwidth, int fbheight)
		{
			glBindTexture(GL_TEXTURE_2D, pos_buffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, norm_buffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glBindTexture(GL_TEXTURE_2D, col_buffer_);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, fbwidth, fbheight, 0, GL_RGB, GL_FLOAT, NULL);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_rbo_);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, fbwidth, fbheight);
		}

		GLuint GetFbo() const { return fbo_; }
		GLuint GetPosBuffer() const { return pos_buffer_; }
		GLuint GetNormBuffer() const { return norm_buffer_; }
		GLuint GetColBuffer() const { return col_buffer_; }
		GLuint GetDepthRbo() const { return depth_rbo_; }

	private:
		GLuint fbo_				{ 0 };
		GLuint pos_buffer_		{ 0 };
		GLuint norm_buffer_		{ 0 };
		GLuint col_buffer_		{ 0 };
		GLuint depth_rbo_		{ 0 };
	};
}
