#include "StdAfx.h"
#include "URLToEncode.h"


CURLTOEncode::CURLTOEncode(void)
{
}


CURLTOEncode::~CURLTOEncode(void)
{
}

inline BYTE  CURLTOEncode::toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
};
CStringA CURLTOEncode::URLEncode(CStringA sIn)
{
	CStringA sOut;

	const int nLen = sIn.GetLength() + 1;

	register LPBYTE pOutTmp = NULL;
	LPBYTE pOutBuf = NULL;
	register LPBYTE pInTmp = NULL;
	LPBYTE pInBuf =(LPBYTE)sIn.GetBuffer(nLen);
	BYTE b = 0; 
	pOutBuf = (LPBYTE)sOut.GetBuffer(nLen*3 - 2);//new BYTE [nLen  * 3];

	if(pOutBuf)
	{
		pInTmp   = pInBuf;
		pOutTmp = pOutBuf;

		// do encoding
		while (*pInTmp)
		{
			if(isalnum(*pInTmp))
				*pOutTmp++ = *pInTmp;
			else
				if(isspace(*pInTmp))
					*pOutTmp++ = '+';
				else
				{
					*pOutTmp++ = '%';
					*pOutTmp++ = toHex(*pInTmp>>4);
					*pOutTmp++ = toHex(*pInTmp%16);
				}
				pInTmp++;
		}
		*pOutTmp = '\0';
		//sOut=pOutBuf;
		//delete [] pOutBuf;
		sOut.ReleaseBuffer();
	}
	sIn.ReleaseBuffer();
	return sOut;
};
