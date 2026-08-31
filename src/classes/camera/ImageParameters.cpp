// Copyright 2026 AndreiUtils Authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
// Created by Andrei on 21-Jan-22.
//

#include <AndreiUtils/classes/camera/ImageParameters.h>

using namespace AndreiUtils;

ImageParameters::ImageParameters() : h(0), w(0) {}

ImageParameters::ImageParameters(int height, int width) : h(height), w(width) {}

ImageParameters::~ImageParameters() = default;

void ImageParameters::setImageParameters(int _h, int _w) {
    this->h = _h;
    this->w = _w;
}
