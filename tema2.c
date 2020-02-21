#include <stdio.h>
#include <stdlib.h>

int fact(int n)
{
	if(n == 2)
		return 2;
	if(n <= 1)
		return 1;
	return n*fact(n-1);
}

int comb(int n, int k)
{	
	int a,b,c;
	a = fact(n);
	b = fact(k);
	c = fact(n-k);
	return a/(b*c);
}

int* getindex(int **M, int i, int n)
{
	int *v = calloc(n, sizeof(int));
	int j, nr = 0;
	for(j = 0; j < n; j++)
		if(M[i-1][j] == 1)
		{
			v[nr] = j + 1;
			nr++;
		}
	return v;
}

void chg(int *v, int i, int j)
{
	int aux = v[i];
	v[i] = v[j];
	v[j] = aux;
}

void chg2(int *v, int i, int j)
{
	int k, aux;
	aux = v[j];
	for(k = j-1; k >= i; k--)
		v[k+1] = v[ k ];
	v[i] = aux;

}

int main(int argc, char **argv)
{
	int i, j, k, l;
	int n1, n2;
	FILE *in = fopen("graph.in", "r");//graph.in
	FILE *out = fopen("bexpr.out", "w");//bexpr.out
	int n; 						//numarul de noduri
	int *counter, j_a = 0, test = 0, test1 = 0;
	fscanf(in, "%d\n", &n);
	int **M;

	j_a = n/2 +1;
	counter = calloc(n, sizeof(int));
	M = calloc(n, sizeof(int *));
	for(i = 0; i < n; i++)
		M[i] = calloc(n, sizeof(int));

	while(fscanf(in,"%d %d\n", &n1, &n2) == 2)
	{	

		M[n1 - 1][n2 - 1] = 1;
		M[n2 - 1][n1 - 1] = 1;
	}


	//numarul de aparitii pentru fiecare linie
	for( i = 0 ; i < n ; i++)
	{
		for (j = 0;j < n; j++)
		{
			if(M[i][j] == 1)
				counter[i] ++;
		}
	}

	//prima parte din rezolvare
	for(i = 0; i < n; i ++)
		if (counter[i] < 2)
		{
			fprintf(out, "x1-1 & ~x1-1" );
			return 0;
		}	

	for ( i = 1; i <= n; ++i)
	{
		if(counter[i - 1] <= 2)
		{
			fprintf(out, "((" );
			for(j = 1; j <= n; j++)
			{
				if(M[i-1][j-1] == 1)
				{	
					fprintf(out, "x%d-%d",i,j);
					test ++;
				}
				if(test == 1 && M[i-1][j-1] == 1)
					fprintf(out, "&" );
			}
			test = 0;
			fprintf(out, "))&" );
			for(k = 1; k <= j_a; k ++)
			{
				if(i == 1)
					break;
				if(k == 1)
					fprintf(out, "(" );
				fprintf(out, "a%d-%d", k, i);
				if(k == j_a)
					fprintf(out,")&");
				else
					fprintf(out, "|" );
			}
		}
		else
		{
			int *v = getindex(M,i,n);
			int aux1 = 0, aux2 = 1;

			int nrn = comb(counter[i-1],2);
			for (j = 0; j < nrn ; j++)
			{
				aux1++;
				if(j == 0)
					fprintf(out, "(" );

				for(k = 0; k < counter[i-1]; k++)
				{
					if(k == 0)
						fprintf(out, "(");
					fprintf(out, "x%d-%d",i,v[k] );
					if(k != counter[i-1]-1 && k <1)
						fprintf(out, "&" );
					else if(k != counter[i-1]-1)
						fprintf(out, "&~" );
					if(k == counter[i-1]-1)
						fprintf(out, ")" );
				}

				if(j != nrn - 1)
					fprintf(out, "|" );
				else
					fprintf(out, ")&" );
				if(aux1 == counter[i-1] - 1)
				{
					v = getindex(M,i,n);
					chg2(v, 0, aux2);
					chg2(v, 1, aux2+1);
					aux1 = aux2;
					aux2++;
					continue;
				}
				chg(v, 1, aux1 + 1);
			}

			for(k = 1; k <= j_a; k ++)
			{
				if(i == 1)
					break;
				if(k == 1)
					fprintf(out, "(" );
				fprintf(out, "a%d-%d", k, i);
				if(k == j_a)
					fprintf(out,")&");
				else
					fprintf(out, "|" );
			}

		}
	}

	//partea a doua
	for(i = 1; i <= n; i++)
	{	

		for(j = i; j <= n; j++)
		{
			if(M[i-1][j-1] == 1)
			{
				fprintf(out, "(" );
				fprintf(out,"(x%d-%d|~x%d-%d)",i,j,j,i);
				fprintf(out,"&");
				fprintf(out,"(~x%d-%d|x%d-%d)",i,j,j,i);
				fprintf(out,")&");
			}
		}
	}

	for(i = 1; i <=n; i++)
	{
		if(M[0][i-1] == 1)
		{
			fprintf(out, "((a1-%d|~x1-%d)&", i, i);
			fprintf(out, "(~a1-%d|x1-%d))&", i, i );
		}
	}
	for(i = 1; i <=n; i++)
	{
		if(M[0][i-1] == 0)
			fprintf(out, "~a1-%d&", i);
	}


	//a treia parte din rezolvare
	for(i = 2; i <= j_a; i++)
	{
		for(j = 2; j <= n; j++)
		{
			fprintf(out, "((a%d-%d|~",i,j);
			int *v1 = getindex(M,j,n);
			for(k = 0; k < n; k++)
			{
				if(k == 0)
						fprintf(out, "((" );
				if(v1[k] == 1)
				{
					test1 = 1;
					continue;
				}
				if(v1[k] > 1)
				{
					fprintf(out, "(a%d-%d&x%d-%d)",i-1,v1[k],v1[k],j );
					if(k == n-1)
						fprintf(out, ")&" );
					else if(v1[k +1] == 0 )
						fprintf(out, ")&" );
					else
						 fprintf(out, "|" );
				}
				
			}
			
			test1 = 0;
			for(l = 1; l < i; l++)
			{
				if(l == 1)
					fprintf(out, "~(" );
				fprintf(out, "a%d-%d",l,j );

				if(i > 2 && l != i-1)
					fprintf(out, "|" );
				else
					fprintf(out, ")" );
			}
			fprintf(out, "))&");

			fprintf(out, "(~a%d-%d|",i,j);
			int *v2 = getindex(M,j,n);
			for(k = 0; k < n; k++)
			{
				if(k == 0)
					fprintf(out, "((" );
				if(v2[k] == 1)
				{
					test1 = 1;
					continue;
				}
				if(v2[k] > 1)
				{
					fprintf(out, "(a%d-%d&x%d-%d)",i-1,v2[k],v2[k],j );
					if(k == n-1)
						fprintf(out, ")&" );
					else if(v2[k +1] == 0 )
						fprintf(out, ")&" );
					else
						 fprintf(out, "|" );
		
				}
			}
			for(l = 1; l < i; l++)
			{
				if(l == 1)
					fprintf(out, "~(" );
				fprintf(out, "a%d-%d",l,j );
				if(l == i - 1)
				{
					fprintf(out, "))" );
					break;
				}
				if(i > 2 && l != i-1)
					fprintf(out, "|" );
				else
					fprintf(out, ")" );
			}
			test1 = 0;
			if(i == j_a && j == n)
				fprintf(out, "))" );
			else
				fprintf(out, "))&" );
		}

	}

	for(i = 0; i < n; i++)
		free(M[i]);
	free(M);
	free(counter);

	fclose(in);
	fclose(out);
	return 0;
}