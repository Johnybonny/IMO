# Hybrydowy algorytm ewolucyjny

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp
W ramach zadania należało zaimplementować hybrydowy algorytm ewolucyjny (HAE) i porównać go z metodami MSLS i ILSx zaimplementowanymi w poprzednim zadaniu. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA200` oraz `kroB200` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- hybrydowy algorytm ewolucyjny z lokalnym przeszukiwaniem w wersji zachłannej po rekombinacji,
- hybrydowy algorytm ewolucyjny bez lokalnego przeszukiwania po rekombinacji.

#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów


### Hybrydowy algorytm ewolucyjny
Parametry algorytmu:
- Populacja elitarna o wielkości 20,
- Algorytm steady state,
- W populacji nie mogą się znajdować kopie tego samego rozwiązania (porównywane były wartości funkcji celu),
- Czas trwania równy średniemu czasowi trwania algorytmu MSLS dla danej instancji.

Operator rekombinacji polegał na wyborze jednego z rodziców jako rozwiązania wyjściowego, a potem usunięciu z niego wszystkich krawędzi, które nie występowały w drugim rodzicu. Wierzchołki, które stały się wolne również zostały usunięte. Rozwiązanie naprawiane było za pomocą metody ważonego 2-żalu, analogicznie do metod ILSx. Testowana była również wersja algorytmu bez lokalnego przeszukiwania po rekombinacji.

```
wygeneruj populację losowych rozwiązań startowych.
popraw rozwiązania przy użyciu algorytmu local search.
powtarzaj
{
  wylosuj dwa różne rozwiązania stosując rozkład równomierny (rodzice)
  skonstruuj potomne rozwiązanie poprzez rekombinację rodziców polegającą na wyborze jednego z rodziców jako rozwiązania wyjściowego, a potem usunięciu z niego wszystkich krawędzi, które nie występowały w drugim rodzicu. Następnie napraw rozwiązanie za pomocą algorytmu ważonego 2-żalu. (dziecko)
  popraw dziecko przy użyciu algorytmu local search. (opcjonalnie)
  jeżeli dziecko jest lepsze od najgorszego rozwiązania w populacji i daje inny wynik od wszystkich innych rozwiązań, to dodaj dziecko do populacji i usuń najgorsze rozwiązanie.
  jeśli minął czas przeznaczony na algorytm, to zakończ pętlę.
}
zwróć aktualne rozwiązanie.
```

## Wyniki

W tabeli przedstawione zostały najlepsze, średnie i najgorsze wyniki dla zbadanych algorytmów. Tabela zawiera również wyniki dla metod z poprzedniego zadania.

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
    <td class="tg-73oq">HAE (z LS)</td>
    <td class="tg-wp8o">33101</td>
    <td class="tg-wp8o">34556</td>
    <td class="tg-wp8o">36080</td>
    <td class="tg-wp8o">35096</td>
    <td class="tg-wp8o">36184</td>
    <td class="tg-wp8o">37391</td>
  </tr>
  <tr>
    <td class="tg-73oq">HAE (bez LS)</td>
    <td class="tg-wp8o">35029</td>
    <td class="tg-wp8o">36015</td>
    <td class="tg-wp8o">37456</td>
    <td class="tg-wp8o">35128</td>
    <td class="tg-wp8o">36445</td>
    <td class="tg-wp8o">37576</td>
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
  <tr>
    <td class="tg-0pky">2-Regret (waga 0.6)</td>
    <td class="tg-0pky">32477</td>
    <td class="tg-0pky">35934</td>
    <td class="tg-0pky">39746</td>
    <td class="tg-0pky">34255</td>
    <td class="tg-0pky">36841</td>
    <td class="tg-0pky">38341</td>
  </tr>
</tbody>
</table>

W tabeli poniżej przedstawione zostały liczby perturbacji/iteracji wykonanych w algorytmach z aktualnego i poprzednich laboratoriów.

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
    <td class="tg-73oq">HAE (z LS)</td>
    <td class="tg-wp8o">68</td>
    <td class="tg-wp8o">89.5</td>
    <td class="tg-wp8o">113</td>
    <td class="tg-wp8o">121</td>
    <td class="tg-wp8o">159</td>
    <td class="tg-wp8o">203</td>
  </tr>
  <tr>
    <td class="tg-73oq">HAE (bez LS)</td>
    <td class="tg-wp8o">77</td>
    <td class="tg-wp8o">112.9</td>
    <td class="tg-wp8o">176</td>
    <td class="tg-wp8o">146</td>
    <td class="tg-wp8o">184.7</td>
    <td class="tg-wp8o">229</td>
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

HAE (z LS)          |  HAE (bez LS)
:-------------------------:|:-------------------------:
![](./out/kroA200_map_hae-local-search.png)  |  ![](./out/kroA200_map_hae-no-local-search.png)


## Wizualizacja kroB200.tsp

HAE (z LS)          |  HAE (bez LS)
:-------------------------:|:-------------------------:
![](./out/kroB200_map_hae-local-search.png)  |  ![](./out/kroB200_map_hae-no-local-search.png)


## Wnioski

1. Wersja algorytmu HAE z lokalnym przeszukiwaniem wykonywanym na dziecku po każdej rekombinacji daje nieco lepsze wyniki niż wersja algorytmu bez lokalnego przeszukiwania.
2. W wersji HAE bez lokalnego przeszukiwania, tworzone jest więcej dzieci, dzięki zaoszczędzeniu czasu przeznaczonego na wykonywanie lokalnego przeszukiwania obecnego w alternatywnym rozwiązaniu. Poprawa z pokolenia na pokolenie jest mniejsza, ale pokoleń jest więcej.
3. Algorytmy HAE osiągają wyniki gorsze niż ILSx, ale lepsze niż te, osiągnięte poprzez algorytm heurystyczny ważonego 2-żalu.

# Testy globalnej wypukłości

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp
W ramach zadania należało wykonać testy globalnej wypukłości dla tysiąca optimów lokalnych problemu. Jako metoda dająca najlepsze rozwiązanie wykorzystany został algorytm ILS2 zaimplementowany na poprzednich zajęciach. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA200` oraz `kroB200` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- testy globalnej wypukłości z porównywaniem rozwiązań z rozwiązaniem najlepszym z wygenerowanych oraz liczbą wspólnych wierzchołków jako miarą podobieństwa,
- testy globalnej wypukłości z porównywaniem rozwiązań z rozwiązaniem najlepszym z wygenerowanych oraz liczbą wspólnych krawędzi jako miarą podobieństwa,
- testy globalnej wypukłości z porównywaniem rozwiązań z rozwiązaniem wygenerowanym przez ILS2 oraz liczbą wspólnych wierzchołków jako miarą podobieństwa,
- testy globalnej wypukłości z porównywaniem rozwiązań z rozwiązaniem wygenerowanym przez ILS2 oraz liczbą wspólnych krawędzi jako miarą podobieństwa.

Łącznie uzyskano więc 4 różne wyniki dla każdej z instancji.

## Wyniki

W tabeli poniżej przedstawione zostały wartości współczynników korelacji dla testów wykonanych w zadaniu. Współczynniki korelacji dla wersji z porównywaniem najlepszego wygenerowanego rozwiązania były liczone z jego pominięciem, ponieważ jego podobieństwo było maksymalne.

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
    <th class="tg-wp8o" colspan="2">kroA200</th>
    <th class="tg-wp8o" colspan="2">kroB200</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-73oq"></td>
    <td class="tg-wp8o" colspan="1">Wspólne wierzchołki</td>
    <td class="tg-wp8o" colspan="1">Wspólne krawędzie</td>
    <td class="tg-wp8o" colspan="1">Wspólne wierzchołki</td>
    <td class="tg-wp8o" colspan="1">Wspólne krawędzie</td>
  </tr>
  <tr>
    <td class="tg-73oq">Podobieństwo do najlepszego rozwiązania spośród wygerowanych</td>
    <td class="tg-wp8o">-0.0650726</td>
    <td class="tg-wp8o">-0.0180561</td>
    <td class="tg-wp8o">-0.0323841</td>
    <td class="tg-wp8o">-0.0956069</td>
  </tr>
  <tr>
    <td class="tg-73oq">Podobieństwo do wyniku ILS2</td>
    <td class="tg-wp8o">-0.135313</td>
    <td class="tg-wp8o">-0.0485164</td>
    <td class="tg-wp8o">-0.0834998</td>
    <td class="tg-wp8o">-0.172041</td>
  </tr>
  <tr>
    <td class="tg-73oq">Średnie podobieństwo do pozostałych rozwiązań</td>
    <td class="tg-wp8o">-0.494674</td>
    <td class="tg-wp8o">-0.675888</td>
    <td class="tg-wp8o">-0.432865</td>
    <td class="tg-wp8o">-0.704771</td>
  </tr>
</tbody>
</table>

Poniżej umieszczone zostały wykresy przedstawiające zależności podobieństwa rozwiązań od wyników osiąganych przez rozwiązania.

## Wizualizacja kroA200.tsp

Wspólne wierzchołki, porównanie z najlepszym z wygenerowanych         |  Wspólne krawędzie, porównanie z najlepszym z wygenerowanych
:-------------------------:|:-------------------------:
![](../Lab6/out/kroA200_map_common-vertices-best.png)  |  ![](../Lab6/out/kroA200_map_common-edges-best.png)

Wspólne wierzchołki, porównanie z najlepszą metodą         |  Wspólne krawędzie, porównanie z najlepszą metodą
:-------------------------:|:-------------------------:
![](../Lab6/out/kroA200_map_common-vertices-ils2.png)  |  ![](../Lab6/out/kroA200_map_common-edges-ils2.png)

## Wizualizacja kroB200.tsp

Wspólne wierzchołki, porównanie z najlepszym z wygenerowanych         |  Wspólne krawędzie, porównanie z najlepszym z wygenerowanych
:-------------------------:|:-------------------------:
![](../Lab6/out/kroB200_map_common-vertices-best.png)  |  ![](../Lab6/out/kroB200_map_common-edges-best.png)

Wspólne wierzchołki, porównanie z najlepszą metodą         |  Wspólne krawędzie, porównanie z najlepszą metodą
:-------------------------:|:-------------------------:
![](../Lab6/out/kroB200_map_common-vertices-ils2.png)  |  ![](../Lab6/out/kroB200_map_common-edges-ils2.png)

## Podsumowanie

1. Na wykresach, w których rozwiązania były porównywane z rozwiązaniem najlepszym z wygenerowanych, można zauważyć punkt w lewym górnym rogu. Jest to najlepsze rozwiązanie, które posiada najmniejszą długość cykli oraz największe podobieństwo - jest to podobieństwo równe 200, ponieważ rozwiązanie to porównywane jest z samym sobą.
2. Największa korelacja została osiągnięta w punktach obrazujących średnie podobieństwo rozwiązania do pozostałych z wygenerowanych rozwiązań. Współczynnik korelacji wynosił aż -0.7, co można interpretować jako silną korelację ujemną - im rozwiązanie było lepsze, tym mniej średnio różniło się od innych.
3. Podobieństwo do najlepszego wyniku mierzone w liczbie wspólnych wierzchołków wynosiło od 100 do około 150, podczas gdy podobieństwo mierzone w liczbie wspólnych krawędzi wynosiło od około 15 do około 120. Korelacja również jest ujemna, ale dużo mniej widoczna. Oscyluje ona w okolicach -0.1.
