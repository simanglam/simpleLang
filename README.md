# simpleLang

simpleLang 是為了教學而寫出的小型編譯器，扣除 header 檔後僅有 390 行。simpleLang 有著純手刻的 Tokenizer 與 Parser 並使用 llvm 作為語言的後端，開發前請事先安裝好 llvm。

## 語法
simpleLang 的語法用不正規的方式於 `grammar.md` 中被描述。

### 變數

32-bit signed integer 是世界上最好的型別，因此所有變數都是 32-bit signed integer。宣告語法如下：

```js
var a;
```

### 控制流程

simpleLang 相信使用者，所以將全能的 goto 賜予給你。

```c
label:

goto label2;

goto label;

label2:
```

如果你是謙虛的人，simpleLang 也賞賜了 if 來限縮你的權力。

```c
label:

if (expr) goto label;
```

注意：僅有在 expr 的值等於 0 時才被視為 false。

### 運算子

simplLang 是老古板，僅給了 `+ - * / = ( )`。

### 註解

好的程式應該足夠自我描述，因此 simpleLang 並不提供註解。

### 函數

函數是給弱者的東西，因此 simpleLang 將其刪除。為二例外的是 print 與 get：

```c
get() // 等價於 getchar() in C
print(a) // 等價於 putchar(a) in C
```