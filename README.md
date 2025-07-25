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
* [Casi d'uso](#casi-duso)
* [Progettazione](#progettazione)
  * [Per l'interfaccia utente](#per-linterfaccia-utente)
  * [Per il sistema](#per-il-sistema)
  * [Per il server](#per-il-server)
* [Principali variabili, strutture dati e file](#principali-variabili-strutture-di-dati-e-file)
* [Librerie e funzioni](#librerie-e-funzioni)
  * [Librerie standard utilizzate](#librerie-standard-utilizzate)
  * [Librerie esterne non personalizzate](#librerie-esterne-non-personalizzate)
  * [Librerie esterne personalizzate](#librerie-esterne-personalizzate)
* [Dipendenze tra funzioni](#dipendenze-tra-funzioni)
* [Pseudocodice](#pseudocodice)
* [Codifica](#codifica)
* [Test](#test)


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

## Casi d'uso

|Nome|Pre-condizioni|Descrizione|Post-condizioni|Descrizione|Scenario base|Scenario alternativo|
|----|--------------|-----------|---------------|-----------|-------------|--------------------|
|Play|menù principale creato, play premuto| |play| |il sistema carica il tavolo virtuale| |
|Login (main)|menù principale creato, main premuto| |login| |il sistema carica la schermata di login| |
|Login (login)|login, login premuto| |successo, errore|il login è avvenuto correttamente, il login non è avvenuto correttamente|il sistema carica il menù principale e mostra il bilancio del giocatore, il bilancio verrà caricato nel tavolo virtuale|il sistema carica il menù principale. Il bilancio non verrà caricato|
|Register (main)|menù principale creato, register premuto| |register| |il sistema carica la schermata per la registrazione| |
|Register (register)|register, register premuto| |successo, errore|ritorno al menù principale, l'utente è registrato nel database|ritorno al menù principale, l'utente non è registrato nel database|
|Back|register o login, tasto Back premuto|uno fra i tasti register e login è stato premuto nel menù principale|successo| |torna al menù principale| |
|Hit|play, hit premuto| |successo, errore|successo se sta lo spazio per pescare una carta, errore se non sta|una carta viene pescata e aggiunta alla mano|nessuna carta viene pescata|
|Stand|play, stand premuto| |successo| |il gioco termina e punteggio e risultato vengono calcolati| |
|Surrend|play, surrend premuto| |successo| |il gioco termina e il giocatore si arrende| |
|Double Down|play, double down premuto| |successo| |pesca una carta, raddoppia la puntata e termina il gioco| |
|Bet (game)|play, online, bet premuto|online: il login è avvenuto con successo|bet| |crea la finestra per le puntate| |
|Bet (bet)|bet, bet premuto| |successo| |imposta la puntata e torna alla schermata di gioco| |
|Retry|end, retry premuto|end: la partita è finita|Play|chiama la funzione Play|ricomincia il gioco| |

## Progettazione

### Per l'interfaccia utente

Il sistema è stato progettato con un'interfaccia grafica per facilitare l'interazione dell'utente con il programma.  

All'avvio del programma l'utente si trova davanti a un menù che permette di scegliere se iniziare a giocare senza effettuare un login, se effettuare un login o se registrare un nuovo utente.  
Se l'utente sceglie di effettuare il login, verrà portato a una schermata con due box per l'input e due bottoni. Dopo l'inserimento da parte dell'utente di uno username e una password, l'utente potrà premere il tasto login per collegarsi con il server.  
Il successo del processo di login si può vedere dal menù iniziale: se il login ha avuto successo sarà presente il bilancio dell'utente.  
Se l'utente decide di registrare un nuovo utente, verrà portato in una schermata simile a quella precedente. Dopo l'inserimento di uno username e una password, l'utente potrà premere il tasto register per registrare un nuovo utente.  
L'utente appena registrato potrà poi essere usato nella schermata di login.  
Se nella schermata di login o di registrazione l'utente desiderasse interrompere il processo, potrà premere il tasto back per tornare al menù iniziale.  

Se l'utente decide di premere play, verrà valutato lo stato dell'utente.  
Se l'utente ha fatto il login, il suo bilancio verrà caricato nel tavolo virtuale e le funzionalità per puntare saranno rese disponibili.  
Se l'utente non ha fatto il login, non sarà mostrato nessun bilancio e i tasti per accedere alla puntata non saranno funzionanti.  

Quando l'utente entra nel tavolo virtuale, potrà vedere due carte nella sua mano e due carte nella mano del banco, di cui una coperta.  
L'utente potrà quindi scegliere fra i seguenti comandi: Hit, Stand, Surrend, Double Down, Bet.  
Se viene scelto Hit, verrà pescata una nuova carta che sarà poi aggiunta alla mano.  
Se viene scelto Stand, il banco pescherà fino al raggiungimento o superamento di 17 e rivelerà la sua mano. A quel punto verrà calcolato il risultato della partita, che sarà poi mostrato nella schermata finale.  
Se viene scelto Surrend, la partita finirà immediatamente in una sconfitta.  
Se viene scelto Double Down, verrà pescata un'ultima carta e si procederà al termine della partita e al calcolo del risultato  
Se viene scelto Bet, si aprirà la schermata delle puntate, dove l'utente avrà la possibilità di selezionare una puntata. Questo tasto non sarà funzionante se si sta giocando offline.  

Sul lato sinistro dello schermo, se l'utente è online, sarà visibile il bilancio attuale e la puntata attuale.  
Il bilancio viene modificato una volta che la partita è terminata.  
Se l'utente ha perso, la puntata verrà sottratta. Se l'utente ha vinto, la puntata verrà aggiunta.  
Alla puntata si applicano i seguenti moltiplicatori:  
Se l'utente si è arreso, la puntata sarà dimezzata.  
Se l'utente ha selezionato Double Down, la puntata sarà raddoppiata.  
Se l'utente ha vinto ottenendo un Blackjack, la puntata sarà raddoppiata.  

Quando l'utente sballa, preme Stand o preme Surrend, la partita finirà e verrà creata la schermata finale.  
Nella schermata finale sono presenti due elementi: un testo che descrive il risultato della partita e un bottone Retry.  

Se viene premuto retry, verrà iniziata una nuova partita.  

Se l'utente desidera uscire dall'applicazione, potrà farlo tramite il tasto escape in qualsiasi schermata di gioco.  

Il cambiamento del bilancio dell'utente e la registrazione dell'utente stesso sono salvati su un server in modo da essere disponibili per future partite.  

Il programma è stato pensato per essere leggero e occupare il minor spazio possibile, motivo per cui l'unica risorsa grafica esterna è un font e motivo per il quale la grafica è realizzata attraverso codice piuttosto che attraverso immagini.  

### Per il sistema

Il codice del sistema è stato progettato per effettuare un numero ridotto di ripetizioni e allocazioni in memoria dove possibile, in modo da ridurre l'utilizzo delle risorse del computer.  
All'interno del sistema sono state utilizzate variabili globali. Per facilitare la comprensione dello scopo sono state dichiarate come static, anche se la rimozione di static non cambierebbe il risultato dell'esecuzione del codice.  
Il codice è diviso in parti riutilizzabili e in parti non riutilizzabili, in modo da mantenere l'efficienza del codice mantenendo comunque la riusabilità del codice dove possibile.  

La maggior parte delle interfacce grafiche è creata attraverso il codice senza l'utilizzo di risorse o immagini esterne, con l'eccezione di un font utilizzato per la creazione delle carte e per la schermata del titolo.  
Le altre risorse usate sono parte di raylib e raygui.  

Per facilitare il debugging in caso di errori, molti errori stampano del testo su stdin.  
Raylib e Raygui stampano a loro volta su stdin quando inizializzano il contesto di OpenGL e caricano le risorse.  

### Per il server

Il server è stato progettato per gestire una connessione alla volta, ma in caso di futura distribuzione del software potrebbe essere implementato multithreading.  
A scopo dimostrativo viene usato localhost, ma con aggiornamenti al codice lo stesso codice può essere riutilizzato per server separati.  

Il server è progettato per ricevere in input delle stringhe, che vengono poi interpretate per determinare quali operazioni eseguire.  

Il server ha accesso a un database SQLite, ma nonostante SQLite sia pensato unicamente per utilizzo locale, i query creati dalle funzioni per la gestione del database sono validi per qualsiasi sistema basato su SQL, quindi è possibile implementare un database esterno con modifiche (molto) relativamente minori al codice.  

Il server è in grado di inviare segnali in risposta, sempre sotto forma di stringhe, che possono essere usati per controllare l'esito delle operazioni eseguite dal server e eventuali dati ritornati al client.  

## Principali variabili, strutture di dati e file

|File|Nome|Tipo|Ruolo|Descrizione|Valori|
|---------|---------|---------|-------------|--------------------------------|------------|
|blackjackclient.c|iResult|int|variabile globale|contiene il risultato delle operazioni del client| |
|blackjackclient.c|ConnectSocket|SOCKET|variabile globale|contiene i dettagli del socket per la connessione al server| |
|blackjackclient.c|qbuffer|char[]|array globale|contiene il query da mandare al server| |
|blackjackclient.c|recvbuf|char[]|array globale|contiene i dati inviati dal server al client| |
|gamemenu.c|WINDOW_HEIGHT| |define|contiene l'altezza del menù, usata nello scaling degli oggetti creati per l'interfaccia grafica|625|
|gamemenu.c|WINDOW_WIDTH| |define|WINDOW_HEIGHT ma per la larghezza|1200|
|gamemenu.c|TitleFont|Font|variabile globale|contiene il font per il titolo del gioco|LoadFontEx("JqkasWild-w1YD6.tff",128,0,250)|
|gamemenu.c|DefaultFont|Font|variabile globale|contiene il font per il resto delle scritte|GetFontDefault()|
|gamewindow.c|PlayerHand|CARD[]|array globale|contiene le carte della mano del giocatore| |
|gamewindow.c|DealerHand|CARD[]|array globale|contiene le carte della mano del banco| |
|gamewindow.c|GameDeck|DECK|variabile globale|contiene il mazzo usato nel gioco| |
|gamewindow.c|endgame_code|SIGNAL|variabile globale|contiene il risultato della partita| |
|gamewindow.c|money_value_string|char[]|array globale|contiene la stringa usata come valore per il testo del bilancio| |
|gamewindow.c|money_bet_string|char[]|array globale|contiene la stringa usata come valore per il testo della puntata. IMPORTANTE: non contiene solo il valore| |
|gamewindow.c|screen_width|float|variabile globale|contiene il valore della larghezza dello schermo. Usato per decidere se ridimensionare elementi dell'interfaccia e per trovare le misure corrette| |
|gamewindow.c|screen_height|float|variabile globale|screen_width ma per l'altezza| |
|gamewindow.c|CardFont|Font|variabile globale|contiene il font per il numero/lettera sulle carte|LoadFontEx("JqkasWild-w1YD6.tff",128,0,250)|
|gamewindow.c|SuitFont|Font|variabile globale|contiene il font per il simbolo del seme delle carte|LoadFontEx("JqkasWild-w1YD6.tff",32,0,250)|
|gamewindow.c|DefaultFont|Font|variabile globale|contiene il font per il resto delle scritte|GetFontDefault()|
|blackjackclient.h|login_mode|enum| |enum per la modalità di uso della finestra di login e della funzione di login, typedef come MODE|LOGIN_MODE=20, REGISTER_MODE|
|blackjackclient.h|login_result|enum| |enum per il risultato di login/register, typedef come RESULT|LOGIN_SUCCESS = 2, LOGIN_FAIL, REGISTER_SUCCESS, REGISTER_FAIL|
|cardstructs.h|card|struct| |struct per le carte, typedef come CARD|int rank, char* suit|
|cardstructs.h|deck|struct| |struct per i mazzi, typedef come DECK|CARD* cards, int current_card_number;|
|gamemenu.h|online_mode|enum| |enum per il successo del login, typedef come ONMODE|ONLINE_PLAY = 10, OFFLINE_PLAY|
|gamewindow.h|endgame_signals|enum| |enum per il segnale di fine gioco, typedef come SIGNAL|PLAYER_WIN = 100, PLAYER_LOSE, PLAYER_BUST, PLAYER_CHARLIE, PLAYER_BLACKJACK, PLAYER_DRAW|
| |BlackJackDB.db|file database| |database per il login e il salvataggio del bilancio| |
 
## Librerie e funzioni

### Librerie standard utilizzate

* windows.h: non necessaria ma possibilmente utile per la compatibilità con windows di alcune librerie
* winsock2.h: utilizzata per gestire la connessione al server
* WS2tcpip.h: libreria ausiliaria per winsock2
* iphlpapi.h: usata per la gestione degli indirizzi ip
* stdio.h: usata per l'output di errori su stdin
* string.h: usata per gestire operazioni sulle stringhe
* stdlib.h: usata per alcune operazioni sulle stringhe (come strtol) e per l'allocazione di memoria
* time.h: usata per la generazione casuale di valori
* math.h: usata per le radici quadrate

### Librerie esterne non personalizzate

* raylib.h: usata per la gestione dell'interfaccia grafica
* raygui.h: libreria ausiliare di raylib per la creazione facilitata di elementi per l'interfaccia grafica
* sqlite3.h: usata per la gestione di database SQLite

### Librerie esterne personalizzate

* blackjackclient.h
  * typedef enum login_mode MODE
  * typedef enum login_result RESULT
  * int server_login(char username[], char password[], MODE mode)
  * int money_value_update(int modify_value)
  * int money_value_get()
* cardfunctions.h
  * DECK fill_deck(int deck_number)
  * void shuffle_deck(DECK* to_shuffle)
  * CARD draw_card(DECK* deck_to_use)
  * int calculate_score(CARD hand_to_evaluate[], int card_number)
* cardstructs.h
  * typedef struct card CARD
  * typedef struct deck DECK
* gamemenu.h
  * typedef enum online_mode ONMODE
  * ONMODE MainMenu()
* gamewindow.h
  * typedef enum endgame_signals SIGNAL
  * int GameStart(ONMODE online_mode)
* dbfunctions.h
  * define DB_CONN_FAIL 10
  * define NO_USER_FOUND 11
  * define USER_FOUND 12
  * define DB_ERROR 13
  * define REG_DONE 14
  * define UPDATE_DONE 15
  * define OBRAIN_DONE 16
  * int GameLogin(char username[], char password[], sqlite3* ConnectedDB)
  * int GameRegister(char username[], char password[], sqlite3* ConnectedDB)
  * int AlterMoney(char username[], char password[], int value_modifier, sqlite3* ConnectedDB)
  * int GetMoney(char username[], char password[], char money_buffer[], sqlite3* ConnectedDB)

## Dipendenze tra funzioni

|File|Funzione|Dipende da|
|----|--------|----------|
|blackjackclient.h|server_login|client_connection_init, close_connection|
|blackjackclient.h|money_value_update|client_connection_init, close_connection|
|blackjackclient.h|money_value_get|client_connection_init, close_connection|
|cardfunctions.h|fill_deck| - |
|cardfunctions.h|shuffle_deck|card_swap, fill_deck*|
|cardfunctions.h|draw_card|fill_deck*|
|cardfunctions.h|calculate_score| - |
|gamemenu.h|MainMenu| LoginReisterInterface, money_value_get |
|gamewindow.h|GameStart|GameWindow_init, money_value_get, draw_card, calculate_score, window_draw_card, draw_bet_screen, money_value_update, draw_end_screen|
|dbfunctions.h|GameLogin|login_callback|
|dbfunctions.h|GameRegister| - |
|dbfunctions.h|AlterMoney|GameLogin|
|dbfunctions.h|GetMoney|GameLogin, get_money_callback|

dipendenze segnate con * sono dipendenze consigliate ma non essenziali (ex. fill_deck crea un mazzo con le caratteristiche per essere usato dalle funzioni per le carte, ma sarebbe possibile crearne uno adeguato senza usare la funzione)

## Pseudocodice

### main
```
inizializza_finestra();
menu_principale();
se(errori nel menù principale)
{
 termina il programma;
}
altrimenti
{
 inizia_partita();
 mentre(comincia_nuova_partita);
}
```
### menù principale
```
imposta_sfondo(verde);
disegna_testo("BlackJack");
crea_bottoni();
se (online) disegna_testo("soldi = <soldi>");

se (bottone_play_premuto)
{
 return: giocatore_online;
}

se (bottone_login_premuto)
{
 login_giocatore();
}

se (bottone_register_premuto)
{
 register_giocatore();
}
```
### login giocatore
```
crea_input_box()
crea_bottoni()

se (bottone_login_premuto)
{
 prendi (username) da (input_box_username);
 prendi (password) da (input_box_password);
 server_login(username, password);

 se (server_login)
 {
  return online;
 }
 altrimenti
 {
  return offline;
 }
}

se (bottone_back_premuto)
{
 return online_mode;
}

```
### register giocatore
```
crea_input_box()
crea_bottoni()

se (bottone_register_premuto)
{
 prendi (username) da (input_box_username);
 prendi (password) da (input_box_password);
 server_register (username, password);

 se (server_login)
 {
  return online;
 }
 altrimenti
 {
  return offline;
 }
}

se (bottone_back_premuto)
{
  return online_mode;
}

```
### server login
```
connessione_al_server();
manda("Username = <username>");
manda("Password = <password>");
manda("Login");

ricevi();
se (messaggio_ricevuto = "login avvenuto")
{
 return login_riuscito;
}
altrimenti
{
 return login_fallito;
}
```
### server register
```
connessione_al_server();
manda("Username = <username>");
manda("Password = <password>");
manda("Register");

ricevi();
se (messaggio_ricevuto = "register avvenuto")
{
 return register_riuscito;
}
altrimenti
{
 return register_fallito;
}
```
### Inizia partita
```
inizializza_finestra();

crea_mazzo(numero_mazzi_standard);
mischia_mazzo();

pesca_carte(giocatore);
pesca_carte(banco);
calcola_punteggio(giocatore);
calcola_punteggio(banco);

disegna_carte()
crea_bottoni()

se(bottone_hit_premuto)
{
 pesca_carta(giocatore)
 calcola_punteggio(giocatore);
 disegna_carte();
}

se(bottone_stand_premuto)
{
 termina_gioco;
}

se(bottone_surrend_premuto)
{
 termina_gioco;
 azioni_banco();
}

se(bottone_doubledown_premuto)
{
 pesca_carta(giocatore);
 se (online)
 {
  puntata * 2;
 }
 termina_gioco;
 azioni_banco();
}

se(bottone_bet_premuto) e (online)
{
 apri_schermata_puntata();
}


se(termina_gioco)
{
 se (vittoria)
 {
  apri_schermata_fine(vittoria);
 }
altrimenti se (sconfitta)
{
 apri_schermata_fine(sconfitta);
}
```
### apri schermata puntata
```
inizializza_finestra();

crea_bottoni();
crea_text_input();

se (bottone_bet_premuto)
{
 prendi (puntata)
 return puntata;
}

```
### apri schermata fine
```
inizializza_finestra();

crea_bottoni();

se (vittoria)
{
 disegna_testo("vittoria");
}
altrimenti se (sconfitta)
{
 disegna_testo("sconfitta");
}

se (bottone_retry_premuto)
{
  comincia_nuova_partita
}
```
### crea mazzo
```
per ogni (mazzo_dato)
{
 per (52 carte)
 {
  crea_carta(carta_da_creare)
  metti (carta_creata) in (mazzo_da_restituire)
 }
}

return mazzo_da_restituire
```
### mischia mazzo
```
//algoritmo di Fisher-Yates
```
### pesca carta
```
prendi da (mazzo) posizione (0);
sposta_elementi (mazzo) valore (-1);
return carta_presa;
```
### calcola punteggio
```
punteggio = 0;
per ogni (carta) in (mano_data)
{
 se (carta = asso)
 {
  punteggio + 11;
 }
 altrimenti se (carta = figura)
 {
  punteggio + 10;
 }
 altrimenti
 {
  punteggio + valore_carta;
 }
}

se (punteggio > 21)
{
 fino a che (punteggio > 21) e per ogni (carta) in (mano_data)
 {
  se (carta = asso)
  {
   punteggio - 10;
  }
}

return punteggio;
 
```

## Codifica

La soluzione proposta è stata interamente implementata in linguaggio C usando un approccio procedurale e una struttura modulare.  
Le varie funzioni hanno uno scopo definito, facilitando la manutenzione e il debug.  

La lettura del codice è facilitata attraverso l'uso di enum per i segnali.  

I tipi di dati principali (carte e mazzi) sono gestiti mediante l'uso di struct.  

## Test

|Nome|Descrizione test|Input|Risultato atteso|Risultato ottenuto|
|----|----------------|-----|----------------|------------------|
|Play|offline| |inizio partita senza delay, scritte bilancio nascoste|inizio partita senza delay, scritte bilancio nascoste|
| |online| |inizio partita, scritte bilancio visibili|inizio partita, scritte bilancio visibili|
|Login(login)|Username e password validi inseriti|UCSA, testpass1|ritorno al menù principale, comparsa bilancio|ritorno al menù principale, comparsa bilancio|
| |Username e password non registrati inseriti|NOTEXISTS, NOTEXISTS|ritorno al menù principale, nessuna scritta comparsa, niente delay|ritorno al menù principale, nessuna scritta comparsa, niente delay|
| |Username e password vuoti| |ritorno al menù principale, accesso all'account di test|ritorno al menù principale, accesso all'account di test|
|Register(register)|Username e password validi inseriti|testRegister, testPassRegister|ritorno al menù principale, login con credenziali inserite riuscito|ritorno al menù principale, login con credenziali inserite riuscito|
| |Username e password vuoti| |ritorno al menù principale, chiusura del server per errore*|ritorno al menù principale, chiusura del server per errore|
|Hit|bottone premuto| |pesca carta, mostrata a schermo|pesca carta, mostrata a schermo|
| |bottone premuto mentre altra schermata attiva| |nessuna azione|nessuna azione|
|Double Down|offline| |pesca carta e fine partita|pesca carta e fine partita|
| |online| |pesca carta, fine partita, puntata raddoppiata|pesca carta, fine partita, puntata raddoppiata|
| |offline, altra schermata attiva| |nessuna azione|nessuna azione|
| |online, altra schermata attiva| |nessuna azione|nessuna azione|
|Bet(game)|bottone premuto mentre altra schermata attiva| |nessuna azione|nessuna azione|
| |offline| |nessuna azione|nessuna azione|
| |online| |apertura schermata puntata|apertura schermata puntata|
|Bet(bet)|valore inserito positivo|100|valore della puntata cambiato nel valore inserito|valore della puntata cambiato nel valore inserito|
| |valore inserito negativo|-100|valore della puntata cambiato in 0|valore della puntata cambiato in 0|
| |non numero inserito|abcd|ogni lettera viene convertita in un valore fra 10 e 35 per comporre il numero**|ogni lettera viene convertita in un valore fra 10 e 35|
|Surrend|mano vincente online| |sconfitta, perdita di metà della puntata|sconfitta, perdita di metà della puntata|
| |mano perdente online| |sconfitta, perdita di metà della puntata|sconfitta, perdita di metà della puntata|
| |mano vincente offline| |sconfitta|sconfitta|
| |mano perdente offline| |sconfitta|sconfitta|
|Stand|mano perdente online| |sconfitta, perdita uguale alla puntata|sconfitta, perdita uguale alla puntata|
| |mano vincente online| |vittoria, vincita uguale alla puntata|vittoria, vincita uguale alla puntata|
| |blackjack online| |vittora, blackjack, vincita uguale al doppio della puntata|vittoria, vincita uguale al doppio della puntata|
| |parità online| |pareggio, nessuna vincita|pareggio, nessuna vincita|
| |mano perdente offline| |sconfitta|sconfitta|
| |mano vincente offline| |vittoria|vittoria|
| |blackjack offline| |vittoria, blackjack|vittoria, blackjack|
| |parità offline| |pareggio|pareggio|





*in caso di errore nel database il server è impostato per chiudersi automaticamente. Visto che i nomi utente devono essere unici ed esiste già un utente senza credenziali (usabile come utente test o come utente indipendente da un account) il database ritorna errore, quindi il server si chiude.

**la conversione da stringa a numero è fatta usando strtol(). Il comportamento di strtol() in caso di non numeri è di convertire i non numeri in valori numerici.






