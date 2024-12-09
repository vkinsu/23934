#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t beep_count = 0;  // Счетчик для подсчета сигналов SIGINT

void handle_sigint() {
    beep_count++;
    write(STDOUT_FILENO, "\a", 1);  // Звуковой сигнал
}

void handle_sigquit() {
    printf("\nПрозвучало %d звуковых сигналов. Завершение работы.\n", beep_count);
    exit(0);
}

int main() {
    // Устанавливаем обработчики сигналов
    struct sigaction sa_int, sa_quit;

    sa_int.sa_handler = handle_sigint;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = 0;

    sa_quit.sa_handler = handle_sigquit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = 0;

    sigaction(SIGINT, &sa_int, NULL);
    sigaction(SIGQUIT, &sa_quit, NULL);

    // Бесконечный цикл
    while (1) {
        pause();  // Ожидание сигналов
    }

    return 0;
}

