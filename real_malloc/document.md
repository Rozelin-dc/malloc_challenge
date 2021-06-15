## 概要

`my_malloc`が呼び出された際の空き領域の探索の方法をFirst-fit, Best-fit, Worst-fitの三種類で行い、比較した。また、空き領域を結合した場合と結合しなかった場合でも比較を行った。

## 結果

### 空き領域の結合をしなかった場合

#### First-fit

```
Challenge 1: simple malloc => my malloc
Time: 41 ms => 30 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 28 ms => 27 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 323 ms => 354 ms
Utilization: 7% => 7%
==================================
Challenge 4: simple malloc => my malloc
Time: 60821 ms => 75176 ms
Utilization: 16% => 15%
==================================
Challenge 5: simple malloc => my malloc
Time: 66212 ms => 66183 ms
Utilization: 15% => 14%
==================================
```

#### Best-fit

```
Challenge 1: simple malloc => my malloc
Time: 36 ms => 2689 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 12 ms => 1151 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 223 ms => 1599 ms
Utilization: 7% => 51%
==================================
Challenge 4: simple malloc => my malloc
Time: 53132 ms => 35804 ms
Utilization: 16% => 71%
==================================
Challenge 5: simple malloc => my malloc
Time: 44812 ms => 16093 ms
Utilization: 15% => 71%
==================================
```

#### Worst-fit

```
Challenge 1: simple malloc => my malloc
Time: 44 ms => 2936 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 13 ms => 1313 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 240 ms => 92654 ms
Utilization: 7% => 3%
==================================
Challenge 4: simple malloc => my malloc
Time: 21805 ms => 1118463 ms
Utilization: 16% => 6%
==================================
Challenge 5: simple malloc => my malloc
Time: 30932 ms => 876191 ms
Utilization: 15% => 6%
==================================
```

### 空き領域を連結した場合

#### First-fit

```
Challenge 1: simple malloc => my malloc
Time: 55 ms => 3738 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 22 ms => 1302 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 392 ms => 180470 ms
Utilization: 7% => 7%
==================================
Challenge 4: simple malloc => my malloc
Time: 69002 ms => 1940418 ms
Utilization: 16% => 15%
==================================
Challenge 5: simple malloc => my malloc
Time: 53462 ms => 1775658 ms
Utilization: 15% => 14%
==================================
```

#### Best-fit
要改善
```
Challenge 1: simple malloc => my malloc
Time: 8 ms => 2695 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 7 ms => 1287 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 129 ms => 1581 ms
Utilization: 7% => 51%
==================================
Challenge 4: simple malloc => my malloc
Time: 24739 ms => 20177 ms
Utilization: 16% => 71%
==================================
Challenge 5: simple malloc => my malloc
Time: 18094 ms => 13846 ms
Utilization: 15% => 71%
==================================
```

#### Worst-fit
要改善
```
Challenge 1: simple malloc => my malloc
Time: 9 ms => 2802 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 7 ms => 1381 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 125 ms => 188222 ms
Utilization: 7% => 3%
==================================
Challenge 4: simple malloc => my malloc
Time: 24411 ms => 2175827 ms
Utilization: 16% => 6%
==================================
Challenge 5: simple malloc => my malloc
Time: 18250 ms => 1805953 ms
Utilization: 15% => 6%
==================================
```

## 考察

Best-fitとWorst-fitは空き領域を全探索するので、First-fitと比べて実行時間が長くなっていると考えられる。また、Best-fitだと、空き領域の結合を行わないと細切れの空き領域が大量に発生してしまうので、メモリ使用量の観点からも効率が悪いと思われる。
