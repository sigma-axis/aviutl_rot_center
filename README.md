# 回転中心・上下左右揃え 拡張編集フィルタ

拡張編集フィルタで回転中心を移動させるフィルタがなぜかなかったため追加してみました．

ついでにオブジェクトを上下左右揃えにできるフィルタも一緒に同梱しています． ~~（抱き合わせ商法）~~

@nazonoSAUNA 版の patch.aul の機能の，拡張編集にフィルタ効果を追加するプラグインの練習です．

![操作UI](https://github.com/sigma-axis/aviutl_rot_center/assets/132639613/05f74496-036b-4404-baa2-715a1f52acea)

## 動作要件

- AviUtl 1.10 + 拡張編集 0.92

  http://spring-fragrance.mints.ne.jp/aviutl
  - 拡張編集 0.93rc1 等の他バージョンでは動作しません．

- Visual C++ 再頒布可能パッケージ（\[2015/2017/2019/2022\] の x86 対応版が必要）

  https://learn.microsoft.com/ja-jp/cpp/windows/latest-supported-vc-redist

- patch.aul の `r43 謎さうなフォーク版58` (`r43_ss_58`) 以降

  https://github.com/nazonoSAUNA/patch.aul/releases/latest


## 導入方法

`aviutl.exe` と同階層にある `plugins` フォルダ内に `rot_center.eef` ファイルをコピーしてください．


## 使い方

フィルタ効果の追加メニューに「回転中心」「上下左右揃え」が追加されています．

### 回転中心

- `X`, `Y`, `Z` でオブジェクトの回転中心を指定数値だけ加算します．

  スクリプト制御で次のように書くのと同じ効果です:

  ```lua
  obj.cx = obj.cx + X
  obj.cy = obj.cy + Y
  obj.cz = obj.cz + Z
  ```

### 上下左右揃え

- `左右%` で水平方向の揃え方を指定します．

  - `0` だと中央揃え，`100` だと右揃え，`-100` だと左揃えです．初期値は `0` (中央揃え).

    - 中間の値もそれなりの機能をしますが，実用的な場面は余りないと思います．

- `上下%` で垂直方向の揃え方を指定します．

  - `0` だと中央揃え，`100` だと下揃え，`-100` だと上揃えです．初期値は `0` (中央揃え).

- `左右有効`，`上下有効` のチェックを OFF にすると，それぞれ `左右%`, `上下%` の指定が無効になります．

  - フィルタ効果をかける前の揃え位置を優先させたい場合に利用します．

## その他

1.  スクリプト制御からも他のフィルタ効果と同様，次のように呼び出して利用できます:

    ```lua
    obj.effect("上下左右揃え", "左右%", -100, "上下有効", 0)
    ```

1.  既に同機能のアニメーション効果があります．

    https://github.com/sigma-axis/sigma_aviutl_scripts

1.  他の拡張編集フィルタ制作の息抜きとしてこの拡張編集フィルタを作りました．~~何を言ってるのかわからないと思いますが私もよくわかりません．~~


## ライセンス・免責事項

このプログラムの利用・改変・再頒布等に関しては WTFPL ライセンスに従うものとします．

---

```
            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2024 sigma-axis

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
```

#  Credits

##  aviutl_exedit_sdk

https://github.com/ePi5131/aviutl_exedit_sdk （利用したブランチは[こちら](https://github.com/sigma-axis/aviutl_exedit_sdk/tree/self-use)です．）

---

1条項BSD

Copyright (c) 2022
ePi All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
THIS SOFTWARE IS PROVIDED BY ePi “AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ePi BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#  連絡・バグ報告

- GitHub: https://github.com/sigma-axis
- Twitter: https://twitter.com/sigma_axis
- nicovideo: https://www.nicovideo.jp/user/51492481
- Misskey.io: https://misskey.io/@sigma_axis
- Bluesky: https://bsky.app/profile/sigma-axis.bsky.social
