#pragma once

#include "common_file.h"

enum JsonTokenType {
    EndOfStream,
    Error,

    OpenBrace,
    OpenBracket,
    CloseBrace,
    CloseBracket,
    Comma,
    Colon,
    SemiColon,
    StringLiteral,
    Number,
    True,
    False,
    Null,

    Count
};

struct JsonToken {
    JsonTokenType type;
    Buffer value;
};

struct JsonElement {
    Buffer label;
    Buffer value;
    JsonElement *firstSubElement;
    JsonElement *nextSibling;
};

struct JsonParser {
    Buffer source;
    u64 at;
    b32 hadError;
};