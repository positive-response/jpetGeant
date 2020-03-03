/**
 *  @copyright Copyright 2019 The J-PET Monte Carlo Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file SourceParams.cpp
 */

#include "SourceParams.h"

SourceParams::SourceParams() :
  howManyGammas(1), shape("cylinder"), shapeCenterPosition(0, 0, 0) {}

SourceParams::~SourceParams() {}

void SourceParams::SetShape(G4String& newShape)
{
  if (std::find(std::begin(allowedShapes), std::end(allowedShapes), newShape) != std::end(allowedShapes)) {
    shape = newShape;
  } else {
    G4Exception(
      "SourceParams", "SP01", FatalException, "shape is not allowed"
    );
  }
}
