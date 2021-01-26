#include "StdAfx.h"

UINT Split(CString str,CStringArray& Arr,char ch)
{
	int nFindposi  = str.Find(ch);	
	while( nFindposi > 0)
	{
		Arr.Add(str.Left(nFindposi) );
		str = str.Right( str.GetLength() - nFindposi -1);
		str.TrimLeft(ch);    //warning

		nFindposi  = str.Find(ch);
	}

	if( !str.Trim().IsEmpty() )
		Arr.Add(str);

	return Arr.GetSize();
}

UINT Split(CString str,CStringArray& Arr,CString strDiv)
{
	int nFindposi  = str.Find(strDiv);
	if( nFindposi <0 )
		return 0;

	while( nFindposi > 0)
	{
		Arr.Add(str.Left(nFindposi) );
		str = str.Right( str.GetLength() - nFindposi -1);
		str.TrimLeft(strDiv);    //warning

		nFindposi  = str.Find(strDiv);
	}

	if( !str.IsEmpty() )
		Arr.Add(str);

	return Arr.GetSize();
}