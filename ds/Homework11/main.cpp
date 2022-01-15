#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "LinkedList.h"

using std::string, std::cin, std::cout, std::fstream, std::stringstream, std::getline;

const int MAXLEN  = 2e4 + 5;

template<typename Ta, typename Tb> struct Pair{
	Ta a;
	Tb b;
	Pair(const Ta &a, const Tb &b) : a(a), b(b){}
	Pair() = default;
	constexpr bool operator<(const Pair<Ta, Tb> &p) const {return a == p.a ? b < p.b : a < p.a;}
};

namespace DJS{
    int djs[MAXLEN] = {0};
    int parent(int u){return u == djs[u] ? u : djs[u] = parent(djs[u]);}
	void init()noexcept{for(int i = 0; i < MAXLEN; ++i)djs[i] = i;}
}

int vars[26] = {0};

LinkedList<Pair<int, int>> adjs[MAXLEN];

int main() {
    string fileName;
    cout << "Please input your fileName: ";
	getline(cin, fileName);
    fstream fs(fileName);
    LinkedList<Pair<int, Pair<int, int>>> list;
    char c;
    int cost, n, m, f, t, tot;
    fs >> n;
    while(n--){
        fs >> c >> cost;
        vars[c - 'A'] = cost;
    }
    fs >> n >> m;
	fs.get();
    string line;

    while(m--){
        getline(fs, line);
        cost = 0;
        stringstream ss(line);
        ss >> f >> t;
        while(ss >> c >> tot){
            cost += vars[c - 'A'] * tot;
        }
		list.push_back({cost, {f, t}});
		adjs[f].push_back({t, cost});
		adjs[t].push_back({f, cost});
    }

	list.sort();
	DJS::init();

	cost = 0;
	--n;
	for (auto &[co, point] : list) {

		int p1 = DJS::parent(point.a), p2 = DJS::parent(point.b);
		if(p1 == p2)continue;
		DJS::djs[p1] = p2;
		cost += co;
		if((--n) == 0){
			cout << cost << '\n';
			std::exit(0);
		}
	}

	cout << "No answer\n";



    return 0;
}
