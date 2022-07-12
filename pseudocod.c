Quicksort(vetor v[], var ma, var me)
{
    var pivo = v[0];      // pivo = primeiro elemento
    var troca;
    var i = ma;          // i procura o maior
    var j = me;         // j procura o menor

    enquanto(i < j)
    {
        if(v[i] <= pivo)
        {
            i++;
        }
        
        if(v[j] > pivo)
        {
            j--;
        }

        if(i <= j)
        {
            troca = v[i];
            v[i] = v[j];
            v[j] = troca;
            i++;
            j++;
        }
    }

    se(j > ma)
    {
        Quicksort(v[], ma, j);
    }
    ou então(i < me)
    {
        Quicksort(v[], i, me);
    }
}