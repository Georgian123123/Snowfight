// Copyright 2018 Comana Marian Georgian <georgiancomana1119@gmail.com>
#include <stdio.h>
#define NMAX 250

int validation_1(int board[NMAX][NMAX], int i, int j) {
    // functie de validare a mutarilor
    if (board[i][j] == 1 || board[i][j] == 2) {
        return -1;
    } else {
        return 1;
    }
}

int validation_2(int i, int j, int n) {
    // functie de validare a indicilor
    if (i < 0 || i >= n * n || j < 0 || j >= n * n) {
        return -1;
    } else {
        return 1;
    }
}

int validation_3(char mutare, char mutare_prec) {
    // functie de validare a mutarilor precedente & actuale
    if (mutare == mutare_prec) {
        return -1;
    } else {
        return 1;
    }
}

void afisare(int n, int macro[NMAX][NMAX]) {
      // functi de afisare
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (macro[i][j] == 0) {
                printf("-");
            }
            if (macro[i][j] == 1) {
                printf("X");
            }
            if (macro[i][j] == 2) {
                printf("0");
            }
        }
        printf("\n");
    }
}

int get_number(int x, int y, int n) {
     // functie de calculare a miniboardului
    return ((x / n) * n + y / n);
}

int check_winner(int n, int colt_x, int colt_y, int board[NMAX][NMAX]) {
    // functie de verificare pentru cine castiga
    // verific in primul "for" diagonala principala
    // verific in al doilea "for" diagonala secundara
    // urmatoarele "foruri" verific liniile & coloanele
    int contor = 0, val = board[colt_x][colt_y];
    for (int i = 0; i < n; ++i) {
        if (board[colt_x + i][colt_y + i] == val) {
            ++contor;
        }
    }
    if (contor == n && val > 0) {
        return val;
    }
    contor = 0, val = board[colt_x][colt_y + n - 1];
    for (int i = 0; i < n; ++i) {
        if (board[colt_x + i][colt_y + n - 1 - i] == val) {
            ++contor;
        }
    }
    if (contor == n && val > 0) {
        return val;
    }
    for (int i = 0; i < n; ++i) {
        val = board[colt_x + i][colt_y], contor = 0;
        for (int j = 0; j < n; ++j) {
            if (val == board[colt_x + i][colt_y + j]) {
                ++contor;
            }
        }
        if (contor == n && val > 0) {
            return val;
        }
    }
    for (int j = 0; j < n; ++j) {
        val = board[colt_x][colt_y + j], contor = 0;
        for (int i = 0; i < n; ++i) {
            if (val == board[colt_x + i][colt_y + j]) {
                ++contor;
            }
        }
        if (contor == n && val > 0) {
            return val;
        }
    }
    return 0;
}

int round_robin(int board[NMAX][NMAX], int n, int r, int macro[NMAX][NMAX]) {
     // functie de verificare a diagonalelor
    int colt_x, colt_y, macro_x, macro_y;
    if (r == 'X') {
        r = 1;
    } else {
        r = 2;
    }
    int n2 = n * n, gasit = 0;
    for (int d = 0; d < 2 * n2; ++d) {
        int lin, col;
        if (d % 2 == 0) {
            lin = d / 2;
            col = 0;
        } else {
            lin = 0;
            col = (d + 1) / 2;
        }
        while (lin < n2 && col < n2 && board[lin][col] != 0) {
            lin++;
            col++;
        }
        if (!(lin == n2 || col == n2)) {
            board[lin][col] = r;
            gasit = 1;
            colt_x = get_number(lin, col, n) / n * n;
            colt_y = get_number(lin, col, n) % n * n;
            macro_x = get_number(lin, col, n) / n;
            macro_y = get_number(lin, col, n) % n;
            if (macro[macro_x][macro_y] == 0) {
            macro[macro_x][macro_y] = check_winner(n, colt_x, colt_y, board);
            }
            break;
        }
    }
    if (gasit == 0) {
    printf("FULL BOARD\n");
    return gasit;
    } else {
        return gasit;
    }
}

int player(int i, int j, int board[NMAX][NMAX], int n) {
      // functie de combinare a conditiilor
    if (validation_2(i, j, n) == -1) {
        printf("INVALID INDEX\n");
        return -1;
    }
    if (validation_1(board, i, j) == -1) {
        printf("NOT AN EMPTY CELL\n");
        return -1;
    }
    return 0;
}

int check_macro(int n, int colt_x, int colt_y, int macro[NMAX][NMAX]) {
     // functie de verificare a castigatorului macroboardului
    int contor = 0, val = macro[colt_x][colt_y], winX = 0, win0 = 0;
    for (int i = 0; i < n; ++i) {
        if (macro[colt_x + i][colt_y + i] == val) {
            ++contor;
        }
    }
    if (contor == n) {
        if (val == 1) {
            ++winX;
        } else {
            if (val == 2) {
            ++win0;
            }
        }
    }
    contor = 0, val = macro[colt_x][colt_y + n - 1];
    for (int i = 0; i < n; ++i) {
        if (macro[colt_x + i][colt_y + n - 1 - i] == val) {
            ++contor;
        }
    }
    if (contor == n) {
        if (val == 1) {
            ++winX;
        } else {
            if (val == 2) {
                ++win0;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        val = macro[colt_x + i][colt_y], contor = 0;
        for (int j = 0; j < n; ++j) {
            if (val == macro[colt_x + i][colt_y + j]) {
                ++contor;
            }
        }
        if (contor == n) {
            if (val == 1) {
                ++winX;
            } else {
                if (val == 2) {
                    ++win0;
                }
            }
        }
    }
    for (int j = 0; j < n; ++j) {
        val = macro[colt_x][colt_y + j], contor = 0;
        for (int i = 0; i < n; ++i) {
            if (val == macro[colt_x + i][colt_y + j]) {
                ++contor;
            }
        }
      if (contor == n) {
            if (val == 1) {
                ++winX;
            } else {
                if (val == 2) {
                    ++win0;
                }
            }
        }
    }
    if (winX > win0) {
        return 1;
    }
    if (win0 > winX) {
        return 2;
    }
    if (win0 == winX) {
        return 0;
    }
    return -5;
}

void boarding(int board[NMAX][NMAX], int mutare, int macro[NMAX][NMAX], int n,
int macro_x, int macro_y, int colt_x, int colt_y, int i,
int j, int contorizare[2]) {
     // functie de completare a boardului si macroboardului
    if (mutare == 'X') {
        board[i][j] = 1;
    } else {
        board[i][j] = 2;
    }
    if (macro[macro_x][macro_y] == 0) {
        macro[macro_x][macro_y] = check_winner(n, colt_x, colt_y, board);
        if (macro[macro_x][macro_y] == 1) {
            contorizare[0]++;
        }
        if (macro[macro_x][macro_y] == 2) {
            contorizare[1]++;
        }
    }
}

int main() {
    // declar variabilele
    int n, m, board[NMAX][NMAX] = {0}, i, j, ok = 0, verificare = 1,
    colt_x, colt_y, macro_x, macro_y, macro[NMAX][NMAX],
    contor_x = 0, contor_y = 0, contorizare[2] = {0};
    char mutare, mutare_prec = 'O';
    scanf("%d%d", &n, &m);
    // citesc mutarile si verific conditiile
    for (int k = 0; k < m; ++k) {
        scanf("%c", &mutare);
        scanf("%c%d%d", &mutare, &i, &j);
        if (verificare == 1) {
            if (mutare == '0' && ok == 0) {
                printf("NOT YOUR TURN\n");
            } else {
                ok = 1;
                if (validation_3(mutare, mutare_prec) == -1) {
                    printf("NOT YOUR TURN\n");
                    mutare_prec = mutare;
                } else {
                    mutare_prec = mutare;
                    if (player(i, j, board, n) == -1) {
                        verificare = round_robin(board, n, mutare, macro);
                        if (mutare == 'X') {
                            ++contor_x;
                        }
                        if (mutare == '0') {
                            ++contor_y;
                        }
                    } else {
                        colt_x = get_number(i, j, n) / n * n;
                        colt_y = get_number(i, j, n) % n * n;
                        macro_x = get_number(i, j, n) / n;
                        macro_y = get_number(i, j, n) % n;
                        boarding(board, mutare, macro, n, macro_x, macro_y,
                        colt_x, colt_y, i, j, contorizare);
                        if (mutare == 'X') {
                            ++contor_x;
                        }
                        if (mutare == '0') {
                            ++contor_y;
                        }
                    }
                }
            }
        }
    }
    // afisez macroboardul si afisez cine castiga
    // afisez coeficientul de atentie
    afisare(n, macro);
    if (check_macro(n, 0, 0, macro) == 1) {
        printf("X won\n");
    }
    if (check_macro(n, 0, 0, macro) == 2) {
        printf("0 won\n");
    }
    if (check_macro(n, 0, 0, macro) == 0) {
        printf("Draw again! Let's play darts!\n");
    }
    if (contor_x == 0) {
        printf("X N/A\n");
    } else {
        printf("X %.10lf\n", contorizare[0] * 1.0 / contor_x);
    }
    if (contor_y == 0) {
        printf("0 N/A\n");
    } else {
       printf("0 %.10lf\n", contorizare[1]* 1.0 / contor_y);
    }
    return 0;
}
