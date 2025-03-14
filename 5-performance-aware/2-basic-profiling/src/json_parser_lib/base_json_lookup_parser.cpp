#include "base_json_lookup_parser.h"
#include <iostream>

namespace BaseParser {
    b32 IsJSONDigit(Buffer source, u64 at) {
        return false;
    }

    b32 IsJSONWhitespace(Buffer source, u64 at) {
        return false;
    }

    b32 IsParsing(JsonParser *parser) {
        return false;
    }

    void Error(JsonParser *parser, JsonToken token, char const *message) {}

    void ParseKeyword(Buffer source, u64 *at, Buffer keywordRemaining, JsonTokenType type, JsonToken *result) {}

    JsonToken GetJSONToken(JsonParser *parser) {
        return {};
    }

    JsonElement *ParseJSONList(JsonParser *parser, JsonToken startingToken, JsonToken endType, b32 hasLabels) {
        return {};
    }

    JsonElement *ParseJSONElement(JsonParser *parser, Buffer label, JsonToken value) {
        return {};
    }

    JsonElement *ParseJSON(Buffer inputJson) {
        return {};
    }

    void FreeJSON(JsonElement *element) {}

    JsonElement *LookupElement(JsonElement *object, Buffer elementName) {
        return nullptr;
    }

    f64 ConvertJSONSign(Buffer source, u64 *atResult) {
        return 0.0f;
    }

    f64 ConvertJSONNumber(Buffer source, u64 *atResult) {
        return 0.0f;
    }

    f64 ConvertElementToF64(JsonElement *object, Buffer elementName) {
        return 0.0f;
    }

    u64 ParseHaversinePairs(Buffer inputJson, u64 maxPairCount, HaversinePair *pairs) {
        std::cout << "Hello from parser lib!\n";
        return 0;
    }
}