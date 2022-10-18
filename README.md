# C++ process bar for loop
一個用來輔助 coding 與 debug 的函式庫
![example.cpp的執行範例圖](https://i.imgur.com/3NoKcfx.png)

## 用途
* 執行順序記錄 `record(string msg)`
    * 在執行到該行時，會在視窗最底行記錄msg參數。
    * 若 msg 為空字串，則自動抓取行數以便記錄

* 迴圈記錄 `record(int cur, int size)`
    * 對應 for(int cur = 0; cur < size; cur++) 的 for 迴圈
    * 使用 cur 與 size 計算執行進度，並於視窗底部顯示進度條
    * 支援同時記錄多個單層迴圈/單個多層迴圈

## 支援與尚未支援
### 作業系統
* Windows系統：較能穩定運行
* 各類Unix系統：可能會有顯示位置錯誤等問題，將在未來版本補齊

### C++ 版本
* C++11 與更舊版本：會導致編譯失敗，尚不支援
* C++14 與 C++17：可以正常運作
* C++20 與未來版本：由於 experimental library 的變動，可能會導致編譯失敗
    * 可嘗試將檔案中 `experimental::source_location` 與 `experimental/source_location` 皆改為 `source_location`

### 多執行續專案
* 目前版本可能導致未預期錯誤，將在未來版本修復

### 執行位置
* 各類編輯器 可能會有跑版問題
* 各類終端機 可以正常執行
 
### 與其他標準輸出並行
* 在專案中的輸出滾動至底行時，可能會有文字被覆蓋的問題
    * 建議減少專案中輸出、增加輸出位置可容納行數、定期清空輸出位置以避免問題產生
    * 未來版本預計以重導向輸出流的方式區隔專案與progress.h的輸出

### .h檔實作未來預計完成部分
* 迴圈記錄
    * 新增 `record(int cur, int begin, int end, int step = 1)`，對應到 `for(int cur = begin; cur < end; cur+=step)` 以便更廣泛對應各類迴圈

    * 新增對應迭代迴圈 `for(auto& i : list)` 的記錄函式
    * 適當移除執行完畢之進度條

* 執行順序記錄
    * 優化專案中「不同檔案內」記錄名稱的辨識度與可讀性

* 其他
    * 新增指定輸出流位置的方法
    * 可以設定進度條顯示於最上方/最下方
    * 可以調整顯示樣式相關參數與更新頻率


## 如何使用
1. 加入 .h 檔，你可以將 progress.h 放入
    1. 你的 library 的存放位置
        * 如果你的編輯器設定正確，通常可以使用 ctrl + 點擊任意#include程式碼得到存放位置
        * 或者可以尋找你的電腦中類似以下所列的位置
            - C:\Program Files\Microsoft Visual Studio\VC\Tools\MSVC\14.33.31629\include
            - C:\Program Files\tdm-gcc\lib\gcc\x86_64-w64-mingw32\10.3.0\include\c++
            - C:\Program Files\MinGW\lib\gcc\mingw32\9.2.0\include\c++
    2. 你的專案

2. 在你的 .cpp檔 中加入標頭 
    * `#include <progress.h>`
    * `#include "relative_path/progress.h"`
        如果你直接將 .h檔 放入專案

3. 在專案中使用 record() method，並加入[適當參數](#用途)。