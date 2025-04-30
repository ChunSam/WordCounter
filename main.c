#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN 100 // 한 단어 최대 길이
#define BUF_SIZE 4096 // 버퍼 크기

typedef struct {
    unsigned long word_count;
    unsigned long line_count;
} WordCount;

//함수 선언
void print_usage(const char *progname);
FILE *open_text_file(const char *path);
WordCount count_words(FILE *fp);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    FILE *fp = open_text_file(argv[1]);
    if (!fp) return EXIT_FAILURE;

    WordCount wc = count_words(fp);
    printf("단어 개수: %lu\n", wc.word_count);
    printf("줄 개수: %lu\n", wc.line_count);

    fclose(fp);

    printf("프로그램 종료\n");

    return EXIT_SUCCESS;
}

// 함수 정의
void print_usage(const char *progname)
{
    printf("사용법: %s <텍스트 파일 경로>\n", progname);
}

FILE *open_text_file(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp)
    {
        perror("파일 열기 실패");
    }
    return fp;
}

/* 핵심 함수: 단어 세기 */
WordCount count_words(FILE *fp)
{
    char buffer[BUF_SIZE];
    WordCount wc = {0, 0};
    int in_word = 0;
    int in_line = 0;

    while (fgets(buffer, sizeof(buffer), fp)) {
        for (char *p = buffer; *p; ++p) {
            if (isspace((unsigned char)*p)) {
                in_word = 0;
                if (*p == '\n') in_line = 0;
            } else {
                if (!in_word) {
                    wc.word_count++;
                    in_word = 1;
                    if (!in_line) {
                        wc.line_count++;
                        in_line = 1;
                    }
                }
            }
        }
    }
    return wc;
