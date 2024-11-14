char *clean(char *s, int line) {
    static char temp[512];
    int i, j, len;
    int temp_index = 0;

    /*/ Reverse and trim leading spaces or tabs from both ends*/
    int r;
    for (r = 0; r < 2; r++) {
        len = strlen(s);
        for (i = len - 1; i >= 0 && (s[i] == ' ' || s[i] == '\t'); i--);
        s[i + 1] = '\0';
        for (i = strlen(s) - 1, j = 0; i >= 0; i--, j++) temp[j] = s[i];
        temp[j] = '\0';
        strcpy(s, temp);
    }

    return temp;

void main(){
    char *s = "   hello world   ";
    char *cleaned = clean(s, 0);
    printf("Cleaned string: %s\n", cleaned);
    free(cleaned);
    return 0;
}