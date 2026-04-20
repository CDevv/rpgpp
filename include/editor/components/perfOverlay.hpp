#ifndef _RPGPP_PERFOVERLAY_H
#define _RPGPP_PERFOVERLAY_H
#include "raylib.h"

class PerformanceOverlay {
public:
	PerformanceOverlay();

	void Update();
	void Draw(int x, int y, int w = 420, int h = 120);

	void Toggle();
	bool IsEnabled() const;

private:
	bool enabled{false};
	static const int SAMPLE_COUNT = 240;

	float samples[SAMPLE_COUNT];
	int index;

	float minTime;
	float avgTime;
	float maxTime;

	float graphMax;
	float spikeThreshold;

	void ComputeStats();
	void UpdateScaling();
	void DrawFPSLine(float ms, int x, int y, int w, int h, Color color);
	void DrawLegend(int x, int y);
	;
};
#endif
