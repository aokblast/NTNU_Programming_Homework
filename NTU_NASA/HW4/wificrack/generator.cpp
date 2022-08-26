#include <bits/stdc++.h>


int main(){
    std::ofstream of("dict.txt");
    char buffer[100];
    for(int i = 0; i <  1e9; ++i){
       sprintf(buffer, "09%08d\n", i);
       std::string s(buffer);
       of << s; 
    }
}