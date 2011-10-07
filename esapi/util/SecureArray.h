/**
* OWASP Enterprise Security API (ESAPI)
*
* This file is part of the Open Web Application Security Project (OWASP)
* Enterprise Security API (ESAPI) project. For details, please see
* http://www.owasp.org/index.php/ESAPI.
*
* Copyright (c) 2011 - The OWASP Foundation
*
* @author Kevin Wall, kevin.w.wall@gmail.com
* @author Jeffrey Walton, noloader@gmail.com
*
*/

#pragma once

#include "EsapiCommon.h"
#include "util/zAllocator.h"

#include <vector>
#include <boost/shared_ptr.hpp>

namespace esapi
{
  template <typename T>
  class ESAPI_EXPORT SecureArray
  {
  public:

    typedef typename std::vector< T, zallocator<T> > SecureVector;
    typedef typename zallocator<T>::size_type size_type;

    typedef typename SecureVector::value_type value_type;
    typedef typename SecureVector::pointer pointer;
    typedef typename SecureVector::const_pointer const_pointer;

    typedef typename SecureVector::iterator iterator;
    typedef typename SecureVector::const_iterator const_iterator;
    typedef typename SecureVector::reverse_iterator reverse_iterator;
    typedef typename SecureVector::const_reverse_iterator const_reverse_iterator;

    typedef typename SecureVector::reference reference;
    typedef typename SecureVector::const_reference const_reference;

  public:

    // Construction
    explicit SecureArray(size_type cnt = 0, const T& value = T());
    explicit SecureArray(const T* ptr, size_t cnt);
    template <typename InputIterator>
      SecureArray(InputIterator first, InputIterator last);

    // Destruction
    ~SecureArray() { }

    // Iterators
    iterator begin();
    const_iterator begin() const;

    iterator end();
    const_iterator end() const;

    reference front();
    const_reference front() const;

    reference back();
    const_reference back() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;

    // Copy and assignment
    SecureArray(const SecureArray& sa);
    SecureArray& operator=(const SecureArray& sa);

    // Size and capacity
    size_t max_size() const;
    size_t capacity() const;
    void reserve(size_t cnt = 0);

    bool empty() const;
    size_type size() const;
    size_type length() const;
    void resize(size_type sz, T c = T());

    void clear();

    // Member functions
    const T& operator[](size_t pos) const;
    T& operator[](size_t pos);
    const T& at(size_t pos) const;
    T& at(size_t pos);

    // Value added
    T* data();
    const T* data() const;

    void assign(size_type n, const T& u);
    void assign(const T* ptr, size_t cnt);
    template <typename InputIterator>
      void assign(InputIterator first, InputIterator last);

    iterator insert(iterator pos, const T& x);
    void insert(iterator pos, size_type n, const T& x);
    void insert(iterator pos, const T* ptr, size_t cnt);
    template <typename InputIterator>
      void insert(iterator pos, InputIterator first, InputIterator last);

    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);

    void swap(SecureArray& sa);

    void pop_back();
    void push_back(const T& x);

  private:

    // Helpers to validate parameters in constructors
    inline SecureVector* create_secure_array(size_type cnt, const T& value);
    inline SecureVector* create_secure_array(const T* ptr, size_t cnt);
    template <typename InputIterator>
      inline SecureVector* create_secure_array(InputIterator first, InputIterator last);

  private:

    boost::shared_ptr<SecureVector> m_vector;
  };

  // Non-member swap
  template <typename T>
  void swap(SecureArray<T>& a, SecureArray<T>& b);

  // Convenience
  typedef SecureArray<byte> SecureByteArray;
  typedef SecureArray<int> SecureIntArray;

} // NAMESPACE

namespace std
{
  // Effective C++, Item 25, pp 106-112
  template<>
  void swap(esapi::SecureByteArray& a, esapi::SecureByteArray& b);
  template<>
  void swap(esapi::SecureIntArray& a, esapi::SecureIntArray& b);
}
