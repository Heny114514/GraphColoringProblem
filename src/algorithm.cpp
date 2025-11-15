#include"algorithm.h"
#ifdef DEBUG_CHOSEN_CRITICAL_ONE_MOVE
ofstream cdb("debug_chosen_critical_one_move.txt",ios::out);
#endif
	
void Graph::add(int u,int v){
	edges.push_back({v,head[u]});
	head[u]=edges.size()-1;
}

void Solution::randInit(int ctype){
	this->ctype=ctype;
	color.resize(g.vc);
	// maxColor.resize(g.vc,0);
	// Colormax.resize(g.vc,0);
	// adjTable.resize(g.vc);
	// for(auto &i:adjTable) i.resize(ctype,0);
	// ttt.resize(g.vc,ctype);
	// color[0]=maxColor[0]=0;
	for(int i=0;i<g.vc;i++){
		color[i]=rand()%ctype;
		// maxColor[i]=max(maxColor[i-1],color[i]);
	}
	// Colormax[g.vc-1]=color[g.vc-1];
	// for(int i=g.vc-2;i>=0;i--){
	// 	Colormax[i]=max(Colormax[i+1],color[i]);
	// }
	// conflicts=0;
	// g.vis.assign(g.vc,false);
	// for(int v=0;v<g.vc;v++){
	// 	for(int i=g.head[v];i!=-1;i=g.edges[i].nxt){//遍历所有v的出边
	// 		int to=g.edges[i].to;
	// 		adjTable[v][color[to]]++;//初始化邻接表
	// 		if(g.vis[to]) continue;
	// 		if(color[v]==color[to]){
	// 			conflicts++;
	// 			conflictVertices.insert(v);
	// 			conflictVertices.insert(to);
	// 		}
	// 	}
	// 	g.vis[v]=true;
	// }
	InitConflicts();
}

// void Solution::randInit(int ctype){//贪心构造
//     if(!g) return;
//     this->ctype = ctype;
//     int vcnt = g.vc;
//     if(vcnt <= 0) return;
//     // if requested more colors than vertices, clamp
//     if(ctype > vcnt) ctype = vcnt, this->ctype = ctype;

//     color.assign(vcnt, -1);
//     maxColor.assign(vcnt, -1);
//     // greedy: introduce new colors in ascending order when allowed
//     int nextNew = 0; // next new color to introduce (0..ctype-1)
//     // iterate vertices in order 0..vc-1
//     for(int v = 0; v < vcnt; ++v){
//         int prevMax = (v == 0) ? -1 : maxColor[v-1];
//         int allowMax = std::min(prevMax + 1, ctype - 1);
//         int chosen = -1;

//         // if we still need to introduce nextNew and it's allowed now, do it
//         if(nextNew <= allowMax){
//             chosen = nextNew;
//             ++nextNew;
//         } else {
//             // choose color in [0..allowMax] that minimizes conflicts with already assigned neighbors
//             int best = -1;
//             int bestConf = INT_MAX;
//             // evaluate each candidate
//             for(int c = 0; c <= allowMax; ++c){
//                 int conf = 0;
//                 // count neighbors already assigned with same color
//                 for(int e = g.head[v]; e != -1; e = g.edges[e].nxt){
//                     int u = g.edges[e].to;
//                     if(u < v && color[u] == c) ++conf;
//                 }
//                 if(conf < bestConf){
//                     bestConf = conf;
//                     best = c;
//                 } else if(conf == bestConf && best != -1){
//                     // tie-break randomly
//                     if((rand() & 1) == 0) best = c;
//                 }
//             }
//             if(best == -1) best = 0;
//             chosen = best;
//         }

//         color[v] = chosen;
//         maxColor[v] = std::max(prevMax, color[v]);
//     }
//     // ensure all colors introduced (should hold when vcnt >= original ctype)
//     // final safety: if nextNew < ctype, force-introduce remaining colors from the end where allowed
//     for(int c = nextNew; c < ctype; ++c){
//         // find a position from last to first where we can set color to c without violating constraint
//         bool placed = false;
//         for(int v = vcnt - 1; v >= 0; --v){
//             int prevMax = (v == 0) ? -1 : maxColor[v-1];
//             int allowMax = std::min(prevMax + 1, ctype - 1);
//             if(c <= allowMax){
//                 // replace color at v with c, update maxColor from v..end
//                 color[v] = c;
//                 // recompute maxColor from v to end
//                 for(int i = v; i < vcnt; ++i){
//                     if(i == 0) maxColor[i] = color[i];
//                     else maxColor[i] = std::max(maxColor[i-1], color[i]);
//                 }
//                 placed = true;
//                 break;
//             }
//         }
//         if(!placed){
//             // shouldn't happen if vcnt >= ctype, but fallback: leave as is
//             break;
//         }
//     }

// 	InitConflicts();
// }

void Solution::InitConflicts(){
	minVerofColorSet.resize(ctype,-1);
	standardizedColorTable.resize(ctype,-1);
	// isMinVerofColorSet.resize(g.vc,false);
	adjTable.resize(g.vc);
	for(auto &i:adjTable) i.resize(ctype,0);
	// ttt.resize(g.vc,ctype);
	ttt.resize(g.vc);
	for(auto &i:ttt) i.resize(ctype,0);
	conflicts=0;
	conflictVertices.clear();
	g.vis.assign(g.vc,false);
	for(int v=0;v<g.vc;v++){
		for(int i=g.head[v];i!=-1;i=g.edges[i].nxt){//遍历所有v的出边
			int to=g.edges[i].to;
			adjTable[v][color[to]]++;//初始化邻接表
			if(g.vis[to]) continue;
			if(color[v]==color[to]){
				conflicts++;
				conflictVertices.insert(v);
				conflictVertices.insert(to);
			}
		}
		g.vis[v]=true;
	}
	InitStandardize();

	#ifdef DEBUG
	if(!Check()) cerr<<"init error!\n";
	#endif
	// standardization();
}

void Solution::bind(Graph& g){
	this->g=g;
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

void Solution::TabuSearch(int iter,int bestEver){
	#ifdef DEBUG_CHOSEN_CRITICAL_ONE_MOVE
	bool ifChooseTabu=false;
	#endif

    int minDelta=0x7fffffff,mver=-1,mfrom=-1,mto=-1;
    int tminDelta=0x7fffffff,tmver=-1,tmfrom=-1,tmto=-1;
    for(auto v:conflictVertices){//寻找最优移动
        // 为每个 v 在内层循环提供一个默认指向全局标准化表的指针
        for(int c=0;c<ctype;c++){
            if(c==color[v]) continue;

            // 准备用于禁忌表索引的标准化映射指针
            vector<int> local_sct;           // 若需要，存放临时映射
            vector<int> *sct_ptr = &standardizedColorTable;

            if(minVerofColorSet[color[v]]==v||minVerofColorSet[c]>v){
                // 评估 v 改色后标准化映射
                int ctemp = color[v];
                color[v] = c;
                local_sct.assign(ctype, -1);
				int maxused=0;
                for(int i=0; i<g.vc; ++i){
                    int col = color[i];
                    if(local_sct[col] == -1){
                        local_sct[col] = maxused++;
                    }
                }
				if(maxused<ctype){
					for(int c=0;c<ctype;c++){
						if(local_sct[c]==-1){
							local_sct[c]=maxused++;
						}
					}
				}
                // // 补齐所有未出现的颜色，保证映射覆盖 0..ctype-1
                // for(int old=0, j=0; old<ctype; ++old){
                //     // find next unused index by scanning local_sct (j not used here), simpler fill sequentially
                // }
                // 上面循环没有实际填充剩余项，改用下面方式：
                // {
                //     int nextIdx = 0;
                //     for(int old=0; old<ctype; ++old){
                //         if(local_sct[old] != -1) continue;
                //         // find current max used index
                //         // compute next available index as count of already assigned
                //     }
                //     // 更直接：统计已有最大 j 值并逐一赋值
                //     int maxAssigned = -1;
                //     for(int old=0; old<ctype; ++old) if(local_sct[old] > maxAssigned) maxAssigned = local_sct[old];
                //     int start = (maxAssigned >= 0) ? (maxAssigned + 1) : 0;
                //     for(int old=0; old<ctype; ++old){
                //         if(local_sct[old] == -1) local_sct[old] = start++;
                //     }
                // }

                color[v] = ctemp;
                sct_ptr = &local_sct; // 使用临时映射
            }

            // 计算 delta：变更 v 的颜色会导致的冲突数变化
            int delta = adjTable[v][c] - adjTable[v][color[v]];

            // 使用临时/全局标准化映射来检查禁忌
            int sct_index = (*sct_ptr)[c];
            if(sct_index < 0 || sct_index >= ctype){
                // 防御：若映射未覆盖此颜色，跳过该候选（安全）
                continue;
            }

            if(ttt[v][sct_index] > iter){ // 禁忌
                if(delta < tminDelta||delta==tminDelta&&(rand()%2==0)){
                    tminDelta = delta;
                    tmver = v;
                    tmfrom = color[v];
                    tmto = c;
                }
            }else{
                if(delta < minDelta||delta==minDelta&&(rand()%2==0)){
                    minDelta = delta;
                    mver = v;
                    mfrom = color[v];
                    mto = c;
                }
            }
            // 临时 local_sct 会在下一次循环迭代自动销毁
        }
    }
    if(tminDelta < minDelta && conflicts + tminDelta < bestEver ){ // 满足禁忌接受条件
        minDelta = tminDelta;
        mver = tmver;
        mfrom = tmfrom;
        mto = tmto;

		#ifdef DEBUG_CHOSEN_CRITICAL_ONE_MOVE
		ifChooseTabu=true;
		#endif
    }
    // 如果没有找到合法移动，直接返回
    if(mver == -1){
		#ifdef DEBUG_CHOSEN_CRITICAL_ONE_MOVE
		cdb<<"on iter "<<iter<<", no valid move found\n";
		#endif

        return;
    }

    // 应用移动：先更新颜色与邻接表，再更新冲突计数（以避免时序不一致）
    // 更新邻接表（邻居的统计）和 conflictVertices
    // 先保存旧值以便调试/检查
    int oldColor = mfrom;
    int newColor = mto;

    // 将顶点 mver 的颜色更新为新颜色
    color[mver] = newColor;

    // 对所有邻居调整它们对 mver 颜色统计
    for(int i = g.head[mver]; i != -1; i = g.edges[i].nxt){
        int to = g.edges[i].to;
        // 减去旧颜色计数，增加新颜色计数
        adjTable[to][oldColor]--;
        adjTable[to][newColor]++;
        // 更新 conflictVertices（基于邻居自身颜色与其统计）
        if(isConflict(to)){
            conflictVertices.insert(to);
        }else{
            conflictVertices.erase(to);
        }
    }

    // 更新 mver 本身是否在冲突集合中（其 adjTable 行未变，但其颜色改变）
    if(isConflict(mver)){
        conflictVertices.insert(mver);
    }else{
        conflictVertices.erase(mver);
    }

    // 现在安全地更新全局冲突计数（使用之前计算的 minDelta）
    conflicts += minDelta;

    // 更新禁忌表：使用当前（移动前或移动后语义需一致）的 standardizedColorTable 索引
    // 确保 standardizedColorTable 覆盖所有颜色（不存在 -1）
    // if((int)standardizedColorTable.size() != ctype){
    //     InitStandardize();
    // }
    // if(standardizedColorTable[mfrom] < 0 || standardizedColorTable[mfrom] >= ctype){
    //     // 若原始规范表未覆盖 mfrom（非常罕见），重新标准化再取索引
    //     InitStandardize();
    // }
	ttt[mver][standardizedColorTable[mfrom]] = iter + rand()%10 + conflicts; // 更新禁忌表

    // 若移动影响了某个颜色的最小元，更新标准化信息
    if(minVerofColorSet[mfrom]==mver||minVerofColorSet[mto]>mver){
        InitStandardize();
    }


	#ifdef DEBUG
	if(!Check()){
		cerr<<"in TabuSearch\n";
		cerr<<"mver: "<<mver<<" mfrom: "<<mfrom<<" mto: "<<mto<<"\n";
		cerr<<"condition: "<<(minVerofColorSet[mfrom]==mver||minVerofColorSet[mto]>mver)<<"\n";
		// cerr<<"minver of "
	}
	#endif

	#ifdef DEBUG_CHOSEN_CRITICAL_ONE_MOVE
	cdb<<"on iter "<<iter<<", move vertex "<<mver<<" from "<<mfrom<<" to "<<mto<<" with delta "<<minDelta<<" and tabu tenure "<<ttt[mver][standardizedColorTable[mfrom]]-iter<<"; ";
	if(ifChooseTabu) cdb<<" tabu";
	cdb<<"\nnow conf="<<conflicts<<" bestConf="<<bestEver;
	cdb<<endl;
	#endif
}

void GCP::TabuSearch(int iter){
	generations[0].randInit(rec_color);
	// bestSol=generations;
	int bestConf=generations[0].conflicts;
	vector<int> bestColor=generations[0].color;
	auto start=Timestamp::getTimestampMs();
	int i;
	for(i=0;i<iter||iter==0;i++){
		if(Timestamp::getTimestampMs()-start>timeLimit) break;
		generations[0].TabuSearch(i,bestConf);
		if(bestConf>generations[0].conflicts){
			bestConf=generations[0].conflicts;
			bestColor=generations[0].color;
		}
		if(generations[0].conflicts==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"seed: "<<randSeed<<endl;
	cout<<"iterations: "<<i+1<<endl;
	vector<bool> used(rec_color,false);
	for(auto c:bestColor) used[c]=true;
	int used_c=0;
	for(auto u:used) if(u) used_c++;
	cout<<"color types: "<<used_c<<endl;
	cout<<"conflicts: "<<bestConf<<"\ncoloring:\n";
	for(auto c:bestColor){
		cout<<c<<endl;
	}
}

bool Solution::isConflict(int ver){
	return adjTable[ver][color[ver]]>0;
}

ostream& operator<<(ostream &out,const Solution &sol){
	out<<"conflicts: "<<sol.conflicts<<"\ncolors:\n";
	for(auto c:sol.color) out<<c<<"\n";
	return out;
}

Solution crossover(const Solution &a, const Solution &b){
	Solution child(a.g);
	child.ctype=a.ctype;
	child.color.resize(a.g.vc);
	// Use STL containers instead of VLAs: safer and portable
	vector<char> usedColor(a.ctype, 0);
	vector< set<int> > colorV0(a.ctype), colorV1(a.ctype);
	for(int v=0;v<a.g.vc;v++){
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
	// bestSol=generations;
	// worstSol=generations;
	int bestConf=generations[0].conflicts;
	vector<int> bestColor=generations[0].color;
	auto start=Timestamp::getTimestampMs();
	for(int i=0;i<GenerationSize;i++){
		generations[i].randInit(rec_color);
		for(int j=0;j<TabuSearchIter;j++)
			generations[i].TabuSearch(j,bestConf);
		if(bestConf<generations[i].conflicts){
			bestConf=generations[i].conflicts;
			bestColor=generations[i].color;
		}
			// bestSol=generations+i;
		// if(worstSol->conflicts>generations[i].conflicts)
		// 	worstSol=generations+i;
	}
	int i=0;
	for(;i<iter||iter==0;i++){
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
		Solution *worstSol=&generations[0];
		for(int k=1;k<GenerationSize;k++){
			if(worstSol->conflicts<generations[k].conflicts){
				worstSol=&generations[k];
			}
			if(child.conflicts<worstSol->conflicts){
				// replace worstSol by move-constructing from child in-place to avoid calling deleted copy-assignment
				worstSol->~Solution();
				new (worstSol) Solution(std::move(child));
				if(bestConf>worstSol->conflicts){
					bestConf=worstSol->conflicts;
					bestColor=worstSol->color;
				}
			}
		}
		if(bestConf==0) break;
	}
	cout<<"time: "<<Timestamp::getTimestampMs()-start<<"ms\n";
	cout<<"seed: "<<randSeed<<endl;
	cout<<"iterations: "<<i+1<<endl;
	vector<bool> used(rec_color,false);
	for(auto c:bestColor) used[c]=true;
	int used_c=0;
	for(auto u:used) if(u) used_c++;
	cout<<"color types: "<<used_c<<endl;
	cout<<"conflicts: "<<bestConf<<"\ncoloring:\n";
	for(auto c:bestColor){
		cout<<c<<endl;
	}
}

void Solution::InitStandardize(){
	// isMinVerofColorSet.assign(g.vc,false);
	standardizedColorTable.assign(ctype,-1);
	minVerofColorSet.assign(ctype,-1);
	int maxused=0;
	for(int i=0;i<g.vc;i++){
		if(standardizedColorTable[color[i]]==-1){
			standardizedColorTable[color[i]]=maxused++;
			// isMinVerofColorSet[i]=true;
			minVerofColorSet[color[i]]=i;
		}
	}
	if(maxused<ctype){
		for(int c=0;c<ctype;c++){
			if(standardizedColorTable[c]==-1){
				standardizedColorTable[c]=maxused++;
			}
		}
	}
}

#ifdef DEBUG
bool Solution::Check(){
	//检查冲突数
	int conf=0;
	g.vis.assign(g.vc,false);
	for(int v=0;v<g.vc;v++){
		for(int i=g.head[v];i!=-1;i=g.edges[i].nxt){//遍历所有v的出边
			int to=g.edges[i].to;
			if(g.vis[to]) continue;
			if(color[v]==color[to]){
				conf++;
				// g.vis[to]=true;
				
			}
		}
		g.vis[v]=true;
	}
	if(conf!=conflicts){
		cerr<<"conflict error! calculated: "<<conf<<" stored: "<<conflicts<<endl;
		return false;
	}
	//检查邻接表
	for(int v=0;v<g.vc;v++){
		vector<int> adjtemp(ctype,0);
		for(int i=g.head[v];i!=-1;i=g.edges[i].nxt){//遍历所有v的出边
			int to=g.edges[i].to;
			adjtemp[color[to]]++;
		}
		for(int c=0;c<ctype;c++){
			if(adjtemp[c]!=adjTable[v][c]){
				cerr<<"adjTable error at vertex "<<v<<" color "<<c<<": calculated "<<adjtemp[c]<<" stored "<<adjTable[v][c]<<endl;\
				return false;
			}
		}
	}
	//检查冲突顶点
	for(auto v:conflictVertices){
		if(!isConflict(v)){
			cerr<<"conflictVertices error at vertex "<<v<<endl;
			return false;
		}
	}
	//检查颜色规范表
	vector<int> scttemp(ctype,-1);
	int maxused=0;
	for(int i=0;i<g.vc;i++){
		if(scttemp[color[i]]==-1){
			scttemp[color[i]]=maxused++;
		}
	}
	if(maxused<ctype){
		for(int c=0;c<ctype;c++){
			if(scttemp[c]==-1){
				scttemp[c]=maxused++;
			}
		}
	}
	for(int i=0;i<ctype;i++){
		if(scttemp[i]!=standardizedColorTable[i]){
			cerr<<"standardizedColorTable error at color "<<i<<": calculated "<<scttemp[i]<<" stored "<<standardizedColorTable[i]<<endl;
			return false;
		}
	}
	return true;
}
#endif