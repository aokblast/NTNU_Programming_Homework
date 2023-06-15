#include <vector>
#include <tuple>
#include <queue>
#include <iostream>

using tii = std::tuple<int, int>;

class compare {
public:
    bool operator() (const tii &lhs, const tii &rhs) {
        return std::get<0>(lhs) > std::get<0>(rhs);
    }
};

int
main() {
    std::priority_queue<tii, std::vector<tii>, compare> pq;
    char c;
    int a, b;

    while(std::cin >> c  >> a >> c >> b >> c) {
        pq.push({a, b});
    }

    while(pq.size()) {
        auto top = pq.top(); pq.pop();
        std::cout << "(" << std::get<0>(top) << "," << std::get<1>(top) << ")\n";
    }
}
