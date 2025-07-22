// REVERSE STRING WITH CESAR SYPHER
// 1. Reverse string
// Use two pointer method to replace first with last element in-place
// Edge case: input with new line, new line must be removed
// 2. Cesar cypher
// abc - (offset 2) -> cde
// ABC - (offset 2) -> CDE
// Formula: (char_val - base (lower/upper) + offset) % 26 
// 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
// 122 90
char* encode(char* reversed, int offset) {
    for (int i = 0; reversed[i] != '\0'; i++) {
        if (isdigit(reversed[i])) { continue; }

        int base = 0;
        if (islower(reversed[i])) { base = 97; }
        else { base = 65; }

        int new_val = ((reversed[i] - base + offset) % 26 + base);
        printf("%d\n", new_val);
        reversed[i] = (char)new_val;
    }

    return reversed;
}

char* reverse_string(char* input) {
    int len = strlen(input);

    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    int l = 0, r = len - 1;

    while (l < r) {
        char temp = input[r];
        input[r] = input[l];
        input[l] = temp;
        l++;
        r--;
    }

    return input;
}

const char* solution(char* input, int offset) {
    return encode(reverse_string(input), offset);
}

int main(int argc, char* argv[]) {
    char text[] = "Zz";

    printf("%s\n", solution(text, 1));

    return 0;
}
