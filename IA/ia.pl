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
initial(piece(north, kodama, 4, 3)):-
    between(1, 6, X).

initial(piece(south, oni, 1, 6)).
initial(piece(south, kirin, 2, 6)).
initial(piece(south, koropokkuru, 3, 6)).
initial(piece(south, kirin, 4, 6)).
initial(piece(south, oni, 5, 6)).
initial(piece(south, kodama, 2, 4)).
initial(piece(south, kodama, 3, 4)).
initial(piece(south, kodama, 4, 4)):-
    between(1, 6, X).

initial_board(Board) :-
    findall(Piece, initial(Piece), Board).

% Décrire les adversaires (nord / sud)
opponent(north, south).
opponent(south, north).

% Décrire une case vide

% Décrire un mouvement donnant de nouvelle coordonnées

% Tester la validité d'un nouveau mouvement
%	- si la case est vide >> déplacement
%	- si la case est non vide >> capture ou échec du déplacement

% Avancement du jeu

% Condition de fin du jeu

% Heuristique
