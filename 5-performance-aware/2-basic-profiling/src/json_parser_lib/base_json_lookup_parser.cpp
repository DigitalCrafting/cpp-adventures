#include "base_json_lookup_parser.h"
#include <iostream>

namespace BaseParser {
    b32 IsJSONDigit(Buffer source, u64 at) {
        b32 result = false;
        if (IsInBounds(source, at)) {
            u8 val = source.data[at];
            result = ((val >= '0') && (val <= '9'));
        }
        return result;
    }

    b32 IsJSONWhitespace(Buffer source, u64 at) {
        b32 result = false;
        if (IsInBounds(source, at)) {
            u8 val = source.data[at];
            result = ((val == ' ') || (val == '\t') || (val == '\n') || (val == '\r'));
        }
        return result;
    }

    b32 IsParsing(JsonParser *parser) {
        b32 result = !parser->hadError && IsInBounds(parser->source, parser->at);
        return result;
    }

    void Error(JsonParser *parser, JsonToken token, char const *message) {
        parser->hadError = true;
        fprintf(stderr, "ERROR: \"%.*s\" - %s\n", (u32)token.value.count, (char *)token.value.data, message);
    }

    void ParseKeyword(Buffer source, u64 *at, Buffer keywordRemaining, JsonTokenType type, JsonToken *result) {
        if((source.count - *at) >= keywordRemaining.count) {
            Buffer check = source;
            check.data += *at;
            check.count = keywordRemaining.count;
            if (AreEqual(check, keywordRemaining)) {
                result->type = type;
                result->value.count += keywordRemaining.count;
                *at += keywordRemaining.count;
            }
        }
    }

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