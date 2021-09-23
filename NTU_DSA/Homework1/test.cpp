#include <bits/stdc++.h>

using namespace std;

template <typename eleType>ostream &operator<<(ostream &os, queue<eleType> &qu) {
    if(qu.size() == 0) return os;
    eleType tmp = qu.front();
    do {
        os << qu.front() << " ";
        qu.push(qu.front());
        qu.pop();
    }while(tmp != qu.front());
    return os;
}
 
template <typename eleType> void reverse(queue<eleType> &source, queue<eleType> &help) {
    while(source.size() > 2) {
        help.push(source.front());
        source.pop();
    }
    source.push(source.front());
    source.pop();
    auto size = help.size();
    for(int i = 0; i < size; ++i) {
        for(int j = 0; j < help.size() - 1;++j) {
            help.push(help.front());
            help.pop();
        }
        source.push(help.front());
        help.pop();
        
    }
}

int main(){
    queue <int> source;
    source.push(1);
    source.push(2);
    source.push(3);
    cout << source << endl;
    queue<int> help;
    reverse(source, help);
    cout << source << endl;
    cout << help;
    
    
}
