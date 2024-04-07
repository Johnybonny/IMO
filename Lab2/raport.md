# Lokalne przeszukiwanie

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp

W ramach zadania należało przeprowadzić badania dotyczące lokalnego przeszukiwania. Rozpatrywanym problemem jest zmodyfikowany problem komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. W pracy rozważane są dwie instancje - `kroA100` oraz `kroB100` pochodzące z biblioteki `TSPLib`. Dla każdej z instancji zbadano:
- różne rozwiązania początkowe: losowe oraz uzyskane heurystyką 2-żal,
- różne rodzaje sąsiedztwa: zamianę wierzchołków oraz krawędzi,
- różne rodzaje przeszukiwania: zachłanne oraz strome.

Łącznie uzyskano więc 8 różnych wyników dla każdej z instancji. Dodatkowo przeprowadzono również badanie losowego błądzenia, które polega na wykonywaniu losowych ruchów. Czas trwania takiego błądzenia został ustalony jako średnio najwolniejsza z wersji lokalnego przeszukiwania.

#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów

Każdy z algorytmów akceptuje na wejściu macierz odległości pomiędzy danymi wierzchołkami. Jako algorytm 2-żal użyty został algorytm zaimplementowany w ramach pierwszego sprawozdania. Każdy z algorytmów uruchomiony został 100 razy. Dla algorytmu 2-żal w każdej ze 100 iteracji wybierany był po kolei każdy wierzchołek początkowy jako start pierwszego cyklu i najdalszy od niego wierzchołek jako start drugiego cyklu. Dla algorytmu, w którym rozwiązanie startowe jest losowe w każdej ze 100 iteracji rozwiązanie początkowe było na nowo losowane.

W liście możliwych ruchów znajdują się ruchy oznaczające wymianę wierzchołków pomiędzy cyklami oraz w zależności od rozpatrywanego rodzaju sąsiedztwa:
- ruchy oznaczające wymianę wierzchołków w ramach jednego cyklu
- ruchy oznaczające wymianę krawędzi w cyklu

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
  jeżeli używane ruchy wewnątrztrasowe to wymiana krawędzi 
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

W tabeli poniżej przedstawione zostały czasy wykonania poszczególnych algorytmów w milisekundach.

<center>
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
    <th class="tg-0pky"></th>
    <th class="tg-0pky"></th>
    <th class="tg-0pky" colspan="3">kroA100</th>
    <th class="tg-0pky" colspan="3">kroB100</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"></td>
    <td class="tg-c3ow" colspan="6">czas [ms]</td>
  </tr>
  <tr>
    <td class="tg-0pky">Rozwiązanie początkowe</td>
    <td class="tg-0pky">Przeszukiwanie</td>
    <td class="tg-0pky">Sąsiedztwo</td>
    <td class="tg-0pky">min</td>
    <td class="tg-0pky">średnia</td>
    <td class="tg-0pky">max</td>
    <td class="tg-0pky">min</td>
    <td class="tg-0pky">średnia</td>
    <td class="tg-0pky">max</td>
  </tr>
  <tr>
    <td class="tg-0pky">Losowe</td>
    <td class="tg-0pky">Zachłanne</td>
    <td class="tg-0pky">Wierzchołki</td>
    <td class="tg-0pky">264.53</td>
    <td class="tg-0pky">343.83</td>
    <td class="tg-0pky">516.78</td>
    <td class="tg-0pky">254.95</td>
    <td class="tg-0pky">333.27</td>
    <td class="tg-0pky">438.70</td>
  </tr>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">Krawędzie</td>
    <td class="tg-0pky">285.80</td>
    <td class="tg-0pky">336.63</td>
    <td class="tg-0pky">421.87</td>
    <td class="tg-0pky">256.338</td>
    <td class="tg-0pky">331.508</td>
    <td class="tg-0pky">391.19</td>
  </tr>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">Strome</td>
    <td class="tg-0pky">Wierzchołki</td>
    <td class="tg-0pky">68.68</td>
    <td class="tg-0pky">97.81</td>
    <td class="tg-0pky">133.38</td>
    <td class="tg-0pky">78.32</td>
    <td class="tg-0pky">99.04</td>
    <td class="tg-0pky">136.48</td>
  </tr>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">Krawędzie</td>
    <td class="tg-0pky">64.20</td>
    <td class="tg-0pky">78.66</td>
    <td class="tg-0pky">110.12</td>
    <td class="tg-0pky">64.89</td>
    <td class="tg-0pky">80.663</td>
    <td class="tg-0pky">110.76</td>
  </tr>
  <tr>
    <td class="tg-0pky">2-żal</td>
    <td class="tg-0pky">Zachłanne</td>
    <td class="tg-0pky">Wierzchołki</td>
    <td class="tg-0pky">2.44</td>
    <td class="tg-0pky">11.17</td>
    <td class="tg-0pky">40.31</td>
    <td class="tg-0pky">2.37</td>
    <td class="tg-0pky">11.72</td>
    <td class="tg-0pky">30.74</td>
  </tr>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">Krawędzie</td>
    <td class="tg-0pky">2.49</td>
    <td class="tg-0pky">15.27</td>
    <td class="tg-0pky">46.66</td>
    <td class="tg-0pky">5.56</td>
    <td class="tg-0pky">17.78</td>
    <td class="tg-0pky">39.18</td>
  </tr>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">Strome</td>
    <td class="tg-0pky">Wierzchołki</td>
    <td class="tg-0pky">1.61</td>
    <td class="tg-0pky">7.40</td>
    <td class="tg-0pky">19.35</td>
    <td class="tg-0pky">1.62</td>
    <td class="tg-0pky">7.72</td>
    <td class="tg-0pky">17.55</td>
  </tr>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">Krawędzie</td>
    <td class="tg-0pky">1.41</td>
    <td class="tg-0pky">8.66</td>
    <td class="tg-0pky">18.31</td>
    <td class="tg-0pky">3.52</td>
    <td class="tg-0pky">10.33</td>
    <td class="tg-0pky">21.34</td>
  </tr>
</tbody>
</table>
</center>

Poniżej umieszczone zostały wizualizacje najlepszych z uzyskanych wyników:

## Wizualizacja kroA100.tsp

Losowe          |  Losowe z losowym błądzeniem
:-------------------------:|:-------------------------:
![](./out/kroA100/random/none/edges/map.png)  |  ![](./out/kroA100/random/randomWalk/edges/map.png)

Losowe, zachłanne, krawędzie         |  Losowe, zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/random/greedy/edges/map.png)  |  ![](./out/kroA100/random/greedy/vertices/map.png)

<br><br><br>
Losowe, strome, krawędzie           |  Losowe, strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/random/steepest/edges/map.png)  |  ![](./out/kroA100/random/steepest/vertices/map.png)

2-żal          |  2-żal z losowym błądzeniem
:-------------------------:|:-------------------------:
![](./out/kroA100/regret/none/edges/map.png)  |  ![](./out/kroA100/regret/randomWalk/edges/map.png)

2-żal, zachłanne, krawędzie         |  2-żal, zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/regret/greedy/edges/map.png)  |  ![](./out/kroA100/regret/greedy/vertices/map.png)

<br><br><br>
2-żal, strome, krawędzie           |  2-żal, strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroA100/regret/steepest/edges/map.png)  |  ![](./out/kroA100/regret/steepest/vertices/map.png)

## Wizualizacja kroB100.tsp

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

2-żal, zachłanne, krawędzie         |  2-żal, zachłanne, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroB100/regret/greedy/edges/map.png)  |  ![](./out/kroB100/regret/greedy/vertices/map.png)

<br><br><br><br><br><br><br>
2-żal, strome, krawędzie            |  2-żal, strome, wierzchołki
:-------------------------:|:-------------------------:
![](./out/kroB100/regret/steepest/edges/map.png)  |  ![](./out/kroB100/regret/steepest/vertices/map.png)

## Wnioski

1. Lokalne przeszukiwanie jest efektywną metodą poprawy rozwiązania bazowego. W każdym przypadku udało się poprawić rozwiązanie wygenerowane jako startowe. Dla rozwiązań losowych jest to spora poprawa - w średnim przypadku o 130 tysięcy jednostek (z 170 tysięcy do 40 tysięcy), a dla heurystyki 2-żalu o 1000 jednostek (z 26 tysięcy do 25 tysięcy).
2. W rozpatrywanym problemie lepiej wypada sąsiedztwo definiowane jako wymiana dwóch krawędzi. Jest to prawdziwe dla każdego zbadanego przypadku.
3. Algorytm stromego lokalnego przeszukiwania radzi sobie lepiej niż algorytm zachłannego lokalnego przeszukiwania w przypadku średnim dla zamiany krawędzi. Dla sąsiedztwa, w którym możliwa jest zamiana wierzchołków sytuacja nie jest tak klarowna - dla startowych rozwiązań losowych algorytm zachłanny radzi sobie lepiej i jest to różnica około 1500 jednostek, natomiast dla rozwiązań startowych wygenerowanych heurystyką 2-żal wyniki są do siebie bardzo zbliżone, a często nawet identyczne.
4. Czasy wykonania algorytmów są w sporej mierze uzależnione od rozwiązania startowego. Dla rozwiązań losowych istnieje dużo ruchów, które zdolne są je poprawić, dlatego przetwarzanie trwa o wiele dłużej niż dla rozwiązania startowego 2-żal, gdzie jest mało ruchów, które można wykonać i są one w stanie poprawić ogólny wynik.
5. Różnica w czasie działania pomiędzy algorytmem zachłannym, a stromym wynika z tego, że w przeszukiwaniu zachłannym rozwiązanie poprawiane jest małymi krokami - mimo tego, że nie szukamy lokalnie najlepszej poprawy, tylko akceptujemy pierwszą, którą znajdziemy, to wykonujemy tych popraw bardzo dużo, a przez to często zmieniamy cykle. W przeszukiwaniu stromym podejmujemy lokalnie najlepsze ruchy, więc mimo tego że musimy policzyć dla każdego deltę, to wykonujemy jedynie jeden ruch, który mocno poprawia rozwiązanie.
