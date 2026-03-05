#include "components/perfOverlay.hpp"
#include "raylib.h"
#include <algorithm>
PerformanceOverlay::PerformanceOverlay() {
	for (int i = 0; i < SAMPLE_COUNT; i++)
		samples[i] = 0;

	index = 0;

	minTime = 0;
	avgTime = 0;
	maxTime = 0;

	graphMax = 20.0f;
	spikeThreshold = 1.5f;
}

void PerformanceOverlay::ComputeStats() {
	minTime = 9999;
	maxTime = 0;
	avgTime = 0;

	for (int i = 0; i < SAMPLE_COUNT; i++) {
		float v = samples[i];

		minTime = std::min(minTime, v);
		maxTime = std::max(maxTime, v);

		avgTime += v;
	}

	avgTime /= SAMPLE_COUNT;
}

void PerformanceOverlay::UpdateScaling() {
	float target = std::max(maxTime * 1.2f, 16.67f);

	// Smooth scale interpolation
	graphMax = graphMax * 0.9f + target * 0.1f;
}

void PerformanceOverlay::DrawFPSLine(float ms, int x, int y, int w, int h,
									 Color color) {
	if (ms > graphMax)
		return;

	float norm = ms / graphMax;
	float py = y + h - norm * h;

	DrawLine(x, py, x + w, py, color);
}

void PerformanceOverlay::Update() {
	float ms = GetFrameTime() * 1000.0f;

	samples[index] = ms;
	index = (index + 1) % SAMPLE_COUNT;

	ComputeStats();
	UpdateScaling();
}

void PerformanceOverlay::Draw(int x, int y, int w, int h) {
	if (!enabled)
		return;
	DrawRectangle(x, y, w, h, Fade(BLACK, 0.7f));
	DrawRectangleLines(x, y, w, h, GRAY);

	Vector2 prev = {0};

	for (int i = 0; i < SAMPLE_COUNT; i++) {
		int idx = (index + i) % SAMPLE_COUNT;

		float value = samples[idx];
		float norm = value / graphMax;

		if (norm > 1.0f)
			norm = 1.0f;

		float px = x + ((float)i / (SAMPLE_COUNT - 1)) * w;
		float py = y + h - norm * h;

		Vector2 cur = {px, py};

		Color color = GREEN;

		if (value > avgTime * spikeThreshold)
			color = RED;

		if (i > 0) {
			DrawLineV(prev, cur, color);

			DrawTriangle({prev.x, y * 1.f + h}, {prev.x, prev.y},
						 {cur.x, cur.y}, Fade(color, 0.15f));

			DrawTriangle({prev.x, y * 1.f + h}, {cur.x, cur.y},
						 {cur.x, y * 1.f + h}, Fade(color, 0.15f));
		}

		prev = cur;
	}

	DrawFPSLine(16.67f, x, y, w, h, RED);	// 60 FPS
	DrawFPSLine(8.33f, x, y, w, h, ORANGE); // 120 FPS

	DrawText(TextFormat("FPS: %d", GetFPS()), x, y - 18, 20, WHITE);

	DrawLegend(x + w + 10, y);
}

void PerformanceOverlay::Toggle() { enabled = !enabled; }

bool PerformanceOverlay::IsEnabled() const { return enabled; }

void PerformanceOverlay::DrawLegend(int x, int y) {
	DrawRectangle(x, y, 210, 80, Fade(BLACK, 0.7f));
	DrawRectangleLines(x, y, 210, 80, GRAY);

	DrawLine(x + 10, y + 15, x + 30, y + 15, GREEN);
	DrawText("Frame Time", x + 35, y + 8, 10, WHITE);

	DrawLine(x + 10, y + 30, x + 30, y + 30, RED);
	DrawText("Spike / 60 FPS", x + 35, y + 23, 10, WHITE);

	DrawLine(x + 10, y + 45, x + 30, y + 45, ORANGE);
	DrawText("120 FPS", x + 35, y + 38, 10, WHITE);

	DrawText(TextFormat("Min: %.2f ms", minTime), x + 10, y + 55, 10, WHITE);
	DrawText(TextFormat("Avg: %.2f ms", avgTime), x + 10, y + 65, 10, WHITE);
	DrawText(TextFormat("Max: %.2f ms", maxTime), x + 110, y + 65, 10, WHITE);
}
