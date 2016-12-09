#pragma once
class Time {
public:
	static bool initialized;
	static unsigned long long frameCount, smoothedDeltaTimeFrameCount;
	static double deltaTime, time, framesPerSecond, smoothedDeltaTime;
};