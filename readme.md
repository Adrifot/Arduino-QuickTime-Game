# QuickTime Game - Arduino

## Conținut

1. [Descriere](#descriere)
2. [Proiectul în funcțiune](#proiectul-în-funcțiune)
3. [Componente utilizate](#componente-utilizate)
4. [Funcționalități](#funcționalități)
5. [Video YouTube](#video-youtube)
6. [Schema electrică](#schema-electrică)

## Descriere
Acest proiect reprezintă un joc de rapiditate în apăsarea butoanelor folosind LED-uri RGB, butoane și alte componente. Proiectul este construit folosind un microcontroller **ATMega328P** și API-ul **Arduino**.

## Proiectul în funcțiune
![Proiectul în funcțiune](media/working.png)

## Componente utilizate
![Componentele proiectului](media/components.png)

- **2x LED RGB**: Afișează culoarea ce trebuie selectată cât mai rapid de jucători.
- **6x LED simplu**: Indică LED-ul corespunzător fiecărui buton.
- **6x Butoane**: Alegerea LED-ului corespunzător culorii afișate
- **LCD**: Pentru afișarea detaliilor despre joc
- **Servomotor**: Pentru reprezentarea continuării rundei
- **20x Rezistoare**: 6x 1K ohm pentru LED-uri, 6x 220 ohm pentru LED-urile RGB, [2x 2k ohm, 2x 1k ohm, 2x 220k ohm, 2x 1k ohm] pentru butoane.
- **2x Breadboard**: Pentru conectarea circuitelor.
- **2x Arduino Uno ATMega328P**: Unul este de tip master și controlează LCD-ul, servomotorul și ține în memorie starea jocului, iar celălalt este de tip slave și controlează butoanele și LED-urile în funcție de comenzile primite de la master.
- **Linii de legătură**: Pentru conexiunile pe breadboard.

## Funcționalități

- **Regulile jocului:**
Fiecare jucător trebuie să apese butonul corespunzător culorii afișate pe LED-ul RGB al echipei.
LED-urile RGB indică culoarea activă pentru fiecare jucător.

- **Afișare scor:**
Punctajele fiecărui jucător sunt afișate în timp real pe LCD.
La final, LCD-ul indică numele câștigătorului.

- **Servomotor:**
Indică progresul jocului, rotindu-se pe măsură ce timpul trece.


## Video YouTube 
[![Video YouTube](LINK/maxresdefault.jpg)](LINK)

## Schema electrică
![Schematică TinkerCAD](media/schema_electrica.png)