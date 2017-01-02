# README #

### Opis ###

Program służy do oznaczania bazy treningowej wykorzystywanej w programie do detekcji i rozpoznawania sygnalizacji świetlnej. 

### Konfiguracja ###

* Język programowania: C++
* Kompilator: Qt Creator 3.6.0
* Wykorzystywane biblioteki: OpenCV v3.1

### Informacje wstępne###

Aby możliwe było wyświetlanie w programie obrazka "Przykład zaznaczenia" należy dodać plik "sygnalizacja.png" do projektu. Można to zrobić w następujący sposób. W drzewie projektu w programie Qt Creator kliknąć prawym przyciskiem na projekt i wybrać opcję "Dodaj nowy..", następnie przejść do zakładki "Qt" i wybrać "Qt Resource file", nadać mu nazwę i zatwierdzić. W nowo otwartej zakładce kliknąć "Dodaj", następnie "Dodaj pliki" i dodać plik "sygnalizacja.png.


### Instrukcja użytkowania ###

Po uruchomieniu programu należy kliknąć przycisk "Dodaj nowy", a następnie wybrać materiał wideo wykorzystywany do zebrania danych treningowych. Po wykonaniu tej operacji w zakładce "Plik video" doda się pozycja z nazwą ścieżki wybranego pliku. Należy ją zaznaczyć, a następnie kliknąć przycisk "Otwórz".
Na ekranie pojawi się nowe okno w którym odtwarzany będzie wybrany film. W celu dokonania zaznaczenia na klatce, należy zatrzymać odtwarzanie klikając przycisk "Pauza", wybrać kolor światła jaki będzie zaznaczany, a następnie zaznaczyć lewym przyciskiem myszy obszar sygnalizatora. Klikając w przycisk "Naprzód", zapisane zostanie zaznaczenie z poprzedniej klatki, a na aktualnej klatce zostanie wygenerowane przewidywane zaznaczenie.
W celu automatycznego odtwarzania i zapisywania wygenerowanego, przewidywanego zaznaczenia w czasie rzeczywistym należy kliknąć przycisk "Odtwórz i dopasuj". Aby zaprzestać odtwarzania wraz z zapisywaniem automatycznie wygenerowanego zaznaczenia należy kliknąć ponownie w przycisk "Odtwórz i dopasuj".
Po zakończeniu zaznaczania danych treningowych należy kliknąć przycisk "Zapisz", aby zapisać zebrane dane do pliku YAML powiązanego z danym materiałem wideo. Przy kolejnym otworzeniu tego samego materiału wideo, wcześniej zebrane dane będą widoczne na sekwencji wideo w postaci prostokątów w kolorze danego światła, a nowo zebrane zaznaczenia będą dopisane do istniejącego pliku.