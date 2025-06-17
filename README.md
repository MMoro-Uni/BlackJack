# BlackJack

## Sommario

## Analisi

### Introduzione al problema
Il Blackjack (noto anche come 21) è un gioco di carte tra un giocatore e il banco, in cui l'obiettivo è avvicinarsi il più possibile al punteggio di 21 senza superarlo.  
Ogni carta numerata vale il suo numero, le figure valgono 10 e l'asso vale 1 o 11 a seconda della situazione.  

Si vuole sviluppare un semplice gioco interattivo di Blackjack che permetta agli utenti di giocare contro un banco automatico.  
Il sistema deve consentire agli utendi di accedere a un'interfaccia di gioco e deve gestire l'andamento delle partite, i punteggi e il bilancio degli utenti.

### Funzionalità per il giocatore
Il giocatore avrà la possibilità di registrare un account con un suo saldo virtuale o accedere a un account creato in precedenza.  
Il giocatore potrà accedere a un tavolo da gioco e avrà la possibilità di giocare contro un banco automatico secondo le regole del Blackjack.  
Una volta finita una partita, il giocatore avrà la possibilità di vederne il risultato e di decidere se iniziare una nuova partita.  

### Funzionalità del banco automatico
Il banco automatico sarà in grado di giocare secondo le regole del Blackjack una volta che il giocatore avrà finito di pescare la sua mano.  
Il banco continuerà a pescare fino a che il suo punteggio non sarà maggiore o uguale a 17.

### Funzionalità del sistema
Il sistema è dotato di un interfaccia grafica per facilitare l'interazione del giocatore con il programma.  
Il programma è anche dotato di un interfaccia client-server per interfacciarsi con un database per permettere all'utente di salvare il suo bilancio e evitare la perdita di dati alla chiusura del programma.  

### Analisi delle specifiche del problema
L'applicazione è dedicata a un solo tipo di utente, il giocatore.  
Il sistema ha quindi l'obiettivo di essere semplice da utilizzare per l'utente e di permettere un'esperienza di gioco completa.  

### Attori del sistema

| Attore | Descrizione |
|--------|-------------|
