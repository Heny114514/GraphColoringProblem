#include<bits/stdc++.h>
#include"algorithm.h"
using namespace std;
unsigned int timeLimit=5000,randSeed=0;
int ver_c,edg_c,rec_color;
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
	if(randSeed==0) randSeed=time(0);
	srand(randSeed);

	// cerr<<"timelimit: "<<timeLimit<<" randseed: "<<randSeed<<endl;
	cin>>ver_c>>edg_c>>rec_color;
	// cerr<<ver_c<<" "<<edg_c<<" "<<rec_color<<endl;
	// GCP gcp(ver_c,edg_c,rec_color);
	// gcp.init(cin);

	// Graph g(ver_c,edg_c);
	// g.read(cin);
	// g.randInit(rec_color);
	// cerr<<"conflicts: "<<g.conflicts<<endl;
	// cerr<<g;

	// g.LocalSearch();
	// cerr<<"conflicts: "<<g.conflicts<<endl;
	// cerr<<g;

	// g.LocalSearch();
	// cerr<<"conflicts: "<<g.conflicts<<endl;
	// cerr<<g;

	// cerr<<"success";

	GCP gcp(ver_c,edg_c,rec_color);
	gcp.init();
	
	gcp.HybridEvolutionary(10000/GenerationSize);

	// Graph g(ver_c,edg_c);
	// g.read();
	// Solution a,b,c;
	// a.bind(g);
	// a.randInit(rec_color);
	// cerr<<"a:"<<a;
	// b.bind(g);
	// b.randInit(rec_color);
	// cerr<<"b:"<<b;
	// c.bind(g);
	// c=crossover(a,b);
	// cerr<<"c:"<<c;
	return 0;
}