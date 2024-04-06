# Lokalne przeszukiwanie

## Autorzy sprawozdania
Filip Marciniak 148148, Szymon Pasternak 148146

## Wstęp


#### Kod programu

Kod programu dostępny jest w repozytorium:
https://github.com/Johnybonny/IMO

## Opis algorytmów


## Wyniki

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

## Wnioski


## Bibliografia
