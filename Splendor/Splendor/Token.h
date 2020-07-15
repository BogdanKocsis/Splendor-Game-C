#pragma once
#include "IToken.h"

class Token :IToken
{
public:
	Token(Type type);

private:
	Type m_type;
};

