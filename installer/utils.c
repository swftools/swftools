char* concatPaths(const char*base, const char*add)
{
    int l1 = strlen(base);
    int l2 = strlen(add);
    int pos = 0;
    char*n = 0;
    while(l1 && base[l1-1] == '\\')
	l1--;
    while(pos < l2 && add[pos] == '\\')
	pos++;

    n = (char*)malloc(l1 + (l2-pos) + 2);
    memcpy(n,base,l1);
    n[l1]='\\';
    strcpy(&n[l1+1],&add[pos]);
    return n;
}

