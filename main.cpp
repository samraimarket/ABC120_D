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

class UnionFind {
    map<int, int> parents;
    map<int, set<int>> groups;

    bool const IsSingle(int id) {
        return (groups.find(id) == groups.cend()) && (parents[id] == id);
    }

    size_t const GetCurrentSize(int id) {
        return (IsSingle(id)) ? 1 : groups[id].size();
    }

    struct MergeTarget {
        int const parent;
        int const other;
        MergeTarget(int x, int y) : parent(min(x, y)), other(max(x, y)) {}        
    };
    
    void merge(MergeTarget const& targets) {
        
        const int parent = targets.parent;
        const int other  = targets.other;

        if (IsSingle(parent)) {
            groups[parent] = set<int>();
            groups[parent].insert(parent);
        }
       
        if (IsSingle(other)) {
           
            groups[parent].insert(other);
            parents[other] = parent;

        // When the other is not single, owns group
        } else {
         
            // Change each parent elements of group other   
            for (int i : groups[other]) parents[i] = parent;

            // Then merge all elements in other group to parent group.
            groups[parent].insert(groups[other].begin(), groups[other].end());
            groups.erase(other);
        }
    }

public :

    UnionFind(size_t N) {
        for (size_t i = 0; i <= N; i++) parents[i] = i;
    }
  
    uint64_t TryMerge(int from, int to) {
   
        if (parents.find(from) == parents.cend() || parents.find(to) == parents.cend()) {
            cerr << "Not such element of " << from << " or " << to << "!!" << endl;
        }

        // If They are in same group, the useful point will not be increased.
        if (parents[from] == parents[to]) return 0;

        MergeTarget const targets(parents[from], parents[to]);

        // Save delta size before merge them.
        size_t const deltaSize =  GetCurrentSize(targets.parent) * GetCurrentSize(targets.other); 

        merge(targets);        

        // Otherwise try to calculate current useful value  and adjust group.
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

        currentUseful +=  unionFind.TryMerge(from, to);        
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
    for (unsigned int i = M; 0 < i; i--) islands.Input(A[i - 1], B[i - 1]);

    islands.Calculate();
    islands.Output();
    return 0;
}


