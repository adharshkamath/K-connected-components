#pragma once

#include "defines.h"
#include "LinearHeap.h"

struct Edge
{
	ul vertex;
	Edge *previous, *next, *reverse;
};

class Graph
{
private:
	string input_file;
	ul n, m, *pstart, *edges;

public:
	explicit Graph(const char *dir);
	~Graph();
	void k_edge_connected_component(ul K);
	void read_file();
	void k_core_prune(ul K, ul *Q, ul Q_n, int *computed, ul *degree, ul *p_end);
	void print_components(ul K, ul c_n, ul *cstart, ul *ids);
	void create_partition_graph(ul s, ul *Q, char *vis, int *computed, ul *p_end, Edge **graph_head, Edge *graph_edges, ul *sv_next, ul *sv_last);
	ul decompose(ul s, ul K, ul *cstart, ul *ids, ul c_n, ul *Q, ul *keys, char *vis,
				 Edge **graph_head, Edge *graph_edges, LinearHeap *heap, ul *sv_next, ul *sv_last);
	void merge(Edge **graph_head, ul u, ul v, ul *keys, ul *sv_next, ul *sv_last);
	void add_edge(ul u, Edge *e, Edge **graph_head);
	void delete_edge(ul u, Edge *e, Edge **graph_head);
	void remove_inner_edges(ul K, ul c_n, ul new_cn, ul *cstart, ul *ids, ul *component_id, ul *p_end, ul *Q, int *computed, ul *degree);
};
