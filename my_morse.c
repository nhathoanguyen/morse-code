#include <stdio.h>
#include <string.h>

#define MAX_ENCODE_LEN 10000000L
#define MAX_DECODE_LEN 100L
#define BUFFER_SIZE 512

/* =========================
   KHOI 1: BANG MORSE
   ========================= */

char Morse[36][20] = {
    "+===", "===+++", "===+===+", "===++", "+",
    "++===+", "======+", "++++", "++", "+=========",
    "===+===", "+===++", "======", "===+", "=========",
    "+======+", "======+===", "+===+", "+++", "===",
    "++===", "+++===", "+======", "===++===", "===+======",
    "======++",

    "===============", "+============", "++=========", "+++======",
    "++++===", "+++++", "===++++", "======+++", "=========++",
    "============+"
};

/* =========================
   KHOI 2: HAM TIM MORSE
   Chu -> Morse
   ========================= */

const char* find_morse(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        c = c - 32;
    }

    if (c >= 'A' && c <= 'Z')
    {
        int index = c - 'A';
        return Morse[index];
    }

    if (c >= '0' && c <= '9')
    {
        int index = 26 + (c - '0');
        return Morse[index];
    }

    return NULL;
}

/* =========================
   KHOI 3: HAM TIM CHU
   Morse -> Chu
   ========================= */

char find_char(const char *morse_token)
{
    int i;

    for (i = 0; i < 36; i++)
    {
        if (strcmp(Morse[i], morse_token) == 0)
        {
            if (i < 26)
            {
                return (char)('A' + i);
            }
            else
            {
                return (char)('0' + (i - 26));
            }
        }
    }

    return 0;
}

/* =========================
   KHOI 4: IN 20 KY TU DAU
   ========================= */

int print_preview(void)
{
    FILE *fout;
    char preview[21];
    int preview_len = 0;
    long total_len = 0;
    int ch;

    fout = fopen("output.txt", "r");

    if (fout == NULL)
    {
        printf("Khong doc lai duoc output.txt\n");
        return 1;
    }

    ch = fgetc(fout);

    while (ch != EOF)
    {
        if (preview_len < 20)
        {
            preview[preview_len] = (char)ch;
            preview_len++;
        }

        total_len++;
        ch = fgetc(fout);
    }

    preview[preview_len] = '\0';

    fclose(fout);

    printf("Da ghi ket qua vao output.txt\n");

    if (total_len > 20)
    {
        printf("Ket qua 20 ky tu dau: %s...\n", preview);
    }
    else
    {
        printf("Ket qua: %s\n", preview);
    }

    return 0;
}

/* =========================
   KHOI 4.5: KIEM TRA DO DAI INPUT
   ========================= */

int check_input_length(const char *input_file, long max_len, const char *mode_name)
{
    FILE *fin;
    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    size_t i;
    long count = 0;

    fin = fopen(input_file, "r");

    if (fin == NULL)
    {
        printf("Khong mo duoc file input\n");
        return 1;
    }

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, fin)) > 0)
    {
        for (i = 0; i < bytesRead; i++)
        {
            if (buffer[i] != '\n' && buffer[i] != '\r')
            {
                count++;
            }

            if (count > max_len)
            {
                printf("Loi: input %s vuot qua %ld ky tu\n", mode_name, max_len);
                fclose(fin);
                return 1;
            }
        }
    }

    fclose(fin);
    return 0;
}

/* =========================
   KHOI 5: ENCODE FILE
   Chu thuong -> Morse
   ========================= */

int encode_file(const char *input_file)
{
    FILE *fin;
    FILE *fout;
    int c;
    int has_output = 0;

    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    size_t i;

    if (check_input_length(input_file, MAX_ENCODE_LEN, "encode") != 0)
    {
        return 1;
    }

    fin = fopen(input_file, "r");

    if (fin == NULL)
    {
        printf("Khong mo duoc file input\n");
        return 1;
    }

    fout = fopen("output.txt", "w");

    if (fout == NULL)
    {
        printf("Khong tao duoc file output.txt\n");
        fclose(fin);
        return 1;
    }

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, fin)) > 0)
    {
        for (i = 0; i < bytesRead; i++)
        {
            c = buffer[i];

            if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
            {
                if (has_output == 1)
                {
                    fprintf(fout, " ");
                }
            }
            else
            {
                const char *morse = find_morse((char)c);

                if (morse == NULL)
                {
                    printf("Ky tu khong hop le: %c\n", c);
                    fclose(fin);
                    fclose(fout);
                    return 1;
                }

                if (has_output == 1)
                {
                    fprintf(fout, " ");
                }

                fprintf(fout, "%s", morse);
                has_output = 1;
            }
        }
    }

    fclose(fin);
    fclose(fout);

    return print_preview();
}

/* =========================
   KHOI 6: DECODE FILE
   Morse -> Chu
   ========================= */

int decode_file(const char *input_file)
{
    FILE *fin;
    FILE *fout;
    int c;

    char token[20];
    int token_len = 0;
    int has_output = 0;

    char buffer[BUFFER_SIZE];
    size_t bytesRead;
    size_t i;

    if (check_input_length(input_file, MAX_DECODE_LEN, "decode") != 0)
    {
        return 1;
    }

    fin = fopen(input_file, "r");

    if (fin == NULL)
    {
        printf("Khong mo duoc file input\n");
        return 1;
    }

    fout = fopen("output.txt", "w");

    if (fout == NULL)
    {
        printf("Khong tao duoc file output.txt\n");
        fclose(fin);
        return 1;
    }

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, fin)) > 0)
    {
        for (i = 0; i < bytesRead; i++)
        {
            c = buffer[i];

            if (c == '+' || c == '=')
            {
                if (token_len >= 19)
                {
                    printf("Ma Morse qua dai\n");
                    fclose(fin);
                    fclose(fout);
                    return 1;
                }

                token[token_len] = (char)c;
                token_len++;
            }
            else if (c == ' ')
            {
                if (token_len > 0)
                {
                    char decoded;

                    token[token_len] = '\0';
                    decoded = find_char(token);

                    if (decoded == 0)
                    {
                        printf("Ma Morse khong hop le: %s\n", token);
                        fclose(fin);
                        fclose(fout);
                        return 1;
                    }

                    fprintf(fout, "%c", decoded);

                    has_output = 1;
                    token_len = 0;
                }
                else
                {
                    if (has_output == 1)
                    {
                        fprintf(fout, " ");
                    }
                }
            }
            else if (c == '\n' || c == '\r' || c == '\t')
            {
                if (token_len > 0)
                {
                    char decoded;

                    token[token_len] = '\0';
                    decoded = find_char(token);

                    if (decoded == 0)
                    {
                        printf("Ma Morse khong hop le: %s\n", token);
                        fclose(fin);
                        fclose(fout);
                        return 1;
                    }

                    fprintf(fout, "%c", decoded);

                    has_output = 1;
                    token_len = 0;
                }
            }
            else
            {
                printf("Ky tu khong hop le trong file Morse: %c\n", c);
                fclose(fin);
                fclose(fout);
                return 1;
            }
        }
    }

    if (token_len > 0)
    {
        char decoded;

        token[token_len] = '\0';
        decoded = find_char(token);

        if (decoded == 0)
        {
            printf("Ma Morse khong hop le: %s\n", token);
            fclose(fin);
            fclose(fout);
            return 1;
        }

        fprintf(fout, "%c", decoded);
        has_output = 1;
    }

    fclose(fin);
    fclose(fout);

    return print_preview();
}

/* =========================
   KHOI 7: MAIN
   ========================= */

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Cach dung: %s -e input.txt hoac %s -d input.txt\n", argv[0], argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-e") == 0)
    {
        return encode_file(argv[2]);
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        return decode_file(argv[2]);
    }
    else
    {
        printf("Sai che do. Dung -e de encode hoac -d de decode\n");
        return 1;
    }
}
