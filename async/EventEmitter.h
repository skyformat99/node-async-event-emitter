// Copyright 2015 Markus Tzoe

// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

// http://www.apache.org/licenses/LICENSE-2.0

// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef EventEmitter_h
#define EventEmitter_h

#include "Argument.h"
#include <string>

namespace async {

class EventEmitter {
public:
    EventEmitter() {}
    virtual ~EventEmitter() {}

    virtual bool notify(const std::string& event, const Argument&) = 0; // event, FIFO
    virtual bool prompt(const std::string& event, const Argument&) = 0; // event, LIFO
    virtual bool call(const Argument& argument) { return notify("", argument); } // callback

    template <class... Args>
    bool emit(const std::string& event, const Args&... args)
    {
        const unsigned size = sizeof...(Args);
        Argument m[size] = { args... };
        unsigned i = 1;
        Argument* ptr = &m[0];
        while (i < size) {
            auto p = new Argument{ m[i] };
            ptr->next(p);
            ptr = p;
            ++i;
        }
        return notify(event, m[0]);
    }

    template <class... Args>
    bool urge(const std::string& event, const Args&... args)
    {
        const unsigned size = sizeof...(Args);
        Argument m[size] = { args... };
        unsigned i = 1;
        Argument* ptr = &m[0];
        while (i < size) {
            auto p = new Argument{ m[i] };
            ptr->next(p);
            ptr = p;
            ++i;
        }
        return prompt(event, m[0]);
    }

    template <class... Args>
    bool operator()(const Args&... args)
    {
        const unsigned size = sizeof...(Args);
        Argument m[size] = { args... };
        unsigned i = 1;
        Argument* ptr = &m[0];
        while (i < size) {
            auto p = new Argument{ m[i] };
            ptr->next(p);
            ptr = p;
            ++i;
        }
        return this->call(m[0]);
    }
};

} // namespace async

#endif // EventEmitter_h
