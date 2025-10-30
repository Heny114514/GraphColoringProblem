#include"algorithm.h"
Graph::Graph(int vc,int ec):vc(vc),ec(ec){
	vis.resize(vc,false);
	head.resize(vc,-1);
}
	
void Graph::add(int u,int v){
	edges.push_back({v,head[u]});
	head[u]=edges.size()-1;
}

// void Solution::randInit(int ctype){
// 	this->ctype=ctype;
// 	color.resize(g->vc);
// 	maxColor.resize(g->vc,0);
// 	// Colormax.resize(g->vc,0);
// 	// adjTable.resize(g->vc);
// 	// for(auto &i:adjTable) i.resize(ctype,0);
// 	// ttt.resize(g->vc,ctype);
// 	color[0]=maxColor[0]=0;
// 	for(int i=1;i<g->vc;i++){
// 		color[i]=rand()%min(maxColor[i-1]+2,ctype);
// 		maxColor[i]=max(maxColor[i-1],color[i]);
// 	}
// 	// Colormax[g->vc-1]=color[g->vc-1];
// 	// for(int i=g->vc-2;i>=0;i--){
// 	// 	Colormax[i]=max(Colormax[i+1],color[i]);
// 	// }
// 	// conflicts=0;
// 	// g->vis.assign(g->vc,false);
// 	// for(int v=0;v<g->vc;v++){
// 	// 	for(int i=g->head[v];i!=-1;i=g->edges[i].nxt){//遍历所有v的出边
// 	// 		int to=g->edges[i].to;
// 	// 		adjTable[v][color[to]]++;//初始化邻接表
// 	// 		if(g->vis[to]) continue;
// 	// 		if(color[v]==color[to]){
// 	// 			conflicts++;
// 	// 			conflictVertices.insert(v);
// 	// 			conflictVertices.insert(to);
// 	// 		}
// 	// 	}
// 	// 	g->vis[v]=true;
// 	// }
// 	InitConflicts();
// }

void Solution::randInit(int ctype){//贪心构造
    if(!g) return;
    this->ctype = ctype;
    int vcnt = g->vc;
    if(vcnt <= 0) return;
    // if requested more colors than vertices, clamp
    if(ctype > vcnt) ctype = vcnt, this->ctype = ctype;

    color.assign(vcnt, -1);
    maxColor.assign(vcnt, -1);
    // greedy: introduce new colors in ascending order when allowed
    int nextNew = 0; // next new color to introduce (0..ctype-1)
    // iterate vertices in order 0..vc-1
    for(int v = 0; v < vcnt; ++v){
        int prevMax = (v == 0) ? -1 : maxColor[v-1];
        int allowMax = std::min(prevMax + 1, ctype - 1);
        int chosen = -1;

        // if we still need to introduce nextNew and it's allowed now, do it
        if(nextNew <= allowMax){
            chosen = nextNew;
            ++nextNew;
        } else {
            // choose color in [0..allowMax] that minimizes conflicts with already assigned neighbors
            int best = -1;
            int bestConf = INT_MAX;
            // evaluate each candidate
            for(int c = 0; c <= allowMax; ++c){
                int conf = 0;
                // count neighbors already assigned with same color
                for(int e = g->head[v]; e != -1; e = g->edges[e].nxt){
                    int u = g->edges[e].to;
                    if(u < v && color[u] == c) ++conf;
                }
                if(conf < bestConf){
                    bestConf = conf;
                    best = c;
                } else if(conf == bestConf && best != -1){
                    // tie-break randomly
                    if((rand() & 1) == 0) best = c;
                }
            }
            if(best == -1) best = 0;
            chosen = best;
        }

        color[v] = chosen;
        maxColor[v] = std::max(prevMax, color[v]);
    }
    // ensure all colors introduced (should hold when vcnt >= original ctype)
    // final safety: if nextNew < ctype, force-introduce remaining colors from the end where allowed
    for(int c = nextNew; c < ctype; ++c){
        // find a position from last to first where we can set color to c without violating constraint
        bool placed = false;
        for(int v = vcnt - 1; v >= 0; --v){
            int prevMax = (v == 0) ? -1 : maxColor[v-1];
            int allowMax = std::min(prevMax + 1, ctype - 1);
            if(c <= allowMax){
                // replace color at v with c, update maxColor from v..end
                color[v] = c;
                // recompute maxColor from v to end
                for(int i = v; i < vcnt; ++i){
                    if(i == 0) maxColor[i] = color[i];
                    else maxColor[i] = std::max(maxColor[i-1], color[i]);
                }
                placed = true;
                break;
            }
        }
        if(!placed){
            // shouldn't happen if vcnt >= ctype, but fallback: leave as is
            break;
        }
    }

	InitConflicts();
}

void Solution::InitConflicts(){
	adjTable.resize(g->vc);
	for(auto &i:adjTable) i.resize(ctype,0);
	// ttt.resize(g->vc,ctype);
	ttt.resize(g->vc);
	for(auto &i:ttt) i.resize(ctype,0);
	conflicts=0;
	conflictVertices.clear();
	g->vis.assign(g->vc,false);
	for(int v=0;v<g->vc;v++){
		for(int i=g->head[v];i!=-1;i=g->edges[i].nxt){//遍历所有v的出边
			int to=g->edges[i].to;
			adjTable[v][color[to]]++;//初始化邻接表
			if(g->vis[to]) continue;
			if(color[v]==color[to]){
				conflicts++;
				conflictVertices.insert(v);
				conflictVertices.insert(to);
			}
		}
		g->vis[v]=true;
	}
}

GCP::GCP(int vc,int ec,int rec_color):g(vc,ec),ver_c(vc),edg_c(ec),rec_color(rec_color){
	for(int i=0;i<GenerationSize;i++){
		generations[i].bind(g);
	}
	bestSol=nullptr;
	worstSol=nullptr;
}

void Solution::bind(Graph& g){
	this->g=&g;
}

void GCP::init(){
	g.read();
	// for(int i=0;i<GenerationSize;i++){
	// 	generations[i].bind(g);
	// }
}

void Graph::read(){
	for(int i=0;i<ec;i++){
		int u,v;
		cin>>u>>v;
		add(u,v);
		add(v,u);
	}
}

ostream& operator<<(ostream &out,const Graph &g){
	out<<"Graph: \n";
	for(int i=0;i<g.vc;i++){
		out<<"Vertex "<<i<<" edges: ";
		for(int j=g.head[i];j!=-1;j=g.edges[j].nxt){
			out<<g.edges[j].to<<" ";
		}
		out<<"\n";
	}

	// out<<"adjTable:\n";
	// for(int i=0;i<g.vc;i++){
	// 	out<<"Vertex "<<i<<": ";
	// 	for(auto j:g.adjTable[i]) out<<j<<" ";
	// 	out<<"\n";
	// }

	// out<<"conflicts:\n";
	// for(auto i:g.conflictVertices) out<<i<<" ";
	// out<<"\n";

	return out;
}

bool Solution::LocalSearch(){
	int minDelta=1,mver=-1,mfrom=-1,mto=-1;
	for(auto v:conflictVertices){//寻找最优移动
		for(int c=0;c<ctype;c++){
			if(c==color[v]) continue;
			int delta=adjTable[v][c]-adjTable[v][color[v]];
			if(delta<minDelta){
				minDelta=delta;
				mver=v;
				mfrom=color[v];
				mto=c;
			}
		}
	}
	if(minDelta<=0){//更新邻接表和冲突顶点
		conflicts+=minDelta;
		color[mver]=mto;
		for(int i=g->head[mver];i!=-1;i=g->edges[i].nxt){
			int to=g->edges[i].to;
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
	generations[0].randInit(rec_color);
	bestSol=generations;
	auto start=Timestamp::getTimestampMs();
	int i;
	for(i=0;i<iter;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		bool improved=generations[0].LocalSearch();
		if(bestSol->conflicts>generations[0].conflicts){
			bestSol=generations;
		}
		if(!improved) break;
		if(generations[0].conflicts==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"iterations: "<<i+1<<endl;
	cout<<*bestSol;
}

void Solution::TabuSearch(int iter,int bestEver){
	int minDelta=1,mver=-1,mfrom=-1,mto=-1;
	int tminDelta=1,tmver=-1,tmfrom=-1,tmto=-1;
	for(auto v:conflictVertices){//寻找最优移动
		if(v==0||color[v]==maxColor[v-1]+1) continue;
		for(int c=0;c<min(ctype,maxColor[v]+2);c++){
		// for(int c=0;c<ctype;c++){
			if(c==color[v]) continue;
			// if(c>maxColor[v]) continue;
			int delta=adjTable[v][c]-adjTable[v][color[v]];

			if(ttt[v][c]>iter){//禁忌
				if(delta<tminDelta){
					tminDelta=delta;
					tmver=v;
					tmfrom=color[v];
					tmto=c;
				}
			}else{
				if(delta<minDelta){
					minDelta=delta;
					mver=v;
					mfrom=color[v];
					mto=c;
				}
			}
		}
	}
	if(tminDelta<minDelta&&conflicts-tminDelta<bestEver){//满足禁忌条件
		minDelta=tminDelta;
		mver=tmver;
		mfrom=tmfrom;
		mto=tmto;
	}
	// If no move was found (e.g. conflictVertices empty), mver will still be -1.
	// Protect against using invalid indices which causes out-of-bounds writes.
	if(mver==-1){
		return;
	}
	//更新邻接表和冲突顶点以及禁忌表
	conflicts+=minDelta;
	color[mver]=mto;
	for(int i=g->head[mver];i!=-1;i=g->edges[i].nxt){
		int to=g->edges[i].to;
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
	//更新maxColor
	for(int i=mver;i<g->vc;i++){
		maxColor[i]=max(maxColor[i-1],color[mver]);
	}
	ttt[mver][mfrom]=iter+rand()%10+conflicts;//更新禁忌表
}

void GCP::TabuSearch(int iter){
	generations[0].randInit(rec_color);
	bestSol=generations;
	auto start=Timestamp::getTimestampMs();
	int i;
	for(i=0;i<iter||iter==0;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		generations[0].TabuSearch(i,bestSol->conflicts);
		if(bestSol->conflicts>generations[0].conflicts){
			bestSol=generations;
		}
		if(generations[0].conflicts==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"iterations: "<<i+1<<endl;
	vector<bool> used(rec_color,false);
	for(auto c:bestSol->color) used[c]=true;
	int used_c=0;
	for(auto u:used) if(u) used_c++;
	cout<<"color types: "<<used_c<<endl;
	cout<<(*bestSol);
}

bool Solution::isConflict(int ver){
	return adjTable[ver][color[ver]]>0;
}

// TabuTenureTable::TabuTenureTable():vc(0),ctype(0),table(nullptr){}

// void TabuTenureTable::resize(int vc,int ctype){
// 	if(table){
// 		this->~TabuTenureTable();
// 	}
// 	this->vc=vc;
// 	this->ctype=ctype;
// 	table = new int*[vc];
// 	for(int i=0;i<vc;i++){
// 		table[i] = new int[ctype];
// 		for(int j=0;j<ctype;j++){
// 			table[i][j]=0;
// 		}
// 	}
// }

// TabuTenureTable::~TabuTenureTable(){
// 	for(int i=0;i<vc;i++){
// 		delete[] table[i];
// 	}
// 	delete[] table;
// }

// int& TabuTenureTable::operator()(int v,int c){
// 	return table[v][c];
// }

// Solution::Solution(const Graph* g):conflicts(0){
// 	this->g=(Graph*)g;
// }

// Solution& Solution::operator=(const Graph &g){
// 	conflicts=g.conflicts;
// 	color.clear();
// 	for(auto v:g.vertices){
// 		color.push_back(v.color);
// 	}
// 	return *this;
// }

Solution::Solution():conflicts(0),g(nullptr){}

ostream& operator<<(ostream &out,const Solution &sol){
	out<<"conflicts: "<<sol.conflicts<<"\ncolors:\n";
	for(auto c:sol.color) out<<c<<"\n";
	return out;
}

Solution crossover(const Solution &a, const Solution &b){
	Solution child;
	child.g=a.g;
	child.ctype=a.ctype;
	child.color.resize(a.g->vc);
	// Use STL containers instead of VLAs: safer and portable
	vector<char> usedColor(a.ctype, 0);
	vector< set<int> > colorV0(a.ctype), colorV1(a.ctype);
	for(int v=0;v<a.g->vc;v++){
		colorV0[a.color[v]].insert(v);
		colorV1[b.color[v]].insert(v);
	}
	for(int l=0;l<a.ctype;l++){
		// choose current side and other side
		vector< set<int> > &cur = (l%2==0) ? colorV0 : colorV1;
		vector< set<int> > &oth = (l%2==0) ? colorV1 : colorV0;
		int M = 0;
		for(int i=1;i<a.ctype;i++){
			if(cur[i].size() > cur[M].size()) M = i;
		}
		int color = -1;
		int remaining = a.ctype - l;
		if(remaining <= 0) remaining = 1; // guard
		int colorIndex = rand() % remaining;
		for(int i=0;i<a.ctype;i++){
			if(usedColor[i]) continue;
			if(colorIndex==0){ color = i; break; }
			colorIndex--;
		}
		if(color==-1){ // fallback
			for(int i=0;i<a.ctype;i++) if(!usedColor[i]){ color=i; break; }
		}
		usedColor[color]=1;
		// assign vertices from the largest color class on current side
		// and remove them from the corresponding sets on the other side
		vector<int> toErase;
		toErase.reserve(cur[M].size());
		for(auto v: cur[M]){
			child.color[v] = color;
			int index = (l%2==0) ? b.color[v] : a.color[v];
			if(index>=0 && index < (int)oth.size()){
				oth[index].erase(v);
			}
			toErase.push_back(v);
		}
		cur[M].clear();
	}
	// remaining vertices in colorV0 (original first parent's classes) assign random colors
	for(int i=0;i<a.ctype;i++){
		for(auto v: colorV0[i]){
			child.color[v]=rand()%a.ctype;
		}
	}
	child.InitConflicts();
	return child;
}

void GCP::HybridEvolutionary(int iter){
	bestSol=generations;
	// worstSol=generations;
	auto start=Timestamp::getTimestampMs();
	for(int i=0;i<GenerationSize;i++){
		generations[i].randInit(rec_color);
		for(int j=0;j<TabuSearchIter;j++)
			generations[i].TabuSearch(j,bestSol->conflicts);
		if(bestSol->conflicts<generations[i].conflicts)
			bestSol=generations+i;
		// if(worstSol->conflicts>generations[i].conflicts)
		// 	worstSol=generations+i;
	}
	int i=0;
	for(;i<iter;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		//选择两个父代
		int a=rand()%GenerationSize;
		int b=rand()%(GenerationSize-1);
		if(b>=a) b++;
		//交叉生成子代
		Solution child=crossover(generations[a],generations[b]);
		//对子代进行禁忌搜索
		for(int j=0;j<TabuSearchIter;j++)
			child.TabuSearch(j,child.conflicts);
		//替换最差个体
		worstSol=&generations[0];
		for(int k=1;k<GenerationSize;k++){
			if(worstSol->conflicts<generations[k].conflicts){
				worstSol=&generations[k];
			}
		}
		if(child.conflicts<worstSol->conflicts){
			*worstSol=std::move(child);
			if(bestSol->conflicts>worstSol->conflicts){
				bestSol=worstSol;
			}
		}
		if(bestSol->conflicts==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"generations: "<<i+1<<endl;
	cout<<(*bestSol);
}

// Solution& Solution::operator=(const Solution &b){
// 	if(this==&b) return *this;
// 	g=b.g;
// 	conflicts=b.conflicts;
// 	ctype=b.ctype;
// 	color=b.color;
// 	adjTable=b.adjTable;
// 	conflictVertices=b.conflictVertices;
// 	ttt=b.ttt;
// 	return *this;
// }

// Solution& Solution::operator=(Solution &&b){
// 	cerr<<"called\n";
// 	if(this==&b) return *this;
// 	g=b.g;
// 	b.g=nullptr;
// 	conflicts=b.conflicts;
// 	ctype=b.ctype;
// 	color=std::move(b.color);
// 	adjTable=std::move(b.adjTable);
// 	conflictVertices=std::move(b.conflictVertices);
// 	ttt=std::move(b.ttt);
// 	return *this;
// }

// Solution::~Solution(){
// 	g=nullptr;
// 	// conflicts=0;
// 	// ctype=0;
// 	// color.clear();
// 	// adjTable.clear();
// 	// conflictVertices.clear();
// 	ttt.~TabuTenureTable();
// }

// TabuTenureTable::TabuTenureTable(const TabuTenureTable &b){
// 	vc=b.vc;
// 	ctype=b.ctype;
// 	table = new int*[vc];
// 	for(int i=0;i<vc;i++){
// 		table[i] = new int[ctype];
// 		for(int j=0;j<ctype;j++){
// 			table[i][j]=b.table[i][j];
// 		}
// 	}
// }

// TabuTenureTable& TabuTenureTable::operator=(const TabuTenureTable &b){
// 	if(this==&b) return *this;
// 	if(table){
// 		this->~TabuTenureTable();
// 	}
// 	vc=b.vc;
// 	ctype=b.ctype;
// 	table = new int*[vc];
// 	for(int i=0;i<vc;i++){
// 		table[i] = new int[ctype];
// 		for(int j=0;j<ctype;j++){
// 			table[i][j]=b.table[i][j];
// 		}
// 	}
// 	return *this;
// }

// TabuTenureTable::TabuTenureTable(TabuTenureTable &&b){
// 	vc=b.vc;
// 	ctype=b.ctype;
// 	table=b.table;
// 	b.vc=0;
// 	b.ctype=0;
// 	b.table=nullptr;
// }

// TabuTenureTable& TabuTenureTable::operator=(TabuTenureTable &&b){
// 	if(this==&b) return *this;
// 	if(table){
// 		this->~TabuTenureTable();
// 	}
// 	vc=b.vc;
// 	ctype=b.ctype;
// 	table = b.table;
// 	b.vc=0;
// 	b.ctype=0;
// 	b.table=nullptr;
// 	return *this;
// }