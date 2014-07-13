#include "auto-release-pool.h"
#include "ast/node.h"

USE_SWIFT_NS



AutoReleasePool::AutoReleasePool()
{

}
AutoReleasePool::~AutoReleasePool()
{
    purgeAll();
}

void AutoReleasePool::purgeAll()
{
    while(!objs.empty())
    {
        Node* obj = *objs.begin();
        obj->setAutoReleasePool(NULL);
        delete obj;
    }
}
void AutoReleasePool::removeAll()
{
    while(!objs.empty())
    {
        Node* obj = *objs.begin();
        obj->setAutoReleasePool(NULL);
    }
}
void AutoReleasePool::remove(Node* node)
{
    objs.erase(node);
    node->setAutoReleasePool(NULL);
}
void AutoReleasePool::add(Node* node)
{
    objs.insert(node);
    node->setAutoReleasePool(NULL);
}
int AutoReleasePool::numObjects()const
{
    return objs.size();
}
