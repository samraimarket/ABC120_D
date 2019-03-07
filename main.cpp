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

    map<int, set<int>>::iterator master;
    map<int, int>& parents;

public:
    Merger(map<int, set<int>>::iterator target, map<int, int>& parents_p) : master(target), parents(parents_p) {}

    void Merge(int target) {
        master->second.insert(target);
        parents[target] = master->first;
    }

    void Merge(set<int> const& slave) {

        // Change each parent elements of group other   
        for (int i : slave) parents[i] = master->first;

        // Then merge all elements in other group to parent group.
        master->second.insert( slave.begin(), slave.end());
    }
};

class UnionFind {
    map<int, int> masters;
    map<int, set<int>> groups;

    bool IsSingle(int id) const {
        return groups.find(masters.find(id)->first) == groups.cend();
    }

    uint64_t GetCurrentSize(int id) const {
        return IsSingle(id) ? 1 : groups.find(id)->second.size();
    }

public :

    UnionFind(size_t N) {
        for (size_t i = 0; i <= N; i++) masters[i] = i;
    }
 
    struct MergeTarget {
        int const parent;
        int const other;
        MergeTarget(int parent1, int parent2) : parent(min(parent1, parent2)), other(max(parent1, parent2)) {}
    };

    uint64_t GetDeltaUseful(MergeTarget const& mergeTarget) const {
        return GetCurrentSize(mergeTarget.parent) * GetCurrentSize(mergeTarget.other); 
    }

    void Merge(MergeTarget const& target) {
   
        // When it is not exist. make target group.
        if (IsSingle(target.parent)) {
            groups[target.parent] = set<int>();
            groups[target.parent].insert(target.parent);
        }

        Merger merger(groups.find(target.parent), masters);

        if (IsSingle(target.other)) {

            merger.Merge(target.other);
        } else {
    
            auto ite = groups.find(target.other);
            merger.Merge(ite->second);
            groups.erase(ite);
        }
    }

    MergeTarget GetMergeTarget (int from, int to) const {
        return MergeTarget(masters.find(from)->second, masters.find(to)->second);
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

        UnionFind::MergeTarget const mergeTarget = unionFind.GetMergeTarget(from, to);

        if (mergeTarget.parent != mergeTarget.other) {
            currentUseful += unionFind.GetDeltaUseful(mergeTarget);
            unionFind.Merge(mergeTarget);        
        }

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


