#include "graph.h"

int main(int argc, char *argv[]) {
	Graph *graph = new Graph(argv[1]);
	if(argc < 3) {
		cout << "Usage: " << argv[0] << " [input-file] [k]\n";
		return -1;
	}
	graph->k_edge_connected_component((unsigned int)atoi(argv[2]));
	cout << "--------------" << endl;
	return 0;
}

