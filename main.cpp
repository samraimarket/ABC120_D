#include <iostream>
#include <map>
#include <deque>
#include <algorithm>
using namespace std;
template <typename T = int>
T in() {
    T temp;
    cin >> temp;
    return temp;
}

class Merger {

    map<int, deque<int>>::iterator master;
    map<int, int>& parents;

public:
    Merger(map<int, deque<int>>::iterator target, map<int, int>& parents_p) : master(target), parents(parents_p) {}

    void Merge(int target) {
        master->second.push_back(target);
        parents[target] = master->first;
    }

    void Merge(deque<int> const& slave) {

        // Change each parent elements of group other   
        for (int i : slave) parents[i] = master->first;

        // Then merge all elements in other group to parent group.
        master->second.insert(master->second.end() , slave.begin(), slave.end());
    }
};


// Wrapping map<int, deque<int>>
class Groups {
    
    map<int, deque<int>> groups;

public:
    bool IsSingle(int id) const {
        return groups.find(id) == groups.cend();
    }

    uint64_t GetCurrentSize(int id) const {
        return IsSingle(id) ? 1 : groups.find(id)->second.size();
    }

    map<int, deque<int>>::const_iterator Find(int target) const {
        return groups.find(target);
    }

    map<int, deque<int>>::iterator GetMaster(int target) {

        auto ite = groups.find(target);
        if (ite != groups.cend()) return ite;

        /// Not excist, return after create it.
        groups[target] = deque<int>();
        groups[target].push_back(target);
        return groups.find(target);
    }

    void erase(int target) {
        groups.erase(target);
    }
};

class UnionFind {
    map<int, int> masters;
    Groups groups;

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
        return groups.GetCurrentSize(mergeTarget.parent) * groups.GetCurrentSize(mergeTarget.other); 
    }

    void Merge(MergeTarget const& target) {
  
        Merger merger(groups.GetMaster(target.parent), masters);

        if (groups.IsSingle(target.other)) {

            merger.Merge(target.other);
        } else {
   
            merger.Merge(groups.Find(target.other)->second);
            groups.erase(target.other);
        }
    }

    MergeTarget GetMergeTarget (int from, int to) const {
        return MergeTarget(masters.find(from)->second, masters.find(to)->second);
    }
};

class Network {

    deque<uint64_t> scores;
    uint64_t currentUseful;
    UnionFind unionFind;
    
public :
    Network(size_t N) : currentUseful(0), unionFind(N) {}

    void Input(int from, int to) {

        UnionFind::MergeTarget const mergeTarget = unionFind.GetMergeTarget(from, to);

        if (mergeTarget.parent != mergeTarget.other) {
            currentUseful += unionFind.GetDeltaUseful(mergeTarget);
            unionFind.Merge(mergeTarget);        
        }

        scores.push_back(currentUseful);
    }
    
    deque<uint64_t> const& GetScores() const {
        return scores;
    }
};


void Output(deque<uint64_t> const& scores, uint64_t n) {

    uint64_t const maxUseful(n * (n - 1) / 2);
    auto ite = scores.end();
    ite--;

    // Output calculated value from back.
    while (ite != scores.cbegin()) {
        ite--; 
        cout << maxUseful - *ite << endl;
    };
   
    cout << maxUseful << endl;
}

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

    Output(islands.GetScores(), N);
    return 0;
}


