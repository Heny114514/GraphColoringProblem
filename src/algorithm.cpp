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
	for(auto &i:adjTable) i.resize(ctype);
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

void Graph::LocalSearch(){
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
	if(minDelta<=0){//更新邻接表
		conflicts+=minDelta;
		vertices[mver].color=mto;
		for(int i=vertices[mver].head;i!=-1;i=edges[i].nxt){
			int to=edges[i].to;
			adjTable[to][mfrom]--;
			adjTable[to][mto]++;
		}
	}
}

void GCP::LocalSearch(int iter){
	g.randInit(rec_color);
	auto start=Timestamp::getTimestampMs();
	for(int i=0;i<iter;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		g.LocalSearch();
		if(g.conflicts==0) break;
	}
	cout<<"conflicts: "<<g.conflicts<<endl;
	for(auto v:g.vertices){
		cout<<v.color<<endl;
	}
}