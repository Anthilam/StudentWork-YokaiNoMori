% Flag pour affichage liste longue
:-set_prolog_flag(toplevel_print_options,
    [quoted(true), portrayed(true), max_depth(0)]).

%----------------------------------------------------------------
% deplacement : deplacement d'un pion selon les 4 types de coups
%----------------------------------------------------------------

% deplacement à gauche d'un pion noir
deplacement([#|[n|T]], [n|[#|T]]).

% deplacement à droite d'un pion blanc
deplacement([b|[#|T]], [#|[b|T]]).

% saut à gauche d'un pion noir par dessus un blanc
deplacement([#,b|[n|T]], [n,b|[#|T]]).

% saut à droite d'un pion blanc par dessus un noir
deplacement([b,n|[#|T]], [#,n|[b|T]]).

% parcours liste
deplacement([H|T], [H|L]):-
	deplacement(T, L).

%----------------------------------------------------------------
% profondeur(L1,Sol) : recherche en profondeur d'une solution
%----------------------------------------------------------------

profondeur([[n,n,n,#,b,b,b]|T], [[n,n,n,#,b,b,b]|T]).

profondeur([H|T], Sol):-
	deplacement(H, Y),
	\+member(Y, T),
	profondeur([Y, H|T], Sol).

%----------------------------------------------------------------
% cm(L) : prédicat de résolution du problème avec profondeur
%----------------------------------------------------------------

cm(L):-
	profondeur([[b,b,b,#,n,n,n]], L),
	reverse(L, Sol).

%----------------------------------------------------------------
% createLists(L1,Sol) : créé l'ensemble de listes correctes
% lors d'un appel à étage suivant
%----------------------------------------------------------------

createLists(_, [], R, R):-!.

createLists([H|T], [HLch|TLch], Acc, R):-
	\+member(HLch, [H|T]),
	createLists([H|T], TLch, [[HLch, H|T]|Acc], R).

createLists([H|T], [HLch|TLch], Acc, R):-
	createLists([H|T], TLch, Acc, R).

%----------------------------------------------------------------
% etageSuivant(L,Acc,R) : génération des états n+1 pour largeur
%----------------------------------------------------------------

etageSuivant([], Acc, Acc).

etageSuivant([[H|T]|LLL], Acc, R):-
	findall(Ch, deplacement(H, Ch), Lch),
	createLists([H|T], Lch, Acc, Z),
	etageSuivant(LLL, Z, R).

etageSuivant([[H|T]|LLL], Acc, R):-
	etageSuivant(LLL, Acc, R).

%----------------------------------------------------------------
% largeur(L,Sol) : recherche en largeur d'une solution
%----------------------------------------------------------------

largeur([], _):-
	!,fail.

largeur([[[n,n,n,#,b,b,b]|T]|LL], [[[n,n,n,#,b,b,b]|T]|LL]).

largeur(LLL, Los):-
	etageSuivant(LLL, [], NvLLL),
	largeur(NvLLL, Los).

%----------------------------------------------------------------
% cm2(L) : prédicat de résolution du problème avec largeur
%----------------------------------------------------------------

cm2(L):-
	largeur([[[b,b,b,#,n,n,n]]], L),
	reverse(L, Sol).

%----------------------------------------------------------------
% Solution avec heuristique
%----------------------------------------------------------------

depart([b, b, b, #, n, n, n]).

arrivee([n, n, n, #, b, b, b]).

% etat_final
etat_final([Los | _], Los):-
    arrivee(E),
        Los = [E | _].
etat_final([_ | LL], Los):-
    etat_final(LL, Los).

% compareUA
compareUA(Same, Same, 0):- !.

compareUA(_, _, 1).

% h
h([], [], 0).

h([U | UL], [A | AL], NC):-
    compareUA(U, A, C1),
    h(UL, AL, C),
    NC is C + C1.

h(U, C):-
    arrivee(E),
    h(U, E, C).

% g
g(L, T):-
    length(L, Taille),
    T is Taille - 1.

% deplaceH
deplaceH([C |Ch], NvCh, Cout):-
    deplacement(C, NvC),
    NvCh = [NvC | [C | Ch]],
    h(NvC, NouveauCout),
    g(NvCh, T),
    Cout is T + NouveauCout.

% parcoursSuivant
parcoursSuivant(P, Acc, R):-
    deplaceH(P, NvCh, CoutD),
    \+ member([CoutD, NvCh], Acc), !,
    parcoursSuivant(P, [[CoutD, NvCh] | Acc], R).

parcoursSuivant(_, Acc, Acc).

% insere
insere([], L1, L1).

insere([X | L2], L1, R):-
    insereC(L1, X, LR),
    insere(L2, LR, R).

insereC([], P, [P]).

insereC([[C1, Ch1] | L1], [C2, Ch2], [[C2, Ch2], [C1, Ch1] | L1]):-
        C1 > C2, !.

insereC([[C1, Ch1] | L1], [C2, Ch2], [[C2, Ch2], [C1, Ch1] | L1]):-
    C1 = C2,
    g(Ch1, T1),
    g(Ch2, T2),
    T1 =< T2, !.

insereC([[C1, Ch1] | L1], X, [[C1, Ch1] | LR]):-
    insereC(L1, X, LR).

% parcoursH
parcoursH([[Cout, P] | _], [Cout, P]):-
    P = [C | _],
    arrivee(C).

parcoursH([[_, P] | L], S):-
    P = [C | _],
    \+ arrivee(C), !,
    parcoursSuivant(P, [], L1),
    insere(L1, L, R),
    parcoursH(R, S).

parcoursH([_ | LParcours], Los):-
    parcoursH(LParcours, Los).

% heuristiqueDepart
heuristiqueDepart(Sol):-
    depart(D),
    h(D, Cout),
    parcoursH([[Cout, [D]]], [_ | [Los]]),
    reverse(Los, Sol).
