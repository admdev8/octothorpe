#include "datatypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// entropy calculation
// this source code is based on example from http://rosettacode.org/wiki/Entropy#C
 
static int makehist(byte* S,int *hist,int len)
{
	int wherechar[256];
	int i,histlen;
	histlen=0;
	for(i=0;i<256;i++)wherechar[i]=-1;
	for(i=0;i<len;i++)
	{
		if(wherechar[(int)S[i]]==-1)
		{
			wherechar[(int)S[i]]=histlen;
			histlen++;
		}
		hist[wherechar[(int)S[i]]]++;
	}
	return histlen;
}
 
static double entropy_helper(int *hist,int histlen,int len)
{
	int i;
	double H;
	H=0;
	for(i=0;i<histlen;i++)
	{
		H-=(double)hist[i]/len*log2((double)hist[i]/len);
	}
	return H;
}

double entropy (byte* buf, size_t bufsize)
{
	int *hist,histlen;
	
	hist=(int*)calloc(bufsize,sizeof(int));

	histlen=makehist(buf,hist,bufsize);
	//hist now has no order (known to the program) but that doesn't matter
	return entropy_helper(hist,histlen,bufsize);
}
 

