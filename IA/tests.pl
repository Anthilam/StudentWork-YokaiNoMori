:-use_module(library(plunit)).
:-use_module(library(lists)).

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

:-end_tests(tp4tests).
