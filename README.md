一､作業目的 
熟悉如何利用fork()系統呼叫產生新的process，以及process彼此之間如何使用POSIX shared memory互相 傳遞資料。 
二､作業內容 
基本要求： 設計一個程式，讀入一個2-30的整數 n。程式就會產生一個子行程。父行程透過POSIX shared memory傳 兩個Lucas number給子行程，由子行程計算出下一個 Lucas number。L0=2, L1=1。 Ln=Ln-1 + Ln-2
執行流程如下：父行程讀入一個2-30的整數 n，然後產生一個子行程。要計算Li (2≤ i ≤ n)時，父行程將Li1與Li-2 透過POSIX shared memory傳給子行程。子行程負責計算出Li結果，然後用同一塊POSIX shared memory傳回給父行程。父行程再將Li與Li-1傳給子行程計算出Li+1結果，如此反覆計算，一直到父行程得 到Ln為止。 在計算過程中，父行程與子行程都要印出參數傳遞與 Lucas number 計算的過程，以及自己的pid。 例如若父行程為 2370，子行程為2389，程式進行的一個可能的過程如下： 
> prog1 
Input number: 10 
[2370 Parent]: 2 1 
[2389 Child]: 2+1=3 
[2370 Parent]: 1 3 
[2389 Child]: 1+3=4 
… 
[2389 Child]: 47+76=123 
[2370 Parent]: L(10)=123


三､程式說明
利用fork()產生父行程與子行程，並由control來選擇該執行父行程或是子行程：當control==1，執行父行程;當control==0，執行子行程。
創建shared memory shm使父子行程能夠共用記憶體，然後建立一個指向shm 中structure 的指標d來選取Lucas functions中的Ln,Ln-1或是Ln-2。

