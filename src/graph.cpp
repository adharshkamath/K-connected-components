#include "graph.h"

using namespace std;

Graph::Graph(const char *filename) : input_file(string(filename)), m(0), n(0), pstart(nullptr), edges(nullptr) {}

Graph::~Graph()
{
	if (pstart != nullptr)
	{
		delete[] pstart;
		pstart = nullptr;
	}
	if (edges != nullptr)
	{
		delete[] edges;
		edges = nullptr;
	}
}

void Graph::k_edge_connected_component(ul K)
{
	if (K < 2)
	{
		cout << "K must be at least 2!" << endl;
		return;
	}
	read_file();
	auto start = chrono::high_resolution_clock::now();
	ul *pend = new ul[n];
	for (ul i = 0; i < n; i++)
	{
		pend[i] = pstart[i + 1];
	}
	int *computed = new int[n];
	memset(computed, 0, sizeof(int) * n);
	ul *degree = new ul[n];
	ul *Q = new ul[n];
	ul Q_n = 0;

	for (ul i = 0; i < n; i++)
	{
		degree[i] = pend[i] - pstart[i];
		if (degree[i] < K)
		{
			Q[Q_n++] = i;
			computed[i] = 1;
		}
	}
	k_core_prune(K, Q, Q_n, computed, degree, pend);

	ul *ids = new ul[n];
	ul *component_start = new ul[n + 1];
	ul c_n = 0;
	component_start[0] = 0;

	Edge **graph_head = new Edge *[n];
	Edge *graph_edges = new Edge[m];
	for (ul i = 0; i < n; i++)
	{
		graph_head[i] = nullptr;
	}

	ul *sv_next = new ul[n];
	ul *sv_last = new ul[n];

	char *vis = new char[n];
	memset(vis, 0, sizeof(char) * n);

	LinearHeap *heap = new LinearHeap(n, K);

	ul *keys = new ul[n];

	for (ul i = 0; i < n;)
	{
		if (computed[i])
		{
			++i;
			continue;
		}
		create_partition_graph(i, Q, vis, computed, pend, graph_head, graph_edges, sv_next, sv_last);
		ul new_cn = decompose(i, K, component_start, ids, c_n, Q, keys, vis, graph_head, graph_edges, heap, sv_next, sv_last);
		if (new_cn == c_n + 1)
		{
			for (ul j = component_start[c_n]; j < component_start[c_n + 1]; j++)
			{
				computed[ids[j]] = 1;
			}
			++c_n;
		}
		else
		{
			remove_inner_edges(K, c_n, new_cn, component_start, ids, keys, pend, Q, computed, degree);
		}
	}

	print_components(K, c_n, component_start, ids);

	delete[] keys;
	delete heap;
	delete[] vis;
	delete[] sv_next;
	delete[] sv_last;
	delete[] graph_head;
	delete[] graph_edges;
	delete[] component_start;
	delete[] ids;
	delete[] Q;
	delete[] degree;
	delete[] computed;
	delete[] pend;

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << "\tTotal processing time excluding I/O: " << duration.count() << " (microseconds)" << endl;
}

void Graph::read_file()
{
	printf("Start reading graph, Require files \"b_degree.bin\" and \"b_adj.bin\"\n");
	vector<ul> edge, degrees;
	ul i = 0;
	n = 0;
	string line;
	ifstream myfile(input_file);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			cout << "New line" << endl;
			n++;
			cout << line << '\n';
			char *tokens = strtok(&line[0], " ");
			while (tokens != NULL)
			{
				edge.pb((ul)atoll(tokens));
				tokens = strtok(NULL, " ");
				i++;
			}
			degrees.pb(i);
		}
		myfile.close();
	}
	m = i;
	edges = new ul[m];
	for (i = 0; i < m; i++)
	{
		edges[i] = edge[i];
	}
	if (pstart != nullptr)
		delete[] pstart;
	pstart = new ul[n + 1];
	pstart[0] = 0;
	for (i = 0; i < n; i++)
	{
		pstart[i + 1] = degrees[i];
	}
}

void Graph::k_core_prune(ul K, ul *Q, ul Q_n, int *computed, ul *degree, ul *pend)
{
	for (ul i = 0; i < Q_n; i++)
	{
		ul u = Q[i];
		for (ul j = pstart[u]; j < pend[u]; j++)
			if (!computed[edges[j]])
			{
				ul v = edges[j];
				if (degree[v] == K)
				{
					Q[Q_n++] = v;
					computed[v] = 1;
				}
				--degree[v];
			}
	}
}

void Graph::create_partition_graph(ul s, ul *Q, char *vis, int *computed, ul *pend, Edge **graph_head, Edge *graph_edges, ul *sv_next, ul *sv_last)
{
	ul cnt = 0;
	Q[0] = s;
	vis[s] = 1;
	ul Q_n = 1;
	assert(!computed[s]);

	for (ul i = 0; i < Q_n; i++)
	{
		ul u = Q[i];
		sv_next[u] = sv_last[u] = u;
		for (ul j = pstart[u]; j < pend[u];)
		{
			ul v = edges[j];
			if (computed[v])
				swap(edges[j], edges[--pend[u]]);
			else
			{
				if (!vis[v])
				{
					Q[Q_n++] = v;
					vis[v] = 1;
				}

				if (v > u)
				{
					graph_edges[cnt].vertex = v;
					graph_edges[cnt].reverse = &graph_edges[cnt + 1];
					add_edge(u, &graph_edges[cnt], graph_head);
					++cnt;

					graph_edges[cnt].vertex = u;
					graph_edges[cnt].reverse = &graph_edges[cnt - 1];
					add_edge(v, &graph_edges[cnt], graph_head);
					++cnt;
				}
				++j;
			}
		}
	}
	for (ul i = 0; i < Q_n; i++)
		vis[Q[i]] = 0;
}

ul Graph::decompose(ul s, ul K, ul *component_start, ul *ids, ul c_n, ul *Q, ul *keys, char *vis, Edge **graph_head, Edge *graph_edges, LinearHeap *heap, ul *sv_next, ul *sv_last)
{
	while (graph_head[s] != nullptr)
	{
		heap->init(0, K, nullptr, nullptr);
		heap->insert(s, 0);
		ul u, key, Q_n = 0;
		while (heap->extract_max(u, key))
		{
			Q[Q_n++] = u;
			vis[u] = 1;
			keys[u] = key;

			ul new_Qn = Q_n;
			for (ul j = Q_n - 1; j < new_Qn; j++)
			{
				ul v = Q[j];

				for (Edge *e = graph_head[v]; e != nullptr; e = e->next)
					if (vis[e->vertex] != 1)
					{
						ul w = e->vertex;

						if (vis[w] == 3)
						{
							++keys[w];
							continue;
						}

						if (vis[w] == 2)
							key = heap->remove(w);
						else
							key = 0;
						assert(key < K);

						++key;
						if (key >= K)
						{
							Q[new_Qn++] = w;
							keys[w] = key;
							vis[w] = 3;
						}
						else
						{
							heap->insert(w, key);
							vis[w] = 2;
						}
					}

				if (v == u)
					continue;
				vis[v] = 0;
				keys[u] += keys[v];
				merge(graph_head, u, v, keys, sv_next, sv_last);
			}
		}

		while (Q_n > 0 && keys[Q[Q_n - 1]] < K)
		{
			ul u = Q[--Q_n];
			vis[u] = 0;

			for (Edge *e = graph_head[u]; e != nullptr; e = e->next)
				delete_edge(e->vertex, e->reverse, graph_head);
			graph_head[u] = nullptr;

			ul pos = component_start[c_n];
			ids[pos++] = u;
			while (sv_next[u] != u)
			{
				u = sv_next[u];
				ids[pos++] = u;
			}
			component_start[++c_n] = pos;
		}

		for (ul i = 0; i < Q_n; i++)
			vis[Q[i]] = 0;
	}
	return c_n;
}

void Graph::merge(Edge **graph_head, ul u, ul v, ul *keys, ul *sv_next, ul *sv_last)
{
	for (Edge *e = graph_head[v]; e != nullptr;)
	{
		Edge *tmp = e->next;

		if (e->vertex == u)
		{
			--keys[u];
			delete_edge(u, e->reverse, graph_head);
		}
		else
		{
			assert(e->reverse->vertex == v);
			e->reverse->vertex = u;

			add_edge(u, e, graph_head);
		}

		e = tmp;
	}
	graph_head[v] = nullptr;

	sv_next[sv_last[u]] = v;
	sv_last[u] = sv_last[v];
}

void Graph::delete_edge(ul u, Edge *e, Edge **graph_head)
{
	if (e->previous == nullptr)
	{
		assert(graph_head[u] == e);
		e = e->next;
		if (e != nullptr)
			e->previous = nullptr;
		graph_head[u] = e;
	}
	else
	{
		assert(graph_head[u] != e);
		e->previous->next = e->next;
		if (e->next != nullptr)
			e->next->previous = e->previous;
	}
}

void Graph::add_edge(ul u, Edge *e, Edge **graph_head)
{
	if (graph_head[u] != nullptr)
		graph_head[u]->previous = e;
	e->next = graph_head[u];
	graph_head[u] = e;
	e->previous = nullptr;
}

void Graph::remove_inner_edges(ul K, ul c_n, ul new_cn, ul *component_start, ul *ids, ul *component_id, ul *pend, ul *Q, int *computed, ul *degree)
{
	for (ul i = c_n; i < new_cn; i++)
		for (ul j = component_start[i]; j < component_start[i + 1]; j++)
			component_id[ids[j]] = i;

	for (ul i = c_n; i < new_cn; i++)
		for (ul j = component_start[i]; j < component_start[i + 1]; j++)
		{
			ul u = ids[j];
			for (ul k = pstart[u]; k < pend[u];)
			{
				ul v = edges[k];
				assert(!computed[v]);
				if (component_id[v] != component_id[u])
					swap(edges[k], edges[--pend[u]]);
				else
					++k;
			}
		}

	ul Q_n = 0;
	for (ul i = c_n; i < new_cn; i++)
		for (ul j = component_start[i]; j < component_start[i + 1]; j++)
		{
			ul u = ids[j];
			degree[u] = pend[u] - pstart[u];
			if (degree[u] < K)
			{
				Q[Q_n++] = u;
				computed[u] = 1;
			}
		}

	k_core_prune(K, Q, Q_n, computed, degree, pend);
}

void Graph::print_components(ul K, ul c_n, ul *component_start, ul *ids)
{
	ofstream outfile;
	outfile.open("output.txt", ios::out | ios::trunc);

	for (ul i = 0; i < c_n; i++)
	{
		sort(ids + component_start[i], ids + component_start[i + 1]);
		for (ul j = component_start[i]; j < component_start[i + 1]; j++)
			outfile << ids[j] << " ";
		outfile << endl;
	}
	outfile.close();
}
