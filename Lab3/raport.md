# Optymalizacja efektywności lokalnego przeszukiwania

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp

W ramach zadania należało przeprowadzić badania dotyczące wykorzystania ocen ruchów z poprzednich iteracji i ruchów
kandydackich w lokalnym przeszukiwaniu. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA200` oraz `kroB200` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- algorytm lokalnego przeszukiwania w wersji stromej,
- różne rodzaje algorytmów aptymalizacji lokalnego przeszukiwania: algorytm lokalnego przeszukiwania z listą ruchów przynoszących poprawę oraz ruchy kandydackie,
- heurystykę opartą o 2-żal z wagą żalu równą 0,4.

Łącznie uzyskano więc 4 różne wyniki dla każdej z instancji. Dodatkowo przeprowadzono również badania algorytmu z wykorzystaniem ruchów kandydackich, którego celem było porównanie wyników oraz czasów wykonywania algorytmu dla różnej liczby rozpatrywanych sąsiadów.
#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów

Każdy z algorytmów akceptuje na wejściu macierz odległości pomiędzy danymi wierzchołkami. Jako punkt odniesienia, użyty został algorytm 2-żal zaimplementowany w ramach pierwszego sprawozdania. Drugim punktem odniesienia był algorytm lokalnego przeszukiwania w wersji stromej zaimplementowany w ramach drugiego sprawozdania. Algorytm 2-żal został uruchomiony dla każdego wierzchołka startowego i najdalszego od niego wierzchołka jako startu drugiego cyklu. Każdy z pozostałych algorytmów uruchomiony został 100 razy z losowymi początkowymi rozwiązaniami. W algorytmach, w ruchach wewnątrztrasowych, rozpatrywany był rodzaj sąsiedztwa polegający na wymianie krawędzi w cyklu, a w ruchach międzytrasowych, ruch polegał na wymianie wierzchołków pomiędzy dwoma cyklami.

### Algorytm wykorzystania ocen ruchów z poprzednich iteracji

W algorytmie korzystamy z wektora LM, który jest posortowaną od najlepszego według delty listą ruchów.
W LM oprócz ruchu przechowywana jest jego delta, aby nie było potrzeby jej liczyć w każdej iteracji.
Podczas dodawania ruchów związanych z wymianą krawędzi w cyklu, dodawane były również ruchy dla odwróconego względnego kierunku krawędzi.

```
wygeneruj losowe rozwiązanie startowe i listę możliwych ruchów.
Zainicjuj listę LM i dodaj do niej możliwe ruchy przynoszące poprawę zachowując odpowiednią kolejność.
powtarzaj
{
  dla każdego ruchu z LM
  {
    jeśli ruch jest międzytrasowy
    {
      jeśli ruch nie jest aplikowalny, czyli wierzchołki znajdują się w tym samym cyklu, usuń go z LM.
      jeśli ruch jest aplikowalny, wykonaj go i usuń z LM.
    }

    jeśli ruch jest wewnątrztrasowy
    {
      jeśli ruch nie jest aplikowalny, czyli krawędzie nie występują w cyklu, usuń go z LM.
      jeśli ruch jest aplikowalny, czyli krawędzie występują w cyklu w tym samym kierunku, wykonaj go i usuń z LM.
      jeśli krawędzie występują w cyklu, ale w różnych kierunkach, nie wykonuj go i przejdź do kolejnej iteracji.
    }

    dodaj do LM nowe możliwe ruchy przynoszące poprawę oraz ich deltę
    {
      jeśli wykonany ruch był międzytrasowy, to dodaj ruchy związane z wymianą nowych wierzchołków oraz wymianą nowo powstałych krawędzi z wszystkimi innymi krawędziami w cyklu.
      jeśli wykonany ruch był wewnątrztrasowy, to dodaj ruchy związane z wymianą nowo powstałych krawędzi z wszystkimi innymi krawędziami w cyklu, w którym wykonany był ruch.
    }
  }

  jeśli nie znaleziono żadnego aplikowalnego ruchu w LM, to zakończ pętlę.
}
zwróć rozwiązanie.
```

### Algorytm ruchów kandydackich

```
wygeneruj losowe rozwiązanie startowe.
wygeneruj listę k najbliższych sąsiadów dla każdego punktu.
powtarzaj
{
  dla każdego punktu w zbiorze wszystkich punktów
  {
    dla każdego sąsiada z listy najbliższych sąsiadów
    {
      jeżeli punkt i sąsiad są w różnych cyklach
      {
        oblicz deltę wymiany punktu i sąsiada pomiędzy cyklami.
        jeżeli delta jest mniejsza od 0 oraz najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.
      }

      jeżeli punkt i sąsiad są w tym samym cyklu
      {
        oblicz deltę wymiany krawędzi rozpoczynającej się od punktu oraz krawędzi rozpoczynającej się od sąsiada.
        jeżeli delta jest mniejsza od 0 oraz najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.

        oblicz deltę wymiany krawędzi kończącej się na punkcie oraz krawędzi kończącej się na sąsiedzie.
        jeżeli delta jest mniejsza od 0 oraz najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.
      }
    }
  }

  wykonaj zapamiętany ruch.
  jeżeli nie została znaleziona żadna ujemna delta, to zakończ pętlę.
}
zwróć rozwiązanie.
```

## Wyniki

W tabeli przedstawione zostały najlepsze, średnie i najgorsze wyniki dla zbadanych algorytmów.

<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-wp8o{border-color:#000000;text-align:center;vertical-align:top}
.tg .tg-73oq{border-color:#000000;text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-73oq"></th>
    <th class="tg-wp8o" colspan="3">kroA200</th>
    <th class="tg-wp8o" colspan="3">kroB200</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-73oq"></td>
    <td class="tg-wp8o">min</td>
    <td class="tg-wp8o">mean</td>
    <td class="tg-wp8o">max</td>
    <td class="tg-wp8o">min</td>
    <td class="tg-wp8o">mean</td>
    <td class="tg-wp8o">max</td>
  </tr>
  <tr>
    <td class="tg-73oq">2-regret</td>
    <td class="tg-wp8o">32518</td>
    <td class="tg-wp8o">36024</td>
    <td class="tg-wp8o">40288</td>
    <td class="tg-wp8o">34477</td>
    <td class="tg-wp8o">36917</td>
    <td class="tg-wp8o">38341</td>
  </tr>
  <tr>
    <td class="tg-73oq">lokalne przeszukiwanie - strome</td>
    <td class="tg-wp8o">36566</td>
    <td class="tg-wp8o">38526</td>
    <td class="tg-wp8o">41002</td>
    <td class="tg-wp8o">35572</td>
    <td class="tg-wp8o">38744</td>
    <td class="tg-wp8o">40726</td>
  </tr>
  <tr>
    <td class="tg-73oq">oceny poprzednich ruchów</td>
    <td class="tg-wp8o">37407</td>
    <td class="tg-wp8o">40924</td>
    <td class="tg-wp8o">45363</td>
    <td class="tg-wp8o">37369</td>
    <td class="tg-wp8o">40942</td>
    <td class="tg-wp8o">44831</td>
  </tr>
  <tr>
    <td class="tg-73oq">ruchy kandydackie (k=10)</td>
    <td class="tg-wp8o">37683</td>
    <td class="tg-wp8o">39809</td>
    <td class="tg-wp8o">41710</td>
    <td class="tg-wp8o">38208</td>
    <td class="tg-wp8o">39778</td>
    <td class="tg-wp8o">42169</td>
  </tr>
</tbody>
</table>

W tabeli poniżej przedstawione zostały czasy wykonania poszczególnych algorytmów w milisekundach.

<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-wp8o{border-color:#000000;text-align:center;vertical-align:top}
.tg .tg-73oq{border-color:#000000;text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-73oq"></th>
    <th class="tg-wp8o" colspan="3">kroA200</th>
    <th class="tg-wp8o" colspan="3">kroB200</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-73oq"></td>
    <td class="tg-wp8o">min</td>
    <td class="tg-wp8o">mean</td>
    <td class="tg-wp8o">max</td>
    <td class="tg-wp8o">min</td>
    <td class="tg-wp8o">mean</td>
    <td class="tg-wp8o">max</td>
  </tr>
  <tr>
    <td class="tg-73oq">2-regret</td>
    <td class="tg-wp8o">131</td>
    <td class="tg-wp8o">145</td>
    <td class="tg-wp8o">208</td>
    <td class="tg-wp8o">133</td>
    <td class="tg-wp8o">158</td>
    <td class="tg-wp8o">300</td>
  </tr>
  <tr>
    <td class="tg-73oq">lokalne przeszukiwanie - strome</td>
    <td class="tg-wp8o">693</td>
    <td class="tg-wp8o">890</td>
    <td class="tg-wp8o">1176</td>
    <td class="tg-wp8o">712</td>
    <td class="tg-wp8o">840</td>
    <td class="tg-wp8o">1170</td>
  </tr>
  <tr>
    <td class="tg-73oq">oceny poprzednich ruchów</td>
    <td class="tg-wp8o">9156</td>
    <td class="tg-wp8o">12509</td>
    <td class="tg-wp8o">16546</td>
    <td class="tg-wp8o">9091</td>
    <td class="tg-wp8o">11660</td>
    <td class="tg-wp8o">14963</td>
  </tr>
  <tr>
    <td class="tg-73oq">ruchy kandydackie (k=10)</td>
    <td class="tg-wp8o">434</td>
    <td class="tg-wp8o">517</td>
    <td class="tg-wp8o">729</td>
    <td class="tg-wp8o">449</td>
    <td class="tg-wp8o">506</td>
    <td class="tg-wp8o">745</td>
  </tr>
</tbody>
</table>

Poniżej umieszczone zostały wizualizacje najlepszych z uzyskanych wyników:

## Wizualizacja kroA200.tsp

2-Regret          |  Lokalne przeszukiwanie - strome
:-------------------------:|:-------------------------:
![](./out/kroA200_map_regret.png)  |  ![](./out/kroA200_map_steepest.png)

Oceny poprzednich ruchów         |  Ruchy kandydackie
:-------------------------:|:-------------------------:
![](./out/kroA200_map_memory2.png)  |  ![](./out/kroA200_map_candidate.png)

## Wizualizacja kroB200.tsp

2-Regret          |  Lokalne przeszukiwanie - strome
:-------------------------:|:-------------------------:
![](./out/kroB200_map_regret.png)  |  ![](./out/kroB200_map_steepest.png)

Oceny poprzednich ruchów         |  Ruchy kandydackie
:-------------------------:|:-------------------------:
![](./out/kroB200_map_memory2.png)  |  ![](./out/kroB200_map_candidate.png)

## Badanie wpływu liczby sąsiadów na wynik oraz czas działania algorytmu ruchów kandydackich

Wyniki - kroA200          |  Wyniki - kroB200
:-------------------------:|:-------------------------:
![](./out/kroA200_lengths.png)  |  ![](./out/kroB200_lengths.png)

Czasy - kroA200         |  Czasy - kroB200
:-------------------------:|:-------------------------:
![](./out/kroA200_times.png)  |  ![](./out/kroB200_times.png)

## Wnioski

1. Lokalne przeszukiwanie z zapamiętywaniem poprzednich ruchów jest efektywną metodą optymalizacji lokalnego przeszukiwania. W każdym przypadku udało się poprawić rozwiązanie wygenerowane jako startowe.
2. Algorytm ruchów kandydackich jest efektywną heurystyką poprawiającą rozwiązania startowe. W każdym przypadku udało się je poprawić.
3. Lokalne przeszukiwanie z zapamiętywaniem poprzednich ruchów radzi sobie gorzej niż przeszukiwanie strome, ale za to działa szybciej. Wadą tej metody jest duża trudność w implementacji algorytmu. Algorytm ruchów kandydackich radzi sobie nieco gorzej niż przeszukiwanie strome, ale lepiej niż lokalne przeszukiwanie z zapamiętywaniem poprzednich ruchów. Zaletą algorytmu jest jego prostota i możliwość dobrania wartości k oznaczającej liczbę rozpatrywanych sąsiadów. Najlepsze wyniki oraz czasy osiągała heurystyka konstrukcyjna oparta na 2-żalu.
4. Dobór wartości k w algorytmie wykorzystującym ruchy kandydackie znacząco wpływa na otrzymane wyniki oraz czas działania algorytmu. Dla k>6 można oczekiwać podobnych, dobrych wyników. Dalsze zwiększanie k może poprawić wynik, ale nie w tak znaczący sposób. Widocznie wydłuża się natomiast czas działania algoytmu. Zwiększa się on liniowo przy dobieraniu coraz większych wartości k.
