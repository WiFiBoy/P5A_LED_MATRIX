WiFi WebDraw LED Matrix 範例說明 (Feb 17, 2023, Derek & Ricky)

1. 編譯 WebDraw-P5A: 
   ＊ 請先將 library/WebSockets 拷貝到 Arduino library 目錄
   ＊ 修改 WebDraw-P5A.ino 的 WiFi 密碼： AP, PWD （第 8, 9 行)
   ＊ 修改完成後，請直接燒錄執行，燈板會連接 WiFi AP，如果成功就會在燈板上顯示 IP 地址。

2. 執行 web-draw-html/WebDraw.html
   ＊ 請依據連接成功的 IP 地址，修改網頁上的 IP 地址欄。

3. 如果燈板能自動清除，則可以開始用滑鼠在網頁上點圖。
   ＊ 左上角可以變換顏色。左鍵可以畫一個點，右鍵可以清除一個點。
   ＊ [CLEAR] 可以清除整個燈板。

