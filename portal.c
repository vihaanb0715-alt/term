#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// Define virtual terminal processing flag if missing in older headers
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

// Adapt POSIX sleep functions to Windows API (Sleep accepts milliseconds)
#define usleep(us) Sleep((us) / 1000)
#define sleep(s) Sleep((s) * 1000)

// Function to print text letter-by-letter with a delay
void type_text(const char *text, unsigned int delay_ms) {
    while (*text) {
        putchar(*text++);
        fflush(stdout);
        usleep(delay_ms * 1000);
    }
}

// Helper function to safely play your own local MP3 sound file in the background on Windows
void play_sound() {
    // Uses Windows PowerShell and WMPlayer COM object in background mode to play alarm.mp3
    system("start /b powershell -c \"$player = New-Object -ComObject WMPlayer.OCX; $player.URL = 'alarm.mp3'; $player.controls.play(); Start-Sleep -s 1\" >nul 2>&1");
}

// Generates the terminal glitch animation frame with centered text
void play_glitch_screen() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    const char glitch_chars[] = "0123456789ABCDEF@#$%&*µßΩ█▓▒░";
    int pool_size = sizeof(glitch_chars) - 1;

    const char msg[] = "YoUrE cOoKeD bUdDy";
    int msg_len = sizeof(msg) - 1;
    int center_row = rows / 2;
    int start_col = (cols - msg_len) / 2;

    for (int frame = 0; frame < 40; frame++) {
        printf("\033[2J\033[H");

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                if (r == center_row && c >= start_col && c < start_col + msg_len && frame % 2 == 0) {
                    printf("\033[1;31m%c", msg[c - start_col]);
                } else {
                    if (rand() % 15 == 0) {
                        printf("\033[1;32m%c", glitch_chars[rand() % pool_size]);
                    } else if (rand() % 200 == 0) {
                        printf("\033[1;37m█");
                    } else {
                        printf(" ");
                    }
                }
            }
            if (r < rows - 1) printf("\n");
        }
        fflush(stdout);
        usleep(100000);
    }
}

// Generates a hybrid screen: fixed critical warnings at the top, and live glitch characters at the bottom
void play_overload_countdown() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    const char glitch_chars[] = "0123456789ABCDEF@#$%&*µßΩ█▓▒░";
    int pool_size = sizeof(glitch_chars) - 1;

    // Loop through 3 distinct tracking seconds to draw the countdown live
    for (int count = 3; count > 0; count--) {
        play_sound(); // Play your custom alarm sound file on each second tick

        // Render 10 visual frame refreshes per second to keep the glitch characters moving fluidly
        for (int frame = 0; frame < 10; frame++) {
            printf("\033[2J\033[H"); // Reset cursor to top left

            // Render row tracking layout
            for (int r = 0; r < rows; r++) {
                if (r == 1) {
                    // Row 1: Print the primary alert status banner in bright red
                    printf("\033[1;31m[!] CRITICAL SYSTEM OVERLOAD DETECTED.");
                    // Fill remaining columns in the line so background formatting doesn't break
                    for(int pad = 38; pad < cols; pad++) printf(" ");
                } 
                else if (r == 3) {
                    // Row 3: Print the moving countdown ticker line
                    printf("\033[1;33mSelf-destructing terminal in %d... \033[0m", count);
                    for(int pad = 34; pad < cols; pad++) printf(" ");
                } 
                else if (r >= 5) {
                    // Rows 5 and below: Draw the active glitch background matrix columns
                    for (int c = 0; c < cols; c++) {
                        if (rand() % 12 == 0) {
                            printf("\033[0;32m%c", glitch_chars[rand() % pool_size]);
                        } else {
                            printf(" ");
                        }
                    }
                } 
                else {
                    // Handle buffer padding spaces for empty structural tracking rows
                    for(int pad = 0; pad < cols; pad++) printf(" ");
                }
                if (r < rows - 1) printf("\n");
            }
            fflush(stdout);
            usleep(100000); // 100ms refresh rate loop pattern
        }
    }
}

int main() {
    // Enable ANSI escape sequence formatting natively in Windows Console
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    // Declared as a buffer array to properly accept keyboard input text strings
    char input[256];

    // 1. Set terminal text to dark green (\033[0;32m) and clear screen
    printf("\033[0;32m\033[2J\033[H");
    fflush(stdout);

    // 2. Type out setup text letter-by-letter
    type_text("Setting up hacking portal...\n", 80);
    sleep(1); 

    // 3. Prompt user for command input
    type_text("Enter command to hack into system: ", 50);
    
    // 4. Accept input safely via standard stdin channel
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }

    // 5. Run "type file", print its contents, and hold it on screen for 1 second (Windows equivalent of cat)
    system("type file");
    sleep(1);

    // 6. Run the terminal-native full screen glitch font animation
    play_glitch_screen();

    // 7. Run the color flashing loop sequence
    // Red background
    printf("\033[41m\033[2J\033[H");
    fflush(stdout);
    usleep(400000);

    // Green background
    printf("\033[42m\033[2J\033[H");
    fflush(stdout);
    usleep(400000);

    // Blue background
    printf("\033[44m\033[2J\033[H");
    fflush(stdout);
    usleep(400000);

    // 8. Reset terminal back to default text/background colors and clear
    printf("\033[0m\033[2J\033[H");
    fflush(stdout);

    // 9. Execute the customized dual overlay glitch countdown and audio track script
    play_overload_countdown();

    // 10. Send close command to the console window to close the application
    PostMessage(GetConsoleWindow(), WM_CLOSE, 0, 0);

    return 0;
}