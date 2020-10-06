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

    clock = new Clock(num_pages);
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) { 
    if (Size() == 0) return false;
    m.lock();
    clock->findVictim(frame_id);
    //clock->remove(*frame_id);
    m.unlock();
    return true;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    m.lock();
    if (clock->already_present(frame_id, 0))clock->remove(frame_id);
    m.unlock();
}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    
    m.lock();
    if(!clock->already_present(frame_id,1))clock->add(frame_id);
    m.unlock();
    
}

size_t ClockReplacer::Size() {
    m.lock(); 
    size_t size_= clock->size();
    m.unlock();
    return size_;
}  // namespace bustub

}
