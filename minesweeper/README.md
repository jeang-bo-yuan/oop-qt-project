# User Manual

-------------------------------------------------------------------------------------
GUI Mode
=====================================================================================

直接雙擊 minesweeper.exe 或在終端輸入 `minesweeper.exe GUI` 以啟動程式。

在這款踩地雷遊戲中，你有三種載入盤面的方式：
1. 從盤面檔載入（Load From BoardFile）
2. 自定義盤面大小和地雷數量（Customize Size & Bombs）
3. 自定義盤面大小和地雷生成機率（Customize Size & Bomb Generating Rate）

選好載入方式並設定好相關設定後，按下Start Game開始遊戲。

## How To Play

左鍵點擊開啟格子；右鍵點擊放置（旗子／問號／取消放置）。

獲勝方式為開啟所有不是地雷的格子；若遊戲中開到地雷就算輸了。







-------------------------------------------------------------------------------------
CLI Mode
=====================================================================================

在終端輸入 `MineSweeper.exe CommandInput` 開啟程式的CLI介面，並透過輸入指令來進行遊戲。

或者也可輸入 `MineSweeper.exe CommandFile <in>.txt <out>.txt` ，他和上面的指令是差不多的，只不過是從`<in>.txt`讀入指令，並將程式的輸出輸出到`<out>.txt`中。

## Program's State

程式執行分3個階段：Standby -> Playing -> GameOver

在Standby階段，你需要載入盤面，載入好後輸入 `StartGame` 開始遊戲

開始遊戲後會進入Playing階段，輸入 `LeftClick` 來打開格子；輸入 `RightClick` 來放置旗子／問號／取消標記

如果你在踩地雷遊戲中獲勝或輸掉，程式接著會進入GameOver階段，你可以選擇離開遊戲（`Quit`）或重新開始（`Replay`）

## Command

依程式目前的執行階段的不同，可以輸入的指令也不同

### Standby
- `Load BoardFile <file>.txt`
   從`<file>.txt`載入盤面，`<file>.txt`必須符合特定格式（見<#BoardFile Format>）

- `Load RandomCount <row> <col> <mine>`
   指定盤面的列數、欄數、地雷數，並且載入隨機的盤面

- `Load RandomRate <row> <col> <rate>`
   指定盤面的列數、欄數、地雷生成率，並且載入隨機的盤面

- `StartGame`
      開始遊戲

### Playing
- `LeftClick <rowPos> <colPos>`
   左鍵點擊位於第rowPos列、第colPos欄的格子

- `RightClick <rowPos> <colPos>`
   右鍵點擊位於第rowPos列、第colPos欄的格子

### GameOver
- `Quit`
   離開遊戲

- `Replay`
   重新開始遊戲
   > 你需要再次載入盤面

### All Time
- `Print GameBoard`
   印出盤面

- `Print GameAnswer`
   印出答案（地雷的分佈、每一格的數字） （Debug模式下才有）

- `Print GameState`
   印出目前的執行階段（Standby、Playing、GameOver）

- `Print BombCount`
   印出地雷的數量

- `Print FlagCount`
   印出放置的旗子數量

- `Print OpenBlankCount`
   印出已經開啟的格子數量

- `Print RemainBlankCount`
   印出尚未打開的格子（不包含地雷）的數量
   





-------------------------------------------------------------------------------------
BoardFile Format
=====================================================================================

兩個數字代表盤面的列數和欄數

接下來會有每一格的答案（由左至右、由上至下），'X'為地雷，其餘（空白、tab、換行符除外）皆視為空地

若檔案中的答案太多會被忽略；太少會報錯
