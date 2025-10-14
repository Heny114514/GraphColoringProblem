#include<bits/stdc++.h>
using namespace std;
int main(){
	for(int i=0;i<=9;i++){
		string cmd="./main 30000 12345 <../data/"+to_string(i)+ \
		".txt >../result/LS/"+to_string(i)+".txt";
		system(cmd.c_str());
	}
	return 0;
}