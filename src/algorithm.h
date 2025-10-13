#ifndef _algorithm_
#define _algorithm_

#include<bits/stdc++.h>
using namespace std;
class Graph{
	private:
	int vc,ec;
	struct Vertex{
		int head;
		int color;
	};
	struct Edge{
		int to,nxt;
	};
	vector<bool> vis;
	vector<Vertex> vertices;
	vector<Edge> edges;
	public:
	Graph(int vc,int ec);
	void add(int u,int v);
};

#endif