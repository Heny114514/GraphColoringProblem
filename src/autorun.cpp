#include<bits/stdc++.h>
using namespace std;
int main(){
	for(int i=0;i<=9;i++){
		string cmd="./main 600000 20060713 <../data/"+to_string(i)+ \
		".txt >../result/TS/"+to_string(i)+".txt";
		system(cmd.c_str());
	}
	return 0;
}