#ifndef _algorithm_
#define _algorithm_

#include<bits/stdc++.h>
#include"time.h"
using namespace std;

extern unsigned int timeLimit;

class TabuTenureTable{
	private:
	int vc,ctype;
	int** table;
	public:
	void resize(int vc,int ctype);
	int& operator()(int v,int c);
	~TabuTenureTable();
};

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
	TabuTenureTable ttt;//禁忌步长表

	public:
	Graph(int vc,int ec);
	void add(int u,int v);//添加边
	void randInit(int ctype);//随机染色
	bool LocalSearch();//简单局部搜索
	void read(istream &in);
	void TabuSearch(int iter);//禁忌搜索
	bool isConflict(int ver);//判断顶点ver是否冲突
};
ostream& operator<<(ostream &out,const Graph &g);

class Solution{
	public:
	int conflicts;
	vector<int> color;
	Solution();
	Solution(Graph &g);
	Solution& operator=(const Graph &g);
};
ostream& operator<<(ostream &out,const Solution &sol);

class GCP{//GCP问题核心类
	private:
	int ver_c,edg_c,rec_color;
	Graph g;
	Solution bestSol;

	public:
	GCP(int vc,int ec,int rec_color);
	void init(istream &in);//初始化
	void LocalSearch(int iter=100);
	void TabuSearch(int iter=100);
};

#endif

/*
TS在选择禁忌移动时需不需要更新非禁忌移动的ttt

*/