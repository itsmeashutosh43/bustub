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
    if (clock->size() == 0) return false;
    int idx;
    clock->findVictim(frame_id, idx);
    pages.erase(pages.begin() + idx);
    return true;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    int idx;
    std::vector<int>::iterator it = std::find(pages.begin(), pages.end(), frame_id);

    if (it==pages.end()) return; 
    clock->remove(frame_id,idx);
    pages.erase(pages.begin() + idx);  
    std::cout<<"Size s" << pages.size()<<std::endl;  

}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    
    if (clock->size() == 0){
        clock->add(frame_id);
        pages.push_back(frame_id);
        return;
    }

    if (!clock->already_present(frame_id)){
        clock->add(frame_id);
        pages.push_back(frame_id);
    }
    
}

size_t ClockReplacer::Size() { 
    return clock->size();
}  // namespace bustub

}
