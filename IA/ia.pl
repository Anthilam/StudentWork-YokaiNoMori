%----------------------------------------------------------------
%	ia.pl
%
%	BOISSON Romain, GUY Timothee
%
%	Projet Yokai No-Mori
%
%----------------------------------------------------------------

% Flag for long list
:-set_prolog_flag(toplevel_print_options,
    [quoted(true), portrayed(true), max_depth(0)]).

% Jeu d'échec en Prolog : http://boxbase.org/entries/2018/nov/19/modeling-chess-in-prolog/

% Pieces :
% kodama, samourai
% oni, superoni
% kirin, koropokkuru

%----------------------------------------------------------------
% Initial board generation
%----------------------------------------------------------------

% inital( pieces(side, type, x, y) ) : put a piece on the board
initial(piece(north, oni, 1, 1)).
initial(piece(north, kirin, 2, 1)).
initial(piece(north, koropokkuru, 3, 1)).
initial(piece(north, kirin, 4, 1)).
initial(piece(north, oni, 5, 1)).
initial(piece(north, kodama, 2, 3)).
initial(piece(north, kodama, 3, 3)).
initial(piece(north, kodama, 4, 3)).

initial(piece(south, oni, 1, 6)).
initial(piece(south, kirin, 2, 6)).
initial(piece(south, koropokkuru, 3, 6)).
initial(piece(south, kirin, 4, 6)).
initial(piece(south, oni, 5, 6)).
initial(piece(south, kodama, 2, 4)).
initial(piece(south, kodama, 3, 4)).
initial(piece(south, kodama, 4, 4)).

% initial_board : initialize the board
initial_board(Board) :-
    findall(Piece, initial(Piece), Board).

%----------------------------------------------------------------
% opponent : describes a player and its opponent
%----------------------------------------------------------------
opponent(north, south).
opponent(south, north).

%----------------------------------------------------------------
% isEmpty : check if a square is empty
%----------------------------------------------------------------
isEmpty(X, Y, Board):-
	\+(member(piece(_, _, X, Y), Board)).


% /!\ Warning /!\ : capture and move, used alone, duplicate pieces

%----------------------------------------------------------------
% capture : capture an opponent piece
% TODO : store somewhere the captured pieces
%----------------------------------------------------------------
capture(piece(Side, Type, X, Y), Board, NewBoard):-
	% Get the opponent
	opponent(Side, OtherSide),
	% Extract the board without the captured piece
	select(piece(OtherSide, _, X, Y), Board, TempBoard),
	% Create the new board
	NewBoard = [piece(Side, Type, X, Y) | TempBoard].

%----------------------------------------------------------------
% move : move a piece without capturing
%----------------------------------------------------------------
move(Piece, Board, NewBoard):-
	NewBoard = [Piece, Board].

%----------------------------------------------------------------
% try_move : try to move a piece
%----------------------------------------------------------------

% try_move north kodama
try_move(Board, north, NewBoard):-
	select(piece(north, kodama, X, Y), Board, TempBoard),
	(
		N_X is X, N_Y is Y + 1
	),
	(
		isEmpty(N_X, N_Y, Board),
		move(piece(north, kodama, N_X, N_Y), TempBoard, NewBoard)
		;
		capture(piece(north, kodama, N_X, N_Y), TempBoard, NewBoard)
	).

% try_move south kodama
try_move(Board, south, NewBoard):-
	select(piece(south, kodama, X, Y), Board, TempBoard),
	(
		N_X is X, N_Y is Y - 1
	),
	(
		isEmpty(N_X, N_Y, Board),
		move(piece(south, kodama, N_X, N_Y), TempBoard, NewBoard)
		;
		capture(piece(south, kodama, N_X, N_Y), TempBoard, NewBoard)
	).

% try_move north samourai
try_move(Board, north, NewBoard):-
	select(piece(north, samourai, X, Y), Board, TempBoard),
	(
		N_X is X, N_Y is Y + 1;
		N_X is X, N_Y is Y - 1;
		N_X is X + 1, N_Y is Y;
		N_X is X - 1, N_Y is Y;
		N_X is X + 1, N_Y is Y + 1;
		N_X is X - 1; N_Y is Y + 1
	),
	(
		isEmpty(N_X, N_Y, Board),
		move(piece(north, samourai, N_X, N_Y), TempBoard, NewBoard)
		;
		capture(piece(north, samourai, N_X, N_Y), TempBoard, NewBoard)
	).

% try_move south samourai
try_move(Board, south, NewBoard):-
	select(piece(south, samourai, X, Y), Board, TempBoard),
	(
		N_X is X, N_Y is Y + 1;
		N_X is X, N_Y is Y - 1;
		N_X is X + 1, N_Y is Y;
		N_X is X - 1, N_Y is Y;
		N_X is X + 1, N_Y is Y - 1;
		N_X is X - 1; N_Y is Y - 1
	),
	(
		isEmpty(N_X, N_Y, Board),
		move(piece(south, samourai, N_X, N_Y), TempBoard, NewBoard)
		;
		capture(piece(south, samourai, N_X, N_Y), TempBoard, NewBoard)
	).

% TODO: oni, superoni, kirin, koropokkuru

% Tester la validité d'un nouveau mouvement
%	- si la case est vide >> déplacement
%	- si la case est non vide >> capture ou échec du déplacement

% Avancement du jeu

% Condition de fin du jeu

% Heuristique

%----------------------------------------------------------------
% Print functions
%----------------------------------------------------------------

% draw_board : draw the board
draw_board(Board) :-
	nl, draw_row(1, 1, Board), nl.

% draw_row : draw a row of the board
draw_row(X, Y, Board) :-
	X =< 5,
	draw_piece(X, Y, Board),
	X1 is X+1,
	draw_row(X1, Y, Board).

draw_row(6, Y, Board) :-
	Y =< 5,
	nl,
	Y1 is Y+1,
	draw_row(1, Y1, Board).

draw_row(5, 5, _) :- nl.

% draw_piece : draw a piece of the board
draw_piece(X, Y, Board) :-
	X == 1,
	write('|'),
	draw_piece_on_board(X, Y, Board),
	write('|').

draw_piece(X, Y, Board) :-
	X > 1,
	draw_piece_on_board(X, Y, Board),
	write('|').

draw_piece_on_board(X, Y, Board) :-
	(member(piece(_, Which, X, Y), Board) ->
		letter(Which, Letter),
		write(Letter)
		;
		write('_')
	).

% letter : assign a letter to each piece type
letter(oni, 'o').
letter(kirin, 'k').
letter(koropokkuru, 'p').
letter(kodama, 'i').
letter(samourai, 'I').
letter(superoni, 'O').
