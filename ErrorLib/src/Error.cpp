#include "Error.h"

Error::Error(ErrorCode ErrID) : _ErrID(ErrID)
{
	_ErrMsg = "";
}

Error::Error(ErrorCode ErrID, std::string ErrMsg) : _ErrID(ErrID), _ErrMsg(ErrMsg)
{
}

std::string Error::GetMsg()
{
	if (!_ErrMsg.empty())
		return _ErrMsg;
	else
		return "Unknown Error.";
}
