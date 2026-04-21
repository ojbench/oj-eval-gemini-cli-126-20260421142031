/**
 * This is the student's part for Conway's Game of Life Simulator.
 * You need to finish all functions marked with "TODO" label.
 * When submitting, you should hand in THIS header file (i.e. game_of_life.h) to OJ to get correct mark.
 *
 * By the way, you SHOULD NOT try to read or write any extra information to the console.
 * Any attempt to hack the OJ or special judger WILL BE voided (and punished if it causes severe results).
 *
 * This is part of the 1st homework of Class A and 2nd homework of Class B. The workload isn't large.
 * We've also prepared a small visualizer for you. Enjoy coding ~
 */
#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

/**
 * Feel free to define/modify variables here.
 */
inline int row = -1, col = -1;
inline std::vector<bool> is_alive;
inline std::vector<int> live_cells_indices;

/**
 * This function is called at the beginning of every game.
 * Read the initial map of the game from stdin.
 * The format of the input is written in the problem description.
 *
 * TODO: Read the initial map from input and initialize your variables.
 */
inline void Initialize() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  if (!(std::cin >> col >> row)) return;
  is_alive.assign(row * col, false);
  live_cells_indices.clear();

  char ch;
  int num = 0;
  bool has_num = false;
  int r = 0, c = 0;

  while (std::cin >> ch) {
    if (std::isdigit(ch)) {
      num = num * 10 + (ch - '0');
      has_num = true;
    } else if (ch == 'b' || ch == 'o') {
      int count = has_num ? num : 1;
      if (ch == 'o') {
        for (int i = 0; i < count; ++i) {
          if (r < row && c + i < col) {
            int pos = r * col + (c + i);
            if (!is_alive[pos]) {
              is_alive[pos] = true;
              live_cells_indices.push_back(pos);
            }
          }
        }
      }
      c += count;
      num = 0;
      has_num = false;
    } else if (ch == '$') {
      int count = has_num ? num : 1;
      r += count;
      c = 0;
      num = 0;
      has_num = false;
    } else if (ch == '!') {
      break;
    }
  }
  std::sort(live_cells_indices.begin(), live_cells_indices.end());
}

/**
 * This function is called once in every round of the game.
 * You need to simulate a round of game in this program.
 *
 * Note: You don't need to input or output anything. Just change the variables you defined so that they're in the new round.
 *
 * TODO: Simulate a new round of the game.
 */
inline void Tick() {
  if (live_cells_indices.empty()) return;

  std::vector<int> candidates;
  candidates.reserve(live_cells_indices.size() * 9);
  for (int pos : live_cells_indices) {
    int r = pos / col;
    int c = pos % col;
    for (int dr = -1; dr <= 1; ++dr) {
      int nr = r + dr;
      if (nr < 0 || nr >= row) continue;
      for (int dc = -1; dc <= 1; ++dc) {
        int nc = c + dc;
        if (nc < 0 || nc >= col) continue;
        candidates.push_back(nr * col + nc);
      }
    }
  }
  std::sort(candidates.begin(), candidates.end());
  candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

  std::vector<int> next_live_cells;
  for (int pos : candidates) {
    int r = pos / col;
    int c = pos % col;
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr) {
      int nr = r + dr;
      if (nr < 0 || nr >= row) continue;
      for (int dc = -1; dc <= 1; ++dc) {
        if (dr == 0 && dc == 0) continue;
        int nc = c + dc;
        if (nc < 0 || nc >= col) continue;
        if (is_alive[nr * col + nc]) count++;
      }
    }

    if (is_alive[pos]) {
      if (count == 2 || count == 3) next_live_cells.push_back(pos);
    } else {
      if (count == 3) next_live_cells.push_back(pos);
    }
  }

  for (int pos : live_cells_indices) is_alive[pos] = false;
  for (int pos : next_live_cells) is_alive[pos] = true;
  live_cells_indices = std::move(next_live_cells);
}

/**
 * This function may be called at any time during the game.
 * You need to print the map of the game in a certain format.
 * Output format is written in the problem description.
 *
 * Note: In this function, you just needs to print the map. DO NOT simulate a new round of game.
 *
 * TODO: Print the whole map.
 */
inline void PrintGame() {
  std::cout << col << " " << row << std::endl;
  int empty_lines = 0;
  bool first_line = true;

  int current_idx = 0;
  for (int r = 0; r < row; ++r) {
    int first_in_row = current_idx;
    while (current_idx < (int)live_cells_indices.size() && live_cells_indices[current_idx] / col == r) {
      current_idx++;
    }

    if (first_in_row == current_idx) {
      empty_lines++;
      continue;
    }

    if (!first_line) {
      if (empty_lines > 0) {
        if (empty_lines + 1 > 1) std::cout << (empty_lines + 1);
        std::cout << '$';
      } else {
        std::cout << '$';
      }
    } else {
      if (empty_lines > 0) {
        if (empty_lines > 1) std::cout << empty_lines;
        std::cout << '$';
      }
    }
    first_line = false;
    empty_lines = 0;

    int last_o_pos = live_cells_indices[current_idx - 1];
    int last_o_col = last_o_pos % col;

    int c = 0;
    int live_ptr = first_in_row;
    while (c <= last_o_col) {
      bool alive = (live_ptr < current_idx && live_cells_indices[live_ptr] == r * col + c);
      char type = alive ? 'o' : 'b';
      int count = 0;
      if (alive) {
        while (c <= last_o_col && live_ptr < current_idx && live_cells_indices[live_ptr] == r * col + c) {
          count++;
          c++;
          live_ptr++;
        }
      } else {
        while (c <= last_o_col && (live_ptr >= current_idx || live_cells_indices[live_ptr] != r * col + c)) {
          count++;
          c++;
        }
      }
      if (count > 1) std::cout << count;
      std::cout << type;
    }
  }
  std::cout << '!' << std::endl;
}

/**
 * This function may be called at any time during the game.
 * You need to return the number of live cells.
 *
 * @returns the number of live cells in the map.
 *
 * Note: In this function, you SHOULD NOT simulate a new round of game or print anything to stdout.
 *
 * TODO: Returns the number of live cells.
 */
inline int GetLiveCell() {
  return (int)live_cells_indices.size();
}
