----TEMA1 SO----
Nume: Stoica
Prenume: Theodor Nicolae
Grupa: 334CB

-----Solutie-----
Solutia aleasa presupune implementarea unui hashtable
pentru a retine perechile de tipul (SYMBOL,MAPARE).
Am ales implementarea cu liste pentru buckets-uri, vectorul
de liste fiind alocat static cu dimensiune cunoscuta(definita).
Am implementat functia de inserare intr-un hash, de stergere,
de intoarcere a unei perechi (SYMBOL,MAPARE) pe baza unei cheie
obtinute aplicand functia hash, precum si functii ajutatoare
pentru definitivarea acestora. Apoi am parsat atat linia
de comanda cat si fisierele primite, efectuand "search_and_replace"
tinand cont de ordinea directivelor de preprocesare, dar si de 
valoarea de adevar a acestora.
Functia principala din tema este functia care inlocuieste simbolurile
dintr-o propozitie cu maparea lor.
Spre exemplu daca avem maparea: x->y si propozitia: int z = x;
Aceasta prpozitie se va mapa la int z = y;
Pentru implementarea ei am impartit propozitia in tokens, iar in momentul
in care unul din tokeni se afla in hashTable il inlocuiesc cu valoarea sa mapata.
In ceea ce priveste parsarea liniei de comanda am facut distinctia intre cazurile
specificate in tema. Daca define nu are valoarea, daca primesc mai multe definuri,
daca primesc mai multe includuri, daca -D este legat de restul maparii,
la fel si in cazul lui -I si -o, daca fisierul de intrare/iesire lipseste,
daca inputul trebuie citit de la stdin, daca outputul trebuie
afisat la stdout.
In ceea ce priveste parserul sursei de C am citit linie cu line si am facut distinctie
intre linile normale si cele care contin directive de preprocesare:
#define, #if, #else, #ifdef, #endif, #elif, #ifndef, #include.
In cazul unui #define parsez cheia si valoarea. Valoarea poate necesita o inlocuire
a unuor defineuri deja existente, asa ca voi verifica asta pentru inceput.
Spre exemplu:
#define x 1
#define y x + 1
In primul caz cheia este x si valoarea este 1 si introduc maparea in hash.
In al doilea caz cheia este y si valoarea este x + 1, dar x + 1 necesita
o prelucrare in plus care sa inlocuiasca cheia x cu 1 si in final va fi introdusa
in hash cheia y cu valoarea 1 + 1.
Pentru directive de tipul #if, #elif, #ifdef, #ifndef, #else, #endif imi tin 
variabile care sa imi spuna daca branchul current al codului este corect,
daca ma aflu sau nu in if si daca pe lantul existent de if-uri si
elseif-uri a existat macar unul dintre ele care sa fie corecte.
Strategia de parsare este una destul de triviala.
In cazul in care am un #if ma uit sa vad daca ce urmaeaza dupa if este un numar.
Daca este numar voi compara numarul cu 0 ca sa verific satisfacerea if-ului.
Daca nu este un numar atunci voi cauta simbolul in hashTable si in cazul in care
este gasit voi face din nou verificarea la 0 a valorii.
In cazul in care gasesc un #elif verific daca pana atunci a fost gasit un #if sau alt 
#elif satisfacut si apoi fac aceleasi verificari ca si la if-ul normal
In cazul unui #else aleg sa ma duc pe acel branch doar in cazul in care nu a existat
un #if sau un #elif inainte pe care am facut branching
In cazul unui #ifdef/#ifndef verific daca macroul respectiv se afla/nu se afla in hash
si in functie de asta aleg daca ma duc pe ranch sau nu
In cazul unui #endif marchez ca am iesit dintr-o structura de tipul if/elif/else
In cazul unui #undef parsez cheia si o elimin din hash in cazul in care aceasta exista.
Consider ca tema a fost utila,intrucat m-a trecut prin aproape
tot ceea ce tine de programare, am invatat lucruri utile(folosirea
corecta a lui static, debugging cu gdb si valgrind pentru depanarea
memory leakurilor si a erorilor, dar si pentru a gasi diverse solutii
pentru a face un cod compatabil atat pe windows cat si pe linux:evitarea
functiilor deprecated(adica folosirea lui memcpy in loc de strcpy, simulare
strcat etc), initializarea variabilelor inainte de instructiuni etc).



Consider ca tema a fost utila,intrucat m-a trecut prin aproape
tot ceea ce tine de programare, am invatat lucruri utile(folosirea
corecta a lui static, debugging cu gdb si valgrind pentru depanarea
memory leakurilor si a erorilor, dar si pentru a gasi diverse solutii
pentru a face un cod compatabil atat pe windows cat si pe linux:evitarea
functiilor deprecated(adica folosirea lui memcpy in loc de strcpy, simulare
strcat etc), initializarea variabilelor inainte de instructiuni etc).

Consider ca as fi putut implementa mai bine parsarea, dar si ultimul test,
folosind o stiva pentru a tine evidenta if-urilor imbricate si pentru a sti
in ce "state" ma aflu.De asemenea, as fi putut sa folosesc macro-ul DIE,
intrucat ar fi fost mai elegant sa afisez si un mesaj de eroare in cazul in
care alocarea dinamica sau deschiderea unui fisier esueaza.Am folosit direct
exit(12).

Dificultati:
Am modificat in checker numarul de rulari (in loc de 1000, 1) si trecea (initial
ma gandeam ca e seg-fault din cauza implementarii ). Apoi am luat si am tratat pas cu pas
fiecare eroare(si surpriza a trecut cand am pus 1000 la loc).
Enuntul a fost destul de neclar(dovada fiind si numarul intrebarilor adresate pe forum),
insa consider aceasta parte ca fiind si o responsabilitate a noastra de a cauta mai mult.


-----Neimplementat

Nu am implementat testul "multi-lines" (not worthed).

----- Compilare

Makefile-urile genereaza mai intai fisierele obiect so-cpp.o, hashmap.o,
parser.o pentru linux iar pentru windows so-cpp.obj, hashmap.obj si parser.obj.
combinate pentru a obtine executabilul final: so-cpp, respectv so-cpp.exe.
Executabilele se numesc so-cpp respectiv so-cpp.exe

Rulare:
	Linux: ./so-cpp [-D <SYMBOL>[=<MAPPING>]] [-I <DIR>] [<INFILE>] [ [-o] <OUTFILE>]
	Windows: .\so-cpp.exe [-D <SYMBOL>[=<MAPPING>]] [-I <DIR>] [<INFILE>] [ [-o] <OUTFILE>]
Bibliografie:
	Makefile-urile sunt inspirate atat din laborator, cat si de la o tema din anii trecuti, si
adaptate.



