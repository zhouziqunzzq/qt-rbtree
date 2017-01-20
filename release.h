#ifndef RELEASE_H
#define RELEASE_H

#include <typeinfo>

template <typename T> struct Cleaner {
   static void clean ( T x ) { //相当于递归基
   }
};

template <typename T> struct Cleaner<T*> {
   static void clean ( T* x ) {
      if ( x ) { delete x; } //如果其中包含指针，递归释放
   }
};

template <typename T> void release ( T x ) { Cleaner<T>::clean ( x ); }

#endif // RELEASE_H
