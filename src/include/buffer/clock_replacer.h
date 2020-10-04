//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.h
//
// Identification: src/include/buffer/clock_replacer.h
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include <list>
#include <mutex>  // NOLINT
#include <vector>

#include <iostream>

#include "buffer/replacer.h"
#include "common/config.h"

namespace bustub {

/**
 * Structure to hold all information in a single frame 
 */
struct Frame{
  frame_id_t id;
  bool ref_id;
  Frame* next;
};

struct Clock{
  frame_id_t current_id;
  size_t c_size;
  Frame* head;
  Frame* lastnode;
  Frame* first_head;

  Clock(size_t size)
  {
    c_size = size;
    head = NULL;
  }

  void add(frame_id_t id)
  {
    Frame* temp = new Frame();
    temp->id = id;
    temp->ref_id = false;
    temp->next = NULL;

    if (head == NULL)
    {
      current_id = id;
      first_head = temp;
      head = temp;
      return;
    }

    Frame* lastnode = head;
    unsigned int iter = 1;
    while(lastnode->next != NULL)
    {
      lastnode = lastnode->next;
      ++iter;
    }

    if (iter == c_size)
    {
      std::cout<<"Current id "<<current_id<<std::endl;
      std::cout<<"Here"<<std::endl;
      temp->next = first_head;
    }
    lastnode->next = temp;

    return;

  }

  size_t size()
  {
    Frame* temp = head->next;
    int count = 1;

    while((temp->next != NULL))
    {
      if(temp->next->id == current_id) break;
      temp = temp->next;
      count += 1;
    }
    return count + 1 ;
  }

  void findVictim(frame_id_t* value)
  {
    Frame* temp = head;
    while(temp->ref_id)
    {
      temp = temp->next;
    }
    head = temp->next;
    current_id = temp->next->id;
    *value = temp->id;

    delete temp;
  }
};



/**
 * ClockReplacer implements the clock replacement policy, which approximates the Least Recently Used policy.
 */
class ClockReplacer : public Replacer {
 public:
  /**
   * Create a new ClockReplacer.
   * @param num_pages the maximum number of pages the ClockReplacer will be required to store
   */
  explicit ClockReplacer(size_t num_pages);

  /**
   * Destroys the ClockReplacer.
   */
  ~ClockReplacer() override;

  bool Victim(frame_id_t *frame_id) override;

  void Pin(frame_id_t frame_id) override;

  void Unpin(frame_id_t frame_id) override;

  size_t Size() override;

 private:
 Clock* clock;
  // TODO(student): implement me!
};

}  // namespace bustub
