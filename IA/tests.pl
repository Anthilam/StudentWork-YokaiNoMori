:-use_module(library(plunit)).

:-include(ia).

:-begin_tests(tp4tests).

	% Test deplacement glissement gauche
	test('initial_board (true)'):-
		initial_board(Board),
		member(piece(north, oni, 1, 1), Board),
		member(piece(north, kirin, 2, 1), Board),
		member(piece(north, kirin, 4, 1), Board),
		member(piece(north, koropokkuru, 3, 1), Board),
		member(piece(north, oni, 5, 1), Board),
		member(piece(north, kodama, 2, 3), Board),
		member(piece(north, kodama, 3, 3), Board),
		member(piece(north, kodama, 4, 3), Board),
		member(piece(south, oni, 1, 6), Board),
		member(piece(south, kirin, 2, 6), Board),
		member(piece(south, koropokkuru, 3, 6), Board),
		member(piece(south, kirin, 4, 6), Board),
		member(piece(south, oni, 5, 6), Board),
		member(piece(south, kodama, 2, 4), Board),		member(piece(south, kodama, 3, 4), Board),
		member(piece(south, kodama, 4, 4), Board),
		draw_board(Board).

	test('initial_board (false)', [fail]):-
		initial_board(Board),
		member(piece(south, oni, 1, 1), Board).

	test('isEmpty (false)', [fail]):-
		initial_board(Board),
		isEmpty(1, 1, Board).

	test('isEmpty (true)'):-
		initial_board(Board),
		isEmpty(1, 2, Board).

	test('capture'):-
		initial_board(Board),
		capture(piece(north, oni, 3, 6), Board, NewBoard),
		member(piece(north, oni, 3, 6), NewBoard),
		draw_board(NewBoard).

	test('move'):-
		initial_board(Board),
		move(piece(north, oni, 1, 2), Board, NewBoard),
		member(piece(north, oni, 1, 2), NewBoard),
		draw_board(NewBoard).

	test('try_move (north kodama)'):-
		Board = [piece(north, kodama, 1, 1)],
		draw_board(Board),
		try_move(Board, north, NewBoard),
		member(piece(north, kodama, 1, 2), NewBoard),
		draw_board(NewBoard).

	test('try_move (south kodama)'):-
		Board = [piece(south, kodama, 1, 6)],
		draw_board(Board),
		try_move(Board, south, NewBoard),
		member(piece(south, kodama, 1, 5), NewBoard),
		draw_board(NewBoard).

	test('try_move (north samourai)'):-
		Board = [piece(north, samourai, 2, 1)],
		draw_board(Board),
		try_move(Board, north, NewBoard),
		member(piece(north, samourai, 2, 2), NewBoard),
		draw_board(NewBoard).

	test('try_move (south samourai)'):-
		Board = [piece(south, samourai, 3, 6)],
		draw_board(Board),
		try_move(Board, south, NewBoard),
		member(piece(south, samourai, 3, 5), NewBoard),
		draw_board(NewBoard).

	test('try_move (north oni)'):-
		Board = [piece(north, oni, 5, 1)],
		draw_board(Board),
		try_move(Board, north, NewBoard),
		member(piece(north, oni, 5, 2), NewBoard),
		draw_board(NewBoard).

	test('try_move (south oni)'):-
		Board = [piece(south, oni, 1, 6)],
		draw_board(Board),
		try_move(Board, south, NewBoard),
		member(piece(south, oni, 1, 5), NewBoard),
		draw_board(NewBoard).

	test('try_move (north superoni)'):-
		Board = [piece(north, superoni, 5, 1)],
		draw_board(Board),
		try_move(Board, north, NewBoard),
		member(piece(north, superoni, 5, 2), NewBoard),
		draw_board(NewBoard).

	test('try_move (south superoni)'):-
		Board = [piece(south, superoni, 1, 6)],
		draw_board(Board),
		try_move(Board, south, NewBoard),
		member(piece(south, superoni, 1, 5), NewBoard),
		draw_board(NewBoard).

	test('try_move (north kirin)'):-
		Board = [piece(north, kirin, 3, 2)],
		draw_board(Board),
		try_move(Board, north, NewBoard),
		member(piece(north, kirin, 3, 3), NewBoard),
		draw_board(NewBoard).

	test('try_move (south kirin)'):-
		Board = [piece(south, kirin, 3, 6)],
		draw_board(Board),
		try_move(Board, south, NewBoard),
		member(piece(south, kirin, 3, 5), NewBoard),
		draw_board(NewBoard).

	test('try_move (north koropokkuru)'):-
		Board = [piece(north, koropokkuru, 3, 2)],
		draw_board(Board),
		try_move(Board, north, NewBoard),
		member(piece(north, koropokkuru, 3, 3), NewBoard),
		draw_board(NewBoard).

	test('try_move (south koropokkuru)'):-
		Board = [piece(south, koropokkuru, 3, 6)],
		draw_board(Board),
		try_move(Board, south, NewBoard),
		member(piece(south, koropokkuru, 3, 5), NewBoard),
		draw_board(NewBoard).

:-end_tests(tp4tests).
