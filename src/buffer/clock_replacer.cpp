//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// clock_replacer.cpp
//
// Identification: src/buffer/clock_replacer.cpp
//
// Copyright (c) 2015-2019, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/clock_replacer.h"

namespace bustub {

ClockReplacer::ClockReplacer(size_t num_pages) {

    head = new Frame();
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) { return false; }

void ClockReplacer::Pin(frame_id_t frame_id) {}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    Frame* frame = new Frame();
    frame->id = frame_id;
    frame->ref_id = false;
    frame->next = NULL;
    if (head != NULL) frame->next = head;
    head = frame;
    
}

size_t ClockReplacer::Size() { 
    int count = 0;
    Frame* temp_head = head;

    while (temp_head -> next != NULL) 
    {
        temp_head = temp_head->next;
        ++count;
    }
    
    return count - 1; 
    }

}  // namespace bustub
