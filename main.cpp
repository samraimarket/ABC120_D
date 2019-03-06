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
    list<uint64_t> scores;
    uint64_t currentUseful;
    uint64_t const GetMaxUseful() {
        return (uint64_t)N * (N - 1) / 2;
    }
    bool const Connected(int from, int to) {
        return parents[from] == parents[to];
    }

    bool const IsSingle(int id) {
        return (groups.find(id) == groups.cend()) && (parents[id] == id);
    }

    size_t const GetCurrentSize(int id) {
        return (IsSingle(id)) ? 1 : groups[id].size();
    }

    size_t const GetDeltaSize(int parent, int other) {
        return GetCurrentSize(other) * GetCurrentSize(parent);
    }

    void merge(int parent, int other) {
        parents[other] = parent;
        if (groups.find(parent) == groups.cend()) {
            groups[parent] = set<int>();
            groups[parent].insert(parent);
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
                groups[from].insert(from);
                groups[from].insert(to);
                currentUseful++;
            } else {

                int const parent = min(parents[from], parents[to]);
                int const other = (parent == parents[from]) ? parents[to] : parents[from];  
                size_t deltaSize = GetDeltaSize(parent, other);

                merge(parent, other);
                currentUseful += deltaSize;
            }
        }
        
        scores.push_back(currentUseful);
    }
    void Calculate(size_t const M) {
        list<uint64_t> tempList;
        uint64_t maxUseful = N * (N - 1) / 2;
        tempList.push_back(GetMaxUseful());
        auto ite = scores.begin();
        for (size_t i = 1; i < M; i++) {            
            tempList.push_back(maxUseful - *ite);
            ite++;
        }
        scores = tempList;
    }

    void Output() {
        scores.reverse();
        for (uint64_t score :  scores) {
            cout << score << endl;
        }
    }
};
int main() {
    auto const N(in<size_t>()), M(in<size_t>());
    Network islands(N);
    islands.initialize();
    int A[M], B[M];
    for (unsigned int i = 0; i < M; i++) {
        A[i] = in();
        B[i] = in();
    }

    for (unsigned int i = M; 0 < i; i--) {
        islands.Input(A[i - 1], B[i - 1]);
    }
    islands.Calculate(M);
    islands.Output();
    return 0;
}


