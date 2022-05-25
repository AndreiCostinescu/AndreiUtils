//
// Created by Andrei on 24-May-22.
//

#include <AndreiUtils/classes/UnionFind.h>
#include <stdexcept>
#include <string>

using namespace AndreiUtils;
using namespace std;

UnionFind::UnionFind(size_t n) : parents(n), sizes(n) {
    this->nrDistinctComponents = n;
    for (size_t i = 0; i < n; i++) {
        this->parents[i] = i;
        this->sizes[i] = 1;
    }
}

size_t UnionFind::size() const {
    return this->parents.size();
}

const size_t &UnionFind::numberOfDistinctComponents() const {
    return this->nrDistinctComponents;
}

void UnionFind::add() {
    // the size of the union-find data structure is always in parents.size()
    size_t n = this->size();
    this->parents.push_back(n);
    this->sizes.push_back(1);
    this->nrDistinctComponents++;
}

size_t UnionFind::find(size_t id) {
    if (id >= this->size()) {
        throw runtime_error("Index " + to_string(id) + " out of bounds (" + to_string(this->size()) + ")!");
    }
    if (this->parents[id] != id) {
        this->setParentIndex(id, find(this->parents[id]));
    }
    return this->parents[id];

    // chase parent of current element until it reaches root.
    while (this->parents[id] != id) {
        this->parents[id] = this->parents[this->parents[id]];
        id = this->parents[id];
    }
    return id;
}

bool UnionFind::find(size_t id1, size_t id2) {
    return (find(id1) == find(id2));
}

bool UnionFind::unite(size_t id1, size_t id2) {
    size_t root1 = find(id1);
    size_t root2 = find(id2);
    return this->uniteImpl(root1, root2);
}

const vector<size_t> &UnionFind::getParents() const {
    return this->parents;
}

const vector<size_t> &UnionFind::getSizes() const {
    return this->sizes;
}

void UnionFind::setParentIndex(size_t index, size_t parentIndex) {
    this->parents[index] = parentIndex;
}

bool UnionFind::uniteImpl(size_t root1, size_t root2) {
    if (root1 == root2) {
        return false;
    }

    if (this->sizes[root1] < this->sizes[root2]) {
        this->setParentIndex(root1, root2);
        this->sizes[root2] += this->sizes[root1];
    } else {
        this->setParentIndex(root2, root1);
        this->sizes[root1] += this->sizes[root2];
    }
    this->nrDistinctComponents--;
    return true;
}
