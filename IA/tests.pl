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
		member(piece(south, kodama, 2, 4), Board),
		member(piece(south, kodama, 3, 4), Board),
		member(piece(south, kodama, 4, 4), Board),
		draw_board(Board, _, _).

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
		capture(piece(north, oni, 3, 6), Board, NewBoard, _, NewCapt),
		member(piece(south, koropokkuru, 0, 0), NewCapt),
		member(piece(north, oni, 3, 6), NewBoard),
		\+member(piece(south, koropokkuru, 3, 6), NewBoard).

	test('capture superoni'):-
		Board = [piece(north, superoni, 1, 1)],
		capture(piece(south, kodama, 1, 1), Board, NewBoard, _, NewCapt),
		member(piece(north, oni, 0, 0), NewCapt),
		member(piece(south, kodama, 1, 1), NewBoard),
		\+member(piece(north, superoni, 1, 1), NewBoard).

	test('capture samourai'):-
		Board = [piece(north, samourai, 1, 1)],
		capture(piece(south, oni, 1, 1), Board, NewBoard, _, NewCapt),
		member(piece(north, kodama, 0, 0), NewCapt),
		member(piece(south, oni, 1, 1), NewBoard),
		\+member(piece(north, samourai, 1, 1), NewBoard).

	test('capture same side (fail)', [fail]):-
		Board = [piece(north, samourai, 1, 1)],
		capture(piece(north, oni, 1, 1), Board, _, _, _).

	test('move'):-
		initial_board(Board),
		move(piece(north, oni, 1, 2), Board, NewBoard),
		member(piece(north, oni, 1, 2), NewBoard).

	test('try_move (north kodama)'):-
		Board = [piece(north, kodama, 1, 1)],
		try_move(Board, _, _, north, NewBoard, _, _, kodama, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 1,
		N_X = 1, N_Y = 2,
		Capture = 0,
		member(piece(north, kodama, 1, 2), NewBoard).

	test('try_move (south kodama)'):-
		Board = [piece(south, kodama, 1, 6)],
		try_move(Board, _, _, south, NewBoard, _, _, kodama, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 6,
		N_X = 1, N_Y = 5,
		Capture = 0,
		member(piece(south, kodama, 1, 5), NewBoard).

	test('try_move promote (north kodama)'):-
		Board = [piece(north, kodama, 1, 4)],
		try_move(Board, _, _, north, NewBoard, _, _, kodama, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 4,
		N_X = 1, N_Y = 5,
		Capture = 0,
		member(piece(north, samourai, 1, 5), NewBoard).

	test('try_move promote (south kodama)'):-
		Board = [piece(south, kodama, 1, 3)],
		try_move(Board, _, _, south, NewBoard, _, _, kodama, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 3,
		N_X = 1, N_Y = 2,
		Capture = 0,
		member(piece(south, samourai, 1, 2), NewBoard).

	test('try_move (north samourai)'):-
		Board = [piece(north, samourai, 2, 1)],
		try_move(Board, _, _, north, NewBoard, _, _, samourai, X, Y, N_X, N_Y, Capture),
		X = 2, Y = 1,
		N_X = 2, N_Y = 2,
		Capture = 0,
		member(piece(north, samourai, 2, 2), NewBoard).

	test('try_move (south samourai)'):-
		Board = [piece(south, samourai, 3, 6)],
		try_move(Board, _, _, south, NewBoard, _, _, samourai, X, Y, N_X, N_Y, Capture),
		X = 3, Y = 6,
		N_X = 3, N_Y = 5,
		Capture = 0,
		member(piece(south, samourai, 3, 5), NewBoard).

	test('try_move (north oni)'):-
		Board = [piece(north, oni, 5, 1)],
		try_move(Board, _, _, north, NewBoard, _, _, oni, X, Y, N_X, N_Y, Capture),
		X = 5, Y = 1,
		N_X = 5, N_Y = 2,
		Capture = 0,
		member(piece(north, oni, 5, 2), NewBoard).

	test('try_move (south oni)'):-
		Board = [piece(south, oni, 1, 6)],
		try_move(Board, _, _, south, NewBoard, _, _, oni, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 6,
		N_X = 1, N_Y = 5,
		Capture = 0,
		member(piece(south, oni, 1, 5), NewBoard).

	test('try_move promote (north oni)'):-
		Board = [piece(north, oni, 1, 4)],
		try_move(Board, _, _, north, NewBoard, _, _, oni, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 4,
		N_X = 1, N_Y = 5,
		Capture = 0,
		member(piece(north, superoni, 1, 5), NewBoard).

	test('try_move promote (south oni)'):-
		Board = [piece(south, oni, 1, 3)],
		try_move(Board, _, _, south, NewBoard, _, _, oni, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 3,
		N_X = 1, N_Y = 2,
		Capture = 0,
		member(piece(south, superoni, 1, 2), NewBoard).

	test('try_move (north superoni)'):-
		Board = [piece(north, superoni, 5, 1)],
		try_move(Board, _, _, north, NewBoard, _, _, superoni, X, Y, N_X, N_Y, Capture),
		X = 5, Y = 1,
		N_X = 5, N_Y = 2,
		Capture = 0,
		member(piece(north, superoni, 5, 2), NewBoard).

	test('try_move (south superoni)'):-
		Board = [piece(south, superoni, 1, 6)],
		try_move(Board, _, _, south, NewBoard, _, _, superoni, X, Y, N_X, N_Y, Capture),
		X = 1, Y = 6,
		N_X = 1, N_Y = 5,
		Capture = 0,
		member(piece(south, superoni, 1, 5), NewBoard).

	test('try_move (north kirin)'):-
		Board = [piece(north, kirin, 3, 2)],
		try_move(Board, _, _, north, NewBoard, _, _, kirin, X, Y, N_X, N_Y, Capture),
		X = 3, Y = 2,
		N_X = 3, N_Y = 3,
		Capture = 0,
		member(piece(north, kirin, 3, 3), NewBoard).

	test('try_move (south kirin)'):-
		Board = [piece(south, kirin, 3, 6)],
		try_move(Board, _, _, south, NewBoard, _, _, kirin, X, Y, N_X, N_Y, Capture),
		X = 3, Y = 6,
		N_X = 3, N_Y = 5,
		Capture = 0,
		member(piece(south, kirin, 3, 5), NewBoard).

	test('try_move (north koropokkuru)'):-
		Board = [piece(north, koropokkuru, 3, 2)],
		try_move(Board, _, _, north, NewBoard, _, _, koropokkuru, X, Y, N_X, N_Y, Capture),
		X = 3, Y = 2,
		N_X = 3, N_Y = 3,
		Capture = 0,
		member(piece(north, koropokkuru, 3, 3), NewBoard).

	test('try_move (south koropokkuru)'):-
		Board = [piece(south, koropokkuru, 3, 6)],
		try_move(Board, _, _, south, NewBoard, _, _, koropokkuru, X, Y, N_X, N_Y, Capture),
		X = 3, Y = 6,
		N_X = 3, N_Y = 5,
		Capture = 0,
		member(piece(south, koropokkuru, 3, 5), NewBoard).

	test('put first opponent line (north, fail)', [fail]):-
		Capt = [piece(south, kodama, 0, 0)],
		put(piece(south, kodama, 1, 1), _, Capt, _, _).

	test('put first opponent line (south, fail)', [fail]):-
		Capt = [piece(north, kodama, 0, 0)],
		put(piece(north, kodama, 1, 6), _, Capt, _, _).

	test('put south'):-
		Capt = [piece(south, kodama, 0, 0)],
		put(piece(south, kodama, 3, 3), [], Capt, NewBoard, NewCapt),
		member(piece(south, kodama, 3, 3), NewBoard),
		\+member(piece(south, kodama, 0, 0), NewCapt).

	test('put north'):-
		Capt = [piece(north, kodama, 0, 0)],
		put(piece(north, kodama, 3, 3), [], Capt, NewBoard, NewCapt),
		member(piece(north, kodama, 3, 3), NewBoard),
		\+member(piece(north, kodama, 0, 0), NewCapt).

	test('put column not empty', [fail]):-
		Capt = [piece(south, kodama, 0, 0)],
		Board = [piece(south, kodama, 3, 2)],
		put(piece(south, kodama, 3, 3), Board, Capt, _, _).

	test('force_move south'):-
		Board = [piece(south, kodama, 1, 1)],
		force_move(south, kodama, 1, 1, 5, 6, Board, _, _, NewBoard, _, _),
		member(piece(south, kodama, 5, 6), NewBoard),
		\+member(piece(south, kodama, 1, 1), NewBoard).

	test('force_move north'):-
		Board = [piece(north, kodama, 1, 1)],
		force_move(north, kodama, 1, 1, 5, 6, Board, _, _, NewBoard, _, _),
		member(piece(north, kodama, 5, 6), NewBoard),
		\+member(piece(north, kodama, 1, 1), NewBoard).

	test('force_move south with capture'):-
		Board = [piece(south, kodama, 1, 1), piece(north, kirin, 5, 6)],
		force_move(south, kodama, 1, 1, 5, 6, Board, _, _, NewBoard, _, C),
		member(piece(south, kodama, 5, 6), NewBoard),
		\+member(piece(south, kodama, 1, 1), NewBoard),
		member(piece(south, kirin, 0, 0), C).

	test('force_move north with capture'):-
		Board = [piece(north, kodama, 1, 1), piece(south, kirin, 5, 6)],
		force_move(north, kodama, 1, 1, 5, 6, Board, _, _, NewBoard, B, _),
		member(piece(north, kodama, 5, 6), NewBoard),
		\+member(piece(north, kodama, 1, 1), NewBoard),
		member(piece(north, kirin, 0, 0), B).

:-end_tests(tp4tests).
