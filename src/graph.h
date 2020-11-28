#pragma once

#include "../utils/def.h"
#include "LinearHeap.h"

struct Edge
{
	ul vertex;
	Edge *previous, *next, *reverse;
};

class Graph
{
private:
	string dir;
	ul n, m, *pstart, *edges;

public:
	explicit Graph(const char *dir);
	~Graph();
	void k_edge_connected_component(ul K);
	void read_graph_binary();
	void k_core_prune(ul K, ul *Q, ul Q_n, char *computed, ul *degree, ul *p_end);
	void print_kecc(ul K, ul c_n, ul *cstart, ul *ids);
	void construct_pgraph(ul s, ul *Q, char *vis, char *computed, ul *p_end, Edge **graph_head, Edge *graph_edges, ul *sv_next, ul *sv_last);
	ul decomposition(ul s, ul K, ul *cstart, ul *ids, ul c_n, ul *Q, ul *keys, char *vis, 
								Edge **graph_head, Edge *graph_edges, LinearHeap *heap, ul *sv_next, ul *sv_last);
	void merge(Edge **graph_head, ul u, ul v, ul *keys, ul *sv_next, ul *sv_last);
	void add_edge(ul u, Edge *e, Edge **graph_head);
	void delete_edge(ul u, Edge *e, Edge **graph_head);
	void remove_inter_edges(ul K, ul c_n, ul new_cn, ul *cstart, ul *ids, ul *component_id, ul *p_end, ul *Q, char *computed, ul *degree);
};
