# Lokalne przeszukiwanie

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp

W ramach zadania należało przeprowadzić badania dotyczące lokalnego przeszukiwania. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA100` oraz `kroB100` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- różne rozwiązania początkowe: losowe oraz uzyskane heurystyką 2-żal,
- różne rodzaje sąsiedztwa: zamianę wierzchołków oraz krawędzi,
- różne rodzaje przeszukiwania: zachłanne oraz strome.

Łącznie uzyskano więc 8 różnych wyników dla każdej z instancji. Dodatkowo przeprowadzono również badanie losowego błądzenia, które polega na wykonywaniu losowych ruchów. Czas trwania takiego błądzenia został ustalony jak średnio najwolniejsza z wersji lokalnego przeszukiwania.

#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów

Każdy z algorytmów akceptuje na wejściu macierz odległości pomiędzy danymi wierzchołkami. Jako algorytm 2-żal użyty został algorytm zaimplementowany w ramach pierwszego sprawozdania. Każdy z algorytmów uruchomiony został 100 razy. Dla algorytmu 2-żal w każdej ze 100 iteracji wybierany był po kolei każdy wierzchołek początkowy jako start pierwszego cyklu i najdalszy od niego wierzchołek jako start drugiego cyklu. Dla algorytmu, w którym rozwiązanie startowe jest losowe w każdej ze 100 iteracji rozwiązanie początkowe było na nowo losowane.

W poniższych pseudokodach wykorzystana została notacja:
- distances[a][b] - oznacza odległość pomiędzy punktami `a` i `b`


### Algorytm delty funkcji celu

- Wymiana wierzchołków A i B pomiędzy dwoma cyklami
```
ustaw deltę na 0.
odejmij od delty distances[wierzchołek przed A][A] oraz distances[A][wierzchołek po A].
odejmij od delty distances[wierzchołek przed B][B] oraz distances[B][wierzchołek po B].
dodaj do delty distances[wierzchołek przed A][B] oraz distances[wierzchołek po A][B].
dodaj do delty distances[wierzchołek przed B][A] oraz distances[wierzchołek po B][A].
zwróć deltę.
```

- Zamiana miejscami wierzchołków A i B w cyklu
```
ustaw deltę na 0.
odejmij od delty distances[wierzchołek przed A][A] oraz distances[A][wierzchołek po A].
odejmij od delty distances[wierzchołek przed B][B] oraz distances[B][wierzchołek po B].
dodaj do delty distances[wierzchołek przed A][B] oraz distances[B][wierzchołek po A].
dodaj do delty distances[wierzchołek przed B][A] oraz distances[A][wierzchołek po B].
jeżeli wierzchołki A i B sąsiadują ze sobą 
{
  dodaj do delty 2*distances[A][B].
}
zwróć deltę.
```

- Zamiana miejscami krawędzi A->(wierzchołek po A) i B->(wierzchołek po B)
```
ustaw deltę na 0.
odejmij od delty distances[A][wierzchołek po A] oraz distances[B][wierzchołek po B].
dodaj do delty distances[A][B] oraz distances[wierzchołek po A][wierzchołek po B].
zwróć deltę.
```

### Algorytm lokalnego błądzenia
```
wygeneruj rozwiązanie startowe i listę możliwych ruchów.
oblicz łączną długość rozwiązania startowego.
dopóki nie skończy się czas przeznaczony na działanie algorytmu
{
  wylosuj ruch z listy możliwych.
  oblicz deltę korzystając z odpowiedniego algorytmu.
  wykonaj ruch.
  dodaj do łącznej długości obliczoną deltę.
  jeżeli łączna długość jest najmniejsza z dotychczasowych to zapamiętaj ją.
}
zwróć zapamiętaną łączną długość.
```

### Algorytm zachłannego lokalnego przeszukiwania

```
wygeneruj rozwiązanie startowe i listę możliwych ruchów.
powtarzaj 
{
  przetasuj listę możliwych ruchów.
  dla każdego ruchu w liście
  {
    oblicz deltę korzystając z odpowiedniego algorytmu.
    jeżeli delta jest ujemna to wykonaj ruch i zakończ pętlę.
  }
  jeżeli nie została znaleziona żadna ujemna delta to zakończ pętlę.
}
zwróć rozwiązanie.
```

### Algorytm stromego lokalnego przeszukiwania

```
wygeneruj rozwiązanie startowe i listę możliwych ruchów.
powtarzaj 
{
  dla każdego ruchu w liście dotyczącego wymiany wierzchołków pomiędzy cyklami
  {
    oblicz deltę korzystając z odpowiedniego algorytmu.
    jeżeli delta jest najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.
  }

  jeżeli używane ruchy wewnątrztrasowe to wymiana wierzchołków
  {
    dla każdego ruchu w liście dotyczącego wymiany wierzchołków w cyklu
    {
      oblicz deltę korzystając z odpowiedniego algorytmu.
      jeżeli delta jest najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.
    }
  }
  jeżeli używane urchy wewnątrztrasowe to wymiana krawędzi 
  {
    dla każdego ruchu w liście dotyczącego wymiany krawędzi w cyklu
    {
      oblicz deltę korzystając z odpowiedniego algorytmu.
      jeżeli delta jest najmniejsza z dotychczasowych to zapamiętaj ją oraz ruch.
    }
  }
  wykonaj zapamiętany ruch.
  jeżeli nie została znaleziona żadna ujemna delta to zakończ pętlę.
}
zwróć rozwiązanie.
```

## Wyniki

W tabeli przedstawione zostały najlepsze, średnie i najgorsze wyniki dla zbadanych algorytmów. 

<center>
<style type="text/css">
.tg  {border-collapse:collapse;border-spacing:0;}
.tg td{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  overflow:hidden;padding:10px 5px;word-break:normal;}
.tg th{border-color:black;border-style:solid;border-width:1px;font-family:Arial, sans-serif;font-size:14px;
  font-weight:normal;overflow:hidden;padding:10px 5px;word-break:normal;}
.tg .tg-iks7{background-color:#ffffff;border-color:#000000;text-align:left;vertical-align:top}
.tg .tg-jbyd{background-color:#ffffff;border-color:#000000;text-align:center;vertical-align:top}
.tg .tg-vhtn{background-color:#ffffff;border-color:#000000;text-align:center;vertical-align:middle}
</style>
<table class="tg">
<thead>
  <tr>
    <th class="tg-iks7"></th>
    <th class="tg-iks7"></th>
    <th class="tg-iks7"></th>
    <th class="tg-jbyd" colspan="3">kroA100</th>
    <th class="tg-jbyd" colspan="3">kroB100</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-iks7">Rozwiązanie początkowe</td>
    <td class="tg-iks7">Przeszukiwanie</td>
    <td class="tg-iks7">Sąsiedztwo</td>
    <td class="tg-iks7">min</td>
    <td class="tg-iks7">mean</td>
    <td class="tg-iks7">max</td>
    <td class="tg-iks7">min</td>
    <td class="tg-iks7">mean</td>
    <td class="tg-iks7">max</td>
  </tr>
  <tr>
    <td class="tg-vhtn" rowspan="6">Losowe</td>
    <td class="tg-jbyd" colspan="2">Brak (rozwiązanie startowe)</td>
    <td class="tg-iks7">154430</td>
    <td class="tg-iks7">170325</td>
    <td class="tg-iks7">190820</td>
    <td class="tg-iks7">144358</td>
    <td class="tg-iks7">166180</td>
    <td class="tg-iks7">184229</td>
  </tr>
  <tr>
    <td class="tg-vhtn" colspan="2">Losowe błądzenie</td>
    <td class="tg-iks7">132402</td>
    <td class="tg-iks7">141202</td>
    <td class="tg-iks7">147487</td>
    <td class="tg-iks7">128926</td>
    <td class="tg-iks7">139112</td>
    <td class="tg-iks7">143824</td>
  </tr>
  <tr>
    <td class="tg-vhtn" rowspan="2">Zachłanne</td>
    <td class="tg-iks7">Wierzchołki</td>
    <td class="tg-iks7">36281</td>
    <td class="tg-iks7">44947</td>
    <td class="tg-iks7">56245</td>
    <td class="tg-iks7">36570</td>
    <td class="tg-iks7">44582</td>
    <td class="tg-iks7">52943</td>
  </tr>
  <tr>
    <td class="tg-iks7">Krawędzie</td>
    <td class="tg-iks7">26286</td>
    <td class="tg-iks7">29476</td>
    <td class="tg-iks7">33116</td>
    <td class="tg-iks7">27387</td>
    <td class="tg-iks7">30127</td>
    <td class="tg-iks7">33795</td>
  </tr>
  <tr>
    <td class="tg-vhtn" rowspan="2">Strome</td>
    <td class="tg-iks7">Wierzchołki</td>
    <td class="tg-iks7">37023</td>
    <td class="tg-iks7">46350</td>
    <td class="tg-iks7">56882</td>
    <td class="tg-iks7">39659</td>
    <td class="tg-iks7">46747</td>
    <td class="tg-iks7">56728</td>
  </tr>
  <tr>
    <td class="tg-iks7">Krawędzie</td>
    <td class="tg-iks7">26370</td>
    <td class="tg-iks7">29293</td>
    <td class="tg-iks7">32858</td>
    <td class="tg-iks7">27001</td>
    <td class="tg-iks7">29924</td>
    <td class="tg-iks7">32460</td>
  </tr>
  <tr>
    <td class="tg-vhtn" rowspan="6">Regret</td>
    <td class="tg-jbyd" colspan="2">Brak (rozwiązanie startowe)</td>
    <td class="tg-iks7">22922</td>
    <td class="tg-iks7">25870</td>
    <td class="tg-iks7">30183</td>
    <td class="tg-iks7">23876</td>
    <td class="tg-iks7">27094</td>
    <td class="tg-iks7">31185</td>
  </tr>
  <tr>
    <td class="tg-jbyd" colspan="2">Losowe błądzenie</td>
    <td class="tg-iks7">24008</td>
    <td class="tg-iks7">30714</td>
    <td class="tg-iks7">41296</td>
    <td class="tg-iks7">24017</td>
    <td class="tg-iks7">30634</td>
    <td class="tg-iks7">40911</td>
  </tr>
  <tr>
    <td class="tg-vhtn" rowspan="2">Zachłanne</td>
    <td class="tg-iks7">Wierzchołki</td>
    <td class="tg-iks7">22746</td>
    <td class="tg-iks7">25112</td>
    <td class="tg-iks7">28644</td>
    <td class="tg-iks7">23453</td>
    <td class="tg-iks7">26103</td>
    <td class="tg-iks7">30859</td>
  </tr>
  <tr>
    <td class="tg-iks7">Krawędzie</td>
    <td class="tg-iks7">22141</td>
    <td class="tg-iks7">24603</td>
    <td class="tg-iks7">29245</td>
    <td class="tg-iks7">23412</td>
    <td class="tg-iks7">25615</td>
    <td class="tg-iks7">28790</td>
  </tr>
  <tr>
    <td class="tg-vhtn" rowspan="2">Strome</td>
    <td class="tg-iks7">Wierzchołki</td>
    <td class="tg-iks7">22746</td>
    <td class="tg-iks7">25111</td>
    <td class="tg-iks7">28673</td>
    <td class="tg-iks7">23453</td>
    <td class="tg-iks7">26115</td>
    <td class="tg-iks7">30859</td>
  </tr>
  <tr>
    <td class="tg-iks7">Krawędzie</td>
    <td class="tg-iks7">22093</td>
    <td class="tg-iks7">24325</td>
    <td class="tg-iks7">28776</td>
    <td class="tg-iks7">23412</td>
    <td class="tg-iks7">25448</td>
    <td class="tg-iks7">28790</td>
  </tr>
</tbody>
</table>
</center>

W tabeli poniżej przedstawione zostały czasu wykonania poszczególnych algorytmów w milisekundach:

```
TODO tabelka z czasami
```

Poniżej umieszczone zostały wizualizacje najlepszych z uzyskanych wyników:

### kroA100.tsp

Losowe          |  Losowe z losowym błądzeniem
:-------------------------:|:-------------------------:
![](./out/kroA100/random/none/edges/map.png)  |  ![](./out/kroA100/random/randomWalk/edges/map.png)

Losowe, zachłanne, krawędzie         |  Losowe, zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/random/greedy/edges/map.png)  |  ![](./out/kroA100/random/greedy/vertices/map.png)

Losowe, strome, krawędzie           |  Losowe, strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/random/steepest/edges/map.png)  |  ![](./out/kroA100/random/steepest/vertices/map.png)

2-żal          |  2-żal z losowym błądzeniem
:-------------------------:|:-------------------------:
![](./out/kroA100/regret/none/edges/map.png)  |  ![](./out/kroA100/regret/randomWalk/edges/map.png)

2-żal , zachłanne, krawędzie         |  2-żal , zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/regret/greedy/edges/map.png)  |  ![](./out/kroA100/regret/greedy/vertices/map.png)

2-żal , strome, krawędzie           |  2-żal , strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/regret/steepest/edges/map.png)  |  ![](./out/kroA100/regret/steepest/vertices/map.png)

### kroB100.tsp

Losowe         |  Losowe z losowym błądzeniem
:-------------------------:|:-------------------------:
![](./out/kroB100/random/none/edges/map.png)    |  ![](./out/kroB100/random/randomWalk/edges/map.png)

Losowe, zachłanne, krawędzie         |  Losowe, zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroB100/random/greedy/edges/map.png)  |  ![](./out/kroB100/random/greedy/vertices/map.png)

Losowe, strome, krawędzie            |  Losowe, strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroB100/random/steepest/edges/map.png)  |  ![](./out/kroB100/random/steepest/vertices/map.png)

2-żal                                |  2-żal z losowym błądzeniem
:-------------------------:|:-------------------------:
![](./out/kroB100/regret/none/edges/map.png)  |  ![](./out/kroB100/regret/randomWalk/edges/map.png)

2-żal , zachłanne, krawędzie         |  2-żal , zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroB100/regret/greedy/edges/map.png)  |  ![](./out/kroB100/regret/greedy/vertices/map.png)

2-żal , strome, krawędzie            |  2-żal , strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroB100/regret/steepest/edges/map.png)  |  ![](./out/kroB100/regret/steepest/vertices/map.png)

## Wnioski

