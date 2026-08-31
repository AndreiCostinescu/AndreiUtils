/*
 * Copyright 2026 AndreiUtils Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// Created by Andrei on 23.05.22.
//

#ifndef ANDREIUTILS_UNIONFIND_H
#define ANDREIUTILS_UNIONFIND_H

#include <cstddef>
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
} // namespace AndreiUtils

#endif // ANDREIUTILS_UNIONFIND_H
