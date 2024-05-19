# Rozszerzenia lokalnego przeszukiwania

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp
W ramach zadania należało zaimplementować trzy metody rozszerzające lokalne przeszukiwanie. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA200` oraz `kroB200` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- algorytm lokalnego przeszukiwania z różnych losowych punktów startowych,
- algorytm iteracyjnego przeszukiwania lokalnego z niewielką perturbacją,
- algorytm iteracyjnego przeszukiwania lokalnego z Large-scale neighborhood search, tj. większą perturbacją typu Destroy-Repair,
- algorytm iteracyjnego przeszukiwania lokalnego z Large-scale neighborhood bez uruchamiania lokalnego przeszukiwania po perturbacjach.

Łącznie uzyskano więc 4 różne wyniki dla każdej z instancji.

#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów
Każdy z algorytmów akceptuje na wejściu macierz odległości pomiędzy danymi wierzchołkami. Każdy z algorytmów uruchomiony został 10 razy z losowymi początkowymi rozwiązaniami. Jako algorytm przeszukiwania lokalnego użyta została wersja stroma, w której w ruchach wewnątrztrasowych, rozpatrywany był rodzaj sąsiedztwa polegający na wymianie krawędzi w cyklu, a w ruchach międzytrasowych, ruch polegał na wymianie wierzchołków pomiędzy dwoma cyklami. Algorytm został opisany szczegółowo w raporcie na drugim laboratorium.

### Multiple start local search

```
powtórz 100 razy
{
  wygeneruj losowe rozwiązanie startowe.
  popraw rozwiązanie przy użyciu algorytmu local search.
  jeśli rozwiązanie daje wynik najlepszy z dotychczasowych, to zapamiętaj je.
}
zwróć zapamiętane rozwiązanie.
```

### Iterated local search z niewielką perturbacją
W algorytmie wykorzystana została perturbacja będąca losową liczbą (2-5) losowych ruchów wykonanych na dotychczasowym rozwiązaniu. Warunkiem stopu w algorytmie jest osiągnięcie czasu równego średniemu czasowi Multiple start local search dla tej samej instancji.

```
wygeneruj losowe rozwiązanie startowe.
popraw rozwiązanie przy użyciu algorytmu local search.
powtarzaj
{
  skopiuj dotychczasowe rozwiązanie do nowego rozwiązania.
  wykonaj perturbacje na nowym rozwiązaniu.
  popraw nowe rozwiązanie przy użyciu algorytmu local search.
  jeśli nowe rozwiązanie daje wynik lepszy od aktualnego, to za aktualne rozwiązanie podstaw nowe.
  jeśli minął czas przeznaczony na algorytm, to zakończ pętlę.
}
zwróć aktualne rozwiązanie.
```

### Iterated local search z niewielką perturbacją
W algorytmie wykorzystana została perturbacja będąca usunięciem 30% wierzchołków w cyklu w pobliżu najbliższej pary wierzchołków, w której wierzchołki należą do innych cykli (destroy). Naprawienie rozwiązania było zaimplementowane poprzez algorytm korzystający z heurystyki 2-żalu z ważonym żalem opisany dokładnie w raporcie na pierwsze laboratorium (repair). Warunkiem stopu w algorytmie jest osiągnięcie czasu równego średniemu czasowi Multiple start local search dla tej samej instancji. Algorytm został również uruchomiony w wersji bez lokalnego przeszukiwania po fazie naprawy cykli (wielkoskalowe przeszukiwanie sąsiedztwa).

```
wygeneruj losowe rozwiązanie startowe.
popraw rozwiązanie przy użyciu algorytmu local search.
powtarzaj
{
  skopiuj dotychczasowe rozwiązanie do nowego rozwiązania.
  wykonaj operację destroy na nowym rozwiązaniu.
  wykonaj operację repair na nowym rozwiązaniu.
  popraw nowe rozwiązanie przy użyciu algorytmu local search. (opcjonalnie)
  jeśli nowe rozwiązanie daje wynik lepszy od aktualnego, to za aktualne rozwiązanie podstaw nowe.
  jeśli minął czas przeznaczony na algorytm, to zakończ pętlę.
}
zwróć aktualne rozwiązanie.
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
    <td class="tg-73oq">MSLS</td>
    <td class="tg-wp8o">35443</td>
    <td class="tg-wp8o">35944</td>
    <td class="tg-wp8o">36362</td>
    <td class="tg-wp8o">34831</td>
    <td class="tg-wp8o">35954</td>
    <td class="tg-wp8o">36309</td>
  </tr>
  <tr>
    <td class="tg-73oq">ILS1</td>
    <td class="tg-wp8o">33755</td>
    <td class="tg-wp8o">34621</td>
    <td class="tg-wp8o">35573</td>
    <td class="tg-wp8o">34002</td>
    <td class="tg-wp8o">34712</td>
    <td class="tg-wp8o">35863</td>
  </tr>
  <tr>
    <td class="tg-73oq">ILS2</td>
    <td class="tg-wp8o">30599</td>
    <td class="tg-wp8o">31034</td>
    <td class="tg-wp8o">31600</td>
    <td class="tg-wp8o">30774</td>
    <td class="tg-wp8o">31238</td>
    <td class="tg-wp8o">31669</td>
  </tr>
  <tr>
    <td class="tg-73oq">ILS2a</td>
    <td class="tg-wp8o">30535</td>
    <td class="tg-wp8o">31378</td>
    <td class="tg-wp8o">32666</td>
    <td class="tg-wp8o">29930</td>
    <td class="tg-wp8o">31990</td>
    <td class="tg-wp8o">34134</td>
  </tr>
</tbody>
</table>

W tabeli poniżej przedstawione zostały czasy wykonania poszczególnych algorytmu MSLS w sekundach. Pozostałe algorytmy były uruchamiane na czas średni wykonania MSLS dla danej instancji.

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
    <td class="tg-73oq">MSLS</td>
    <td class="tg-wp8o">45</td>
    <td class="tg-wp8o">48</td>
    <td class="tg-wp8o">52</td>
    <td class="tg-wp8o">46</td>
    <td class="tg-wp8o">50</td>
    <td class="tg-wp8o">53</td>
  </tr>
</tbody>
</table>

W tabeli poniżej przedstawione zostały liczby perturbacji wykonanych w algorytmach.

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
    <td class="tg-73oq">ILS1</td>
    <td class="tg-wp8o">3188</td>
    <td class="tg-wp8o">3991</td>
    <td class="tg-wp8o">4656</td>
    <td class="tg-wp8o">3599</td>
    <td class="tg-wp8o">4206</td>
    <td class="tg-wp8o">4626</td>
  </tr>
  <tr>
    <td class="tg-73oq">ILS2</td>
    <td class="tg-wp8o">760</td>
    <td class="tg-wp8o">885</td>
    <td class="tg-wp8o">950</td>
    <td class="tg-wp8o">934</td>
    <td class="tg-wp8o">973</td>
    <td class="tg-wp8o">1015</td>
  </tr>
  <tr>
    <td class="tg-73oq">ILS2a</td>
    <td class="tg-wp8o">1465</td>
    <td class="tg-wp8o">1505</td>
    <td class="tg-wp8o">1534</td>
    <td class="tg-wp8o">1003</td>
    <td class="tg-wp8o">1234</td>
    <td class="tg-wp8o">1570</td>
  </tr>
</tbody>
</table>

Poniżej umieszczone zostały wizualizacje najlepszych z uzyskanych wyników:

## Wizualizacja kroA200.tsp

MSLS          |  ILS1
:-------------------------:|:-------------------------:
![](./out/kroA200_map_multiple.png)  |  ![](./out/kroA200_map_iterated.png)

ILS2         |  ILS2a
:-------------------------:|:-------------------------:
![](./out/kroA200_map_large-scale.png)  |  ![](./out/kroA200_map_large-scale-a.png)

## Wizualizacja kroB200.tsp

MSLS          |  ILS1
:-------------------------:|:-------------------------:
![](./out/kroB200_map_multiple.png)  |  ![](./out/kroB200_map_iterated.png)

ILS2         |  ILS2a
:-------------------------:|:-------------------------:
![](./out/kroB200_map_large-scale.png)  |  ![](./out/kroB200_map_large-scale-a.png)

## Wnioski
1. Wszystkie algorytmy są efektywnymi metodami rozszerzenia lokalnego przeszukiwania. W każdym przypadku udało się poprawić rozwiązanie wygenerowane jako startowe.
2. Najlepiej radzi sobie algorytm Iterated local search z większą perturbacją typu Destroy-Repair w wersji z przeszukiwaniem stromym po każdej perturbacji. Wersja bez przeszukiwania stromego daje bardzo podobne rozwiązania, wykonując dużo więcej perturbacji. Iterated local search z niewielką perturbacją radzi sobie trochę gorzej, ale wykonuje zdecydowanie więcej perturbacji. Algorytm Multiple start local search radzi sobie najgorzej, ale jest wciąż dobrą metodą rozszerzenia lokalnego przeszukiwania, ponieważ daje lepsze wyniki w porównaniu z pojedynczym uruchomieniem algorytmu przeszukiwania stromego. Oczywiście dzieje się to kosztem czasu obliczeń.
3.

