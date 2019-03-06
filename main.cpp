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

class UnionFind {
    map<int, int> parents;
    map<int, set<int>> groups;

    void Initialize(int id) {
        groups[id] = set<int>();
        groups[id].insert(id);
    }

    bool const IsSingle(int id) {
        return (groups.find(id) == groups.cend()) && (parents[id] == id);
    }

    bool const Connected(int from, int to) {
        return parents[from] == parents[to];
    }

public :
    UnionFind(size_t N) {
        for (size_t i = 0; i <= N;i++) {
            parents[i] = i;
        }
    }
   
    size_t const GetCurrentSize(int id) {
        return (IsSingle(id)) ? 1 : groups[id].size();
    }

    size_t const GetDeltaSize(int parent, int other) {
        return GetCurrentSize(other) * GetCurrentSize(parent);
    }

    void merge(int parent, int other) {

        parents[other] = parent;
        if (groups.find(parent) == groups.cend()) Initialize(parent);
        
        if (groups.find(other) == groups.cend()) {
           
            groups[parent].insert(other);

        // When the other owns group
        } else {
         
            // Change each parent elements of group other   
            for (int i : groups[other]) parents[i] = parent;

            // Then merge otehr group to parent
            groups[parent].insert(groups[other].begin(), groups[other].end());
            groups.erase(other);
        }
    }

    uint64_t Input(int from, int to) {

        if (!Connected(from, to)) {
            if (IsSingle(from) && IsSingle(to)) {

                Initialize(from);
                parents[to] = from;
                groups[from].insert(to);
                return 1;

            } else {

                int const parent = min(parents[from], parents[to]);
                int const other = (parent == parents[from]) ? parents[to] : parents[from];  
                size_t const deltaSize = GetDeltaSize(parent, other);

                merge(parent, other);
                return deltaSize;
            }
        }
        return 0;
    }
};

class Network {
    size_t const N;
    list<uint64_t> scores;
    uint64_t currentUseful;
    UnionFind unionFind;
    
public :
    Network(size_t N_p) : N(N_p), currentUseful(0), unionFind(N) {}

    void Input(int from, int to) {

        // If They are already connected, no need to calculate.
        // Otherwise try to calculate current useful value  and adjust group.
        currentUseful += unionFind.Input(from, to);
        scores.push_back(currentUseful);
    }

    void Calculate() {


        // At final(id == N) unuseful point must be max useful value.
        uint64_t const maxUseful = N * (N - 1) / 2;

        //scores.push_back(GetMaxUseful());
        
        // Required elements are only 1 ~ N -1
        auto ite = scores.begin();

        // Remember last one.
        auto end = scores.end();
        end--;

        // Overwrite each elements to calculated one.
        for (; ite != end ; ite++) *ite = (maxUseful - *ite);

        // so delete last one.
        scores.erase(end);

        // insert max useful after reversing
        scores.reverse();
        scores.push_back(maxUseful);
    }

    void Output() {
        for (uint64_t score :  scores) cout << score << endl;
    }
};

int main() {
    auto const N(in<size_t>()), M(in<size_t>());
    int A[M], B[M];
    
    for (size_t i = 0; i < M; i++) {
        A[i] = in();
        B[i] = in();
    }

    Network islands(N);

    // Input each value as inversed.
    for (unsigned int i = M; 0 < i; i--) islands.Input(A[i - 1], B[i - 1]);

    islands.Calculate();
    islands.Output();
    return 0;
}


