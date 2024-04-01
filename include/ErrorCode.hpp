#ifndef ERROR_CODE_HPP_
#define ERROR_CODE_HPP_

enum class ErrorCode {
    NO_ERRORS          = 0,
    ALLOCATION_ERROR   = 1 << 0,
    NO_OBJECT          = 1 << 1,
    FILE_IO_ERROR      = 1 << 2,
    FILE_TYPE_ERROR    = 1 << 3,
    BMP_SETTINGS_ERROR = 1 << 4,
};

#endif
