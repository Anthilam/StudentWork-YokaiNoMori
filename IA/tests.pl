:-use_module(library(plunit)).
:-use_module(library(lists)).

:-include(ia).

:-begin_tests(tp4tests).

	% Test deplacement glissement gauche
	test('deplacement glissement gauche', [true(L==[n,n,#])]):-
		deplacement([n,#,n], L).

	% Test deplacement glissement droite
	test('deplacement glissement droite', [true(L==[#,b,b])]):-
		deplacement([b,#,b], L).

	% Test deplacement saut gauche
	test('deplacement saut gauche', [true(L==[n,b,#])]):-
		deplacement([#,b,n], L).

	% Test deplacement saut droite
	test('deplacement saut droite', [true(L==[#,n,b])]):-
		deplacement([b,n,#], L).

:-end_tests(tp4tests).
