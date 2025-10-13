#ifndef _algorithm_
#define _algorithm_

#include<bits/stdc++.h>
#include"time.h"
using namespace std;

extern unsigned int timeLimit;

class Graph{//利用链式前向星存储图
	public:
	int vc,ec,conflicts,ctype;
	// int **adjTable;
	vector<vector<int>> adjTable;
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
	set<int> conflictVertices;//冲突顶点集合

	public:
	Graph(int vc,int ec);
	void add(int u,int v);//添加边
	void randInit(int ctype);//随机染色
	void LocalSearch();//简单局部搜索
	void read(istream &in);
};

ostream& operator<<(ostream &out,const Graph &g);

class GCP{//GCP问题核心类
	private:
	int ver_c,edg_c,rec_color;
	Graph g;

	public:
	GCP(int vc,int ec,int rec_color);
	void init(istream &in);//初始化
	void LocalSearch(int iter=10);

};

#endif