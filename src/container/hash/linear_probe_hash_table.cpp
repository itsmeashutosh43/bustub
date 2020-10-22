//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// linear_probe_hash_table.cpp
//
// Identification: src/container/hash/linear_probe_hash_table.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "common/exception.h"
#include "common/logger.h"
#include "common/rid.h"
#include "container/hash/linear_probe_hash_table.h"

namespace bustub {

template <typename KeyType, typename ValueType, typename KeyComparator>
HASH_TABLE_TYPE::LinearProbeHashTable(const std::string &name, BufferPoolManager *buffer_pool_manager,
                                      const KeyComparator &comparator, size_t num_buckets,
                                      HashFunction<KeyType> hash_fn)
    : buffer_pool_manager_(buffer_pool_manager), comparator_(comparator), hash_fn_(std::move(hash_fn)) {
    
      auto page = buffer_pool_manager_->NewPage(&header_page_id_, nullptr);
      auto header_page = reinterpret_cast<HashTableHeaderPage *>(page->GetData());
      
      header_page->SetSize(num_buckets);
      buffer_pool_manager_->FlushPage(header_page_id_,nullptr);
      size_t num_pages = num_buckets /BLOCK_ARRAY_SIZE;



      for (size_t idx = 0 ; idx < num_pages ; idx++)
      {
        page_id_t next_id;
        Page* new_page = buffer_pool_manager_->NewPage(&next_id , nullptr);
        assert(new_page != nullptr);

        buffer_pool_manager_->UnpinPage(next_id , true);
        buffer_pool_manager_->FlushPage(next_id, nullptr);

        header_page->AddBlockPageId(next_id);

      }
      

    }

/*****************************************************************************
 * SEARCH
 *****************************************************************************/
template <typename KeyType, typename ValueType, typename KeyComparator>
bool HASH_TABLE_TYPE::GetValue(Transaction *transaction, const KeyType &key, std::vector<ValueType> *result) {
  auto header_page = reinterpret_cast<HashTableHeaderPage*>(buffer_pool_manager_->FetchPage(header_page_id_));

  auto expected_index = this->hash_fn_.GetHash(key) % header_page->GetSize();
  auto starting_point = true;

  for(auto i = expected_index; ; i = ((i + 1) % header_page->GetSize()))
  {
    if (i == expected_index)
    {
      if (starting_point)
      {
        starting_point = false;
      }
      else{
        break;
      }
    }
    size_t current_page_id = i / BLOCK_ARRAY_SIZE;
    size_t slot_id = i % BLOCK_ARRAY_SIZE;

    auto page = this->buffer_pool_manager_->FetchPage(header_page->GetBlockPageId(current_page_id));
    auto block = reinterpret_cast<HashTableBlockPage<KeyType,ValueType,KeyComparator>*>(page->GetData());

    if (!block->IsOccupied(slot_id)){break;}

    if (block->IsReadable(slot_id))
    {
      result->push_back(block->ValueAt(slot_id));
    }
  }

  if (result->size() > 0) return true;
  return false;
  
}
/*****************************************************************************
 * INSERTION
 *****************************************************************************/
template <typename KeyType, typename ValueType, typename KeyComparator>
bool HASH_TABLE_TYPE::Insert(Transaction *transaction, const KeyType &key, const ValueType &value) {
  /*
  Insert into hash table
  Insert Key value pair to the hashtable. 
  1) get header page
  2) get corresponding block page and location
  */
  auto header_page = reinterpret_cast<HashTableHeaderPage*>(buffer_pool_manager_->FetchPage(header_page_id_,nullptr));
  auto expected_index = this->hash_fn_.GetHash(key) % header_page->GetSize();
  
  auto starting_point = true;

  for(auto i = expected_index; ; i = ((i + 1) % header_page->GetSize()))
  {
    if (i == expected_index)
    {
      if (starting_point)
      {
        starting_point = false;
      }
      else{
        break;
      }
    }

    size_t current_page_id = i / BLOCK_ARRAY_SIZE;
    size_t slot_id = i % BLOCK_ARRAY_SIZE;
    auto page = this->buffer_pool_manager_->FetchPage(header_page->GetBlockPageId(current_page_id));
    auto block = reinterpret_cast<HashTableBlockPage<KeyType,ValueType,KeyComparator>*>(page->GetData());

    auto success = block->Insert(slot_id, key, value);
    if (success)
    {
      this->buffer_pool_manager_->FlushPage(page->GetPageId());
      return true;
    }
  }

  return false;


}

/*****************************************************************************
 * REMOVE
 *****************************************************************************/
template <typename KeyType, typename ValueType, typename KeyComparator>
bool HASH_TABLE_TYPE::Remove(Transaction *transaction, const KeyType &key, const ValueType &value) {
  return false;
}

/*****************************************************************************
 * RESIZE
 *****************************************************************************/
template <typename KeyType, typename ValueType, typename KeyComparator>
void HASH_TABLE_TYPE::Resize(size_t initial_size) {}

/*****************************************************************************
 * GETSIZE
 *****************************************************************************/
template <typename KeyType, typename ValueType, typename KeyComparator>
size_t HASH_TABLE_TYPE::GetSize() {
  auto header_page = reinterpret_cast<HashTableHeaderPage*>(buffer_pool_manager_->FetchPage(header_page_id_,nullptr));
  return header_page->GetSize();
}




template class LinearProbeHashTable<int, int, IntComparator>;

template class LinearProbeHashTable<GenericKey<4>, RID, GenericComparator<4>>;
template class LinearProbeHashTable<GenericKey<8>, RID, GenericComparator<8>>;
template class LinearProbeHashTable<GenericKey<16>, RID, GenericComparator<16>>;
template class LinearProbeHashTable<GenericKey<32>, RID, GenericComparator<32>>;
template class LinearProbeHashTable<GenericKey<64>, RID, GenericComparator<64>>;

}  // namespace bustub
