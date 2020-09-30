/*
    Copyright (c) 2007-2016 Contributors as noted in the AUTHORS file

    This file is part of libstub_helper, the Stub core engine in C++.

    libstub_hepler is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    As a special exception, the Contributors give you permission to link
    this library with independent modules to produce an executable,
    regardless of the license terms of these independent modules, and to
    copy and distribute the resulting executable under terms of your choice,
    provided that you also meet, for each linked independent module, the
    terms and conditions of the license of that module. An independent
    module is a module which is not derived from or based on this library.
    If you modify this library, you must extend this exception to your
    version of the library.

    libstub_hepler is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
    License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "stub_memory.h"

#include <map>
#include "string.h"

#include "stdio.h"

#define LOG(A) printf("%s\n",A);
#define LOG_D(A) printf("%d\n",A);
#define LOG_P(A) printf("%p\n",A);

namespace dtest {

class StubInfo
{
public:
    void* value;
    size_t size;
};

typedef std::map<std::string, StubInfo>::iterator StubMemoryIterator;
static std::map<std::string, StubInfo> stub_container;

StubMemory::StubMemory()
{

}

StubMemory::~StubMemory()
{

}

void StubMemory::set(const char* key, int value) const
{
    set(key, (void*)&value, sizeof(int));
}

void StubMemory::set(const char* key, void* value, size_t size) const
{
    void* memory = malloc(size);
    if ( !memory )
    {
        LOG("malloc failed!")
        return;
    }
    memset(memory, 0x0, size);
    memcpy(memory, value, size);

    stub_container.insert(std::make_pair(std::string(key), StubInfo{memory, size}));
}

bool StubMemory::get(const char *key, void **value) const
{
    StubMemoryIterator it = stub_container.find(std::string(key));
    if ( it == stub_container.end())
    {
        return false;
    }

    (*value) = it->second.value;
    return true;
}

void StubMemory::clear() const
{
    for (StubMemoryIterator it = stub_container.begin(); it != stub_container.end(); it
         ++)
    {
        free(it->second.value);
        it->second.value = NULL;
        it->second.size = 0;
    }
    stub_container.clear();
}

} // namespace dtest
