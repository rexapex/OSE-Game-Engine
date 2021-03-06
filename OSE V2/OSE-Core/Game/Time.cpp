#include "stdafx.h"
#include "Time.h"

namespace ose
{
	Time::Time() {}

	Time::~Time() {}

	void Time::Init(double current_time_seconds)	//Set the initial values of the timing variables
	{
		num_frames_ = frames_per_second_ = 0;
		millis_per_frame_ = 0.0;
		current_time_seconds_ = last_time_seconds_per_second_ = last_time_seconds_ = current_time_seconds;
	}

	void Time::Update(double current_time_seconds)
	{
		current_time_seconds_ = current_time_seconds;
		CalcDeltaTime();
	}

	void Time::CalcFPS()
	{
		num_frames_++;
		if(current_time_seconds_ - last_time_seconds_per_second_ >= 1.0)
		{
			frames_per_second_ = num_frames_;
			millis_per_frame_ = 1000.0/double(num_frames_);
			num_frames_ = 0;
			last_time_seconds_per_second_ += 1.0;
		}
	}

	void Time::CalcDeltaTime()	//Calculates and returns the delta time in seconds
	{
		delta_time_seconds_ = current_time_seconds_ - last_time_seconds_;			//Calculate the time passed between frames
		CalcFPS();
		last_time_seconds_ = current_time_seconds_;
	}
}
