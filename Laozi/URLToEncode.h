#pragma once

class CURLTOEncode
{
public:
	CURLTOEncode(void);
	~CURLTOEncode(void);
	BYTE toHex(const BYTE &x);
	CStringA URLEncode(CStringA sIn);



};