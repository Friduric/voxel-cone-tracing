#include "Time.h"
bool Time::initialized = false;
unsigned long long Time::frameCount = 0, Time::smoothedDeltaTimeFrameCount = 100;
double Time::deltaTime = 0, Time::framesPerSecond = 1, Time::time = 0, Time::smoothedDeltaTime = 0;