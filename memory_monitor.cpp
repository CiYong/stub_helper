#include "memory_monitor.h"

#include <map>

#include "logger.h"

namespace dtest {

class MemoryInfo
{
public:
    size_t size;
    const char* filename;
    size_t line;
    const char* function;
};

typedef std::map<void*, MemoryInfo>::iterator StubMemoryIterator;
static std::map<void*, MemoryInfo> memory_container;

MemoryMonitor::MemoryMonitor()
{

}

MemoryMonitor::~MemoryMonitor()
{

}

void* MemoryMonitor::allocate(size_t size)
{
    return malloc(size);
}

void* MemoryMonitor::allocate(size_t size, const char *filename, const size_t line, const char* function)
{
    void* memory = malloc(size);
    if ( !memory )
    {
        return NULL;
    }

    memory_container.insert(std::make_pair(memory, MemoryInfo{size, filename, line, function}));

    m_usage += size;

    return memory;
}

void MemoryMonitor::deallocate(void *memory)
{
    StubMemoryIterator it = memory_container.find(memory);
    if ( it != memory_container.end())
    {
        memory_container.erase(memory);
        free(memory);
    }
}

void MemoryMonitor::dump() const
{
    size_t leak = 0;
    for ( StubMemoryIterator it = memory_container.begin(); it != memory_container.end(); it++ )
    {
        Logger::instance()->log("filename=[%s], line=[%d], function=[%s], size=[%u], address=[%p]\n",
                                it->second.filename,
                                it->second.line,
                                it->second.function,
                                it->second.size,
                                it->first);
        leak += it->second.size;
    }

    Logger::instance()->log("===============Memory Usage===============\n");
    Logger::instance()->log("Memory usage=[%u], memory leak=[%u]\n", m_usage, leak);
    Logger::instance()->log("===============Memory Usage===============\n");
}

} // namespace dtest
