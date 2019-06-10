Introduzione
============

    "Software application that is able to monitor a group of file taking information about them"

L'utility di sistema realizzata prende il nome di **filestat** e
consente il monitoraggio avanzato di file e direcotry all'interno del
sitema, forenendo dei report e delle statistiche riguardo a
quest'utlimi.

Sinossi del programma e avvio
-----------------------------

La sinossi del programma è:

    filestat [options] [input] [output]

Dove:

-   `input` è il file di input dove vengono definiti i parametri di
    esecuzione del programma, se omesso viene usato il file
    `filestat.in`;

-   `output` è il file di output dove vengono collezionati i dati
    raccolti, se omesso viene usato il file `filestat.db`. Le
    informazioni presenti nel file di output vengono *aggiornate* ad
    ogni esecuzione del programma **(e non soprascritte)**.

Le possibili opzioni sono:

    --verbose|-v
    --stat|-s
    --report|-r
    --history|-h <filepah>
    --user|-u <userId>
    --group|-g <groupId>
    --length|-l <min>:<max>
    --noscan

La descrizione è la seguente:

-   `--verbose|-v`: durante l'esecuzione il programma mostra a video le
    informazioni sui file elaborati, ed i dati raccolti;

-   `--stat|-s`: vengono mostrate sullo standard output le seguenti
    statistiche:

    -   numero di file monitorati;
    -   numero di link;
    -   numero di directory;
    -   dimensione totale;
    -   dimensione media;
    -   dimensione massima;
    -   dimensione minima (in byte).

-   `--report|-r`: al termine dell'esecuzione vengono mostrati sullo
    standard output le informazioni riguardanti numero di file
    elaborati, tempo di elaborazione, dimensione massima del file;

-   `--history|-h <filepah>`: stampa sullo standard output la cronologia
    delle informazioni riguardanti il file `<filepah>`;

-   `--user|-u <userId>`: stampa sullo standard output le informazioni
    di tutti i file di proprietà di `<userId>`

-   `--group|-g <groupId>`: stampa sullo standard output le informazioni
    di tutti i file di proprietà di `<groupId>`

-   `--length|-l <min>:<max>`: stampa sullo schermo le informazioni di
    tutti i file di dimensione (in byte) compresa tra `<min>` e `<max>`
    (`:<max>` indica ogni file di dimensione al più `<max>`, `<min>:` e
    `<min>` indicano ogni file di dimensione almeno `<min>`)

-   `--noscan`: se presente questa opzione non viene effettuata la
    raccolta dei dati, ma vengono presentati solo le informazioni
    presenti del file di output.

Formato del file di input
-------------------------

I parametri di esecuzione di un programma vengono definiti in un file di
testo costituito da una sequenza di righe della seguente forma:

    <path> [r] [l]

Dove `r` indica che occorre leggere ricorsivamente i file nelle
directory sottostanti (applicando le stesse opzioni) mentre `l` indica
che i link devono essere trattati come file/directory regolari, in
questo caso le informazioni collezionate fanno riferimento al file riferito dal link e non a link stesso.

Formato del file di output
--------------------------

I dati raccolti vengono salvati usando il seguente formato:

    # <path1>
    <data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1>
    ...
    <data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>
    ###
    # <path2>
    <data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1>
    ...
    <data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>
    ###
    ...
    # <pathm>
    <data1> <uid1> <gid1> <dim1> <perm1> <acc1> <change1> <mod1> <nlink1>
    ...
    <data_n> <uid_n> <gid_n> <dim_n> <perm_n> <acc_n> <change_n> <mod_n> <nlink_n>
    ###
    ###

Le informazioni associate al file/directory `<path>` iniziano con la
riga:

    # <path>

Successivamente si trovano una sequenza di righe (una per ogni analisi
svolta) della forma:

    <data> <uid> <gid> <dim> <perm> <acc> <change> <mod> <nlink>

Dove:

  * `<data>` indica ora-data in cui sono recuperate le informazioni;
  * `<uid>` è l'id dell'utente proprietario del file;
  * `<gid>` è l'id del gruppo del file;
  * `<perm>` è la stringa con i diritti di accesso al file;
  * `<acc>` data dell'ultimo accesso;
  * `<change>` data dell'ultimo cambiamento;
  * `<mod>` data dell'ultima modifica dei permessi;
  * `<nlink>` numero di link verso il file.

Le informazioni terminano con la riga:

    ###

Il file di output termina con una riga:

    ###