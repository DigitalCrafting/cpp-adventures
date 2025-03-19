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
        JsonToken result = {};

        Buffer source = parser->source;
        u64 at = parser->at;

        while(IsJSONWhitespace(source, at)) {
            ++at;
        }

        if (IsInBounds(source, at)) {
            result.type = JsonTokenType::Error;
            result.value.count = 1;
            result.value.data = source.data + at;
            u8 val = source.data[at++];

            switch (val) {
                case '{': {result.type = JsonTokenType::OpenBrace;} break;
                case '[': {result.type = JsonTokenType::OpenBracket;} break;
                case '}': {result.type = JsonTokenType::CloseBrace;} break;
                case ']': {result.type = JsonTokenType::CloseBracket;} break;
                case ',': {result.type = JsonTokenType::Comma;} break;
                case ':': {result.type = JsonTokenType::Colon;} break;
                case ';': {result.type = JsonTokenType::SemiColon;} break;

                case 'f': {
                    ParseKeyword(source, &at, CONSTANT_STRING("alse"), JsonTokenType::False, &result);
                } break;
                case 'n': {
                    ParseKeyword(source, &at, CONSTANT_STRING("ull"), JsonTokenType::Null, &result);
                } break;
                case 't': {
                    ParseKeyword(source, &at, CONSTANT_STRING("rue"), JsonTokenType::True, &result);
                } break;

                case '"': {
                    result.type = JsonTokenType::StringLiteral;
                    u64 stringStart = at;

                    while (IsInBounds(source, at) && (source.data[at] != '"')) {
                        if (IsInBounds(source, (at + 1)) && (source.data[at] == '\\') && (source.data[at + 1] == '"')) {
                            // Skip escaped quotation marks
                            ++at;
                        }
                        ++at;
                    }

                    result.value.data = source.data + stringStart;
                    result.value.count = at - stringStart;
                    if (IsInBounds(source, at)) {
                        ++at;
                    }
                } break;

                case '-':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9': {
                    u64 start = at - 1;
                    result.type = JsonTokenType::Number;

                    if ((val == '-') && IsInBounds(source, at)) {
                        val = source.data[at++];
                    }

                    if (val != '0') {
                        while(IsJSONDigit(source, at)) {
                            ++at;
                        }
                    }

                    if (IsInBounds(source, at) && (source.data[at] == '.')) {
                        ++at;
                        while (IsJSONDigit(source, at)) {
                            ++at;
                        }
                    }

                    // Scientific notation
                    if (IsInBounds(source, at) && ((source.data[at] == 'e') || (source.data[at] == 'E'))) {
                        ++at;

                        if (IsInBounds(source, at) && ((source.data[at] == '+') || (source.data[at] == '-'))) {
                            ++at;
                        }

                        while (IsJSONDigit(source, at)) {
                            ++at;
                        }
                    }

                    result.value.count = at - start;
                } break;

                default: {} break;
            }

            parser->at = at;
            return result;
        }

        return result;
    }

    JsonElement *ParseJSONList(JsonParser *parser, JsonToken startingToken, JsonTokenType endType, b32 hasLabels) {
        JsonElement *firstElement = {};
        JsonElement *lastElement = {};

        while (IsParsing(parser)) {
            Buffer label = {};
            JsonToken value = GetJSONToken(parser);
            if (hasLabels) {
                if (value.type == JsonTokenType::StringLiteral) {
                    label = value.value;
                    JsonToken colon = GetJSONToken(parser);

                    if (colon.type == JsonTokenType::Colon) {
                        value = GetJSONToken(parser);
                    } else {
                        Error(parser, colon, "Expected colon after field name");
                    }
                } else if (value.type != endType) {
                    Error(parser, value, "Unexpected token in JSON");
                }
            }

            JsonElement *element = ParseJSONElement(parser, label, value);
            if (element) {
                lastElement = (lastElement ? lastElement->nextSibling : firstElement) = element;
            } else if (value.type == endType) {
                break;
            } else {
                Error(parser, value, "Unexpected token in JSON");
            }

            JsonToken comma = GetJSONToken(parser);
            if (comma.type == endType) {
                break;
            } else if (comma.type != JsonTokenType::Comma) {
                Error(parser, value, "Unexpected token in JSON");
            }
        }

        return firstElement;
    }

    JsonElement *ParseJSONElement(JsonParser *parser, Buffer label, JsonToken value) {
        b32 valid = true;

        JsonElement *subElement = 0;
        if (value.type == JsonTokenType::OpenBracket) {
            subElement = ParseJSONList(parser, value, JsonTokenType::CloseBracket, false);
        } else if (value.type == JsonTokenType::OpenBrace) {
            subElement = ParseJSONList(parser, value, JsonTokenType::CloseBrace, true);
        } else if (
           value.type == JsonTokenType::StringLiteral ||
           value.type == JsonTokenType::True ||
           value.type == JsonTokenType::False ||
           value.type == JsonTokenType::Null ||
           value.type == JsonTokenType::Number
        ) {
            // Nothing to do here
        } else {
            valid = false;
        }

        JsonElement *result = 0;

        if (valid) {
            result = (JsonElement*) malloc(sizeof(JsonElement));
            result->label = label;
            result->value = value.value;
            result->firstSubElement = subElement;
            result->nextSibling = 0;
        }

        return result;
    }

    JsonElement *ParseJSON(Buffer inputJson) {
        JsonParser parser = {};
        parser.source = inputJson;

        JsonElement *result = ParseJSONElement(&parser, {}, GetJSONToken(&parser));
        return result;
    }

    void FreeJSON(JsonElement *element) {
        while (element) {
            JsonElement *freeElement = element;
            element = element->nextSibling;

            FreeJSON(freeElement->firstSubElement);
            free(freeElement);
        }
    }

    JsonElement *LookupElement(JsonElement *object, Buffer elementName) {
        JsonElement *result = 0;

        if (object) {
            for (JsonElement *search = object->firstSubElement; search; search = search->nextSibling) {
                if (AreEqual(search->label, elementName)) {
                    result = search;
                    break;
                }
            }
        }

        return result;
    }

    f64 ConvertJSONSign(Buffer source, u64 *atResult) {
        u64 at = *atResult;

        f64 result = 1.0;
        if (IsInBounds(source, at) && (source.data[at] == '-')) {
            result = -1.0;
            ++at;
        }

        *atResult = at;

        return result;
    }

    f64 ConvertJSONNumber(Buffer source, u64 *atResult) {
        u64 at = *atResult;
        f64 result = 0.0;

        while (IsInBounds(source, at)) {
            u8 character = source.data[at] - (u8)'0';
            if (character < 10) {
                result = 10.0 * result + (f64) character;
                ++at;
            } else {
                break;
            }
        }

        *atResult = at;

        return result;
    }

    f64 ConvertElementToF64(JsonElement *object, Buffer elementName) {
        f64 result = 0.0;

        JsonElement *element = LookupElement(object, elementName);
        if (element) {
            Buffer source = element->value;
            u64 at = 0;

            f64 sign = ConvertJSONSign(source, &at);
            f64 number = ConvertJSONNumber(source, &at);

            if (IsInBounds(source, at) && (source.data[at] == '.')) {
                ++at;
                f64 c = 1.0 / 10.0;

                while (IsInBounds(source, at)) {
                    u8 character = source.data[at] - (u8)'0';
                    if (character < 10) {
                        number = number + c*(f64)character;
                        c *= 1.0/10.0;
                        ++at;
                    } else {
                        break;
                    }
                }
            }

            if (IsInBounds(source, at) && ((source.data[at] == 'e') || (source.data[at] == 'E'))) {
                ++at;
                if (IsInBounds(source, at) && (source.data[at] == '+')) {
                    ++at;
                }

                f64 exponentSign = ConvertJSONSign(source, &at);
                f64 exponent = exponentSign* ConvertJSONNumber(source, &at);
                number *= pow(10.0, exponent);
            }

            result = sign * number;
        }

        return result;
    }

    u64 ParseHaversinePairs(Buffer inputJson, u64 maxPairCount, HaversinePair *pairs) {
        u64 pairCount = 0;

        JsonElement *json = ParseJSON(inputJson);
        JsonElement *pairsArray = LookupElement(json, CONSTANT_STRING("pairs"));
        if (pairsArray) {
            for (JsonElement *element = pairsArray->firstSubElement;
                element && (pairCount < maxPairCount);
                element = element->nextSibling
            ) {
                HaversinePair *pair = pairs + pairCount++;

                pair->x0 = ConvertElementToF64(element, CONSTANT_STRING("x0"));
                pair->y0 = ConvertElementToF64(element, CONSTANT_STRING("y0"));
                pair->x1 = ConvertElementToF64(element, CONSTANT_STRING("x1"));
                pair->y1 = ConvertElementToF64(element, CONSTANT_STRING("y1"));
            }
        }

        FreeJSON(json);
        return pairCount;
    }
}