#include "paper_util.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
char* paper_util_new_guid()
{
	GUID guid;
	char* szGuid = malloc(128);
	HRESULT res = CoCreateGuid(&guid);
	if (S_OK != res)
	{
		return NULL;
	}
	_snprintf(szGuid, 128,
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2],
		guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return szGuid;
}

void paper_util_free_guid(char** guid)
{
	free(*guid);
	*guid = NULL;
}
