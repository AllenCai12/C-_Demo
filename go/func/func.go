package main

import "fmt"

func add(x, y int) int {
    return x + y
}

func swap(x, y int) (int, int) {
    return y, x
}

func main() {
    x := 5
    y := 9
    fmt.Printf("x: %v + y: %v = %v\n", x, y, add(x, y))

    x, y = swap(x, y)
    fmt.Println("after swap")
    fmt.Printf("x: %v + y: %v = %v\n", x, y, add(x, y))
}
