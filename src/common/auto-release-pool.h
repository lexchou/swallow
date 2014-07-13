#ifndef AUTO_RELEASE_POOL_H
#define AUTO_RELEASE_POOL_H
#include "swift_conf.h"
#include <set>
SWIFT_NS_BEGIN

class Node;
class AutoReleasePool
{
public:
    AutoReleasePool();
    ~AutoReleasePool();

public:
    void purgeAll();
    void removeAll();
    void remove(Node* node);
    void add(Node* node);
    int numObjects()const;
private:
    std::set<Node*> objs;
};



SWIFT_NS_END


#endif//AUTO_RELEASE_POOL_H