#pragma once
#define RENDER_RENDER_H

int frameCount;

int render();

Rect viewport;

int renderedObjects;

float* renderVertices[MAX_RENDER_LAYERS];
int* renderElements[MAX_RENDER_LAYERS];
int renderCounts[MAX_RENDER_LAYERS];