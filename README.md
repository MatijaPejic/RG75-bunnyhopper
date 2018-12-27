# RG75-bunnyhopper
BunnyHopper
Program kompajlirati uz pomoc gcc ili g++ , linkovati sa -lGL -lGLU -lglut i pokrenuti ./a.out .
Uputstva za igru:
-korisnik se krece uz pomoc 'w' ,'a', 'd' (pravo,levo,desno)
-korisnik moze da skoci uz pomoc 'c' sto mu omogucava da preskace prepreke
-za svaki korak napred korisnik dobije 5 poena, 
 njegov rezultat se prati u gornjem levom uglu zajedno sa najvecim rezultatom trenutne partije
-ako korisnik ode previse levo ili desno moze da padne sa platforme nakon cega se vraca na pocetak i rezultati azuriraju
-ako se korisnik sudari sa nekom preprekom igra se zaustavlja, rezultati azuriraju i korisnik se vraca na pocetak
-igra nema kraja, platforma na kojoj se igra se neprestano nadogradjuje, cilj je ostvariti sto veci rezultat
-igra se moze pauzirati na space karakter
-program se iskljucuje na escape karakter
