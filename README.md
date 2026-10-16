W zad1.cpp znajdują się dwa podpunkty do zadania 1, w tym pliku dane zapisują się do pliku visualization_data.csv, które będą potrzebne do pliku visualization.py
1.1
Tutaj miałam taki problem, żeby wychwycić i złączyć punkty należące do jednego pachołka, jeśli są za daleko od siebie,
a leżą na przykład pomiędzy trzecim punktem pachołka, który jest w bliskiej odległości od pierwszego i drugiego.
Tutaj stosujemy algorytm kaskadowy, który grupuje pachołki blisko siebie.
Konstrukcja (while (added_new_to_group)) służy do rozszerzania grupy pachołków dopóki dodawane są nowe punkty pachołków, 
co rozwiązuje mój wcześniejszy problem.

1.2
Używamy algorytmu Andrew. Na początku porządkujemy punkty rosnąco po x, a jeśli któreś są równe to po y.
Następnie sprawdzamy orientację skrętu. Jeśli punkt skręca w prawo to wyrzucamy z otoczki wcześniejszy pachołek,
ponieważ okazuje się, że tworzył „wklęśnięcie”. Ponownie sprawdzamy pachołek z jeszcze wcześniejszym.
Na końcu sumujemy odległości między kolejnymi pachołkami, aby uzyskać obwód otoczki.

W pliku visualization.py znajduje się wykres przedstawiający punkty przed scaleniem, po scaleniu oraz narysowana otoczka. W tym pliku należy zmienić: (r'd:\Karusia\studia\visualization_data.csv') na prawdziwe miejsce zapisanych danych z pliku visualization_data.csv
