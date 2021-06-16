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
Time: 34 ms => 3494 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 13 ms => 1304 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 384 ms => 57207 ms
Utilization: 7% => 7%
==================================
Challenge 4: simple malloc => my malloc
Time: 23303 ms => 929085 ms
Utilization: 16% => 15%
==================================
Challenge 5: simple malloc => my malloc
Time: 18844 ms => 785748 ms
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

```
Challenge 1: simple malloc => my malloc
Time: 21 ms => 6508 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 12 ms => 3006 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 213 ms => 422833 ms
Utilization: 7% => 3%
==================================
Challenge 4: simple malloc => my malloc
Time: 50392 ms => 2823976 ms
Utilization: 16% => 6%
==================================
Challenge 5: simple malloc => my malloc
Time: 18020 ms => 2487321 ms
Utilization: 15% => 6%
==================================
```

## 考察

Best-fitとWorst-fitは、mallocの際に空き領域を全探索するので、十分な大きさの空き領域が見つかった時点で探索を終了するFirst-fitと比べて実行時間が長くなっていると考えられる。

メモリ効率の観点からは、Worst-fitでは、大きい空き領域から使用していくので、大きいサイズのmallocを要求された際に、残っている空き領域の合計は大きいにもかかわらず、連続した十分な大きさの空き領域が存在しないためにOSからメモリをもらわなければならないことになり、メモリ効率が悪くなったいると思われる。逆に、Best-fitでは、mallocで要求されたサイズに最適な空き領域を提供しているので、大きいサイズのmallocを要求されても十分な大きさの空き領域が残っている可能性が高く、メモリ効率が良くなっていると思われる。
