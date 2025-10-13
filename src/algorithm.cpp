#include"algorithm.h"
Graph::Graph(int vc,int ec):vc(vc),ec(ec){
	vis.resize(vc,false);
	vertices.resize(vc,{-1,-1});
}

void Graph::add(int u,int v){
	edges.push_back({v,vertices[u].head});
	vertices[u].head=edges.size()-1;
}