//
// Created by Andrei on 12.11.20.
//

#ifndef ANDREIUTILS_UTILSOPENPOSE_H
#define ANDREIUTILS_UTILSOPENPOSE_H

#include <openpose/headers.hpp>

void configureOpenposeWrapper(op::WrapperT<op::Datum> &opWrapper, bool trackPeople = false, bool verbose = false);

#endif //ANDREIUTILS_UTILSOPENPOSE_H
