#pragma once

#include <string>
#include <stdexcept>
#include "ErrorLibExports.h"

enum class ERRORLIB_API ErrorCode
{
    NullPtr,
	PathNotFound
};

class ERRORLIB_API Error : public std::exception
{
public:
    Error(ErrorCode ErrID);
	Error(ErrorCode ErrID, std::string ErrMsg);

	std::string GetMsg();
private:
	ErrorCode _ErrID;
	std::string _ErrMsg;
};

