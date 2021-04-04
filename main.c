#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "editor_structs.h"
#include "gap_buffer.h"
#include "init_editor.h"
#include "editor_func.h"
#include "line.h"
#include "stack.c"
#include "features.h"

void print_loc(int x, int y) {
        mvprintw(0, 90, "x: %d y: %d", x, y);
}

void print_debug(int x, int y) {
        mvprintw(1, 90, "k: %d b: %d", x, y);
}


/*use to print contents of ADT - for testing*/
void print_page(win w, TrieNode *keyword) {
	line *lne;
	int h_indx, line_no;
	char word_arr[100];
	for(int i = 0; i < w.tot_lines; i++) {
		// for keyword coloring
		int windx = 0;
		char comment = 0;
		char color = WHITE;

		h_indx = head_index(w, i);
		/*
		h_indx = i;
		// circular array
		if(h_indx + w.head_indx >= w.tot_lines)
			h_indx = h_indx - w.tot_lines;*/

		lne = &((w.head)[h_indx].line);
		char c = 1;
		int indx = 0;

		if(lne->curr_line[0] == MAX_CHAR)
                        return;

		// to clear previously written line from screen
		move(i, 0);
                clrtoeol();
		int col = 0;
	
		// 3(orange), 15(white), 9(red), 10(light_green), 11(yerllow), 21(dark blue), 39(light_blue)

		int brk = 0;
                while(1) {
                        if(lne->gap_size != 0 && indx == lne->gap_left)
				indx = lne->gap_right + 1;
			if(indx == MAX_CHAR_IN_SUBLINE) {
				if(lne->rem_line == NULL) {
					brk = 1;
					goto LABEL;
					// new line
				}
                                lne = lne->rem_line;
                                indx = 0;
				continue;
                        }
			
			c = lne->curr_line[indx++];
			if(c == '/' && comment == 0)
				comment = '/';
			else if(c == '/' && comment == '/') {
				comment = 1;
				attron(COLOR_PAIR(COMMENT));
                                mvaddch(i, col++, '/');
                                attroff(COLOR_PAIR(COMMENT));
			}			
	
		LABEL:
			if(comment == 1) {
				if(brk)
					break;
				attron(COLOR_PAIR(COMMENT));
                                mvaddch(i, col++, c);
                                attroff(COLOR_PAIR(COMMENT));
			}
			else if(c == ' ' || c == '(' || c == ';' || brk) {
				word_arr[windx++] = '\0';
				if(! search(keyword, word_arr, &color))
					color = WHITE;

				attron(COLOR_PAIR(color));
				for(int k = 0; word_arr[k] != '\0'; k++)
					mvaddch(i, col++, word_arr[k]);
				attroff(COLOR_PAIR(color));
				
				if(brk)
					break;
				mvaddch(i, col++, c);
				windx = 0;
			}
			else
				word_arr[windx++] = c;
                }
        }
	return;
}

void print_line();


void print(win w) {
        line *lne;
        int h_indx;
	int a = 0;
        for(int i = 0; i < w.tot_lines; i++) {
                h_indx = i;
                // circular array
                if(h_indx + w.head_indx >= w.tot_lines)
                        h_indx = h_indx - w.tot_lines;

                lne = &((w.head)[h_indx].line);
                char c = 1;
                int indx = 0;

                if(lne->curr_line[0] == MAX_CHAR)
                        return;

		while(1) {
                        if(lne->gap_size != 0 && indx == lne->gap_left)
                                indx = lne->gap_right + 1;
			if(indx == MAX_CHAR_IN_SUBLINE) {
                                if(lne->rem_line == NULL) {
                                        printf("\n");
                                        break;
                                }
                                lne = lne->rem_line;
                                indx = 0;
				continue;
                        }
                        c = lne->curr_line[indx++];
			printf("%c", c);
		}
        }
        return;
}



int main() {
	win window_1;
	FILE *fd_store_prev, *fd_store_next, *fd_main;
	init_window(&window_1, TOT_LINES_IN_WINDOW);
	stack st;
	init(&st);

	fd_main = load_file(&window_1,"1.c");
	fd_store_prev = fopen(".hi_pr.tmp", "w+");
        fd_store_next = fopen(".hi_nxt.tmp", "w+");
	
/*	
	for(int i = 0; shortcut_key[4][i] != 0; i++)
		printf("%c", shortcut_key[4][i]);
	return 0;
*/	
	/*
	printf("\n");
	int a = 0, b = 0;
	for(int i = 0; i < 1; i++) {
		//load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
                //print(window_1);
                //printf("abc %d\n",window_1.head_indx);
                //printf("\n");
		a = 4, b = 0;
		load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
		print(window_1);
	        printf("\n");
		del_from_pos(&window_1, &a, &b, fd_store_prev, fd_store_next, fd_main);
		//load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
		//load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
		//load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
		print(window_1);
		printf("\n");
		exit(1);
		
		a= 0, b = 100;
		del_from_pos(&window_1, &a, &b, fd_store_prev, fd_store_next, fd_main);
		print(window_1);
                printf("\n");
		a= 0, b = 100;
                del_from_pos(&window_1, &a, &b, fd_store_prev, fd_store_next, fd_main);
                print(window_1);
                printf("\n");
		load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
		print(window_1);
		//printf("abc %d\n",window_1.head_indx);
		printf("\n");
		exit(1);
		load_prev_line(&window_1, fd_store_prev, fd_store_next);
                print(window_1);
		printf("abc %d\n",window_1.head_indx);
		printf("\n");
		load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
                print(window_1);
		printf("abc %d\n",window_1.head_indx);
                printf("\n");
		load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
                print(window_1);
                printf("abc %d\n",window_1.head_indx);
                printf("\n");
		load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
                print(window_1);
                printf("abc %d\n",window_1.head_indx);
                printf("\n");
	}
	exit(1);
*/
	// curses interface 
        initscr();
        noecho();
        keypad(stdscr, true);

	init_colors();
	TrieNode *keyword = init_keywords();
	char **shortcut_key = init_shortcut_keys();

	int ch;
	int win_line = 0, win_col = 0, line_no = 0;
	int pos_changed = 0;

	print_page(window_1, keyword);
	print_loc(line_no, win_col);
	move(line_no, win_col);

	int cnt = 0;
	while(1) {
		ch = getch();
		cnt++;

		int move_left = 0;
		char start_bracket, end_bracket;
		int sk_index = shortcut_key_indx(&ch, &move_left);
		//print_debug(ch, 5);
		check_bracket(&ch, &start_bracket, &end_bracket);
		//printf(ch, 5);
		
		switch(ch) {
			case 'q':
				endwin();
				return 0;
			case 'z':
				undo(&st, &window_1, &line_no, &win_line, &win_col, fd_store_prev, fd_store_next, fd_main);
				break;

			case KEY_LEFT:
				pos_changed = 1;
	                        if(win_col)
        	                        win_col--;
                  	      	break;

			case KEY_RIGHT:
				pos_changed = 1;
	                        if(win_col < (window_1.head)[head_index(window_1, win_line)].line_size)
        	                        win_col++;
                	        break;

			case KEY_DOWN:
				pos_changed = 1;
                        	if(win_line < window_1.tot_lines - 1) {
                                	win_line++;
                                	line_no++;

                                	int h_indx = head_index(window_1, win_line);
                               		if(win_col > (window_1.head)[h_indx].line_size)
                       	        	        win_col = (window_1.head)[h_indx].line_size;
                	        }
        	                else {
	                                int check = load_next_line(&window_1, fd_store_prev, fd_store_next, fd_main);
                                	if(check == SUCCESS) {
                        	                line_no++;

                	                	int h_indx = head_index(window_1, win_line);
        	                        	if(win_col > (window_1.head)[h_indx].line_size)
	                                        	win_col = (window_1.head)[h_indx].line_size;

                                		store_info(&st, 0, ch, LOAD_NEXT_LINE, win_line, win_col);
					}
                        	}
                        	break;

			case KEY_UP:
				pos_changed = 1;
                        	if(win_line > 0) {
                	                win_line--;
        	                        line_no--;
	
        	                        int h_indx = head_index(window_1, win_line);
	                                if(win_col > (window_1.head)[h_indx].line_size)
                                	        win_col = (window_1.head)[h_indx].line_size;

                        	}
                        	else {
                                	int check = load_prev_line(&window_1, fd_store_prev, fd_store_next);
                                	if(check == SUCCESS) {
                                        	line_no--;

                                		int h_indx = head_index(window_1, win_line);
                                		if(win_col > (window_1.head)[h_indx].line_size)
                                	 	       win_col = (window_1.head)[h_indx].line_size;

						store_info(&st, 0, ch, LOAD_PREV_LINE, win_line, win_col);
					}
                        	}
                        	break;

			case KEY_BACKSPACE: {
        	                // at start of file, do nothing
	                        if(line_no == 0 && win_col == 0)
                        	        continue;

                	        char operation = DEL_CHAR;
        	                if(win_col == 0) {
	                                operation = DEL_LINE;
                                	line_no--;
                        	}
                        	char data = del_from_pos(&window_1, &win_line, &win_col, fd_store_prev, fd_store_next, fd_main);
                        	store_info(&st, pos_changed, data, operation, win_line, win_col);
                	        pos_changed = 0;
	                        break;
        	        }


			case '\n':
	                        line_no++;
                        	insert_new_line_at_pos(&window_1, &win_line, &win_col, fd_store_prev, fd_store_next, fd_main);
        	                store_info(&st, pos_changed, ch, INSERT_NEW_LINE, win_line, win_col);
                	        pos_changed = 0;
	                        break;
			
			case '\t': {
				int h_indx = head_index(window_1, win_line);
                                for(int i = 0; i < TAB_SPACE; i++) {
                                        (window_1.head)[h_indx].line_size++;
                                        insert_at_pos(&((window_1.head)[h_indx].line), win_col++, ' ');
                                        store_info(&st, pos_changed, ch, INSERT_CHAR, win_line, win_col);
                                        if(pos_changed)
                                                pos_changed = 0;
                                }
				break;
			}

			case BRACKET: { 
				int h_indx = head_index(window_1, win_line);
                                (window_1.head)[h_indx].line_size += 2;
                                insert_at_pos(&((window_1.head)[h_indx].line), win_col++, start_bracket);
				insert_at_pos(&((window_1.head)[h_indx].line), win_col++, end_bracket);
                                store_info(&st, pos_changed, ch, INSERT_CHAR, win_line, win_col);
				win_col--;
                                pos_changed = 0;
				break;
			}
			
			case CTRL(';'): {
				int h_indx = head_index(window_1, win_line);
				insert_at_pos(&((window_1.head)[h_indx].line), (window_1.head)[h_indx].line_size++, ';');
				break;
			}

			case CTRL('h'):
				pos_changed = 1;
				win_col = 0;
				break;
			case CTRL('l'):
				pos_changed = 1;
                                win_col = (window_1.head)[head_index(window_1, win_line)].line_size;
                                break;
			case CTRL('o'): {
				pos_changed = 1;
				line_no -= win_line;
                                win_line = 0;

                                int h_indx = head_index(window_1, win_line);
                                if(win_col > (window_1.head)[h_indx].line_size)
                                        win_col = (window_1.head)[h_indx].line_size;
                                break;
			}
                        case CTRL('k'): {
				pos_changed = 1;
				line_no += window_1.tot_lines-1 - win_line;
				win_line = window_1.tot_lines-1;

				int h_indx = head_index(window_1, win_line);
				if(win_col > (window_1.head)[h_indx].line_size)
					win_col = (window_1.head)[h_indx].line_size;
                                break;
			}	

			case SHORTCUT_KEY: {
				for(int i = 0; shortcut_key[sk_index][i] != '\0'; i++) {
					int h_indx = head_index(window_1, win_line);

					if(shortcut_key[sk_index][i] == '\n') {
						line_no++;
		                                insert_new_line_at_pos(&window_1, &win_line, 
								&win_col, fd_store_prev, fd_store_next, fd_main);
                		                store_info(&st, pos_changed, ch, INSERT_NEW_LINE, win_line, win_col);
					}
					else {
						(window_1.head)[h_indx].line_size++;
	                                	insert_at_pos(&((window_1.head)[h_indx].line), win_col++, shortcut_key[sk_index][i]);
        	                        	store_info(&st, pos_changed, ch, INSERT_CHAR, win_line, win_col);
					}
					if(pos_changed)
						pos_changed = 0;
				}
				for(int i = 0; i < move_left; i++) {
                                        win_col--;
					if(win_col < 0) {
						line_no--;
						win_line--;
						win_col = (window_1.head[head_index(window_1, win_line)]).line_size;
					}
				}
				break;
			}

			default: {
                        	int h_indx = head_index(window_1, win_line);
                        	(window_1.head)[h_indx].line_size++;
                        	insert_at_pos(&((window_1.head)[h_indx].line), win_col++, ch);
                        	store_info(&st, pos_changed, ch, INSERT_CHAR, win_line, win_col);
                        	pos_changed = 0;
			}
		}

		print_page(window_1, keyword);
		print_loc(line_no, win_col);
		move(win_line, win_col);
	}

	return 0;
}
