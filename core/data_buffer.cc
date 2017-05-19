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
#include "core/data_buffer.h"

namespace draco {

DataBuffer::DataBuffer() : data_(nullptr), data_size_(0) {}
DataBuffer::~DataBuffer() {}

bool DataBuffer::Update(const void *data, int64_t size) {
  if(size < 0)
    return false;

  resize(size);

  if(data)
    std::memcpy(data_, data, size);
  
  descriptor_.buffer_update_count++;
  return true;
}

bool DataBuffer::Update(const void *data, int64_t size, int64_t offset) {
  if(data == nullptr) {
    if(size + offset < 0)
      return false;
    // If no data is provided, just resize the buffer.
    resize(size + offset);
  }
  else {
    if(size < 0)
      return false;
    if(size + offset > static_cast<int64_t>(data_size_))
      resize(size + offset);
    std::memcpy(data_ + offset, data, size);
  }
  descriptor_.buffer_update_count++;
  return true;
}


void DataBuffer::WriteDataToStream(std::ostream &stream) {
  if (data_size_ == 0)
    return;
  stream.write(reinterpret_cast<char *>(data_), data_size_);
}

VectorDataBuffer::VectorDataBuffer() {}

VectorDataBuffer::~VectorDataBuffer() {}

void VectorDataBuffer::resize(int64_t size)
{
  vector_.resize(size);
  data_ = vector_.data();
  data_size_ = size;
}




}  // namespace draco
