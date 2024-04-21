# Optymalizacja efektywności lokalnego przeszukiwania

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp

W ramach zadania należało przeprowadzić badania dotyczące wykorzystania ocen ruchów z poprzednich iteracji i ruchów
kandydackich w lokalnym przeszukiwaniu. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA200` oraz `kroB200` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- algorytm lokalnego przeszukiwania w wersji stromej,
- różne rodzaje algorytmów aptymalizacji lokalnego przeszukiwania: algorytm lokalnego przeszukiwania z listą ruchów przynoszących poprawę oraz ruchy kandydackie,
- heurystykę opartą o 2-żal z wagą żalu równą 0,4.

Łącznie uzyskano więc 8 różnych wyników dla każdej z instancji. Dodatkowo przeprowadzono również badanie losowego błądzenia, które polega na wykonywaniu losowych ruchów. Czas trwania takiego błądzenia został ustalony jako średnio najwolniejsza z wersji lokalnego przeszukiwania.

#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów

Każdy z algorytmów akceptuje na wejściu macierz odległości pomiędzy danymi wierzchołkami. Jako punkt odniesienia, użyty został algorytm 2-żal zaimplementowany w ramach pierwszego sprawozdania. Drugim punktem odniesienia był algorytm lokalnego przeszukiwania w wersji stromej zaimplementowany w ramach drugiego sprawozdania. Algorytm 2-żal został uruchomiony dla każdego wierzchołka startowego i najdalszy od niego wierzchołka jako startu drugiego cyklu. Każdy z pozostałych algorytmów uruchomiony został 100 razy z losowymi początkowymi rozwiązaniami. W algorytmach, w ruchach wewnątrztrasowych, rozpatrywany był rodzaj sąsiedztwa polegający na wymianie krawędzi w cyklu, a w ruchach międzytrasowych, ruch polegał na wymianie wierzchołków pomiędzy dwoma cyklami.

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
      jeśli wykonany ruch był międzytrasowy, to dodaj ruchy związane z wymianą nowych wierzchołków oraz wymianą nowych krawędzi.
      jeśli wykonany ruch był wewnątrztrasowy, to dodaj ruchy związane z wymianą nowych krawędzi w cyklu, w którym wykonany był ruch.
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
  dla każdego punktu
  {
    dla każdego sąsiada z listy najbliższych sąsiadów
    {
      jeżeli punkt i sąsiad są w różnych cyklach
      {
        oblicz deltę wymiany wierzchołka i sąsiada pomiędzy cyklami.
        jeżeli delta jest mniejsza od 0 oraz najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.
      }

      jeżeli punkt i sąsiad są w tym samym cyklu
      {
        oblicz deltę wymiany krawędzi rozpoczynającej się od wierzchołka oraz krawędzi rozpoczynającej się od sąsiada.
        jeżeli delta jest mniejsza od 0 oraz najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.

        oblicz deltę wymiany krawędzi kończącej się na wierzchołku oraz krawędzi kończącej się na sąsiedzie.
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
.tg .tg-baqh{text-align:center;vertical-align:top}
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-0pky{border-color:inherit;text-align:left;vertical-align:top}
.tg .tg-0lax{text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-0pky"></th>
    <th class="tg-c3ow" colspan="3">kroA200</th>
    <th class="tg-c3ow" colspan="3">kroB200</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-c3ow">min</td>
    <td class="tg-c3ow">mean</td>
    <td class="tg-c3ow">max</td>
    <td class="tg-c3ow">min</td>
    <td class="tg-baqh">mean</td>
    <td class="tg-baqh">max</td>
  </tr>
  <tr>
    <td class="tg-0pky">2-regret</td>
    <td class="tg-c3ow">32518</td>
    <td class="tg-c3ow">36024</td>
    <td class="tg-c3ow">40288</td>
    <td class="tg-c3ow">34477</td>
    <td class="tg-baqh">36917</td>
    <td class="tg-baqh">38341</td>
  </tr>
  <tr>
    <td class="tg-0pky">lokalne przeszukiwanie - strome</td>
    <td class="tg-c3ow">36566</td>
    <td class="tg-c3ow">38526</td>
    <td class="tg-c3ow">41002</td>
    <td class="tg-c3ow">35572</td>
    <td class="tg-baqh">38744</td>
    <td class="tg-baqh">40726</td>
  </tr>
  <tr>
    <td class="tg-0lax">oceny poprzednich ruchów</td>
    <td class="tg-baqh">41053</td>
    <td class="tg-baqh">42062</td>
    <td class="tg-baqh">43615</td>
    <td class="tg-baqh">42252</td>
    <td class="tg-baqh">43461</td>
    <td class="tg-baqh">45718</td>
  </tr>
  <tr>
    <td class="tg-0lax">ruchy kandydackie (k=10)</td>
    <td class="tg-baqh">37683</td>
    <td class="tg-baqh">39809</td>
    <td class="tg-baqh">41710</td>
    <td class="tg-baqh">38208</td>
    <td class="tg-baqh">39778</td>
    <td class="tg-baqh">42169</td>
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
.tg .tg-c3ow{border-color:inherit;text-align:center;vertical-align:top}
.tg .tg-0pky{border-color:inherit;text-align:left;vertical-align:top}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-0pky"></th>
    <th class="tg-c3ow" colspan="3">kroA200</th>
    <th class="tg-c3ow" colspan="3">kroB200</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-c3ow">min</td>
    <td class="tg-c3ow">mean</td>
    <td class="tg-c3ow">max</td>
    <td class="tg-c3ow">min</td>
    <td class="tg-c3ow">mean</td>
    <td class="tg-c3ow">max</td>
  </tr>
  <tr>
    <td class="tg-0pky">2-regret</td>
    <td class="tg-c3ow">131</td>
    <td class="tg-c3ow">145</td>
    <td class="tg-c3ow">208</td>
    <td class="tg-c3ow">133</td>
    <td class="tg-c3ow">158</td>
    <td class="tg-c3ow">300</td>
  </tr>
  <tr>
    <td class="tg-0pky">lokalne przeszukiwanie - strome</td>
    <td class="tg-c3ow">693</td>
    <td class="tg-c3ow">890</td>
    <td class="tg-c3ow">1176</td>
    <td class="tg-c3ow">712</td>
    <td class="tg-c3ow">840</td>
    <td class="tg-c3ow">1170</td>
  </tr>
  <tr>
    <td class="tg-0pky">oceny poprzednich ruchów</td>
    <td class="tg-c3ow">49688</td>
    <td class="tg-c3ow">61353</td>
    <td class="tg-c3ow">69440</td>
    <td class="tg-c3ow">43545</td>
    <td class="tg-c3ow">54466</td>
    <td class="tg-c3ow">59934</td>
  </tr>
  <tr>
    <td class="tg-0pky">ruchy kandydackie (k=10)</td>
    <td class="tg-c3ow">1434</td>
    <td class="tg-c3ow">1579</td>
    <td class="tg-c3ow">1779</td>
    <td class="tg-c3ow">1352</td>
    <td class="tg-c3ow">1463</td>
    <td class="tg-c3ow">1703</td>
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
![](./out/kroA200_map_pastMoves.png)  |  ![](./out/kroA200_map_candidate.png)

## Wizualizacja kroB200.tsp

2-Regret          |  Lokalne przeszukiwanie - strome
:-------------------------:|:-------------------------:
![](./out/kroB200_map_regret.png)  |  ![](./out/kroB200_map_steepest.png)

Oceny poprzednich ruchów         |  Ruchy kandydackie
:-------------------------:|:-------------------------:
![](./out/kroB200_map_pastMoves.png)  |  ![](./out/kroB200_map_candidate.png)

## Wnioski

