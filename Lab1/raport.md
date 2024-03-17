# Heurystyki zachłanne

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp

W ramach zadania należało przeprowadzić badania dotyczące trzech algorytmów:
1. Heurystyka najbliższego sąsiada (nearest neighbor)
2. Metoda rozbudowy cyklu (greedy cycle)
3. Heurystyka zachłanna oparta na żalu (regret heuristics)

na przykładzie zmodyfikowanego problemu komiwojażera. W problemie należy dla danego zbioru wierzchołków oraz symetrycznej macierzy odległości ułożyć dwa rozłączne cykle, z których każdy zawiera 50% wierzchołków. Celem jest minimalizacja łącznej długości obu cykli. 

W pracy rozważane są dwie instancje - `kroA100` oraz `kroB100` pochodzące z biblioteki `TSPLib`[1]. Odległości pomiędzy wierzchołkami obliczone zostały jako odległości euklidesowe oraz zaokrąglone matematycznie.

#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów

Wszystkie z podanych poniżej algorytmów akceptują na wejściu macierz odległości pomiędzy danymi wierzchołkami. 

### Algorytm Nearest Neighbor

```
Pseudokod programu
```

### Algorytm Greedy Cycle

```
Pseudokod programu
```

### Algorytm 2-regret

```
Pseudokod programu
```

## Wyniki
Każdy algorytm uruchomiony został 100 razy - dla każdego wierzchołka, który wyznaczany był jako startowy. W tabeli przedstawione zostały wyniki działania programu.

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
    <th class="tg-c3ow" colspan="3">kroA100</th>
    <th class="tg-c3ow" colspan="3">kroaB</th>
  </tr>
</thead>
<tbody>
  <tr>
    <td class="tg-0pky"></td>
    <td class="tg-0pky">min</td>
    <td class="tg-0pky">mean</td>
    <td class="tg-0pky">max</td>
    <td class="tg-0pky">min</td>
    <td class="tg-0pky">mean</td>
    <td class="tg-0pky">max</td>
  </tr>
  <tr>
    <td class="tg-0pky">Nearest neighbor</td>
    <td class="tg-0pky">26521.5</td>
    <td class="tg-0pky">30908</td>
    <td class="tg-0pky">33929.6</td>
    <td class="tg-0pky">26937.5</td>
    <td class="tg-0pky">29309.5</td>
    <td class="tg-0pky">31521.7</td>
  </tr>
  <tr>
    <td class="tg-0pky">Greedy cycle</td>
    <td class="tg-0pky">26308.9</td>
    <td class="tg-0pky">28710.8</td>
    <td class="tg-0pky">29981.1</td>
    <td class="tg-0pky">27177.4</td>
    <td class="tg-0pky">28538.1</td>
    <td class="tg-0pky">30196.7</td>
  </tr>
  <tr>
    <td class="tg-0pky">2-Regret</td>
    <td class="tg-0pky">22317.6</td>
    <td class="tg-0pky">27738.5</td>
    <td class="tg-0pky">34116.2</td>
    <td class="tg-0pky">23876.2</td>
    <td class="tg-0pky">28720.3</td>
    <td class="tg-0pky">34631.9</td>
  </tr>
</tbody>
</table>

## Wnioski

## Bibliografia
[1] https://github.com/mastqe/tsplib