#ifndef ARRAY_H
#define ARRAY_H

#include "debug.h"
#include <algorithm>

//#define ARRAY_CHECK
namespace near {

#ifdef ARRAY_CHECK
template <typename T> inline void _check_array_access(const T& a, int i) { error(i >= 0 && i < a.size(), "out of bounds"); }
template <typename T> inline void _check_array_capacity(const T& a, int i) { error(i >= 0 && i < a.capacity(), "out of bounds"); }
template <typename T> inline void _check_array_newcapacity(const T& a, int c) { error(c >= 0 && c >= a.capacity(), "out of bounds"); }
template <typename T> inline void _check_array2_access(const T& a, int i, int j) { error(i >= 0 && i < a.size1() && j >= 0 && j < a.size2(), "out of bounds"); }
#else
// disable checks
#define _check_array_access(a,i)
#define _check_array_capacity(a,i)
#define _check_array_newcapacity(a,c)
#define _check_array2_access(a,i,j)
#endif

// forward declaration
template <typename> struct darray;
template <typename> struct sarray;
template <typename,int> struct barray;
template <typename,int> struct carray;
template <typename> struct sarray2;
template <typename,int,int> struct carray2;

template <typename T>
struct sarray {
    typedef T   value_type;
    typedef T*  iterator;

    sarray() { _l = 0; _d = 0; }
    explicit sarray(int l) { _l = 0; _d = 0; resize(l); }
    explicit sarray(int l, const T& v) { _l = 0; _d = 0; assign(l, v); }
    explicit sarray(int l, const T* v) { _l = 0; _d = 0; assign(l, v); }
    sarray(const sarray<T>& v) { _l = 0; _d = 0; assign(v); }
    explicit sarray(const darray<T>& v) { _l = 0; _d = 0; assign(v); }
    
    ~sarray() { clear(); }
    
    sarray<T>& operator= (const sarray<T>& v) { assign(v); return *this; }
    sarray<T>& operator= (const darray<T>& v) { assign(v); return *this; }
    
    iterator begin()    { return _d; }
    iterator end()      { return _d + _l; }
    
    int size() const { return _l; }
    bool empty() const { return _l == 0; }
    
    void clear() { if(_d) delete [] _d; _l = 0; _d = 0; }
    void resize(int l) { if(l == _l) return; if(_d) delete [] _d; _l = l; _d = new T[_l]; }
    
    T& at(int i) { _check_array_access(*this, i); return _d[i]; }
    const T& at(int i) const { _check_array_access(*this, i); return _d[i]; }
    T& operator[](int i) { _check_array_access(*this, i); return _d[i]; }
    const T& operator[](int i) const { _check_array_access(*this, i); return _d[i]; }
    
    T* data() { return _d; }
    const T* data() const { return _d; }
    
    void assign(const T& v) { for(int i = 0; i < size(); i ++) _d[i] = v; }
    void assign(int l, const T& v) { resize(l); assign(v); }
    void assign(int l, const T* v) { resize(l); for(int i = 0; i < size(); i ++) _d[i] = v[i]; }
    void assign(const sarray<T>& v) { resize(v.size()); for(int i = 0; i < size(); i ++) _d[i] = v._d[i]; }
    void assign(const darray<T>& v) { resize((int)v.size()); for(int i = 0; i < size(); i ++) _d[i] = v[i]; }
    
    int _l;
    T* _d;
};

template <typename T>
struct darray {
    typedef T   value_type;
    typedef T*  iterator;
    
    darray() { _l = 0; _c = 0; _d = 0; }
    explicit darray(int l) { _l = 0; _c = 0; _d = 0; assign(l, T()); }
    explicit darray(int l, const T& v) { _l = 0; _c = 0; _d = 0; assign(l, v); }
    explicit darray(int l, const T* v) { _l = 0; _c = 0; _d = 0; assign(l, v); }
    darray(const darray<T>& v) { _l = 0;_c = 0;  _d = 0; assign(v); }
    
    ~darray() { if(_d) delete [] _d; _l = 0; _c = 0; _d = 0; }
    
    darray<T>& operator= (const darray<T>& v) { assign(v); return *this; }    
    
    iterator begin()    { return _d; }
    iterator end()      { return _d + _l; }
    
    int size() const { return _l; }
    int capacity() const { return _c; }
    bool empty() const { return _l == 0; }
    
    void clear() { if(_d) delete [] _d; _l = 0; _c = 0; _d = 0; } // this is different than STL
    void resize(int l) { 
        if(l == _l) return;
        if(l > _c) _growcapacity(l);
        if(l >_l) for(int i = _l; i < l; i ++) _d[i] = T();
        _l = l;
    }
    void reserve(int c) { if(c <= _c) return; _growcapacity(c); }
    
    void add(const T& v) { if(_l == _c) _growcapacity(std::max(_c*2,1)); _d[_l] = v; _l ++; }
    void push_back(const T& v) { add(v); }
    
    T& at(int i) { _check_array_access(*this, i); return _d[i]; }
    const T& at(int i) const { _check_array_access(*this, i); return _d[i]; }
    T& operator[](int i) { _check_array_access(*this, i); return _d[i]; }
    const T& operator[](int i) const { _check_array_access(*this, i); return _d[i]; }
    
    T* data() { return _d; }
    const T* data() const { return _d; }
    
    void assign(const T& v) { for(int i = 0; i < size(); i ++) _d[i] = v; }
    void assign(int l, const T& v) { resize(l); assign(v); }
    void assign(int l, const T* v) { resize(l); for(int i = 0; i < size(); i ++) _d[i] = v[i]; }
    void assign(const darray<T>& v) { resize(v.size()); for(int i = 0; i < size(); i ++) _d[i] = v._d[i]; }
    
    void _growcapacity(int c) {
        _check_array_newcapacity(*this, c);
        if(_d) {
            T* d = new T[c];
            for(int i = 0; i < _l; i ++) d[i] = _d[i];
            std::swap(d,_d);
            std::swap(c,_c);
            delete[] d;
        } else {
            _d = new T[c];
            _c = c;
            _l = 0;
        }
    }
    
    int _l, _c;
    T* _d;
};

template <typename T, int N>
struct carray {
    typedef T   value_type;
    typedef T*  iterator;
    
    iterator begin()    { return _d; }
    iterator end()      { return _d + _l; }
    
    int size() const { return _l; }
    bool empty() const { return _l == 0; }
    
    T& at(int i) { _check_array_access(*this, i); return _d[i]; }
    const T& at(int i) const { _check_array_access(*this, i); return _d[i]; }
    T& operator[](int i) { _check_array_access(*this, i); return _d[i]; }
    const T& operator[](int i) const { _check_array_access(*this, i); return _d[i]; }
    
    T* data() { return _d; }
    const T* data() const { return _d; }
    
    void assign(const T& v) { for(int i = 0; i < size(); i ++) _d[i] = v; }
    
    static const int _l = N;
    T _d[_l];
};

template <typename T, int N>
struct barray {
    typedef T   value_type;
    typedef T*  iterator;
    
    barray() { _l = 0; }
    explicit barray(int l) { _l = 0; _d = 0; resize(l); }
    explicit barray(int l, const T& v) { _l = 0; _d = 0; assign(l, v); }
    
    iterator begin()    { return _d; }
    iterator end()      { return _d + _l; }
    
    int size() const { return _l; }
    bool empty() const { return _l == 0; }
    
    void clear() { _l = 0; }
    void resize(int l) { error_if_not(l >= 0 && l <= _c, "out of bounds"); _l = l; }
        
    void add(const T& v) { _check_array_capacity(*this, _l); _d[_l] = v; _l ++; }
    T& at(int i) { _check_array_access(*this, i); return _d[i]; }
    const T& at(int i) const { _check_array_access(*this, i); return _d[i]; }
    T& operator[](int i) { _check_array_access(*this, i); return _d[i]; }
    const T& operator[](int i) const { _check_array_access(*this, i); return _d[i]; }
    
    T* data() { return _d; }
    const T* data() const { return _d; }
    
    void assign(const T& v) { for(int i = 0; i < size(); i ++) _d[i] = v; }
    void assign(int l, const T& v) { resize(l); assign(v); }
    
    int _l;
    const static int _c = N;
    T _d[_c];
};

template <typename T>
struct sarray2 {
    typedef T value_type;
    
    sarray2() { _l1 = 0; _l2 = 0; _d = 0; }
    explicit sarray2(int l1, int l2) { _l1 = 0; _l2 = 0; _d = 0; realloc(l1, l2); }
    explicit sarray2(int l1, int l2, const T& v) { _l1 = 0; _l2 = 0; _d = 0; realloc(l1, l2); assign(v); }
    sarray2(const sarray2<T>& v) { _l1 = 0; _l2 = 0; _d = 0; assign(v); }
    
    ~sarray2() { clear(); }
    
    sarray2<T>& operator= (const sarray2<T>& v) { assign(v); return *this; }
    
    int size() const { return _l1 * _l2; }
    int size1() const { return _l1; }
    int size2() const { return _l2; }
    bool empty() const { return _l1 == 0 || _l2 == 0; }

    int width() const { return size1(); }
    int height() const { return size2(); }
    
    void clear() { if(_d) delete [] _d; _l1 = 0; _l2 = 0; _d = 0; }
    void realloc(int l1, int l2) { 
        if(l1 == _l1 && l2 == _l2) return; 
        if(_d) delete [] _d; 
        _l1 = l1; _l2 = l2; _d = new T[_l1 * _l2]; 
    }
    void resize(int l1, int l2) {
        if(l1 == _l1 && l2 == _l2) return; 
        if(_l1 == 0 && _l2 == 0) return realloc(l1, l2);
        sarray2<T> newarray(l1, l2);
        for(int j = 0; j < std::min(_l2,l2); j ++) {
            for(int i = 0; i < std::min(_l1,l1); i ++) { 
                newarray.at(i,j) = at(i,j);
            }
        }
        std::swap(newarray._d,_d);
        std::swap(newarray._l1,_l1);
        std::swap(newarray._l2,_l2);
    }
    
    T& at(int i) { _check_array_access(*this, i); return _d[i]; }
    const T& at(int i) const { _check_array_access(*this, i); return _d[i]; }
    T& at(int i, int j) { _check_array2_access(*this, i, j); return _d[j*_l1+i]; }
    const T& at(int i, int j) const { _check_array2_access(*this, i, j); return _d[j*_l1+i]; }
    T& operator[](int i) { _check_array_access(*this, i); return _d[i]; }
    const T& operator[](int i) const { _check_array_access(*this, i); return _d[i]; }
    
    void assign(const T& v) { for(int i = 0; i < size(); i ++) _d[i] = v; }
    void assign(int l1, int l2, const T& v) { realloc(l1, l2); assign(v); }
    void assign(const sarray2<T>& v) { realloc(v.size1(),v.size2()); 
        for(int i = 0; i < size(); i ++) _d[i] = v._d[i]; }
    
    int _l1, _l2;
    T* _d;
};

template <typename T, int N1, int N2>
struct carray2 {
    typedef T value_type;
    
    int size() const { return _l1 * _l2; }
    int size1() const { return _l1; }
    int size2() const { return _l2; }
    bool empty() const { return _l1 == 0 || _l2 == 0; }
    
    T& at(int i) { _check_array_access(*this, i); return _d[i]; }
    const T& at(int i) const { _check_array_access(*this, i); return _d[i]; }
    T& at(int i, int j) { _check_array2_access(*this, i, j); return _d[j*_l1+i]; }
    const T& at(int i, int j) const { _check_array2_access(*this, i, j); return _d[j*_l1+i]; }
    T& operator[](int i) { _check_array_access(*this, i); return _d[i]; }
    const T& operator[](int i) const { _check_array_access(*this, i); return _d[i]; }
    
    static const int _l1 = N1;
    static const int _l2 = N2;
    T _d[_l1*_l2];
};

template<typename T1, typename T2>
struct sbimap_elem {
    T1 t1;
    T2 t2;
};

template<typename T1, typename T2>
inline T2 sbimap_find2(const sarray<sbimap_elem<T1,T2> >& m, const T1& t1) {
    for(int i = 0; i < m.size(); i ++) if(m[i].t1 == t1) return m[i].t2;
    error("unknown value");
    return T2();
}

template<typename T1, typename T2>
inline T1 sbimap_find1(const sarray<sbimap_elem<T1,T2> >& m, const T2& t2) {
    for(int i = 0; i < m.size(); i ++) if(m[i].t2 == t2) return m[i].t1;
    error("unknown value");
    return T1();
}
    
}

#endif
