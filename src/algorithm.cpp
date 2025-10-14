#include"algorithm.h"
Graph::Graph(int vc,int ec):vc(vc),ec(ec),conflicts(0){
	vis.resize(vc,false);
	vertices.resize(vc,{-1,-1});
}

void Graph::add(int u,int v){
	edges.push_back({v,vertices[u].head});
	vertices[u].head=edges.size()-1;
}

void Graph::randInit(int ctype){
	this->ctype=ctype;
	adjTable.resize(vc);
	for(auto &i:adjTable) i.resize(ctype,0);
	ttt.resize(vc,ctype);
	for(auto &v:vertices){
		v.color=rand()%ctype;
	}
	conflicts=0;
	vis.assign(vc,false);
	for(int v=0;v<vc;v++){
		for(int i=vertices[v].head;i!=-1;i=edges[i].nxt){//遍历所有v的出边
			int to=edges[i].to;
			adjTable[v][vertices[to].color]++;//初始化邻接表
			if(vis[to]) continue;
			if(vertices[v].color==vertices[to].color){
				conflicts++;
				conflictVertices.insert(v);
				conflictVertices.insert(to);
			}
		}
		vis[v]=true;
	}
}

GCP::GCP(int vc,int ec,int rec_color):g(vc,ec),ver_c(vc),edg_c(ec),rec_color(rec_color){}

void GCP::init(istream &in){
	g.read(in);
}

void Graph::read(istream &in){
	for(int i=0;i<ec;i++){
		int u,v;
		in>>u>>v;
		add(u,v);
		add(v,u);
	}
}

ostream& operator<<(ostream &out,const Graph &g){
	out<<"Graph: \n";
	for(int i=0;i<g.vc;i++){
		out<<"Vertex "<<i<<": color="<<g.vertices[i].color<<" edges: ";
		for(int j=g.vertices[i].head;j!=-1;j=g.edges[j].nxt){
			out<<g.edges[j].to<<" ";
		}
		out<<"\n";
	}

	out<<"adjTable:\n";
	for(int i=0;i<g.vc;i++){
		out<<"Vertex "<<i<<": ";
		for(auto j:g.adjTable[i]) out<<j<<" ";
		out<<"\n";
	}

	out<<"conflicts:\n";
	for(auto i:g.conflictVertices) out<<i<<" ";
	out<<"\n";

	return out;
}

bool Graph::LocalSearch(){
	int minDelta=1,mver=-1,mfrom=-1,mto=-1;
	for(auto v:conflictVertices){//寻找最优移动
		for(int c=0;c<ctype;c++){
			if(c==vertices[v].color) continue;
			int delta=adjTable[v][c]-adjTable[v][vertices[v].color];
			if(delta<minDelta){
				minDelta=delta;
				mver=v;
				mfrom=vertices[v].color;
				mto=c;
			}
		}
	}
	if(minDelta<=0){//更新邻接表和冲突顶点
		conflicts+=minDelta;
		vertices[mver].color=mto;
		for(int i=vertices[mver].head;i!=-1;i=edges[i].nxt){
			int to=edges[i].to;
			adjTable[to][mfrom]--;
			adjTable[to][mto]++;
			if(isConflict(to)){
				conflictVertices.insert(to);
			}else{
				conflictVertices.erase(to);
			}
		}
		if(isConflict(mver)){
			conflictVertices.insert(mver);
		}else{
			conflictVertices.erase(mver);
		}
		return true;
	}else{
		return false;
	}
}

void GCP::LocalSearch(int iter){
	g.randInit(rec_color);
	bestSol=g;
	auto start=Timestamp::getTimestampMs();
	int i;
	for(i=0;i<iter;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		bool improved=g.LocalSearch();
		if(bestSol.conflicts>g.conflicts){
			bestSol=g;
		}
		if(!improved) break;
		if(g.conflicts==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"iterations: "<<i+1<<endl;
	cout<<bestSol;
}

void Graph::TabuSearch(int iter){
	int minDelta=1,mver=-1,mfrom=-1,mto=-1;
	int tminDelta=1,tmver=-1,tmfrom=-1,tmto=-1;
	for(auto v:conflictVertices){//寻找最优移动
		for(int c=0;c<ctype;c++){
			if(c==vertices[v].color) continue;
			int delta=adjTable[v][c]-adjTable[v][vertices[v].color];

			if(ttt(v,c)>iter){//禁忌
				if(delta<tminDelta){
					tminDelta=delta;
					tmver=v;
					tmfrom=vertices[v].color;
					tmto=c;
				}
			}else{
				if(delta<minDelta){
					minDelta=delta;
					mver=v;
					mfrom=vertices[v].color;
					mto=c;
				}
			}
		}
	}
	if(tminDelta<minDelta){//满足禁忌条件
		minDelta=tminDelta;
		mver=tmver;
		mfrom=tmfrom;
		mto=tmto;
	}
	//更新邻接表和冲突顶点以及禁忌表
	conflicts+=minDelta;
	vertices[mver].color=mto;
	for(int i=vertices[mver].head;i!=-1;i=edges[i].nxt){
		int to=edges[i].to;
		adjTable[to][mfrom]--;
		adjTable[to][mto]++;
		if(isConflict(to)){
			conflictVertices.insert(to);
		}else{
			conflictVertices.erase(to);
		}
	}
	if(isConflict(mver)){
		conflictVertices.insert(mver);
	}else{
		conflictVertices.erase(mver);
	}
	ttt(mver,mfrom)=iter+rand()%10+conflicts;//更新禁忌表
}

void GCP::TabuSearch(int iter){
	g.randInit(rec_color);
	bestSol=g;
	auto start=Timestamp::getTimestampMs();
	int i;
	for(i=0;i<iter;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		g.TabuSearch(i);
		if(bestSol.conflicts>g.conflicts){
			bestSol=g;
		}
		if(g.conflicts==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"iterations: "<<i+1<<endl;
	cout<<bestSol;
}

bool Graph::isConflict(int ver){
	return adjTable[ver][vertices[ver].color]>0;
}

void TabuTenureTable::resize(int vc,int ctype){
	table.resize(vc);
	for(auto &i:table) i.resize(ctype,0);
}

int& TabuTenureTable::operator()(int v,int c){
	return table[v][c];
}

Solution::Solution(Graph &g):conflicts(g.conflicts){
	for(auto v:g.vertices){
		color.push_back(v.color);
	}
}

Solution& Solution::operator=(const Graph &g){
	conflicts=g.conflicts;
	color.clear();
	for(auto v:g.vertices){
		color.push_back(v.color);
	}
	return *this;
}

Solution::Solution():conflicts(0){}

ostream& operator<<(ostream &out,const Solution &sol){
	out<<"conflicts: "<<sol.conflicts<<"\ncolors:\n";
	for(auto c:sol.color) out<<c<<"\n";
	return out;
}