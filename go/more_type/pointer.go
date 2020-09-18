package main

import (
    "fmt"
)

func main() {
    var p *int
    i := 90

//    fmt.Println(*p)

    p = &i

    fmt.Println(*p)

    *p = 128

    fmt.Println(i)
}
