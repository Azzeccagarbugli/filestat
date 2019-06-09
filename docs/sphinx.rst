Sphinx e le sue potenzialità
===============================

L'intera documentazione generata della quale si sta usufruendo è frutto dell'unione tra **Sphinx** e **hawkmoth**, due strumenti
dedicati alla generazione di testi a partire da del mero e puro codice.

Strumenti con cui è stata realizzata
----------------------------------------------

Solitamente quando si affronta il tema della generazione di una documentazione per un progetto di natura C
si incappa in una serie di difficoltà dettate dall'assenza di uno standard noto per la stesura dei commenti 
relativi ai metodi e alle variabili presenti nel codice stesso.

Per soccombere a tale problematiche si è deciso di usufruire di un'estensione per **Sphinx** [1]_ che fosse in grado
di convertire uno standard noto in una sintassi facilmente interpolabile da Sphinx stesso per 
ottenere una documentazione *elegante* e *facilmente usufruibile*.

Tale estensione è chiamata **hawkmoth**, software realizzato in **Pyhton** il quale si basa su un progetto **Open Source** disponibile su GitHub.


Autogenerazione della sintassi convertita da C a Sphinx
----------------------------------------------------------------

Una volta installata la dipendenza mediante ``pip``, attraverso il seguente comando::

    $ sudo pip install hawkmoth

Sarà possibile integrarla nel file ``conf.py`` e adattare la propria documentazione nel codice
sorgente seguendo un noto tipo di standard.

Quest'ultimi sono molteplici e si possono facilmente reperire all'interno delle specifiche del progetto 
online.

Vi è per esempio la possibilità di scrivere commenti in ``JavaDoc`` [2]_ per un codice che in
realtà col Java a ben poco in comune. 


.. [1] *Software Open Source per l'autogenerazione di documentazioni a partire da un codice sorgente generico*
.. [2] *Javadoc è un applicativo incluso nel Java Development Kit della Sun Microsystems, utilizzato per la generazione automatica della documentazione del codice sorgente scritto in linguaggio Java*
