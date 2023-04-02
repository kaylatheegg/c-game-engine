#include "engine.h"

/* tile layout in space

 0
3 1
 2

*/

WFCTile* generateWFCTile(int_Texture* tx, int* edges) {
	WFCTile* intTile = gmalloc(sizeof(intTile));
	memcpy(edges, intTile->edges, sizeof(int)*4);
	intTile->tx = tx;
	return intTile;
}

WFCTile* generateRotatedTile(WFCTile* tile, int shift) {
	int* edges = gmalloc(sizeof(int) * 4);
	memcpy(edges, tile->edges, sizeof(int) * 4);
	int copiedEdges[4] = {0};
	for (int i = 0; i < shift; i++) {
		copiedEdges[0] = edges[3];
		copiedEdges[1] = edges[2];
		copiedEdges[2] = edges[1];
		copiedEdges[3] = edges[0];
		for (int j = 0; j < 4; j++) {
			edges[i] = copiedEdges[i];
		}
	}
	WFCTile* intTile = generateWFCTile(tile->tx, edges);
	gfree(edges);
	return intTile;
}

int generateWFCWorld() {
	WFCTile* intTile = generateWFCTile(getTexture("amogus"), (int[]){0, 0, 1, 0});
	UNUSED(intTile);

	WFCTile** tileArray = gmalloc(sizeof(*tileArray) * 32);
	for (int i = 0; i < 32; i++) {
		tileArray[i] = gmalloc(sizeof(**tileArray) * 32);
	}

	return 0;
}