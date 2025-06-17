# BlackJack

## Sommario
* [Analisi](#analisi)
  * [Introduzione al problema](#introduzione-al-problema)
  * [Funzionalità per il giocatore](#funzionalità-per-il-giocatore)
  * [Funzionalità del banco automatico](#funzionalità-del-banco-automatico)
  * [Funzionalità del sistema](funzionalità-del-sistema)
  * [Analisi delle specifiche del problema](#analisi-delle-specifiche-del-problema)
  * [Attori del sistema](#attori-del-sistema)
  * [Funzionalità per il giocatore](#funzionalità-per-il-giocatore-1)
  * [Funzionalità per il sistema](#funzionalità-per-il-sistema)
  * [Funzionalità per il server](#funzionalità-per-il-server)
  * [Requisiti non funzionali](#requisiti-non-funzionali)
  * [Flusso utente](#flusso-utente---esempio-partita-con-connessione-al-server)
  * [Requisiti funzionali](#requisiti-funzionali)
    * [Giocatore](#giocatore)
    * [Funzioni del sistema](#funzioni-del-sistema)
    * [Funzioni del server](#funzioni-del-server)

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
|Giocatore|Utente finale del sistema, interessato a giocare|
|Sistema|Gestisce l'interfaccia e la logica di gioco, si interfaccia con il server|
|Server|Gestisce il database con le credenziali del giocatore e il suo bilancio|

### Funzionalità per il giocatore

#### Registrazione e Login al server
* Registrazione con username e password dati
* Login su account esistenti
* Salvataggio automatico dei risultati sul server per utilizzo successivo
* Possibilità di giocare senza account

#### Giocare contro il banco automatico
* possibilità di hit, stand, double down e forfeit
* vedere graficamente l'andamento della partita e la propria mano
* ricevere il risultato della partita una volta terminata

#### Scommettere
* avere un bilancio digitale salvato sul server
* aumentare o diminuire il bilancio digitale attraverso scommesse sull'esito della partita
* vincere o perdere di più o di meno in base all'azione scelta o al risultato ottenuto

### Funzionalità per il sistema

#### Gestione della logica di gioco
* calcolare correttamente il punteggio della mano del giocatore e del banco
* trovare l'esito corretto della partita
* gestire la creazione e distribuzione di carte
* gestire la logica del banco automaticoù

#### Interfacciamento con il server
* gestire il login e la registrazione sul server
* gestire la modifica dei dati presenti sul server (esempio: bilancio)
* ricevere e interpretare risultati dal server
* gestire la mancanza di connessione

### Funzionalità per il server

#### Interfacciamento con il database
* cercare un utente all'interno del database
* creare un nuovo utente all'interno del database
* trovare il bilancio di un dato utente
* modificare il bilancio di un dato utente
* inviare segnali sull'esito delle operazioni al client

#### Lettura di segnali inviati dal client
* ricevere query dal client
* interpretare i query ricevuti e gli eventuali dati inclusi
* eseguire le operazioni richieste
* inviare al client una conferma

### Requisiti non funzionali

|Requisito|Descrizione|
|---------|-----------|
|Affidabilità|Gestione degli errori, mancanza di crash che richiedano riavvio del sistema|
|Interfaccia Utente|Interfaccia grafica facile da usare per l'utente|
|Performanca|Tempo di risposta dell'interfaccia < 0.25s, tempo di risposta del server < 0.5s|

### Flusso utente - Esempio (Partita con connessione al server)

1. L'utente apre il programma
2. Effettua la registrazione sul server (salta il passaggio se già registrato)
3. Effettua il login
4. Preme play ed entra in partita
5. Vede le carte iniziali e la prima carta del banco
6. Imposta una puntata
7. Pesca carte fino al raggiungimento di un valore vicino al 21
8. Preme Stand per finire di pescare
9. Vede le carte pescate dal dealer e ottiene il risultato della partita
10. Preme Retry per iniziare una nuova partita

### Requisiti funzionali
di seguito sono descritte schematicamente le funzionalità implementate

#### Giocatore

|Luogo|Funzionalità|Descrizione|
|-----|------------|-----------|
|Menù iniziale|Play|permette di entrare nel tavolo virtuale|
|Menù iniziale|Login|permette di passare all'interfaccia di login|
|Menù iniziale|Register|permette di passare all'interfaccia di registrazione|
|Menù login|Login|permette di connettersi al server con le credenziali inserite|
|Menù login|Back|permette di tornare al menù iniziale|
|Menù register|Register|permette di registrare le credenziali inserite sul server|
|Menù register|Back|permette di tornare al menù iniziale|
|Tavolo|Hit|permette di pescare una carta|
|Tavolo|Stand|permette di finire di pescare|
|Tavolo|Surrend|permette di arrendersi e perdere la metà della puntata|
|Tavolo|Double Down|permette di pescare un'ultima carta e raddoppiare la puntata|
|Tavolo|Bet|permette di accedere al menù della puntata|
|Menù puntata|Bet|permette di impostare la puntata al valore dato|
|Menù finale|Retry|permette di iniziare una nuova partita|

#### Funzioni del sistema

|File|Funzione|Descrizione|
|----|--------|-----------|
|main.c|int main(void)|funzione main del programma, chiama le funzioni per il menù e la schermata di gioco. Chiama nuovamente la funzione di gioco quando l'utente preme retry|
|blackjackclient.c|int server_login(char username[], char password[], MODE mode)|manda un segnale di login o register (in base a mode) al server e restituisce il risultato. 1 se la richiesta è fallita, 0 se ha avuto successo|
|blackjackclient.c|int money_value_update(int modify_value)|manda al server un segnale per cambiare il bilancio di un giocatore di modify_value. Restituisce 1 in caso di errori, 0 in caso di successo|
|blackjackclient.c|int money_value_get()|manda al server un segnale per ricevere il bilancio di un utente. Ritorna 1 in caso di errori, il bilancio dell'utente in caso di successo|
|cardfunctions.c|DECK fill_deck(int deck_number)|crea e restituisce un mazzo di carte con numero di carte uguale al numero di carte nel numero di mazzi da 52 carte passati|
|cardfunctions.c|void shuffle_deck(DECK* to_shuffle)|mischia il mazzo puntato dal puntatore dato|
|cardfunctions.c|CARD draw_card(DECK* deck_to_use)|pesca una carta dal mazzo puntato dal puntatore dato e la restituisce|
|cardfunctions.c|int calculate_score(CARD hand_to_evaluate[], int card_number)|calcola il punteggio della mano di blackjack data|
|gamemenu.c|ONMODE MainMenu()|crea il menù principale e chiama le funzioni associate ai bottoni del menù|
|gamemenu.c|RESULT LoginRegisterInterface(MODE mode)|crea la schermata di registrazione o la schermata di login in base al valore di mode e chiama funzioni del client per la comunicazione con il server|
|gamewindow.c|int GameStart(ONMODE online_mode)|crea la finestra di gioco e gestisce la logica del gioco. Comunica con il server se online_mode dice che si è collegati a un server. Ritorna 1 se deve iniziare una nuova partita, altrimenti 0|
|gamewindow.c|void window_draw_card(CARD card_to_draw, Rectangle place_to_draw)|gestisce la creazione delle carte sull'interfaccia grafica|
|gamewindow.c|void draw_end_screen(SIGNAL mode)|crea la schermata finale. Usa mode per scegliere il messaggio da visualizzare|
|gamewindow.c|int draw_bet_screen()|crea la schermata per impostare la puntata, ritorna il valore della puntata|

#### Funzioni del server

|File|Funzione|Descrizione|
|----|--------|-----------|
|BlackJackServer.c|int main(int argc, char argv[])|funzione main del server, esegue la logica del server, inclusa la ricezione e l'invio di segnali e la gestione degli input ricevuti|
|dbfunctions.c|int GameLogin(char username[], char password[], sqlite3* ConnectedDB)|controlla che l'utente esista all'interno del database dato. Ritorna NO_USER_FOUND (11) se l'utente non esiste, menre ritorna USER_FOUND (12) se l'utente esiste|
|dbfunctions.c|int GameRegister(char username[], char password[], sqlite3* ConnectedDB)|Crea una nuova riga nel database degli utenti con le credenziali passate. Ritorna REG_DONE (14) se l'operazione ha avuto successo|
|dbfunctions.c|int AlterMoney(char username[], char password[], int value_modifier, sqlite3* ConnectedDB)|modifica il bilancio dell'utende dato del valore indicato da value_modifier. Prima di modificare il valore controlla l'esistenza dell'utente. Ritorna UPDATE_DONE (15) se l'operazione ha avuto successo|
|dbfunctions.c|int GetMoney(char username[], char password[], char money_buffer[], sqlite3* ConnectedDB)|inserisce in money_buffer una stringa contenente il bilancio dell'utente dato. Prima di inserire il valore nella stringa controlla l'esistenza dell'utente. Ritorna OBTAIN_DONE (16) se l'operazione ha avuto successo|


