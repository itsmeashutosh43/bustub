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
#include <algorithm>

#include <iostream>

#include "buffer/replacer.h"
#include "common/config.h"

namespace bustub {

/**
 * Structure to hold all information in a single frame 
 */
struct ClockSlot{
  frame_id_t value;
  frame_id_t position;
  bool ref_id;
  ClockSlot* next;


  ClockSlot()
  {
    this->position = position;
    this->value = -1;
    this->ref_id = false;
  }
};

struct Clock{
  frame_id_t current_head_id;
  frame_id_t ticker_head_pos;
  ClockSlot* head;

  Clock(size_t size)
  {
    head = new ClockSlot();
    head->position = 1;
    head->next = NULL;
    current_head_id = 1;
    ticker_head_pos = 1;
    

    for(size_t i = 1 ; i <= size -1 ; i++)
    {
      ClockSlot* temp = new ClockSlot();
      temp->position = i+1;
      ClockSlot* lastnode = head;
      size_t iter = 0;
      while(lastnode->next != NULL)
      {
        iter+= 1;
        lastnode = lastnode->next;
      }

      if (iter == size-2)
      {
        temp->next = head;

      }
      
      lastnode->next = temp;
    }
  }

  void add(frame_id_t value)
  {
    ClockSlot* temp = head;
    while ((temp->value != -1))
    {
      temp = temp->next;
    }
    temp->value = value;  
    
  }

  size_t size()
  {

    ClockSlot* temp = head->next;

    size_t iter = 1;
    bool n = false;
    while(current_head_id != temp->position)
    {
      if (temp->value == -1)
      {
        temp = temp->next;
        continue;
      }
      n = true;
      temp = temp->next;
      iter += 1;
    }
    if (!n) --iter;
    return iter;
  }

  void findVictim(frame_id_t* value)
  {
    ClockSlot* temp = head;
    ClockSlot* clockHead;

    while(ticker_head_pos != temp->position) temp = temp->next;

    clockHead = temp;

    while(clockHead->ref_id)
    {
      clockHead->ref_id = false;
      clockHead = clockHead->next;
    }

    *value = clockHead->value;
    clockHead->value = -1;
    if (size() == 0)
    {
      ticker_head_pos = 1;
      return;
    }

    while (clockHead->next->value == -1)clockHead = clockHead->next;
    ticker_head_pos = clockHead->next->position;

  }


  void remove(frame_id_t value)
  {
    ClockSlot* temp = head;
    while ((temp->value != value))
    {
      temp = temp->next;
    }
    temp->value = -1;
    temp->ref_id = false;
    
  }

  bool already_present(frame_id_t value, frame_id_t mode)
  {
    ClockSlot* temp = head->next;

    if(head->value == value) 
    {
      if (mode == 1)head->ref_id = true;
      return true;
    }

    while((current_head_id != temp->position))
    {
      if (temp->value == value)
      {
        if (mode == 1)temp->ref_id = true;
        return true;
      }
      temp = temp->next;
    }
    return false;
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
 std::vector<frame_id_t> pages;
 std::mutex m;
  // TODO(student): implement me!
};

}  // namespace bustub
