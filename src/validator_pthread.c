#include "validator_pthread.h"
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

int sudoku[9][9];
bool row_valid[9];
bool col_valid[9];
bool grid_valid[9];

bool check_row(int row) {
    bool seen[10] = { false };
    for (int i = 0; i < 9; i++) {
        int num = sudoku[row][i];
        if (num < 1 || num > 9 || seen[num]) return false;
        seen[num] = true;
    }
    return true;
}

bool check_col(int col) {
    bool seen[10] = { false };
    for (int i = 0; i < 9; i++) {
        int num = sudoku[i][col];
        if (num < 1 || num > 9 || seen[num]) return false;
        seen[num] = true;
    }
    return true;
}

bool check_grid(int startRow, int startCol) {
    bool seen[10] = { false };
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int num = sudoku[startRow + i][startCol + j];
            if (num < 1 || num > 9 || seen[num]) return false;
            seen[num] = true;
        }
    }
    return true;
}

void* check_row_thread(void* arg) {
    int row = *(int*)arg;
    row_valid[row] = check_row(row);
    return NULL;
}

void* check_col_thread(void* arg) {
    int col = *(int*)arg;
    col_valid[col] = check_col(col);
    return NULL;
}

void* check_grid_thread(void* arg) {
    int grid = *(int*)arg;
    int startRow = (grid / 3) * 3;
    int startCol = (grid % 3) * 3;
    grid_valid[grid] = check_grid(startRow, startCol);
    return NULL;
}

bool validate_sudoku(int input_sudoku[9][9]) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            sudoku[i][j] = input_sudoku[i][j];
        }
    }

    for (int i = 0; i < 9; i++) {
        row_valid[i] = col_valid[i] = grid_valid[i] = false;
    }

    pthread_t row_threads[9], col_threads[9], grid_threads[9];
    int row_i[9], col_i[9], grid_i[9];

    for (int i = 0; i < 9; i++) {
        row_i[i] = i;
        pthread_create(&row_threads[i], NULL, check_row_thread, &row_i[i]);
    }

    for (int i = 0; i < 9; i++) {
        col_i[i] = i;
        pthread_create(&col_threads[i], NULL, check_col_thread, &col_i[i]);
    }

    for (int i = 0; i < 9; i++) {
        grid_i[i] = i;
        pthread_create(&grid_threads[i], NULL, check_grid_thread, &grid_i[i]);
    }

    for (int i = 0; i < 9; i++) {
        pthread_join(row_threads[i], NULL);
        pthread_join(col_threads[i], NULL);
        pthread_join(grid_threads[i], NULL);
    }

    for (int i = 0; i < 9; i++) {
        if (!row_valid[i] || !col_valid[i] || !grid_valid[i]) {
            return false;
        }
    }

    return true;
}