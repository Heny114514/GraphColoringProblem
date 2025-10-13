#include<bits/stdc++.h>
#include"algorithm.h"
using namespace std;
unsigned int timeLimit,randSeed;
int ver_c,edg_c,ref_color;
unsigned int str_to_ui(const char *str){
	if(*str=='0'){
		if(*(str+1)=='\0') return 0;
		else{
			throw"parameter error";
			return 0;
		}
	}
	unsigned int ans=0;
	for(;*str!='\0';str++){
		if(*str>='0'&&*str<='9'){
			ans=ans*10+*str-'0';
		}else{
			throw"parameter error";
			return 0;
		}
	}
	return ans;
}
int main(const int argc, const char** argv){
	if(argc!=3&&argc!=1){
		cerr<<"parameter error\n";
		return -1;
	}
	if(argc==3){
		try{
			timeLimit=str_to_ui(argv[1]);
			randSeed=str_to_ui(argv[2]);
		}
		catch(const char *e){
			cerr<<e<<endl;
			return -1;
		}
	}
	cerr<<"timelimit: "<<timeLimit<<" randseed: "<<randSeed<<endl;
	cin>>ver_c>>edg_c>>ref_color;
	cerr<<ver_c<<" "<<edg_c<<" "<<ref_color<<endl;
	Graph g(ver_c,edg_c);
	for(int i=0;i<edg_c;i++){
		int u,v;
		cin>>u>>v;
		g.add(u,v);
		g.add(v,u);
	}
	cerr<<"success";
	return 0;
}