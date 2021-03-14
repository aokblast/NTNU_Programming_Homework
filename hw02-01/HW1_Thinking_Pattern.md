## Homework 1 Thinking pattern

## 1.1 Hex to String

* 特判:長度為偶數，結尾兩個零，有字串不是可視字元(ascii table)。

## 1.2 Replacement

* 特判:No
* 注意:new word輸入完跟輸出Before中間有一個空格

* 可能想法:

  一開始的想法可能會是單純的把檢測到be的位置直接在字串裡面換成do，然後在印出來，但是這樣在顏色(因為沒法把顏色代碼送到字串裡面)跟替換字長度不同(譬如說12換成123)的地方會出現問題。

* 我的做法:

  改變前:

  用**for(int i....)**去遍歷**original text**，

  找到**text[i] == keyword[0]**的地方，

  接著用**strncmp(&text[i], keyword, strlen(keyword)) == 0**去確認是不是一樣，上下可以寫在同一個if裡面用and連起來，

  **注意**:前面**的text[i] == keyword[0]**要先寫，要是不等於電腦就不會做**strncmp**的部分，原因上學期加分題有出過，連結符號會從左做到右，並且做到能得到明確的真假值時便停止。

  如果上述兩個條件都一樣代表為**keyword**，用**printf("顏色代碼%s", keyword)**去顯示，之後把**i + strlen(keyword) - 1**繼續跑回圈。

  為啥要-1，因為當你把%s跑完回到for最上面的時候他會先執行**for(int i ...., ++i)**裡面的++i(**不要再寫i++了!**)，再回到for中間的判斷條件**(text[i] != '\0')**之類的，因此-1在+1才會是剛好印結束的那個字元。

  其他**text[i] != keyword[0]** 跟**strncmp(&text[i], keyword, strlen(keyword)) != 0** 的部分就直接用**printf("%c", text[i])**印出來就好了

  **補充**:顏色代碼是改變顏色代碼代碼後的字的屬性，是可以分開寫的，也就是說

```c
printf("\033[34m");
printf("Hello World!");
printf("\033[0m");
```

​		是合法的，Hello World會被替換成藍色，也可以寫成下面那樣，

```c
printf("\033[34mHello World!\033[0m");
```

​		改變後:

​		用跟上面一樣的**text[i] == keyword[0] + strncmp**去檢測字串是不是keyword

​		找到是keyword的i後，

​		加上顏色後把整串**New word**用**printf("%s", newword)**印出來，迴圈裡面的**i **把它加上 **strlen(keyword) - 1**(前面解釋過-1的原因) ，這樣便可以跳過		   		keyword了。

## 1.3 Swap Strings

* 特判:由於是直接改變記憶體位置，不存在特判。

* 注意:記得**#include "hw0103.h"**

  這題是漏洞，看圖

![image-20210313233432088](C:\Users\blast\AppData\Roaming\Typora\typora-user-images\image-20210313233432088.png)

​	這是兩塊字串的記憶體，做交換的時候我們不用真的一個一個換，把指標指到的空間互換就好了

![image-20210313233612579](C:\Users\blast\AppData\Roaming\Typora\typora-user-images\image-20210313233612579.png)

​	pstr1裡面存的是左邊記憶體區塊的開頭位置(pStr得到的是存的記憶體位置，*pStr得到的是存的記憶體位置的值)，

​	pstr2裡面存的是右邊記憶體區塊的開頭位置，

​	也就是說交換的時候用一個tmp的pointer存pstr1。

​	然後就是正常的交換流程。

## 1.4 Json Reader(有點複雜，有空再寫)

## 1.5 My string library

* 注意:記得把hw0105.c 編譯進去

1. mystrchr:

   遍歷到str[i] == c就停止，如果得到str[i] == '\0'，回傳NULL，其他回傳&str[i]

   **注意**:要用&str[i]去回傳第i項的記憶體位置

2. mystrrchr:

   改變迴圈方向即可，

   用**for(i  = 0; str[i] != '\0'; ++i)**或者**i = strlen(str)**找到最後面的一項。

   **注意**: **i**記得定義在迴圈外面，下面才能使用，之後往回找**str[i] == c**直到i == 0，

   回傳值判斷是i == 0 && str[0] != c才回傳NULL，因為有可能第0項是剛好我們要找的c。

3. mystrspn:

   我的做法比較tricky，for會寫成**(i = 0; mystrchr(accept, str[i]) != NULL; ++i)**，

   **注意**:i要定義在迴圈外面

   我們去呼叫我們剛寫好的**mystrchr**去檢查第i項有沒有在**accept**裡面，==NULL代表str[i]不再**accept**裡面，!=NULL的情況下代表有找到，就繼續遍歷，直到找不到為止。

   最後回傳i就好。

4. mystrcspn:

   跟上面的做法一樣，不過繼續執行的條件是**mystrchr(reject, str[i]) == NULL & str[i] != '\0'**

5. mystrpbrk:

   就是上面的作法，用**mystrchr(reject, str[i]) == NULL**代表**str[i]**不再**reject**裡面，如果找到在裡面的就停止，最後如果str[i] != '\0'就回傳&str[i];

6. mystrstr:

   用**for(i=....)**去遍歷字串，找到**str[i] == needle[0]**，就在裡面開一格for往下比較之後的**str[i + j] == needle[j]**，最後needle[j] == '\0'就代表找對了，直接return &str[i]，終止條件就是找到**str[i] == '\0'**就回傳NULL。

7. mystrtok:

   這個功能比較複雜，事情是這樣的

   ![image-20210314171629883](C:\Users\blast\AppData\Roaming\Typora\typora-user-images\image-20210314171629883.png)

   假設我們拿**.**當切斷點，第一次回傳的pointer顯示出來就要顯示ab，第二次使用回傳c，第三次回傳de，之後都回傳NULL。

   也就是說我們需要把**.**的地方換成'\0'，並且回傳原本開頭的位置，分別為a, c, d。

   一開始需要一個Stiatic的char pointer (我設定成tmp)以確保str傳入NULL得時候可以保持使用上次的字串。

   **static功能**:

   1. 隱藏:設置成global 的變數再和其他c的source code編譯的時候會被其他程式使用，但是static可以使只在原本的函式內被使用。

   2. 內容不會消失:存在static的變數在下次function被Call的時候會使用上次保存在裡面的值，而不是初始值。

   3. 自動初始化:會將變數自動初始化成一個初始值(通常為0)，這點由compiler的實作來決定。
   
       
      
      一開始當str不等於NULL的時候就將str賦值到tmp裡面。
   
   
      接著檢查tmp是不是NULL，如果是的話就代表找到字串的結尾了，直接回傳NULL。
   
      接著用剛剛寫好的**char *next = mystrpbrk(tmp, delim)**去尋找下一個斷點的位置，並用一個char pointer把tmp的值存起來。
   
   ​	如果*next == NULL代表沒有下個斷點，把tmp設成NULL(這樣下次呼叫的時候就會因為檢查tmp == NULL而不會繼續找)，接著回	傳tmp原本的值。
   
   ​	如果*next != NULL 就把 *next設定成'\0'，tmp設定成next + 1(斷點的下一個位置，這樣下次尋找就會從斷點後開始找)，最後回傳	tmp原本的值。
   
   ​	