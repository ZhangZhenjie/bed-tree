#include "NormGramDictOrder.h"

NormGramDictOrder::NormGramDictOrder(int q, int length, int bSize, int mBit) : GramDictOrder(q,length, bSize, mBit)
{

}

NormGramDictOrder::~NormGramDictOrder()
{
}

float NormGramDictOrder::Dist(char* qString, char* tString, float threshold)
{
	distCompTime++;

	qString = RemoveHead(qString);
	tString = RemoveHead(tString);

	int qStrLen = strlen(qString);
	int tStrLen = strlen(tString);

	float maxLen = qStrLen > tStrLen ? qStrLen : tStrLen;
	threshold *= maxLen;

	int lenDiff=(qStrLen-tStrLen)>0?(qStrLen-tStrLen):(tStrLen-qStrLen);

	if(lenDiff > threshold)
		return lenDiff/maxLen;

	int i,j;

	
	//////////////////////////////////////////////////////////
	// letter counting optimization starts here
	//////////////////////////////////////////////////////////
	for(i = 0; i < 26; i++)
		//tLetterCounter[i] = 0;
		tLetterCounter[i] = qLetterCounter[i] = 0;

	for(i = 0; i < tStrLen; i++){
		int pos = tString[i] - 'a';

		if(pos >= 0 && pos < 26)
			tLetterCounter[pos]++;
	}
	for(i = 0; i < qStrLen; i++){
		int pos = qString[i] - 'a';

		if(pos >= 0 && pos < 26)
			qLetterCounter[pos]++;
	}

	int exc_sum = 0;
	int bel_sum = 0;
	for(i = 0; i < 26; i++)
	{
		if(qLetterCounter[i] > tLetterCounter[i])
		{
			exc_sum += qLetterCounter[i] - tLetterCounter[i];
			if(exc_sum > threshold)
				return exc_sum/maxLen;
		}
		else{
			bel_sum += tLetterCounter[i] - qLetterCounter[i];
			if(bel_sum > threshold)
				return bel_sum/maxLen;
		}
	}
	///////////////////////////////////////////////////////
	// letter counting optimization ends here
	///////////////////////////////////////////////////////
	



	int min;

	for(i = 0; i < tStrLen + 1; i++)
	{
		if(i == 0)
		{
			for(j = 0; j < qStrLen + 1; j++)
			{
				matrix[i][j] = j;
#ifdef _DEBUG_
				cout << matrix[i][j] << " ";
#endif
			}

#ifdef _DEBUG_
			cout << endl;
#endif

			continue;
		}

		matrix[i][0] = i;

		min = qStrLen;

#ifdef _DEBUG_
		cout << matrix[i][0] << " ";
#endif
		int startPos = (i - ((int)threshold + 1)) > 1 ? (i - ((int)threshold + 1)) : 1;
		//int startPos = 1;
		int endPos = (i + ((int)threshold + 1)) < qStrLen ? (i + ((int)threshold + 1)) : qStrLen;
		//int endPos = qStrLen;

		if(startPos > 1)
		{
			matrix[i][startPos-1] = matrix[i-1][startPos - 1] < matrix[i-1][startPos-2] ?
				(matrix[i-1][startPos-1] + 1) : (matrix[i-1][startPos-2] + 1);
		}

		for(j = startPos; j <= endPos; j++)
		{
			if(tString[i - 1] == qString[j - 1])
				matrix[i][j] = matrix[i - 1][j - 1];
			else
				matrix[i][j] = matrix[i - 1][j - 1] + 1;

			if( (matrix[i][j - 1] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i][j - 1] + 1;

			if( (matrix[i - 1][j] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i - 1][j] + 1;
#ifdef _DEBUG_
			cout << matrix[i][j] << " ";
#endif
			if(matrix[i][j] < min)
				min = matrix[i][j];
		}
		if(j <= qStrLen)
		{
			matrix[i][j] = matrix[i][j - 1] < matrix[i - 1][j - 1] ?
				(matrix[i][j - 1] + 1) : (matrix[i - 1][j - 1] + 1);
		}


		if(min > threshold)
			return min/maxLen;

#ifdef _DEBUG_
		cout << endl;
#endif	
	}

	return matrix[tStrLen][qStrLen]/maxLen;
}

float NormGramDictOrder::Dist(char* qString, int qStrLen, char* tString, float threshold)
{
	distCompTime++;

	qString = RemoveHead(qString);
	tString = RemoveHead(tString);

	int tStrLen = strlen(tString);
	float maxLen = qStrLen > tStrLen ? qStrLen : tStrLen;
	threshold *= maxLen;

	int lenDiff=(qStrLen-tStrLen)>0?(qStrLen-tStrLen):(tStrLen-qStrLen);

	if(lenDiff > threshold)
		return lenDiff/maxLen;

	int i,j;

	
	//////////////////////////////////////////////////////////
	// letter counting optimization starts here
	//////////////////////////////////////////////////////////
	for(i = 0; i < 26; i++)
		tLetterCounter[i] = 0;
		//tLetterCounter[i] = qLetterCounter[i] = 0;

	for(i = 0; i < tStrLen; i++){
		int pos = tString[i] - 'a';

		if(pos >= 0 && pos < 26)
			tLetterCounter[pos]++;
	}
	
	int exc_sum = 0;
	int bel_sum = 0;
	for(i = 0; i < 26; i++)
	{
		if(qLetterCounter[i] > tLetterCounter[i])
		{
			exc_sum += qLetterCounter[i] - tLetterCounter[i];
			if(exc_sum > threshold)
				return exc_sum / maxLen;
		}
		else{
			bel_sum += tLetterCounter[i] - qLetterCounter[i];
			if(bel_sum > threshold)
				return bel_sum / maxLen;
		}
	}
	///////////////////////////////////////////////////////
	// letter counting optimization ends here
	///////////////////////////////////////////////////////
	



	int min;

	for(i = 0; i < tStrLen + 1; i++)
	{
		if(i == 0)
		{
			for(j = 0; j < qStrLen + 1; j++)
			{
				matrix[i][j] = j;
#ifdef _DEBUG_
				cout << matrix[i][j] << " ";
#endif
			}

#ifdef _DEBUG_
			cout << endl;
#endif

			continue;
		}

		matrix[i][0] = i;

		min = qStrLen;

#ifdef _DEBUG_
		cout << matrix[i][0] << " ";
#endif
		int startPos = (i - ((int)threshold + 1)) > 1 ? (i - ((int)threshold + 1)) : 1;
		//int startPos = 1;
		int endPos = (i + ((int)threshold + 1)) < qStrLen ? (i + ((int)threshold + 1)) : qStrLen;
		//int endPos = qStrLen;

		if(startPos > 1)
		{
			matrix[i][startPos-1] = matrix[i-1][startPos - 1] < matrix[i-1][startPos-2] ?
				(matrix[i-1][startPos-1] + 1) : (matrix[i-1][startPos-2] + 1);
		}

		for(j = startPos; j <= endPos; j++)
		{
			if(tString[i - 1] == qString[j - 1])
				matrix[i][j] = matrix[i - 1][j - 1];
			else
				matrix[i][j] = matrix[i - 1][j - 1] + 1;

			if( (matrix[i][j - 1] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i][j - 1] + 1;

			if( (matrix[i - 1][j] + 1) < matrix[i][j])
				matrix[i][j] = matrix[i - 1][j] + 1;
#ifdef _DEBUG_
			cout << matrix[i][j] << " ";
#endif
			if(matrix[i][j] < min)
				min = matrix[i][j];
		}
		if(j <= qStrLen)
		{
			matrix[i][j] = matrix[i][j - 1] < matrix[i - 1][j - 1] ?
				(matrix[i][j - 1] + 1) : (matrix[i - 1][j - 1] + 1);
		}


		if(min > threshold)
			return min / maxLen;

#ifdef _DEBUG_
		cout << endl;
#endif	
	}

	return matrix[tStrLen][qStrLen] / maxLen;
}

float NormGramDictOrder::LowerBound(char* lsString, char* leString, char* rsString, char* reString, float threshold)
{
	int i;

	lbCompTime++;

	lsString = RemoveHeadAndGram(lsString, qHashBucket);
	leString = RemoveHeadAndGram(leString, rHashBucket);
	rsString = RemoveHeadAndGram(rsString, sHashBucket);
	reString = RemoveHeadAndGram(reString, eHashBucket);

	//////////////////////////////////////////////////////////
	int maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] > maxVal)
			maxVal = sHashBucket[i];
		if(eHashBucket[i] > maxVal)
			maxVal = eHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;
	////////////////////////////////////////////////////////////

	int bitpos;

	for(i = 0; i < bucketSize; i++)
	{
		lbArray[i] = 0;
		ubArray[i] = ( 1 << (maxBit + 1) ) - 1;
	}

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( sHashBucket[i] >> bitpos ) < ( eHashBucket[i] >> bitpos ) )
			{
				break;
			}

			int bit = (sHashBucket[i] >> bitpos) % 2;

			if(bit == 0)
			{
				ubArray[i] -= ( 1 << bitpos );
			}
			else
			{
				lbArray[i] += ( 1 << bitpos );
			}
		}

		if(i < bucketSize)
			break;
	}
	/////////////////////////////////////////////////////////////////
	maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(qHashBucket[i] > maxVal)
			maxVal = qHashBucket[i];
		if(rHashBucket[i] > maxVal)
			maxVal = rHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;

	/////////////////////////////////////////////////////////////////
	for(i = 0; i < bucketSize; i++)
	{
		lbNewArray[i] = 0;
		ubNewArray[i] = ( 1 << (maxBit + 1) ) - 1;
	}

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( qHashBucket[i] >> bitpos ) < ( rHashBucket[i] >> bitpos ) )
			{
				break;
			}

			int bit = (qHashBucket[i] >> bitpos) % 2;

			if(bit == 0)
			{
				ubNewArray[i] -= ( 1 << bitpos );
			}
			else
			{
				lbNewArray[i] += ( 1 << bitpos );
			}
		}

		if(i < bucketSize)
			break;
	}
	////////////////////////////////////////////////////////////////////


	int minstrlen = 0;
	int maxstrlen = 0;
	int newminstrlen = 0;
	int newmaxstrlen = 0;
	for(i = 0; i < bucketSize; i++)
	{
		minstrlen += lbArray[i];
		maxstrlen += ubArray[i];

		newminstrlen += lbNewArray[i];
		newmaxstrlen += ubNewArray[i];
	}
	float maxLen = maxstrlen > newmaxstrlen ? maxstrlen : newmaxstrlen;
	threshold *= maxLen;

	int sur_sum = 0;
	int exc_sum = 0;

	int newThreshold = (int)(threshold * gramSize) + 1;

	for(i = 0; i < bucketSize; i++)
	{
		if(ubNewArray[i] < lbArray[i])
		{
			sur_sum += ( lbArray[i] - ubNewArray[i] );
			if(sur_sum > newThreshold)
				return (threshold + 1) / maxLen;
			continue;
		}

		if(lbNewArray[i] > ubArray[i])
		{
			exc_sum += ( lbNewArray[i] - ubArray[i] );
			if(exc_sum > newThreshold)
				return (threshold + 1) / maxLen;
			continue;
		}
	}

	//minstrlen -= gramSize - 1;
	//minstrlen = minstrlen > qStrLen ? minstrlen : qStrLen;

	//int retval = (sur_sum + exc_sum + 2 * gramSize - 1) / gramSize / 2;

	int retval;

	
	if(sur_sum > exc_sum)
		retval = (sur_sum + gramSize - 1) / gramSize;
	else
		retval = (exc_sum + gramSize - 1) / gramSize;

	if(newmaxstrlen < minstrlen)
	{
		if(minstrlen - gramSize + 1 - newmaxstrlen > retval)
			retval = minstrlen - newmaxstrlen - gramSize + 1;
	}
	
	if(newminstrlen > maxstrlen)
	{
		if(newminstrlen - gramSize + 1 - maxstrlen > retval)
			retval = newminstrlen - minstrlen - gramSize + 1;
	}

	return retval / maxLen;
}

float NormGramDictOrder::LowerBound(char* qString, int qStrLen, char* sString, 
							  char* eString, float threshold)
{
	int i;

	lbCompTime++;

	qString = fixedQString;
	//qString = RemoveHeadAndGram(qString, qHashBucket);
	sString = RemoveHeadAndGram(sString, sHashBucket);
	eString = RemoveHeadAndGram(eString, eHashBucket);

	//int qStrLen = strlen(qString);

	////////////////////////////////////////////////////////////
	int maxVal = 0;

	for(i = 0; i < bucketSize; i++)
	{
		if(sHashBucket[i] > maxVal)
			maxVal = sHashBucket[i];
		if(eHashBucket[i] > maxVal)
			maxVal = eHashBucket[i];
	}
	maxBit = 0;

	while((maxVal >> maxBit) != 0)
		maxBit++;
	maxBit--;
	////////////////////////////////////////////////////////////

	int bitpos;


	for(i = 0; i < bucketSize; i++)
	{
		lbArray[i] = 0;
		ubArray[i] = ( 1 << (maxBit + 1) ) - 1;
	}

	for(bitpos = maxBit; bitpos >= 0; bitpos--)
	{
		for(i = 0; i < bucketSize; i++)
		{
			if( ( sHashBucket[i] >> bitpos ) < ( eHashBucket[i] >> bitpos ) )
			{
				break;
			}

			int bit = (sHashBucket[i] >> bitpos) % 2;

			if(bit == 0)
			{
				ubArray[i] -= ( 1 << bitpos );
			}
			else
			{
				lbArray[i] += ( 1 << bitpos );
			}
		}

		if(i < bucketSize)
			break;
	}

	int sur_sum = 0;
	int exc_sum = 0;

	int minstrlen = 0;
	int maxstrlen = 0;
	for(i = 0; i < bucketSize; i++)
	{
		minstrlen += lbArray[i];
		maxstrlen += ubArray[i];
	}
	float maxLen = maxstrlen > qStrLen ? maxstrlen : qStrLen;
	threshold *= maxLen;

	int newThreshold = (int)(threshold * gramSize) + 1;

	for(i = 0; i < bucketSize; i++)
	{
		if(qHashBucket[i] >= lbArray[i] && qHashBucket[i] <= ubArray[i])
			continue;

		if(qHashBucket[i] < lbArray[i])
		{
			sur_sum += ( lbArray[i] - qHashBucket[i] );
			if(sur_sum > newThreshold)
				return (threshold + 1) / maxLen;
			continue;
		}

		exc_sum += ( qHashBucket[i] - ubArray[i]);
		if(exc_sum > newThreshold)
			return (threshold + 1) / maxLen;
	}

	//minstrlen -= gramSize - 1;
	//minstrlen = minstrlen > qStrLen ? minstrlen : qStrLen;

	//int retval = (sur_sum + exc_sum + 2 * gramSize - 1) / gramSize / 2;

	int retval;

	
	if(sur_sum > exc_sum)
		retval = (sur_sum + gramSize - 1) / gramSize;
	else
		retval = (exc_sum + gramSize - 1) / gramSize;
	

	if(qStrLen < minstrlen)
	{
		if(minstrlen - gramSize + 1 - qStrLen > retval)
			retval = minstrlen - qStrLen - gramSize + 1;
	}
	
	if(qStrLen > maxstrlen)
	{
		if(qStrLen - gramSize + 1 - maxstrlen > retval)
			retval = qStrLen - minstrlen - gramSize + 1;
	}

	return retval / maxLen;
}
