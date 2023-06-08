# Bakenlicht

## Analyse 

Deze opdracht is opgedragen door de haven van Antwerpen Brugge. Deze staan in voor het transport over het water. Dit moet zo veilig mogelijk kunnen gebeuren met duidelijke communicatie. In de haven wordt er veel gecommuniceerd aan de hand van lichten. Zo kunnen boeien die overdag worden gebruikt om overdag vaarwegmarkeringen aan te duiden ook gebruikt worden om s ’nachts aan de hand van een licht de vaarrichting aanduiden. Dit geld ook voor gevaarlijke situaties zoals een scherpe hoek van een kaai of een paal in het water. Op dit laatste zijn we verder op in gegaan.  
De gevaarlijke situaties worden aangeduid aan de hand van 3 lampen boven elkaar. Waarvan de onderste 2 rood zijn en de bovenste groen. Dit is de code voor een gevaarlijke situatie. Deze lampen werken aan de hand van een lichtmeting en werken zo autonoom. Deze worden gevoed door een mobiele voeding aan de hand van een accu en zonnepanelen. In de haven zijn er verschillende soorten lampen te vinden die worden gebruikt om de gevaarlijke situaties te markeren. 

## Probleemstelling 
De lampen die nu worden gebruikt is maar beperkte communicatie met mogelijk. Dit gebeurd door gebruik te maken van een simkaart waar dan je bepaalde data mee kan uitlezen via een sms. Ook zijn deze niet aanstuur baar en kunnen deze dus niet ingeschakeld worden moest er mist optreden. De werking van lampen worden niet gerapporteerd en is een kapotte of niet correct werkende lamp moeilijk op te sporen zonder fysieke controle. Het vervangen van onderdelen is hierdoor dus ook moeilijk in te schatten en vaak ook beperkt aan reparatie.

## Noden 

- Verbinding met een netwerk waarover communicatie kan   opgezet worden 

- De lampen van op afstand aansturen 

- De lampen monitoren en eventueel rapporteren 

- Voeding die afzonderlijk kan werken 

- De nodige data opslaan in een database 

- Een interface waar al dit kan gemonitord en kan bediend worden voor een demo-opstelling 

## Blokdiagram 
![image](https://github.com/S122451/bakenlichten/assets/94835931/3e9caac8-8316-4b3e-8188-482b585b945b)

## Globale hardware analyse
![image](https://github.com/S122451/bakenlichten/assets/94835931/fc74ef78-f1a7-435d-9236-11c2b7f2ecc5)
De baken zal bestaan uit een kast met daarin een pcb en een voeding. Op deze kast zijn aansluitingen waar de lichten op kunnen aangesloten. Dit geld ook voor de sensoren die worden gebruikt. Het zonnepaneel bevind zich bovenop de baken en wordt via een kabel aangesloten op het voeding gedeelte van de kast. Deze kast mag niet te groot zijn omdat deze mee aan de paal wordt bevestigt. Hierdoor zal dus ook de accu een beperkte ruimte hebben. In de kast worden ventilatoren aangesloten om deze te verluchten. Hierover later meer. 

![image](https://github.com/S122451/bakenlichten/assets/94835931/585c6b56-ecd0-4094-ad62-f01a1da15597)

De voeding zal bestaan uit een zonnepaneel met een laadregelaar en een batterij. De laadregelaar zal dan zo de hoofd pcb voorzien van de nodige spanning. De spanning die wordt geleverd is 12V omdat de lampen deze werkspanning hanteren. De voeding is gemaakt op een 12V systeem wat betekent dat de accu een 12V accu zal zijn met een zonnepaneel van 12V.

![image](https://github.com/S122451/bakenlichten/assets/94835931/28ffb9f1-e2a8-472a-a535-d784b51d5963)

De hoofd pcb zal al de componenten regelen en monitoren. Deze bevat het hart van het systeem namelijk de mkr1300. Deze wordt voorzien door voeding van een spanning converter. Op de pcb bevinden zich ook de nodige aansluiting voor de sensoren en de accu meting. Bovenop het bord bevinden zich 3 modules die aansturing van de lamp mogelijk maken.

![image](https://github.com/S122451/bakenlichten/assets/94835931/1f792455-0e82-41e2-8cc5-a03286680e6d)

De sensoren gaan er voor zorgen dat de werking wordt uitgemeten en gerapporteerd. Er zal een meting zijn voor het daglicht zodat de lampen automatisch kunnen schakelen. De baken zal een gps module bevatten om de tijd en de positie van de baken te bepalen. Als de lampen branden zal het licht van de lampen worden opgemeten.

![image](https://github.com/S122451/bakenlichten/assets/94835931/0330a6dc-b0e2-42f8-8807-6f84b3ce25f3)

De lamp aansturing zal de lampen sturen door een signaal van de hoofd pcb en zal zo een mosfet relais schalen. Bij deze schakeling wordt ook de stroom gemeten dat de lamp verbruikt om ook de werking te controleren. Zo krijg je een redundante meting met de lichtsensoren

## Microcontroller
We maken gebruik van de mkr1300. Dit bord is een arduino based board.

Deze microcontroller hebben we gekozen omdat deze te programmeren is met de Arduin IDE die ons reeds bekend is en deze mee op het LoRa WAN netwerk kan communiceren. Dit netwerk bevind zich in de haven en wordt later meer in de detail uitgelegd. Deze controller is een 3.3V microcontroller met 14 digitale pinnen en 6 analoge pinnen. Deze kan gevoed worden met een batterij van 3.3V maar ook met een spanning van 5 tot 5.5V via de Vin pin. 

##Modules
De gy-gps6mv2 wordt gebruikt om de locatie van de baken te bepalen als ook de momentele tijd te bepalen. Dit laatste hebben we nodig om te weten wanneer de baken op het LoRa WAN netwerk een bericht verstuurd. De locatie zal gebruikt kunnen worden om voor eventuele reparatie de nodige positie van de baken te weten.

Om de analoge signalen van de stroom sensor in te lezen zullen we een ads1115 nodig hebben. Dit omdat de stroomsensor een analoog signaal verstuurt tussen de 1.5 en 3.5V op een werkspanning van 5V. De microcontroller kan maar signalen opvangen tot 3.3V. Deze module heeft een nauwkeurige 16 bit adc converter. Dit signaal wordt dan uitgelezen door de microcontroller via I2C. 

## Voeding
De Victron energy pwm laadregelaar gaat de opgewekte spanning van het zonnepaneel omzetten naar een 12V dc spanning. Deze laadregelaar kan een nominale laadstroom leveren van 10A. Deze moet ingesteld te worden voor de gewenste waardes van de accu. Dit gaat via de 4 drukknoppen en een lcd display op de laadregelaar. Deze laadregelaar heeft ook een load uitgang waaruit een 12V dc spanning komt. Dit is ideaal omdat de accu varieert van spanning afhankelijk van de capaciteit deze bezit. De controller gaat de spanning verhogen of verlagen naar de gewenste 12V.

De AP63205WU-7 is een buck converter. Deze gaat de spanning van 12V omzetten naar een spanning van 5V. Deze zal een maximale stroom kunnen leveren van 2A. De 5V spanning hebben we nodig om onze microcontroller te voeden als ook onze ADC module voor de stroommetingen. De converter heeft een zeer gierige efficiëntie rond de 90%.

De BCR421UW6 is de led-driver voor de lampen. De lamp heeft de led-driver nodig om de stroom door de lamp te beperken. Dit omdat er in de lamp leds zitten en geen eigen led-driver bezitten. De driver werkt op een ingangsspanning van 12V en zal een maximale stroom van 500mA leveren. Dit zal praktisch heel wat lager liggen omdat het disipatie vermogen te groot is en de component dus te warm word. De uitgangsstroom wordt bepaald aan de hand van een externe weerstand. 

## Sensoren

De TEMT6000X01 zijn de lichtsensoren die we gaan gebruiken. Dit voor zowel het meten van dag en nacht als ook de werking van de lamp. Deze sturen een spanning uit die omgezet kan worden via een analoge ingang. De sensor heeft een vrij grote maximale bandwith en is hierdoor ideaal geschikt om een vrij nauwkeurige waarde te verkrijgen.

De ACS712-05B is de stroomsensor die we gebruiken om de lichten te controleren. De sensor heeft een maximaal meetbereik van 5A. Dit kan vergroot worden door een andere versie van deze module te plaatsen. De sensor wordt gevoed door een 5V voeding en stuurt het analoog signaal van max 3.5V naar de ADC converter.

De AQY211EHAZ is een mosfet relay die de lamp zal in en uit schakelen. Dit kan door een digitaal signaal te sturen van hoog of laag met een max. stroom van 50mA met een max. van 5V. De stroom door de ralay mag maximaal 1A zijn met een spanning van maximaal 30V. 
 
 ## Schema
![image](https://github.com/S122451/bakenlichten/assets/94835931/8b9e3809-d1aa-4bd9-ac4f-221b3560512b)

Dit is ons pcb schema. Hierop zie je verschillende delen van onze pcb. Je ziet linksboven het voeding gedeelte dat de inkomende 12V omzet naar de gewenste 5V. De microcontroller op deze pcb is de MKR1300 die hier aangeduid is als MKR1. Onder deze controller zie je de ads1115. Deze gaat er voor zorgen dat de ingelezen waardes van de 5V sensoren worden omgezet naar digitale waardes en deze verstuurd met I2C naar onze microcontroller. Op dit schema bevinden zich nog 4 aansluitpinnen voor de lichtsensoren, 4 aansluitpinnen voor de gps en dan nog 4 mosfets met stroomsensor pcb’s. Dit is een zelf ontworpen pcb die je op de pcb klikt. 

![image](https://github.com/S122451/bakenlichten/assets/94835931/0887916e-fe4d-4d86-9fae-eb1c3cfd7fa3)


Deze zelf ontworpen pcb bevat een relais in de vorm van een mosfet om de led driver te schakelen met 12V. De led driver is nodig om de stroom door de lamp te beperken. In deze iteratie is de led driver nog niet goed omdat deze met hogere stromen te warm wordt. Deze wordt nu gebruikt om de stroom door de lamp rond de 165 mA te houden. Je ziet ook 3 solder jumpers om de pcb om te solderen zodat de relais alleen met de stroomsensor werkt zonder de led driver. Dit is bedoeld om externe lampen met ingebouwde led driver te sturen. Op de pcb bevind zich zoals eerder gezegd een stroomsensor om de werking van de lamp te bepalen aan de hand van de stroom door de lamp. Er bevinden zicht 2 aansluiting om data versturen en te ontvangen. 1 analoge om de stoomwaardes in te lezen en 1 digitale om de relais aan te sturen. 

## Microcontroller

De gekozen microcontroller is de mkr1300. We hebben deze gekozen omdat deze een lora chip heeft ingebouwd om te communiceren met het LoRa-wan netwerk. De microcontroller aan boord is de SAMD21 die op een werkspanning werkt tot 3,3V. Hierdoor mogen de signalen die ingelezen worden niet hoger zijn dan 3,3V. Deze controller is te programmeren met de arduino ide waarmee we al reeds bekend zijn.

![image](https://github.com/S122451/bakenlichten/assets/94835931/424ea175-d9b5-4e2a-a780-bcf712ce574b)

## LoraWan

LoraWan is een telecommunicatienetwerk om te comuniceren over een lange afstand met zo min mogelijk vermogen. Dit netwerk is gabaseerd op getways en nodes. De nodes in onze toepassing zijn de mkr1300’s die uitgerust zijn met de CMWX1ZZABZ chip. In ons project maken we gebruik van TTN (The Things Network) om te comuniceren over LoRaWan. Dit is een netwerkserver waarop we de nodige gegevens kunnen vinden van het netwerk. Hierop is het mogelijk om applicaties te maken. In deze applicaties kan je zelf de nodige sensoren toevoegen en monitoren. 

## Instellen TTN

Om gebruik te maken van TTN moeten we onze sensor kopellen aan de applicatie. Dit kan door je microcontroller een programma te laten uitvoeren die zijn EUI teruggeeft. Hierna kan je met de gekregen informatie de sensor toevoegen. Na dit heeft de microcontroller zijn APP EUI nodig en de APP KEY om authenticatie uit te voeren. Dit is eenmalig proces per microcontroller. 

## Classes in TTN

In het netwerk kan je gebruik maken van verschillende classes. Elke classe heeft zijn eigen specificaties:

-	Class A: In deze klasse kan de node op elk moment een uplink sturen naar het netwerk. Na deze uplink is er een korte tijd om een downlink te ontvangen. 

![image](https://github.com/S122451/bakenlichten/assets/94835931/316f0b52-459f-4cc1-bf59-59371ebefff7)

-		Class B: te vergelijken met classe a. Hier zal er om een bepaalde tijd een downlink venster geopend worden, ook wel ping slot genoemd,  waar downlink berichten kunnen ontvangen worden. Ze open, net als klasse a, op elk moment de uplink waarna een een korte tijd is voor een downlink.

![image](https://github.com/S122451/bakenlichten/assets/94835931/5a1fa8dc-5696-4143-843c-bcb14d623aa1)

-	Class C: De klasse zal de hele tijd een ontvangstvenster openhouden. De window sluit alleen als er een uplink verzonden wordt. Let op: class C verbruikt meer dan de ander genoemde classes.

Wij hebben gekozen voor de class c. Door het kiezen van deze class kunnen we onze mcirocontroller altijd aansturen. Dit is nodig om de lampen op het gewenste moment aan te zetten. We voorzien een uplink op vast ingestelde tijdintervallen waarop onze sensoren data versturen in vorm van een string afgescheiden door een ‘:’.  De string data wordt voor het versturen omgezet naar bytes om de payload zo klein mogelijk te houden. Het verbruik is in deze iteratie minder belangrijk en is tegenover de lampen zo goed als verwaarloosbaar.

## Aansturing

We hebben gekozen om de aansturing te benaderen via 2 mogelijke opties. Dit doen we via nod-red en via flask. Dit doen we omdat nod-red vooral is bedoeld voor prototyping en flask al meer gebaseerd is op publicatie. Al dit zal bij finale oplevering niet gebruikt worden omdat de POAB dit gaan implementeren in hun eigen applicatie. Voor demonstratie is het wel nodig om de vorig beschreven applicaties te gebruiken. Om de nodige data naar de applicaties te sturen maken we gebruik van mqtt.

## Mqtt

Mqtt wordt veel gebruikt in IOT toepassingen. Dit is een protocol waarmee we berichten kunnen sturen met zo weinig mogelijk data. Het maakt gebruik van ‘publish-subscribe’ concept om informatie uit te wisselen tussen broker en device. In onze applicatie is de broker het mqtt netwerk waarop wij met de applicatie op subscribe en ook data op kunnen publishen.  Alle devices op het mqtt netwerk zijn clients, zowel de applicatie waarmee we alles aansturen als de microcontroller. Elk van de berichten op dit netwerk valt onder een topic. Deze zijn op de TTN gestandaardiseerd en zoals hieronder weergegeven te gebruiken.
 
![image](https://github.com/S122451/bakenlichten/assets/94835931/c89e1c4f-252d-48ba-ba18-e1bdd568a225)

In elke payload zit een payload_data waar het gestuurde bericht in zit. In de payload kan je ook allerlei andere informatie vinden zoals device-id, topic,… 

## Flask

Flask is een python gebaseerd web framework die gebruik maakt van andere verschillende componenten zoals: Jinja, werkzeug, markupsafe en itsdangerous. Flask is ons al aardig bekend van het vak Web Services Python en Single Board Computers. Hier zie je dat flask een snelle leercurve heeft en snel een resultaat genereert zonder al te veel code te moeten schrijven. De flask toepassing is geprogrammeerd in python en maakt gebruik van een noSQL database waarin alle ontvangen informatie over elk device in het netwerk wordt opgeslagen. Ook worden hier de gebruikers van de aplicatie bijgehouden. De wachtwoorden van de gebruikers zijn geëncrypteerd door het flask_login deel van flask. Dit wil dus zeggen dat je moet ingelogd zijn om de data van de devices te bekijken of eventueel de devices aan te sturen.  In de flask toepassing gebeurd ook de gemiddelde berekenig. Deze bereking is nodig om te bepalen of het licht of donker is buiten. Dit gebeurd door alle devices hun lichtwaarde te nemen en deze te delen door het aantal. Licht deze waarde boven of onder de vooringestelde grens, zal er een bricht worden verzonden naar de devices via mqtt. Hierbij wordt wel rekening gehouden of de devices op automtisch staan of handmatig worden aangestuurd.

## Layout

De layout is niet afgewerkt maar bevat het essentiële. Dit komt omdat de nadruk meer lag op het bouwen van een werkend hardware systeem dan het bouwen van de interface. De layout ziet er als volgt uit voor een ingelogde gebruiker. 

![image](https://github.com/S122451/bakenlichten/assets/94835931/6403c62e-07b4-40f5-937c-48fe59a843b3)

Je ziet hier een dropdown list waarin je een baken kan selecteren om de informatie hiervan op te vragen. Verder zie je een bedieningspaneel om alle bakens tegelijk aan te sturen. Je hebt de keuze uit:

-	Aan en uit: om alle bakens aan of uit te zetten. Hierdoor wordt de automatische functie uitgeschakeld en moeten ze handmatig bediend worden. 
-	Automatisch: dit is om alle bakens terug op automatisch te zetten. Deze functie is ideaal moesten alles bakens terug automatisch mogen werken.
-	
Een verdere uitwerking zal een lijst kunnen zijn om standaard niet geautomatiseerd bakens te filteren en deze niet elke keer terug op handmatig te zetten.
De volgende layout laat zien wanneer je een bepaalde baken hebt aangeduid aan de hand van zijn id.
![image](https://github.com/S122451/bakenlichten/assets/94835931/d7878ac6-0a1a-4efd-a828-99b33d579630)

Hier kan je informatie terugvinden van de aangeduide baken en ook de baken individueel bedienen. De informatie bevat het volgende;
Aan/uit: dit bevat de informatie over de device of deze aan of uit is gestuurd. Let hierbij op dat deze niet real time wordt geüpdatet en dus na bijvoorbeeld inschakeling niet direct mee aangepast wordt. 
Lamp1, lamp2, lamp3: dit laat de status van elke lamp zien op de baken. Deze wordt berekend door de stroom en de lichtintensiteit te meten van de lamp. 0 betekent dat er geen fout is opgetreden en 1 als er een error aanwezig is.
Lichtdetectie: dit is de data dat we nodig hebben om ons gemiddelde te berekenen voor het bepalen van dag of nacht. Deze data is afkomstig van een lichtsensor die de intensiteit meet. 
Autoset: deze data is lokaal en komt niet van de microcontroler. Dit laat zien of de geselecteerde device mee reageert op de automatische pulsen of individueel werkt. 0 betekent individueel 1 betekent automatisch. 
Map: dit is de locatie van de baken. Deze is momenteel ingesteld op een vaste locatie (hard coded ingesteld) ingesteld. Dit zal moeten laten zien waar de baken zich bevind om nodige fysieke acties te ondernemen bij een fout. De map functie werkt nu niet omdat er geen api-key kon aangemaakt worden.

## Node-red

Als alternatief voor de flask toepassing hebben we ook een lange tijd node-red gebruikt. Node red is een visuele programmeer tool die krachtige automatisaties kan uitvoeren. Wij hebben dit dan gebruikt in combinatie met MQTT om alle data te kunnen versturen en te verbinden met een database zodat alle data op één plek samen komen en we hier ook makkelijk verschillende acties op kunnen gaan uitvoeren.
Het grote voordeel aan node-red is dat er veel plugins beschikbaar op zijn en deze kunnen dan ook worden gebruikt bij het verder uitbreiden van onze toepassing indien nodig.

## Voeding

De voeding die we hebben voorzien is mobiel en heeft dus geen netspanning nodig. De spanning aan de componenten wordt geleverd door een batterij. Dit is een AGM loodaccu van 50Ah en een spanning van 12V. AGM is de techniek waarvan de accu is gemaakt. De accu is een loodaccu met glasvezelmatten waardoor de accu geen accuzuur heeft dat zich los in de accu bevind. Deze heeft ook beter oplaad eigenschappen dan een gewone accu en is beter bestemd tegen externe factoren. Zo is het niet erg om deze accu te kantelen. De onderhoud is miniem aan deze accu en gaat langer mee dan de standaard loodaccu’s. De accu is aangesloten op de pwm laadregelaar van Victron Energy. De laadregelaar zorgt er voor dat dat het zonnepaneel de accu laad wanneer nodig. Ook zal deze de accu beschermen tegen een te diepe zelf op- of ontlading. De 12V die we nodig hebben voor onze schakeling is ook afkomstig van de laadregelaar in plaats van de accu om zo moest het nodig zijn de accu af te kopellen van de schakeling. In de schakeling bevind zich ook een DC/DC converter die er voor zorgt dat de spanning van 12V wordt omgezet naar 5V voor de microcntroller en de ADC converter.

## PCB

De onderliggende pcb’s zijn onze 2 zelfgemaakte pcb’s om de schakeling te sturen en uit te lezen. Op de linker afbeelding ziet u het hoofd bord waar de mkr-1300 zich op bevind met de nodige aansluitingen alsook de ADC converter en de 12V naar 5V converter. Op de rechter afbeelding is er een pcb te zien die de lamp stuurt die op het hoofd pcb kan geklikt worden. Dit geld ook voor de GPS-module. De rechter pcb heeft als functie om de led te schakelen aan de hand van een mosfet relais en een led-driver. De sturing wordt uitgemeten door de stroomsensor om te bepalen of de lamp aan of uit geschakeld is. Deze pcb kan ook omgezet worden om de led-driver er tussen uit te schakelen. Dit is handig voor lampen die al een led-driver aan boord hebben en dus gewoon een voedingspanning nodig hebben.

![image](https://github.com/S122451/bakenlichten/assets/94835931/12edb33f-abd4-4ded-861c-4ebf9db108b3)

![image](https://github.com/S122451/bakenlichten/assets/94835931/84ccae5d-f50e-487e-a332-754c890a4557)

# Code

## Flask code

De code is gemaakt in de arduino IDE. We hebben gekozen om 2 mogelijke benaderingen toe te passen. 1 gebeurd via node-red de andere via flask. Hierin zit het verschil zich vooral in de automatische functie die bepaald of het dag of nacht is. De node-red zal deze lokaal bekijken op de microcontroller zelf aan de hand van een vergelijking terwijl de flask toepassing de waarde doorstuurt naar de flask toepassing en deze verder verwerkt. Zo wordt de waarde ingelezen en bewaard in de database. Vervolgens word er van alle ingelezen waardes het gemiddelde berekend en bepaald of de lampen moeten schakelen of niet. Dit heeft als voordeel moest er een detectie niet werken de andere lampen dit op vangen. Je kan de lampen ook handmatig bedienen. Dit gebeurd door een bericht te sturen naar de mkr-1300 en zal dit bericht verwerken en reageren op de juiste status. Let hier wel bij op dat de automatische status dan word uitgeschakeld en deze dan opnieuw zal moeten worden ingeschakeld. De code zal om een bepaalde tijdsduur zijn eigen waardes op het netwerk sturen en zal door beide toepassingen verwerken en weergeven. 

## Nodered code

Het node red gedeelte bestaat uit twee delen. We hebben het eerste deel waar we de globale commando’s hebben. Deze werken aan de hand van een database. Er zal door alle apparaten in de database worden gegaan en telkens zal het id uit de lijst worden gehaald. Deze wordt vervolgens gebruikt om het commando naar to te sturen, dit proces herhaalt zich tot alle apparaten het commando toegezonden hebben gekregen. 
Dan hebben we nog het tweede gedeelte waar we gaan kijken of we een bericht binnen krijgen van een apparaat. Wanneer we een bericht ontvangen zal dit naar de database worden gestuurd en als het apparaat id nog niet bestaat zal deze worden toegevoegd met alle meegestuurde data. Indien het apparaat id al aanwezig is zal het de data gewoon updaten. 
Wanneer we een apparaat gaan selecteren zal er een verzoek naar de database worden gestuurd en zullen we alle specifieke data van dat apparaat krijgen en kunnen we natuurlijk ook commando’s voor dat apparaat gaan laten uitvoeren. 

## Verbeteringen

1.	De DC/DC converter levert geen goede spanning en is gevoelig aan trillingen. Hierdoor hebben we in de opstelling gebruik gemaakt van 7805 linear voltage regulator. Dit zal ook opgelost kunnen worden door de microcontroller te voeden door de 5V en de 12V ook apart te nemen zonder omzetting naar 5V. 
2.	De led-driver wordt warm bij hogere stroom waardes. Hier hebben we geprobeerd om 2 led-drivers parallel te plaatsen maar dit had helaas weinig invloed. De led-driver werkt wel goed op lagere spanningen die ingesteld kan worden door een externe weerstand.
3.	De opstelling is voorzien van een elektrische kast. In deze kast zit er een accu die op en ontladen wordt waardoor er gevaarlijke stoffen in de kast ontstaan. De gevaarlijke stoffen moeten naar de buiten moeten geleid worden.
4.	De stroommeting is niet nauwkeurig genoeg en zal dus beter vervangen worden. 
5.	Kleine aanpassingen aan de aansluitingen op de pcb. 
6.	Aansluitingen aan de kast verbeteren zodat kapotte lampen of controllers makkelijker kunnen vervangen worden.
7.	Indien nodig toepassingen optimaliseren moesten deze niet mee in het portaal van POAB worden geïntegreerd 
