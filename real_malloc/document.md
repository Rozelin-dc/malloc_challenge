## 概要

`my_malloc`が呼び出された際の空き領域の探索の方法をFirst-fit, Best-fit, Worst-fitの三種類で行い、比較した。また、空き領域を結合した場合と結合しなかった場合でも比較を行った。

## 結果

### 空き領域の結合をしなかった場合

#### First-fit

```
Challenge 1: simple malloc => my malloc
Time: 8 ms => 7 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 8 ms => 7 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 118 ms => 83 ms
Utilization: 7% => 8%
==================================
Challenge 4: simple malloc => my malloc
Time: 25014 ms => 26482 ms
Utilization: 16% => 15%
==================================
Challenge 5: simple malloc => my malloc
Time: 19264 ms => 17484 ms
Utilization: 15% => 15%
==================================
```

#### Best-fit

```
Challenge 1: simple malloc => my malloc
Time: 8 ms => 1326 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 7 ms => 570 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 115 ms => 810 ms
Utilization: 7% => 52%
==================================
Challenge 4: simple malloc => my malloc
Time: 24768 ms => 9271 ms
Utilization: 16% => 72%
==================================
Challenge 5: simple malloc => my malloc
Time: 17819 ms => 5701 ms
Utilization: 15% => 72%
==================================
```

#### Worst-fit

```
Challenge 1: simple malloc => my malloc
Time: 7 ms => 1280 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 8 ms => 670 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 118 ms => 63098 ms
Utilization: 7% => 3%
==================================
Challenge 4: simple malloc => my malloc
Time: 24567 ms => 751286 ms
Utilization: 16% => 6%
==================================
Challenge 5: simple malloc => my malloc
Time: 15584 ms => 607540 ms
Utilization: 15% => 6%
==================================
```

### 空き領域を結合した場合

#### First-fit

```
Challenge 1: simple malloc => my malloc
Time: 37 ms => 2675 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 19 ms => 1314 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 253 ms => 46556 ms
Utilization: 7% => 8%
==================================
Challenge 4: simple malloc => my malloc
Time: 22775 ms => 696855 ms
Utilization: 16% => 15%
==================================
Challenge 5: simple malloc => my malloc
Time: 9499 ms => 353356 ms
Utilization: 15% => 15%
==================================
```

#### Best-fit

```
Challenge 1: simple malloc => my malloc
Time: 38 ms => 5343 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 23 ms => 2255 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 205 ms => 2810 ms
Utilization: 7% => 52%
==================================
Challenge 4: simple malloc => my malloc
Time: 32894 ms => 51882 ms
Utilization: 16% => 72%
==================================
Challenge 5: simple malloc => my malloc
Time: 40000 ms => 36019 ms
Utilization: 15% => 72%
==================================
```

#### Worst-fit

```
Challenge 1: simple malloc => my malloc
Time: 8 ms => 2660 ms
Utilization: 70% => 70%
==================================
Challenge 2: simple malloc => my malloc
Time: 8 ms => 1239 ms
Utilization: 40% => 40%
==================================
Challenge 3: simple malloc => my malloc
Time: 118 ms => 171751 ms
Utilization: 7% => 3%
==================================
Challenge 4: simple malloc => my malloc
Time: 23780 ms => 2115979 ms
Utilization: 16% => 6%
==================================
Challenge 5: simple malloc => my malloc
Time: 10065 ms => 1446551 ms
Utilization: 15% => 6%
==================================
```

## 考察

実行時間の観点から見ると、Best-fitとWorst-fitは、mallocの際に空き領域を全探索するので、十分な大きさの空き領域が見つかった時点で探索を終了するFirst-fitと比べて実行時間が長くなっていると考えられる。ただし、後半のチャレンジではなぜかBest-fitの方が早くなっていた。

メモリ効率の観点からは、Worst-fitでは、大きい空き領域から使用していくので、大きいサイズのmallocを要求された際に、残っている空き領域の合計は大きいにもかかわらず、連続した十分な大きさの空き領域が存在しないためにOSからメモリをもらわなければならないことになり、メモリ効率が悪くなったいると思われる。逆に、Best-fitでは、mallocで要求されたサイズに最適な空き領域を提供しているので、大きいサイズのmallocを要求されても十分な大きさの空き領域が残っている可能性が高く、メモリ効率が良くなっていると思われる。

また、空き領域を結合するようにしても、空き領域を結合しなかった場合の同じ方法と比べてUtilizationは向上しなかった。逆に、結合できる空き領域が存在するかを探索するために、実行時間だけ増えてしまった。
