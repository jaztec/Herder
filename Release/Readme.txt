0.6.0:
	Hier zit inmiddels ook route volgen in

0.6.1:
	Route uitegewerkt en volledige besturing hond ingezet, kleine bitmap toegevoegd aan de waypoints

	**Uitleg besturing herder**
	W: omhoog lopen
	A: naar links lopen
	S: omlaag lopen
	D: naar rechts lopen
	****************************
	
	***Uitleg besturing hond****
	Linkermuisknop: 
		- indrukken:
			routepunt zetten
		- indrukken en slepen:
			routepunt zetten
		- loslaten:
			route inladen, de hond gaat deze nu onmiddelijk volgen

	Rechtermuisknop:
		- indrukken:
			als er een route ingeladen is wordt deze gewist
			als de hond stilstaat komt deze terug naar de herder
			als de hond loopt wordt deze stilgezet
	****************************

	De finishstate reageert ook op linkermuisknop, na het klikken springt óf de versnel trigger aan óf het spel wordt herstart.


0.6.2:
	De finishstate reageerd alleen nog op Esc. en de linkermuisknop. Dit omdat het spel bevroor op de enter toets.
	De schapen draaien nu volledig op graden- en afstandberekeningen bij collision detection en bij het wegrennen voor de herder en de hond.

	De middelste muisknop (of het indrukken van de scroller) verschuift de camera van de hond naar de herder en andersom.

	De startpositie van de schapen veranderd per spel. Dit gebeurd via een vierkant waarin de schapen geplaatst worden.


0.6.3:
	De Player klasse bestaat bij het Engine object.

	Het scherm kan worden vergroot en verkleint met de F4 toets en de resize button op het scherm.

0.6.4:
	Er zitten geluidseffecten in. De BaseClass kan 2 geluiden per instantie opslaan en afspelen.

0.6.5:
	De main menu state is ingebouwd. een knop-oject ingevoegd welke reageerd op verschillende muisklikken. Nog een bug in deze knop bij de finish state.
	Animaties van de hond compleet.

	WAARSCHUWING!!! Soms tot regelmatig crash bij afsluiten programma. Dit maakt mem-leak door JEngine, SDL, SDL_mixer, SDL_font.

0.6.5.1:
	Finish state ingebouwd welke gebruik maakt van een algemeen button object. Bug met de kleur opgelost.
