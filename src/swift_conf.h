#ifndef SWIFT_CONF_H
#define SWIFT_CONF_H

#define SWIFT_NS_BEGIN namespace Swift{
#define SWIFT_NS_END }
#define USE_SWIFT_NS using namespace Swift;


SWIFT_NS_BEGIN
/*
template<class T>
inline void SafeDelete(T*& val)
{
    T* v = val;
    val = (T*)0;
    if(v)
        delete v;
}

template<typename ContainerT>
inline void SafeDeleteAll(ContainerT& container)
{
    for(typename ContainerT::iterator iter = container.begin(); iter != container.end(); iter++)
    {
        delete *iter;
    }
    container.clear();
}
*/

SWIFT_NS_END

#endif//SWIFT_CONF_H
