// Copyright 2016 The Draco Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef DRACO_CORE_DRACO_INDEX_TYPE_VECTOR_H_
#define DRACO_CORE_DRACO_INDEX_TYPE_VECTOR_H_

#include <cstddef>
#include <vector>

#include "core/draco_index_type.h"

namespace draco {

template <class IndexTypeT, class ValueTypeT>
class BasicIndexType {
public:
  typedef ValueTypeT& reference;
  typedef const ValueTypeT& const_reference;

  BasicIndexType() : data_(nullptr), data_size_(0) {}
  
  virtual void clear() = 0;
  virtual void reserve(size_t size) = 0;
  virtual void resize(size_t size) = 0;
  virtual void resize(size_t size, const ValueTypeT &val) = 0;
  virtual void assign(size_t size, const ValueTypeT &val) = 0;
  virtual void push_back(const ValueTypeT &val) = 0;
  virtual void push_back(ValueTypeT &&val) = 0;

  size_t size() const { return data_size_; }

  inline reference operator[](const IndexTypeT &index) {
    return data_[index.value()];
  }
  inline const_reference operator[](const IndexTypeT &index) const {
    return data_[index.value()];
  }
  inline reference at(const IndexTypeT &index) {
    return data_[index.value()];
  }
  inline const_reference at(const IndexTypeT &index) const {
    return data_[index.value()];
  }
  const ValueTypeT *data() const { return data_; }

protected:
  ValueTypeT *data_;
  size_t data_size_;
};

template <class IndexTypeT, class ValueTypeT>
class IndexTypeFixedSize
  : public BasicIndexType<IndexTypeT, ValueTypeT>
{
public:
  IndexTypeFixedSize() {}
  explicit IndexTypeFixedSize(ValueTypeT *data, size_t size) { data_ = data; data_size_ = size; }
  
  void clear() override { throw std::logic_error("IndexTypeFixedSize is fixed size");  }
  void reserve(size_t size) override { throw std::logic_error("IndexTypeFixedSize is fixed size"); }
  void resize(size_t size) override { throw std::logic_error("IndexTypeFixedSize is fixed size"); }
  void resize(size_t size, const ValueTypeT &val) override { throw std::logic_error("IndexTypeFixedSize is fixed size"); }
  void assign(size_t size, const ValueTypeT &val) override { throw std::logic_error("IndexTypeFixedSize is fixed size"); }
  void push_back(const ValueTypeT &val) override { throw std::logic_error("IndexTypeFixedSize is fixed size"); }
  void push_back(ValueTypeT &&val) override { throw std::logic_error("IndexTypeFixedSize is fixed size"); }
};

// A wrapper around the standard std::vector that supports indexing of the
// vector entries using the strongly typed indices as defined in
// draco_index_type.h .
// TODO(ostava): Make the interface more complete. It's currently missing
// features such as iterators.
template <class IndexTypeT, class ValueTypeT>
class IndexTypeVector
 : public BasicIndexType<IndexTypeT, ValueTypeT>
{
 public:
  IndexTypeVector() {}
  explicit IndexTypeVector(size_t size) : vector_(size) { update();  }
  IndexTypeVector(size_t size, const ValueTypeT &val) : vector_(size, val) { update(); }

  void clear() override { vector_.clear(); data_ = nullptr; data_size_ = 0; }
  void reserve(size_t size) override { vector_.reserve(size); update(); }
  void resize(size_t size) override { vector_.resize(size); update(); }
  void resize(size_t size, const ValueTypeT &val) override { vector_.resize(size, val); update(); }
  void assign(size_t size, const ValueTypeT &val) override { vector_.assign(size, val); update(); }
  
  void push_back(const ValueTypeT &val) override { vector_.push_back(val); update(); }
  void push_back(ValueTypeT &&val) override { vector_.push_back(std::move(val)); update(); }

  IndexTypeVector<IndexTypeT, ValueTypeT>& operator =(IndexTypeVector<IndexTypeT, ValueTypeT> const &rhs) { vector_ = rhs.vector_; update(); return *this; }

 private:
   inline void update() { data_ = vector_.data(), data_size_ = vector_.size(); }
   
 private:
  std::vector<ValueTypeT> vector_;
};

}  // namespace draco

#endif  // DRACO_CORE_DRACO_INDEX_TYPE_VECTOR_H_
