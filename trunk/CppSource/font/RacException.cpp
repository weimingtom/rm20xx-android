#include "PreCompile.h"
#include "RacException.h"

RacException::RacException(const char* szErr, int num) 
: m_strErr(szErr) 
{
	ostringstream os;
	os << num; 
	m_strErr += os.str();
	cout << m_strErr << endl;
}

RacException::RacException(const ostringstream& os) 
{
	m_strErr = os.str();
}
