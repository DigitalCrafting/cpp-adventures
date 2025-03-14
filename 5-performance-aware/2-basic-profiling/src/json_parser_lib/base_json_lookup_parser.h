#pragma once

#include "common_types.h"
#include "common_file.h"
#include "common_json.h"
#include "haversine.h"

namespace BaseParser {
    b32 IsJSONDigit(Buffer source, u64 at);

    b32 IsJSONWhitespace(Buffer source, u64 at);

    b32 IsParsing(JsonParser *parser);

    void Error(JsonParser *parser, JsonToken token, char const *message);

    void ParseKeyword(Buffer source, u64 *at, Buffer keywordRemaining, JsonTokenType type, JsonToken *result);

    JsonToken GetJSONToken(JsonParser *parser);

    JsonElement *ParseJSONList(JsonParser *parser, JsonToken startingToken, JsonToken endType, b32 hasLabels);

    JsonElement *ParseJSONElement(JsonParser *parser, Buffer label, JsonToken value);

    JsonElement *ParseJSON(Buffer inputJson);

    void FreeJSON(JsonElement *element);

    JsonElement *LookupElement(JsonElement *object, Buffer elementName);

    f64 ConvertJSONSign(Buffer source, u64 *atResult);

    f64 ConvertJSONNumber(Buffer source, u64 *atResult);

    f64 ConvertElementToF64(JsonElement *object, Buffer elementName);

    u64 ParseHaversinePairs(Buffer inputJson, u64 maxPairCount, HaversinePair *pairs);
}