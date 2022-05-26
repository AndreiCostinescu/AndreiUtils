//
// Created by Andrei on 23.05.22.
//

#ifndef ANDREIUTILS_UNIONFIND_H
#define ANDREIUTILS_UNIONFIND_H

#include <vector>

namespace AndreiUtils {
    class UnionFind {
    public:
        explicit UnionFind(size_t n = 0);

        size_t size() const;

        const size_t &numberOfDistinctComponents() const;

        void add();

        size_t find(size_t id);

        bool find(size_t id1, size_t id2);

        bool unite(size_t id1, size_t id2);

        const std::vector<size_t> &getParents() const;

        const std::vector<size_t> &getSizes() const;

        size_t getComponentSize(size_t id);

    protected:
        virtual void setParentIndex(size_t index, size_t parentIndex);

        bool uniteImpl(size_t root1, size_t root2);

        std::vector<size_t> parents;
        std::vector<size_t> sizes;
        size_t nrDistinctComponents{};
    };
}

#endif //ANDREIUTILS_UNIONFIND_H
