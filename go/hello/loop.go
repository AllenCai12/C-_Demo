package main

import "fmt"

func main() {

   /*like c for usage*/
    sum := 0
    for i:=0; i<10; i++ {
        sum += i
        fmt.Println(i, sum)
    }

    for ; sum < 1000;  {
        sum += sum
    }

    fmt.Println(sum)

   /*like c "while" usage*/
    for  sum < 9000 {
        sum += sum
        fmt.Println(sum)
    }
 }
