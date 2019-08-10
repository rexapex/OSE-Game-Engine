#pragma once

namespace ose::game
{
	class Time
	{
	friend class Game;								//Allows Game to access private data members, required for calling update

	public:
		Time();
		~Time();
		Time(Time & t) = delete;
		Time & operator=(Time & t) = delete;
		Time(Time && t) noexcept = default;
		Time & operator=(Time && t) noexcept = default;

		//publically available accessor methods
		//get the current time (in seconds)
		const double GetCurrentTime() const { return current_time_seconds_; }

		//get the delta time (in seconds), i.e. number of seconds since last frame
		const double GetDeltaTime() const { return delta_time_seconds_; }

		//get the frames per second (fps)
		const int GetFps() const { return frames_per_second_; }

		//get the number of milliseconds it took to render the current frame (millis per frame)
		const double GetMpf() const { return millis_per_frame_; }

	private:
		//all things timing
		double current_time_seconds_;					//The current system time in seconds
		double last_time_seconds_;						//The system time of the last frame in seconds
		double last_time_seconds_per_second_;			//The system time of the last second in seconds (i.e. the timestamp of the last fps update)
		double millis_per_frame_;						//The number of milliseconds it takes to render one frame
		int num_frames_;								//The counter for the number of frames rendered in the next (this) second
		int frames_per_second_;							//The number of frames rendered in the last second
		double delta_time_seconds_;

		void Init(const double current_time_seconds);	//Set the initial values of the timing variables
		void Update(const double current_time_seconds);
		void CalcDeltaTime();							//Calculates and returns the delta time in seconds
		void CalcFPS();									//Calculates the fps and the mpf
	};
}

