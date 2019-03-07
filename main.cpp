#include <iostream>
#include <list>
#include <set>
#include <map>
using namespace std;
template <typename T = int>
T in() {
    T temp;
    cin >> temp;
    return temp;
}

class Merger {

    map<int, list<int>>::iterator master;
    map<int, int>& parents;

public:
    Merger(map<int, list<int>>::iterator target, map<int, int>& parents_p) : master(target), parents(parents_p) {}

    void Merge(int target) {
        master->second.push_back(target);
        parents[target] = master->first;
    }

    void Merge(list<int> const& slave) {

        // Change each parent elements of group other   
        for (int i : slave) parents[i] = master->first;

        // Then merge all elements in other group to parent group.
        master->second.insert(master->second.end(), slave.begin(), slave.end());
    }
};

class UnionFind {
    map<int, int> masters;
    map<int, list<int>> groups;

    bool IsSingle(int id) {
        return groups.find(masters[id]) == groups.cend();
    }

    size_t GetCurrentSize(int id) {
        return IsSingle(id) ? 1 : groups[id].size();
    }

public :

    UnionFind(size_t N) {
        for (size_t i = 0; i <= N; i++) masters[i] = i;
    }
  
    uint64_t Merge(int from, int to) {
   
        if (masters.find(from) == masters.cend() || masters.find(to) == masters.cend()) {
            cerr << "Not such element of " << from << " or " << to << "!!" << endl;
        }

        // If They are in same group, the useful point will not be increased.
        if (masters[from] == masters[to]) return 0;

        int const parent = min(masters[from], masters[to]);
        int const other = max(masters[from], masters[to]);

        // Save delta size before merge them.
        size_t const deltaSize =  GetCurrentSize(parent) * GetCurrentSize(other); 

        // When it is not exist. make target group.
        if (IsSingle(parent)) {
            groups[parent] = list<int>();
            groups[parent].push_back(parent);
        }

        Merger merger(groups.find(parent), masters);

        if (IsSingle(other)) {

            merger.Merge(other);
        } else {
    
            auto ite = groups.find(other);
            merger.Merge(ite->second);
            groups.erase(ite);
        }

        return deltaSize;
    }
};

class Network {

    uint64_t const maxUseful;
    list<uint64_t> scores;
    uint64_t currentUseful;
    UnionFind unionFind;
    
public :
    Network(size_t N) : maxUseful((uint64_t)N * (N - 1) / 2), currentUseful(0), unionFind(N) {}

    void Input(int from, int to) {

        currentUseful +=  unionFind.Merge(from, to);        
        scores.push_back(currentUseful);
    }

    void Calculate() {
        
        // Required elements are only 1 ~ N -1
        auto ite = scores.begin();

        // Remember iterator of last one.
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
    for (size_t i = M; 0 < i; i--) islands.Input(A[i - 1], B[i - 1]);

    islands.Calculate();
    islands.Output();
    return 0;
}


