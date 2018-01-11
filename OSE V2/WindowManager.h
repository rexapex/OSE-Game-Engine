#pragma once

#include "stdafx.h"
#include "VideoMode.h"

namespace origami_sheep_engine
{
	class WindowManager
	{
	public:
		WindowManager();
		virtual ~WindowManager() noexcept;
		WindowManager(WindowManager &) = delete;
		WindowManager(WindowManager &&) = delete;
		WindowManager & operator=(WindowManager &) = delete;
		WindowManager & operator=(WindowManager &&) = delete;

		virtual std::vector<VideoMode> getAvailableVideoModes() = 0;

		virtual int		setMouseVisibility(int value) = 0;

		virtual void	setWindowSize(int width, int height) = 0;
		virtual void	setWindowPos(int x, int y) = 0;

		virtual void	setTitle(const std::string & title) = 0;

		virtual void	setNumSamples(int numSamples) = 0;

		virtual void	createWindow(int windowMode, int video_mode = -1) = 0;

		virtual void	update() = 0;

		virtual double  getTimeSeconds() const = 0;
	private:
		virtual int	initWindowingToolkit() const = 0;
	};
}

