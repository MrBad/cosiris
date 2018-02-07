int atoi(const char *s)
{
    int n = 0;

    for (n = 0; *s >= '0' && *s <= '9'; s++)
        n = n * 10 + *s - '0';
    
    return n;
}
