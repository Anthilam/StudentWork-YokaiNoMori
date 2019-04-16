% Flag affichage liste longue
:-set_prolog_flag(toplevel_print_options,
    [quoted(true), portrayed(true), max_depth(0)]).

% Jeu d'échec en Prolog : http://boxbase.org/entries/2018/nov/19/modeling-chess-in-prolog/

% Générer le plateau initial
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

initial_board(Board) :-
    findall(Piece, initial(Piece), Board).

% Décrire les adversaires (nord / sud)
opponent(north, south).
opponent(south, north).

% Décrire une case vide
isEmpty(X, Y, Board):-
	\+(member(piece(_, _, X, Y), Board)).

% Décrire un mouvement donnant de nouvelle coordonnées

% Tester la validité d'un nouveau mouvement
%	- si la case est vide >> déplacement
%	- si la case est non vide >> capture ou échec du déplacement

% Avancement du jeu

% Condition de fin du jeu

% Heuristique

% Affichage
draw_board(Board) :-
	nl, draw_row(1, 1, Board), nl.

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

draw_piece(X, Y, Board) :-
	X == 1,
	write('|'),
	draw_piece_on_board(X, Y, Board),
	write('|').

draw_piece(X, Y, Board) :-
	X > 1,
	draw_piece_on_board(X, Y, Board),
	write('|').

draw_piece_on_board(X, Y, Board) :-(
	member(piece(_, Which, X, Y), Board) ->
	letter(Which, Letter),
	write(Letter)
	;
	write('_')
).

letter(oni, 'o').
letter(kirin, 'k').
letter(koropokkuru, 'p').
letter(kodama, 'i').
letter(samourai, 'I').
letter(superoni, 'O').
