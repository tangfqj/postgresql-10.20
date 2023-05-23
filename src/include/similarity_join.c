#include "postgres.h"
#include "fmgr.h"

#include <ctype.h>

//magic lock
#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

#define L_SIZE 100
#define J_SIZE 128
#define J_SIZE2 256

/* levenshtein distance */
PG_FUNCTION_INFO_V1(levenshtein_distance);

Datum
levenshtein_distance(PG_FUNCTION_ARGS)
{
	/* Get arguments */
	text *src = PG_GETARG_TEXT_PP(0);
	text *dst = PG_GETARG_TEXT_PP(1);

	const char *s_data;
	const char *t_data;
	int s_bytes, t_bytes;
	int i, j;
	int min_dist;

	static int distance[L_SIZE][L_SIZE];

	s_data = VARDATA_ANY(src);
	t_data = VARDATA_ANY(dst);

	/* Length of src and dst */
	s_bytes = VARSIZE_ANY_EXHDR(src);
	t_bytes = VARSIZE_ANY_EXHDR(dst);	

	/* Initialization */
	for(j = 1; j <= s_bytes; j++)
		distance[0][j] = j;
	for(i = 1; i <= t_bytes; i++)
		distance[i][0] = i;

	/* Achieve O(m * n) complexity */
	for(i = 1; i <= t_bytes; i++)
		for(j = 1; j <= s_bytes; j++)
			if(tolower(s_data[j - 1]) == tolower(t_data[i - 1]))
				distance[i][j] = distance[i - 1][j - 1];
			else
			{
				if(distance[i - 1][j] <= distance[i][j - 1])
					min_dist = distance[i - 1][j];
				else
					min_dist = distance[i][j - 1];
				if(min_dist > distance[i - 1][j - 1])
					min_dist = distance[i - 1][j - 1];

				distance[i][j] = min_dist + 1;
			}

	PG_RETURN_INT32(distance[t_bytes][s_bytes]);
}


PG_FUNCTION_INFO_V1(jaccard_index);

Datum
jaccard_index(PG_FUNCTION_ARGS)
{
	/* Get arguments */
	text *src = PG_GETARG_TEXT_PP(0);
	text *dst = PG_GETARG_TEXT_PP(1);

	const char *s_data;
	const char *t_data;
	int	s_bytes,t_bytes;
	int i;
	char a, b;
	double intersect = 0, join = 0;

	static int hash[J_SIZE][J_SIZE];
	static int clear[2][J_SIZE2];
	int cp = 0;

	s_data = VARDATA_ANY(src);
	t_data = VARDATA_ANY(dst);

	/* Length of src and dst */
	s_bytes = VARSIZE_ANY_EXHDR(src);
	t_bytes = VARSIZE_ANY_EXHDR(dst);	

	//O(m + n)
	for(i = -1; i < s_bytes; i++)
	{	
		a = (i < 0) ? '$' : s_data[i];
		b = (i + 1 < s_bytes) ? s_data[i + 1] : '$';
		if(!hash[a][b])
		{
			join++;
			hash[a][b] = 1;
			clear[0][cp] = a;
			clear[1][cp++] = b;
		}	
	}

	//hash dst
	for(i = -1; i < t_bytes; i++)
	{	
		a = (i < 0) ? '$' : t_data[i];
		b = (i + 1 < t_bytes) ? t_data[i + 1] : '$';
		switch(hash[a][b])
		{
			case 0: join++; hash[a][b] = 2; clear[0][cp] = a; clear[1][cp++] = b; break;
			case 1: intersect++; hash[a][b] = 3; break;
			default: break;
		}
	}

	//clear hash[][]
	for(i = 0; i < cp; i++)
		hash[clear[0][i]][clear[1][i]] = 0;

	PG_RETURN_FLOAT8(intersect / join);
}