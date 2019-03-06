#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <algorithm>
using namespace std;
template <typename T = int>
T in() {
    T temp;
    cin >> temp;
    return temp;
}

class Network {
    size_t const N;
    map<int, int> parents;
    map<int, set<int>> groups;
    list<uint64_t> uncalculatedScores;
    uint64_t currentUseful;
    uint64_t const GetMaxUseful() {
        return (uint64_t)N * (N - 1) / 2;
    }
    bool const Connected(int from, int to) {
        return parents[from] == parents[to];
    }

    bool const IsSingle(int id) {
        return groups.find(id) == groups.cend();
    }

    size_t const GetCurrentSize(int id) {
        return groups[id].size();
    }
    size_t const GetDeltaSize(int parent, int other) {
        if (IsSingle(parent)) return GetCurrentSize(other);
        else if (IsSingle(other)) return GetCurrentSize(parent);
        return GetCurrentSize(other) * GetCurrentSize(parent);
    }

    void merge(int parent, int other) {
        parents[other] = parent;
        if (groups.find(parent) == groups.cend()) {
            groups[parent] = set<int>();
        }
        
        if (groups.find(other) == groups.cend()) {
            groups[parent].insert(other);
        } else {
            for (int i : groups[other]) {
                parents[i] = parent;
            }
            groups[parent].insert(groups[other].begin(), groups[other].end());
            groups.erase(other);
        }
    }
public :
    Network(size_t N_p) : N(N_p), currentUseful(0) {}
    void initialize() {
        for (size_t i = 0; i <= N;i++) {
            parents[i] = i;
        }
    }
    void Input(int from, int to) {
        // If They are already connected, no need to calculation
        // Otherwise try to calculate current useful value  and adjust group.
        if (!Connected(from, to)) {
            if (IsSingle(from) && IsSingle(to)) {
                parents[to] = from;
                groups[from] = set<int>();
                groups[from].insert(to);
                currentUseful++;
            } else {

                int const parent = min(parents[from], parents[to]);
                cout << parents[from] << ' ' << parents[to] << ' ' << to << endl;
                int const other = (parent == parents[from]) ? parents[to] : parents[from];  
                size_t deltaSize = GetDeltaSize(parent, other);

                merge(parent, other);
                currentUseful += deltaSize;
                
            }
        }
        uncalculatedScores.push_back(currentUseful);
    }

    void Output() {
        for (uint64_t score :  uncalculatedScores) {
            cout << score << endl;
        }
    }
};
int main() {
    auto const N(in<size_t>()), M(in<size_t>());
    Network islands(N);
    islands.initialize();
    for (unsigned int i = 0; i < M; i++) {
        int const A(in()), B(in());
        islands.Input(A, B);
    }
    islands.Output();
    return 0;
}


