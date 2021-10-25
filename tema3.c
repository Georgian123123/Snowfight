// COPYRIGHT Comana Marian-Georgian 311 CA <georgiancomana1119@gmail.com>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*definesc structurile atat pentru fiecare jucator
cat si pentru fiecare celula din matrice */

typedef struct {
  int hp, stamina, gloves, x, y, id, score;
  char name[256];
} players;

typedef struct {
  int altitude, gloves;
  int  id;
} cells;
void change_gloves(players *vec, cells** matrix, int i) {
int aux;
  if (vec[i].gloves < matrix[vec[i].x][vec[i].y].gloves) {
                aux = vec[i].gloves;
                vec[i].gloves = matrix[vec[i].x][vec[i].y].gloves;
                matrix[vec[i].x][vec[i].y].gloves = aux;
                }
}
// functie de ucidere a unui jucator
void player_kill(players *vec, int i) {
  vec[i].hp = 0;
  vec[i].stamina = 0;
  vec[i].x = 0;
  vec[i].y = 0;
}
// functie de scadere de stamina
void take_stamina(players *vec, int dif, int i) {
  vec[i].stamina -= dif;
}
// functie de schimbare de coordonate in cazul unei mutari din comanda MOVE
// in cazul in care se merge la dreapta creste coordonata pe y
// in cazul in care se merge la stanga scade coordonata pe y
// in cazul in care se merge in jos creste coordonata pe x
// in cazul in care se merge in sus scade coordonata pe y
void change_coords(players *vec, cells **matrix, char move, int i) {
      matrix[vec[i].x][vec[i].y].id = -1;
      if (move == 'R') {
          vec[i].y++;
      }
      if (move == 'L') {
          vec[i].y--;
      }
      if (move == 'U') {
          vec[i].x--;
      }
      if (move == 'D') {
          vec[i].x++;
      }
}
// verific daca jucatorul este pe ghetar
int check_player(players *vec, int R, int i) {
         if ((vec[i].x - R) * (vec[i].x - R) +
         (vec[i].y - R) * (vec[i].y - R) > R * R) {
          return 0;
         }
  return 1;
}
// citesc date din fisier si le bag in matricea mea
cells** read_structs(players vec[256], cells** matrix, int P,
int R, FILE *input, FILE * output, int *total_players) {
cells **temp_matrix;
  temp_matrix = (cells**)malloc((2 * R + 1)*sizeof(cells*));
     for (int i = 0; i < 2 * R + 1; ++i) {
      temp_matrix[i] =(cells*)malloc((2 * R + 1)*sizeof(cells));
     }
// citesc caracteristicile fiecarei celule
     for (int i = 0 ; i < 2 * R + 1; ++i) {
      for (int j = 0; j < 2 * R + 1; ++j) {
        fscanf(input, "%d%d", &temp_matrix[i][j].altitude,
        &temp_matrix[i][j].gloves);
        temp_matrix[i][j].id = -1;
      }
     }
// citesc caracteristicile fiecareu jucator
     for (int i = 0; i < P; ++i) {
      fscanf(input, "%s", vec[i].name);
      fscanf(input, "%d%d%d%d", &vec[i].x,
      &vec[i].y, &vec[i].hp, &vec[i].stamina);
      vec[i].score = 0;
      vec[i].id = i;
      vec[i].gloves = temp_matrix[vec[i].x][vec[i].y].gloves;
      temp_matrix[vec[i].x][vec[i].y].gloves = 0;
      temp_matrix[vec[i].x][vec[i].y].id = i;
      if (check_player(vec, R, i) == 0) {
        fprintf(output, "%s has missed the glacier.\n", vec[i].name);
        player_kill(vec, i);
        *total_players -= 1;
      }
    }
// copiez elementele in matricea mea
    for (int i = 0; i < 2 * R + 1; ++i) {
      for (int j = 0; j < 2 * R + 1; ++j) {
        memcpy(&matrix[i][j], &temp_matrix[i][j], sizeof(cells));
      }
    }
// eliberez matricea temporara
    for (int i = 0; i < 2 * R + 1; ++i) {
      free(temp_matrix[i]);
    }
    free(temp_matrix);
// returnez matricea mea
    return matrix;
}
// functie de lupta a doi elfi
int elves_fight(players vec[256], cells **matrix,
FILE * output, int P, int i , int *total_players) {
  int j = 0;
// verific care jucator e in casuta respectiva
  for (j = 0; j < P; ++j) {
        if (matrix[vec[i].x][vec[i].y].id == vec[j].id) {
          break;
        }
  }
// cel de are stamina mai mare loveste primul
  if (vec[i].stamina >= vec[j].stamina) {
    while (vec[i].hp > 0 && vec[j].hp > 0) {
// cel lovit primeste dmg si ii scade hp-ul
            vec[j].hp -= vec[i].gloves;
            if (vec[j].hp <= 0) {
            vec[i].score++;
            vec[i].stamina += vec[j].stamina;
          fprintf(output, "%s sent %s back home.\n",
          vec[i].name, vec[j].name);
          matrix[vec[i].x][vec[i].y].id = vec[i].id;
          player_kill(vec, j);
          *total_players -= 1;
          return 0;
             } else {
// in caz contrat loveste celalalt.
                     vec[i].hp -= vec[j].gloves;
                     if (vec[i].hp <= 0) {
                      vec[j].score++;
                      vec[j].stamina += vec[i].stamina;
                        fprintf(output, "%s sent %s back home.\n",
                        vec[j].name, vec[i].name);
                        player_kill(vec, i);
                        *total_players -= 1;
                        return 1;
                     }
            }
    }
  }
// acelasi caz ca si mai sus
  if (vec[i].stamina < vec[j].stamina) {
    while (vec[i].hp > 0 && vec[j].hp > 0) {
            vec[i].hp -= vec[j].gloves;
            if (vec[i].hp <= 0) {
            vec[j].score++;
            vec[j].stamina += vec[i].stamina;
          fprintf(output, "%s sent %s back home.\n",
          vec[j].name, vec[i].name);
          player_kill(vec, i);
          *total_players -= 1;
          return 1;
            } else {
                     vec[j].hp -= vec[i].gloves;
                     if (vec[j].hp <= 0) {
                        vec[i].score++;
                        vec[i].stamina += vec[j].stamina;
                        fprintf(output, "%s sent %s back home.\n",
                        vec[i].name, vec[j].name);
                         matrix[vec[i].x][vec[i].y].id = vec[i].id;
                        player_kill(vec, j);
                        *total_players -= 1;
                        return 0;
                     }
            }
        }
  }
  return -1;
}
// functie de mutare a unui jucator
int MOVE_COMMAND(players vec[256], cells **matrix, int P, int R,
FILE *input, FILE * output,  char move[10], int *total_players) {
  char where[20], command[] = "MOVE";
  if (strcmp(move, command) == 0) {
      int id, dif, i = 0, wonner = 0, j, l;
// citesc intrarile.
// citesc id-ul si UDLR-ul
    fscanf(input, "%d", &id);
    fscanf(input, "%s", where);
    for (i = 0; i < P ; ++i) {
        if (vec[i].id == id) {
        break;
        }
    }
     l = strlen(where);
// parcul UDLR-ul
// functionare : observ unde se muta.Verific daca are stamina suficienta
// schimb coordonatele, schimb manusile cu cele din casuta doar daca sunt
// mai mari
// scad stamina.Verific daca jucatorul mai este pe ghetar.
// Verific daca se afla alt elf acolo si daca da, se lupta.
    for (j = 0; j <= l - 1; ++j) {
          if (where[j] == 'R' && vec[i].x != 2 * R) {
             dif = abs(matrix[vec[i].x][vec[i].y + 1].altitude
              - matrix[vec[i].x][vec[i].y].altitude);
             if (vec[i].stamina >= dif) {
                change_coords(vec, matrix, where[j], i);
                change_gloves(vec, matrix, i);
                take_stamina(vec, dif, i);
                if (check_player(vec, R, i) == 0 && vec[i].hp > 0) {
                   fprintf(output, "%s fell off the glacier.\n", vec[i].name);
                   player_kill(vec, i);
                   *total_players -= 1;
                   return 0;
                }
                if (matrix[vec[i].x][vec[i].y].id != -1) {
                 wonner = elves_fight(vec, matrix, output, P, i, total_players);
                  if (wonner == 1) {
                    return 0;
                  }
                }
              if (matrix[vec[i].x][vec[i].y].id == -1) {
              matrix[vec[i].x][vec[i].y].id = vec[i].id;
                }
            }
         }
          if (where[j] == 'L' && vec[i].y != 0) {
             dif = abs(matrix[vec[i].x][vec[i].y - 1].altitude
              - matrix[vec[i].x][vec[i].y].altitude);
             if (vec[i].stamina >=dif) {
                change_coords(vec, matrix, where[j], i);
                change_gloves(vec, matrix, i);
                // printf("%d %d ", vec[i].x, vec[i].y);
                take_stamina(vec, dif, i);
                if (check_player(vec, R, i) == 0 && vec[i].hp > 0) {
                   fprintf(output, "%s fell off the glacier.\n", vec[i].name);
                   player_kill(vec, i);
                   *total_players -= 1;
                   return 0;
                }
                if (matrix[vec[i].x][vec[i].y].id != -1) {
                 wonner = elves_fight(vec, matrix, output, P, i, total_players);
                  if (wonner == 1) {
                    return 0;
                  }
                }
               if (matrix[vec[i].x][vec[i].y].id == -1) {
              matrix[vec[i].x][vec[i].y].id = vec[i].id;
                }
             }
          }
          if (where[j] == 'U' &&  vec[i].x != 0) {
             dif = abs(matrix[vec[i].x - 1][vec[i].y].altitude
              - matrix[vec[i].x][vec[i].y].altitude);
             if (vec[i].stamina >= dif) {
                change_coords(vec, matrix, where[j], i);
                change_gloves(vec, matrix, i);
                take_stamina(vec, dif, i);
                if (check_player(vec, R, i) == 0 && vec[i].hp > 0) {
                   fprintf(output, "%s fell off the glacier.\n", vec[i].name);
                   player_kill(vec, i);
                   *total_players -= 1;
                   return 0;
                }
                if (matrix[vec[i].x][vec[i].y].id != -1) {
                 wonner = elves_fight(vec, matrix, output, P, i, total_players);
                  if (wonner == 1) {
                    return 0;
                  }
                }
               if (matrix[vec[i].x][vec[i].y].id == -1) {
              matrix[vec[i].x][vec[i].y].id = vec[i].id;
                }
            }
         }
          if (where[j] == 'D' &&  vec[i].x != (2 * R)) {
             dif = abs(matrix[vec[i].x + 1][vec[i].y].altitude
              - matrix[vec[i].x][vec[i].y].altitude);
             if (vec[i].stamina >= dif) {
                change_coords(vec, matrix, where[j], i);
                change_gloves(vec, matrix, i);
                take_stamina(vec, dif, i);
                if (check_player(vec, R, i) == 0 && vec[i].hp > 0) {
                   fprintf(output, "%s fell off the glacier.\n", vec[i].name);
                   player_kill(vec, i);
                   *total_players -= 1;
                   return 0;
                }
                if (matrix[vec[i].x][vec[i].y].id != -1) {
                 wonner = elves_fight(vec, matrix, output, P, i, total_players);
                  if (wonner == 1) {
                    return 0;
                  }
                }
              if (matrix[vec[i].x][vec[i].y].id == -1) {
              matrix[vec[i].x][vec[i].y].id = vec[i].id;
              }
            }
         }
      }
    }
  return -1;
}
// functie de afisare pentru caracteristicile unui jucator.
// functionare : se sorteaza dupa : dry / wet, scor , lexicografic
void print_scoreboard(players *vec, int P, FILE *output,  char move[20]) {
  char command[20]= "PRINT_SCOREBOARD";
  if (strcmp(command, move) == 0) {
      int dry_vec[256], wet_vec[256], k = 0, j = 0, aux;
    fprintf(output, "SCOREBOARD:\n");
    for (int i = 0; i < P; ++i) {
    if (vec[i].hp <= 0) {
       wet_vec[k] = vec[i].id;
       k++;
        }
        if (vec[i].hp > 0) {
          dry_vec[j] = vec[i].id;
          j++;
        }
      }
      for (int i = 0; i < k; ++i) {
        for (int p = i + 1; p < k ; ++p) {
          if (vec[wet_vec[i]].score < vec[wet_vec[p]].score) {
            aux = wet_vec[i];
                wet_vec[i] = wet_vec[p];
                wet_vec[p] = aux;
          }
        }
      }
      for (int i = 0; i < j; ++i) {
        for (int p = i + 1; p < j ; ++p) {
          if (vec[dry_vec[i]].score < vec[dry_vec[p]].score) {
            aux = dry_vec[i];
                dry_vec[i] = dry_vec[p];
                dry_vec[p] = aux;
          }
        }
      }
       for (int i = 0; i < k; ++i) {
        for (int p = i + 1; p < k; ++p) {
          if (strcmp(vec[wet_vec[i]].name, vec[wet_vec[p]].name) >
            0 && vec[wet_vec[i]].score == vec[wet_vec[p]].score) {
            aux = wet_vec[i];
            wet_vec[i] = wet_vec[p];
            wet_vec[p] = aux;
          }
        }
      }
      for (int i = 0; i < j; ++i) {
        for (int p = i + 1; p < j; ++p) {
          if (strcmp(vec[dry_vec[i]].name, vec[dry_vec[p]].name) >
           0 && vec[dry_vec[i]].score == vec[dry_vec[p]].score) {
            aux = dry_vec[i];
            dry_vec[i] = dry_vec[p];
            dry_vec[p] = aux;
          }
        }
      }
for (int i = 0; i < j; ++i) {
  fprintf(output, "%s\tDRY\t%d\n", vec[dry_vec[i]].name, vec[dry_vec[i]].score);
}
for (int i = 0; i < k; ++i) {
  fprintf(output, "%s\tWET\t%d\n", vec[wet_vec[i]].name, vec[wet_vec[i]].score);
}
    }
}
// functie de dat dmg intr-o anumita suprafata
// functionare : elfii care sunt pe ghetar, in epicentrul de raza R
// primesc dmg
void snowstorm(players *vec, int P, FILE* input,
FILE * output,  char move[20], int *total_players) {
  char command[10] = "SNOWSTORM";
  if (strcmp(move, command) == 0) {
    int x, y, dmg, R;
  long long K;
    fscanf(input, "%lld", &K);
    x = K & 255;
    K = K >> 8;
    y = K & 255;
    K = K >> 8;
    R = K & 255;
    K = K >> 8;
    dmg = K & 255;
    for (int i = 0; i < P; ++i) {
      if (vec[i].hp > 0) {
        if ((vec[i].x - x) * (vec[i].x - x) +
          (vec[i].y - y) * (vec[i].y - y)<= R * R) {
          vec[i].hp -= dmg;
            if (vec[i].hp <= 0) {
               player_kill(vec, i);
               *total_players -= 1;
               fprintf(output, "%s was hit by snowstorm.\n", vec[i].name);
           }
       }
     }
    }
  }
}
// functie de alocare a unui nou ghetar
// functionare : aloc o noua matrice de raza R - 1 in care copiez elementele
// verific daca un jucator este pe ghetarul nou
// dezaloc vechea matrice
cells** MELTDOWN(players *vec, cells **matrix, int P, int *R,
FILE* input, FILE *output, char move[20], int *total_players) {
  char command[10] = "MELTDOWN";
  if (strcmp(move, command) == 0) {
  int supl;
  int dim = 2 * *R + 1;
  cells **temp_matrix;
  fscanf(input, "%d", &supl);
  *R -= 1;
  temp_matrix = (cells**) malloc((2 * *R + 1) * sizeof(cells*));
  for (int i = 0; i < 2 * *R + 1; ++i) {
    temp_matrix[i] = (cells*)malloc((2 * *R + 1)* sizeof(cells));
  }
    for (int i = 0; i < P; ++i) {
      if (vec[i].hp > 0) {
       vec[i].x--;
       vec[i].y--;
       vec[i].stamina += supl;
          if ((vec[i].x - *R) * (vec[i].x - *R) + (vec[i].y - *R)
          * (vec[i].y - *R) > *R * *R) {
fprintf(output, "%s got wet because of global warming.\n", vec[i].name);
            player_kill(vec, i);
            *total_players -= 1;
          }
        }
    }

     for (int i = 0; i < 2 * *R + 1; ++i) {
         for (int j = 0; j < 2 * *R + 1; ++j) {
          memcpy(&temp_matrix[i][j], &matrix[i+1][j+1], sizeof(cells));
         }
      }
      for (int i = 0; i < dim; ++i) {
       free(matrix[i]);
      }
      free(matrix);
      return temp_matrix;
     }
    return matrix;
}
// deschide fisierul de intrare
int openin_file(FILE **input, char *infile) {
  *input = fopen(infile, "r");
    if (input == NULL) {
      fprintf(stderr, "Nu s-a putut deschide pentru a se citi");
      return -1;
    }
    return 0;
}
// deschid fisierul de iesire
int openout_file(FILE **output, char *outfile) {
  *output = fopen(outfile, "w");
    if (output == NULL) {
      fprintf(stderr, "Nu s-a putut scrie");
      return -1;
    }
    return 0;
}
// functie de dezalocare a unei matrici
void dezalloc(cells **matrix, int R) {
  for (int i = 0; i < 2 * R + 1; ++i) {
    free(matrix[i]);
  }
  free(matrix);
}
// verific daca exista vreun castigator.
// daca da, dezaloc matricea , si inchid fisierele
int check_winner(int total_players, players *vec, cells **matrix,
FILE *output, int P, int R, FILE *input) {
int k = -1, i;
  if (total_players == 1) {
    k = 0;
  }
  if (k == 0) {
    for (i = 0; i < P; ++i) {
           if (vec[i].hp != 0) {
            break;
           }
    }
  fprintf(output, "%s has won.\n", vec[i].name);
  dezalloc(matrix, R);
  fclose(input);
  fclose(output);
  return 0;
  }  else {
    return 1;
  }
}
// mainul in care descriu fisierele
// deschid fisierele, citesc raza si numarul de jucatori
// declar matricea de structuri si vectorul de structuri
// aloc dinamic matricea
// citesc din fisier pana la terminarea lui sau,
// pana cand exista un singur jucator pe mapa
int main() {
    char *infile = "snowfight.in";
    char *outfile = "snowfight.out";
    char move[20];
    FILE *input, *output;
    int  R, P, total_players;
    openin_file(&input, infile);
    openout_file(&output, outfile);
     fscanf(input, "%d%d", &R, &P);
     players vec[256];
     cells **matrix;
    matrix = (cells**)calloc(2 * R + 1, sizeof(cells*));
    for (int i = 0; i < 2 * R + 1; ++i) {
     matrix[i] = (cells*)calloc(2 * R + 1, sizeof(cells));
    }
     total_players = P;
     matrix = read_structs(vec, matrix, P, R, input, output, &total_players);
while (fscanf(input, "%s", move) != EOF && total_players != 1) {
    MOVE_COMMAND(vec, matrix, P, R, input, output, move, &total_players);
    snowstorm(vec, P, input, output, move, &total_players);
    print_scoreboard(vec, P, output, move);
    matrix = MELTDOWN(vec, matrix, P, &R,
    input, output, move, &total_players);
}
  if (check_winner(total_players, vec, matrix, output, P, R, input) == 0) {
  return 0;
  }
dezalloc(matrix, R);
}

