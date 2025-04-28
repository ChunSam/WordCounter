#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN 100 // �� �ܾ� �ִ� ����
#define BUF_SIZE 4096 // ���� ũ��

//�Լ� ����
void print_usage(const char *progname);
FILE *open_text_file(const char *path);
unsigned long count_words(FILE *fp);
int is_delimiter(int ch);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = open_text_file(argv[1]);
    if (!fp) return EXIT_FAILURE;

    unsigned long word_count = count_words(fp);
    printf("�ܾ� ����: %lu\n", word_count);

    fclose(fp);

    printf("���α׷� ����\n");

    return EXIT_SUCCESS;
}

// �Լ� ����
void print_usage(const char *progname)
{
    printf("����: %s <�ؽ�Ʈ ���� ���>\n", progname);
}

FILE *open_text_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        perror("���� ���� ����");
    }
    return fp;
}

/* �ٽ� �Լ�: �ܾ� ���� */
unsigned long count_words(FILE *fp)
{
    char buffer[BUF_SIZE];
    unsigned long wc = 0;
    int in_word = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        for (char *p = buffer; *p; ++p) {
            if (is_delimiter(*p)) {
                in_word = 0;
            } else {
                if (!in_word) {
                    wc++;
                    in_word = 1;
                }
            }
        }
    }
    return wc;
}

int is_delimiter(int ch)
{
    return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r';
}