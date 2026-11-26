enum {
    PARSE_ERR_TRUNCATED_HEADER = -1, //
    PARSE_ERR_TRUNCATED_PAYLOAD = -2, //
    PARSE_ERR_INVALID_VERSION = -3, // version doesnt match
    PARSE_ERR_INVALID_LENGTH = -4, // header reporting its length incorrectly
    PARSE_ERR_RESERVED_BIT = -5 // reserved bits of unexpected values
};