#ifndef _algorithm_
#define _algorithm_

#include<bits/stdc++.h>
#include"time.h"
using namespace std;

extern unsigned int timeLimit;
#define GenerationSize 20
#define TabuSearchIter 100
// #define DEBUG
#define DEBUG_CHOSEN_CRITICAL_ONE_MOVE

// class TabuTenureTable{
// 	private:
// 	int vc,ctype;
// 	int** table;
// 	public:
// 	TabuTenureTable();
// 	TabuTenureTable(const TabuTenureTable &b);
// 	TabuTenureTable(TabuTenureTable &&b);
// 	TabuTenureTable& operator=(const TabuTenureTable &b);
// 	TabuTenureTable& operator=(TabuTenureTable &&b);
// 	void resize(int vc,int ctype);
// 	int& operator()(int v,int c);
// 	~TabuTenureTable();
// };

class Graph{//利用链式前向星存储图
	public:
	int vc,ec;
	struct Edge{
		int to,nxt;
	};
	vector<bool> vis;
	vector<int> head;
	vector<Edge> edges;

	Graph(int vc,int ec);
	void add(int u,int v);//添加边
	void read();
	friend ostream& operator<<(ostream &out,const Graph &g);
};

class Solution{
	public:
	Graph *g;
	int conflicts,ctype;
	// int **adjTable;
	vector<int> color;
	vector<vector<int>> adjTable;
	set<int> conflictVertices;//冲突顶点集合
	vector<vector<int>> ttt;//禁忌步长表
	vector<int> standardizedColorTable;//规范颜色表
	// vector<bool> isMinVerofColorSet;//顶点是否是对应着色集的最小元
	vector<int> minVerofColorSet;//每种颜色对应的最小元顶点编号

	public:
	Solution();
	~Solution()=default;
	Solution(const Solution &b)=default;
	Solution(Solution &&b)=default;
	void bind(Graph& g);
	void InitConflicts();//初始化冲突等成员变量
	void randInit(int ctype);//随机染色
	bool LocalSearch();//简单局部搜索
	void TabuSearch(int iter,int bestEver);//禁忌搜索
	bool isConflict(int ver);//判断顶点ver是否冲突
	void InitStandardize();//颜色规范表初始化

	#ifdef DEBUG
	bool Check();//检查属性是否正确
	#endif

	friend ostream& operator<<(ostream &out,const Solution &sol);
	Solution& operator=(const Solution &b)=default;
	Solution& operator=(Solution &&b)=default;
};
Solution crossover(const Solution &a, const Solution &b);


// class Solution{
// 	public:
// 	int conflicts;
// 	vector<int> color;
// 	Solution();
// 	Solution(Graph &g);
// 	Solution& operator=(const Graph &g);
// };


class GCP{//GCP问题核心类
	private:
	int ver_c,edg_c,rec_color;
	Graph g;
	Solution generations[GenerationSize];
	Solution *bestSol,*worstSol;

	public:
	GCP(int vc,int ec,int rec_color);
	void init();//初始化
	void LocalSearch(int iter=100);
	void TabuSearch(int iter=0);
	void HybridEvolutionary(int iter=100);
};

#endif

/*
考虑轮换对称性：
将顶点编号与颜色编号排序对应

构建规范颜色表：
对每种颜色，记录其出现的最小的定点编号，再按此编号顺序重新排序颜色
c1: v1 v2 
c2: v4 v5
c3: v3 v6
*/