#pragma once

#include "common_types.h"
#include "common_file.h"
#include "haversine.h"

namespace BaseParser {
    u64 ParseHaversinePairs(Buffer inputJson, u64 maxPairCount, HaversinePair *pairs);
}