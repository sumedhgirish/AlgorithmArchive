#pragma once
#ifndef _RADIX_H
#	define RADIX_H
int RadixSort(char* buffer, int n);
#endif

#ifdef RADIX_IMPLEMENTATION
#	include <assert.h>
#	include <stdlib.h>

static void _SortPartion(char* tp, char* bp, char* buffer, int cbit)
{
	if(bp - tp <= 1 || cbit < 0)
		return;

	char *ttp = tp, *tbp = bp;
	while(tp != bp)
	{
		if((*tp & (1 << cbit)) == 0)
			++tp;
		else
		{
			if(tp == bp - 1)
			{
				--bp;
				continue;
			}
			*tp = *tp ^ *(--bp);
			*bp = *bp ^ *tp;
			*tp = *tp ^ *bp;
		}
	}

	if(cbit >= 1)
	{
		_SortPartion(ttp, tp, buffer, cbit - 1);
		_SortPartion(bp, tbp, buffer, cbit - 1);
	}
}

int RadixSort(char* buffer, int n)
{
	assert(buffer != NULL);
	_SortPartion(buffer, buffer + n, buffer, sizeof(char) * 8 - 1);
	return 0;
}
#endif
